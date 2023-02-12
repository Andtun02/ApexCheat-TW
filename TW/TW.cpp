#include <iostream>
#include <thread>

#include "NetworkPass.h" // MFC not windows.h so 放在最上面 真麻烦
#include "Utils.h"
#include "Configs.h"
#include "9rZR9g1uwJA8.h"
#include "Process.h"


BOOL WINAPI HandlerRoutine(DWORD dwCtrlType) {
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT: //CTRL + C
	case CTRL_CLOSE_EVENT: //关闭
	case CTRL_LOGOFF_EVENT: //用户退出
	case CTRL_SHUTDOWN_EVENT: //系统被关闭时.
		Software::run = false;
		Network::LoginOut();
		break;
	}
	return 0;
}

void TuliFEpV1SIGaYi() {
	float QIFHLSMOGWPB = 95566080075838; QIFHLSMOGWPB = 30932990185272;
	if (QIFHLSMOGWPB == 10073911500269) { QIFHLSMOGWPB = 48749534686608; QIFHLSMOGWPB = 47032689774371; }
	if (QIFHLSMOGWPB == 48293952012187) { QIFHLSMOGWPB = 41178278970915; QIFHLSMOGWPB = 10968207662021; }
	if (QIFHLSMOGWPB == 69118156664435) { QIFHLSMOGWPB = 85938000262576; QIFHLSMOGWPB = 54479510681995; }
	if (QIFHLSMOGWPB == 42138480792116) { QIFHLSMOGWPB = 64924811264848; QIFHLSMOGWPB = 90191569486384; }
	if (QIFHLSMOGWPB == 65584431995032) { QIFHLSMOGWPB = 4809017720507; QIFHLSMOGWPB = 510747063948; }
	float JYTJLSNRWLUS = 21608477913074; JYTJLSNRWLUS = 33308809767349;
	if (JYTJLSNRWLUS == 37357493901522) { JYTJLSNRWLUS = 52576931375953; JYTJLSNRWLUS = 39350742782250; }
	if (JYTJLSNRWLUS == 42936835107462) { JYTJLSNRWLUS = 45078599765583; JYTJLSNRWLUS = 47433270823798; }
	if (JYTJLSNRWLUS == 83750282754904) { JYTJLSNRWLUS = 53700004540443; JYTJLSNRWLUS = 77416103894642; }
	if (JYTJLSNRWLUS == 99841244526874) { JYTJLSNRWLUS = 73557450033841; JYTJLSNRWLUS = 57645846562292; }
	if (JYTJLSNRWLUS == 21484664489069) { JYTJLSNRWLUS = 85128297622890; JYTJLSNRWLUS = 89345279844750; }
	float ISOOFNVZMAMQ = 35425675655454; ISOOFNVZMAMQ = 28668078496764;
	if (ISOOFNVZMAMQ == 28429892469809) { ISOOFNVZMAMQ = 47100739381484; ISOOFNVZMAMQ = 5589237125659; }
	if (ISOOFNVZMAMQ == 39790736767812) { ISOOFNVZMAMQ = 45059419008767; ISOOFNVZMAMQ = 68432353123004; }
	if (ISOOFNVZMAMQ == 79101025206084) { ISOOFNVZMAMQ = 96131699136861; ISOOFNVZMAMQ = 48183711975541; }
	if (ISOOFNVZMAMQ == 64614145099447) { ISOOFNVZMAMQ = 87434113977128; ISOOFNVZMAMQ = 63852837643569; }
	if (ISOOFNVZMAMQ == 71993321311393) { ISOOFNVZMAMQ = 59438687131541; ISOOFNVZMAMQ = 9094257888855; }
	float ZQLXWZFTAAQB = 67791031287; ZQLXWZFTAAQB = 77165002758265;
	if (ZQLXWZFTAAQB == 83992315610093) { ZQLXWZFTAAQB = 46420124635597; ZQLXWZFTAAQB = 25740427319462; }
	if (ZQLXWZFTAAQB == 19540781062534) { ZQLXWZFTAAQB = 65474331469506; ZQLXWZFTAAQB = 23254937804290; }
	if (ZQLXWZFTAAQB == 40140817620299) { ZQLXWZFTAAQB = 49456197471319; ZQLXWZFTAAQB = 73224264454254; }
	if (ZQLXWZFTAAQB == 69420438695707) { ZQLXWZFTAAQB = 64377203087629; ZQLXWZFTAAQB = 59146504283659; }
	if (ZQLXWZFTAAQB == 72672989174343) { ZQLXWZFTAAQB = 96431336810461; ZQLXWZFTAAQB = 15306057204315; }

}

int main(int count, char** vector)
{
	TuliFEpV1SIGaYi();

    Protect(_ReturnAddress());
	// 设置窗口
    system("mode con cols=48 lines=14");
	// 随机窗口名
	srand(time(NULL));
	Utils::RenameFile(std::string(vector[0])); // 关闭 符合模式
	Sleep(150);
	// 配置文件加载
	if (Config::Init("null")) Config::LoadConfig();
	// 移除快速编辑模式
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE; //移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE; //移除插入模式
	mode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hStdin, mode);
	// 云端验证
	if (!Network::IniSoftInfo()) { system("pause"); return 0; };
	if (!Network::LoginByCard(false)) { system("pause"); return 0; };
	// 程序关闭事件
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	// get pid
	printf(_("[+] 检测游戏进程...\n"));
	while (!kernelHandler.getPid(_("r5apex.exe"))) Sleep(1000);
	// attach
	string dpath = Network::GetremoteVar("GLOW");//正常版本
	if (!kernelHandler.attach("r5apex.exe", dpath)) { Kdmap::nRWjKZCcn6usWg9(); Sleep(1000); kernelHandler.attach("r5apex.exe", dpath); }
	printf(_("[+] 附加到游戏 | 长时间停留在此 请重启电脑再试\n"));
	// get baseaddress
	while (Mem::base == 0){ Mem::base = kernelHandler.get_module_base("r5apex.exe");Sleep(1000);}
	printf(_("[+] 获取游戏基址：%p\n"), Mem::base);
	// 正式运行
	Beep(659, 100);
	Software::run = true;
	printf(_("\n[+] 正在运行，请勿关闭窗口\n%s", RandomString(24).c_str()));
	// 无后线程
	std::thread(NoRecoil).detach();
	std::thread(Glow).join();
	
	system("pause");
    Unprotect(_ReturnAddress());
}
