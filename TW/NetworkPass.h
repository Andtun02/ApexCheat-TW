#pragma once
#include "NetVerHelp/Help头文件/NetworkVerHelp.h"
#include "Globals.h"
#include "crypt.h"
#include <thread>

string encryptKey = "\";//加密的key
string signSalt = "\";//签名盐
string goodscode = "\";//必填，软件或者商品的编码
string platformUserCode = "\";//个人中心里面可以查看

Model::PlatformType platformtypeid = Model::PlatformType::NetVer;
Model::EncryptType encrypttypeid = Model::EncryptType::RC4;
string maccode = Help::NetworkVerHelp::GetMac();//机器码

//心跳Key,每次心跳此值都会变
string heartbeatkey;
//登录成功后的令牌
string token;

namespace Network {
#pragma region 获取软件变量
	//获取软件变量
	string GetremoteVar(string _testVer)
	{
		//构建获取软件变量入参
		Model::In_getRemoteVarArgs args;
		args.maccode = maccode;//必填
		args.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
		args.requestflag = to_string(args.timestamp);//必填
		args.cardnumorusername = Software::cardnum;//必填
		args.varname = _testVer;//必填，如果此值为空，那么接口返回来的是此软件所有的变量，否则是此变量名所对应的变量值
		args.token = token;//必填
		//请求接口，获取结果
		Model::Result result = Help::NetworkVerHelp::getRemoteVar(args);
		if (result.code == 0)
		{
			//变量具体数据
			Model::Out_getRemoteVar RemoteVar = Help::NetworkVerHelp::ConvertRemoteVar(result.data);
			if (RemoteVar.requestflag != args.requestflag)
			{
				printf(_("[!] 云端反馈数据疑似被劫持\n"));
				return "";
			}
			printf(_("[-] 获取远程变量变量成功\n"));
			return RemoteVar.varValue;
		}
		else
		{
			printf(_("[-] 获取远程变量失败 | 原因：%s\n"), result.msg.c_str());
			return "";
		}
	}
#pragma endregion

#pragma region 初始化服务器
	bool IniSoftInfo()
	{
		//构建初化软件入参
		Model::In_IniSoftInfoArgs args;
		args.maccode = maccode;//必填
		args.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
		args.requestflag = to_string(args.timestamp);//必填
		args.versionname = Software::versionname;//必填

		//请求接口，获取结果
		bool IniResult = false;
		Model::Result result = Help::NetworkVerHelp::GetIniSoftInfo(args, platformtypeid, encryptKey, signSalt, encrypttypeid, goodscode, platformUserCode);

		if (result.code == 0)
		{
			switch (platformtypeid)
			{
			case Model::NetVer://网络验证平台
				//iniSoftInfoData:初始化完后的具体结果
				if (Help::iniSoftInfoData.inisoftkey != "")
				{
					if (Help::iniSoftInfoData.requestflag != args.requestflag)
					{
						printf(_("[!] 云端反馈数据疑似被劫持\n"));
						return false;
					}
					printf(_("[+] 云端连接成功\n"));
					if (Help::iniSoftInfoData.softInfo.newversionnum != "")
					{
						if (Help::iniSoftInfoData.softInfo.newversionnum != Software::versionname)
						{
							printf(_("[+] 发现新版本：%s | 请咨询相关代理\n"), Help::iniSoftInfoData.softInfo.newversionnum.c_str());
						}
					}
					IniResult = true;
				}
				break;
			}
		}
		else
		{
			printf(_("[-] 云端连接失败 | 原因：%s\n"), result.msg.c_str());
			IniResult = false;
		}
		return IniResult;
	}
#pragma endregion

#pragma region 心跳线程示例
	int faildCount = 0;
	bool HeartBeat()
	{
		//注意：心跳是放在死循环里面，此只是做演示，所以没有做死循环了(注意：心跳间隔时间最低不能低于十秒钟一次，不然会引发心跳频繁)
		Model::In_HeartBeatArgs HeartBeatArgs;
		HeartBeatArgs.maccode = maccode;//必填
		HeartBeatArgs.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
		HeartBeatArgs.requestflag = to_string(HeartBeatArgs.timestamp);//必填
		HeartBeatArgs.cardnumorusername = Software::cardnum;//必填
		HeartBeatArgs.token = token;//必填
		HeartBeatArgs.heartbeatkey = heartbeatkey;//必填(每次把最新的心跳Key赋值)
		//请求接口，获取结果
		bool hbResult = true;
		Model::Result	result = Help::NetworkVerHelp::GetHeartBeat(HeartBeatArgs);
		if (result.code == 0)
		{
			//心跳具体数据
			Model::Out_heartBeat heartBeatResult = Help::NetworkVerHelp::ConvertHeartBeat(result.data);
			//这一块相当重要：一定要判断这个“心跳Key(heartbeatkey)”和"请求标识(requestflag)"，防止别人修改你请求的地址，给你返回一个假消息回来
			if (heartBeatResult.heartbeatkey != "" && heartBeatResult.requestflag == HeartBeatArgs.requestflag)
			{
				heartbeatkey = heartBeatResult.heartbeatkey;
				/*Msg = Msg + "最新的心跳Key：" + heartBeatResult.heartbeatkey + "\n";
				Msg = Msg + "到期时间：" + heartBeatResult.endtime + "\n";
				Msg = Msg + "剩余点数：" + to_string(heartBeatResult.surpluspointvalue) + "\n\n";*/
				hbResult = true;
			}
			else
			{
				printf(_("[!] 云端反馈数据疑似被劫持\n"));
				faildCount++;
			}
		}
		else
		{
			printf(_("[-] 心跳失败 | 原因：%s\n"), result.msg.c_str());
			faildCount++;
		}

		if (faildCount >= 5)
		{
			hbResult = false;
		}
		return hbResult;
	}
	void HertBeatThread() {
		while (true)
		{
			if (!HeartBeat()) {
				Software::run = false;
				printf(_("[-] 异常退出 | 来自心跳监控\n"));
			}
			//心跳间隔时间最小不能小于5分钟
			Sleep(1000 * 60 * 5);
		}
	}
#pragma endregion

#pragma region 退出登录示例
	//退出登录示例
	void LoginOut()
	{
		//构建退出登录入参
		Model::In_LoginOutArgs LoginOutArgs;
		LoginOutArgs.maccode = maccode;//必填
		LoginOutArgs.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
		LoginOutArgs.cardnumorusername = Software::cardnum;//必填
		LoginOutArgs.token = token;//必填
		//请求接口，获取结果
		Model::Result result = Help::NetworkVerHelp::GeLoginOut(LoginOutArgs);
		printf(_("[+] 退出登录 | %s\n"), result.msg.c_str());
	}
#pragma endregion

#pragma region 卡密登录示例
	//卡密登录示例
	bool LoginByCard(bool IsLoginOut)
	{
		bool IsLoginOk = false;
		//构建登录入参
		Model::In_CardLoginArgs args;
		args.maccode = maccode;//必填
		args.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
		args.requestflag = to_string(args.timestamp);//必填
		args.cardnum = Software::cardnum;//必填
		//请求接口，获取结果
		Model::Result result = Help::NetworkVerHelp::GetLoginByCard(args);
		if (result.code == 0)
		{
			//登录成功后的具体数据
			Model::Out_Login LoginData = Help::NetworkVerHelp::ConvertLoginData(result.data);
			if (LoginData.requestflag != args.requestflag)
			{
				printf(_("[!] 云端反馈数据疑似被劫持\n"));
				return false;
			}
			token = LoginData.token;
			heartbeatkey = LoginData.heartbeatkey;

			printf(_("[+] 登录成功\n[+] 到期时间：%s\n"), LoginData.endtime.c_str());
			std::thread(HertBeatThread).detach();
			IsLoginOk = true;

		}
		else
		{
			printf(_("[-] 登录失败 | 原因：%s\n"), result.msg.c_str());
			IsLoginOk = false;
		}
		return IsLoginOk;
	}
#pragma endregion
}