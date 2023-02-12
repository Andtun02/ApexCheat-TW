#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

vector<string> strSplit(string str, string separator)
{
    vector<string> result;
    int cutAt;
    while ((cutAt = str.find_first_of(separator)) != str.npos)
    {
        if (cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if (str.length() > 0)
    {
        result.push_back(str);
    }
    return result;
}

namespace Config {
    // 写数据
    bool write_data(const std::string& filename, const std::string& strs)
    {
        std::ofstream fout;
        fout.open(filename);
        if (!fout.is_open())
        {
            printf(_("[!] 无法打开配置文件：%s\n"), filename.c_str());
            return false;
        }
        fout << strs;
        fout.close();
        return true;
    }
    // 读数据
    bool read_data(const std::string& filename, std::stringstream& dest_data)
    {
        std::ifstream fin;
        fin.open(filename);
        if (!fin.is_open())
        {
            printf(_("[!] 无法打开配置文件：%s\n"), filename.c_str());
            return false;
        }
        dest_data << fin.rdbuf();
        fin.close();
        return true;
    }
    // 寻找keyword
    std::string find_keyword(const std::string& strs, const std::string& begin_strs, const std::string& end_strs)
    {
        if (strs.empty() || begin_strs.empty() || end_strs.empty())
            return "";
        int end;
        int begin;
        int strs_size;
        begin = strs.find(begin_strs, 0);
        end = strs.find(end_strs, begin);
        strs_size = begin_strs.size();
        return strs.substr(begin + strs_size, end - begin - strs_size);
    }

    std::string& clear_all_space(std::string& strs)
    {
        int begin = 0;
        if (!strs.empty())
        {
            while ((begin = strs.find(' ', begin)) != std::string::npos)
            {
                strs.erase(begin, 1);
            }
        }
        return strs;
    }

    inline bool exists(const std::string& name) {
        ifstream f(name.c_str());
        return f.good();
    }

    bool Init(string cardnum)
    {
        if (!exists("configs.ini") || cardnum != "null")
        {
            ofstream ofs;
            ofs.open("configs.ini", ios::out);
            ofs << "# 卡密\n";
            ofs << "cardnum=" + cardnum + "\n";

            ofs << "\n# 压枪 | 范围1-85\n";
            ofs << "norecil=false\n";
            ofs << "intensity=65\n";

            ofs << "\n# 倒地 | 可视 | 不可视 的RGB颜色设置\n";
            ofs << "down=10,10,10\n";
            ofs << "visible=40,0,0\n";
            ofs << "invisible=20,10,100\n";
            ofs.close();
        }
        return true;
    }

    void LoadConfig()
    {
        bool ret;
        std::string result;
        std::stringstream content;
        ret = Config::read_data("configs.ini", content);
        if (ret)
        {
            result = content.str();
            result = Config::clear_all_space(result);
            // 卡密
            Software::cardnum = Config::find_keyword(result, "cardnum=", "\n").c_str();
            if (Software::cardnum.length() < 10)
            {
                printf(_("[+] 请输入卡密:"));
                cin >> Software::cardnum;
                Init(Software::cardnum);
            }
            // 压枪
            string tmp1 = Config::find_keyword(result, "norecil=", "\n").c_str();
            if (tmp1 == "true")
                Settings::norecil = true;
            Settings::intensity = atof(Config::find_keyword(result, "intensity=", "\n").c_str());
            if (Settings::intensity < 0 || Settings::intensity >85)
                Settings::intensity = 75;
            // 颜色
            string tmp;
            vector<string> tmp_2;
            tmp = Config::find_keyword(result, "down=", "\n").c_str();
            tmp_2 = strSplit(tmp, ",");
            Settings::down.R = atof(tmp_2[0].c_str());
            Settings::down.G = atof(tmp_2[1].c_str());
            Settings::down.B = atof(tmp_2[2].c_str());

            tmp = Config::find_keyword(result, "visible=", "\n").c_str();
            tmp_2 = strSplit(tmp, ",");
            Settings::visible.R = atof(tmp_2[0].c_str());
            Settings::visible.G = atof(tmp_2[1].c_str());
            Settings::visible.B = atof(tmp_2[2].c_str());

            tmp = Config::find_keyword(result, "invisible=", "\n").c_str();
            tmp_2 = strSplit(tmp, ",");
            Settings::invisible.R = atof(tmp_2[0].c_str());
            Settings::invisible.G = atof(tmp_2[1].c_str());
            Settings::invisible.B = atof(tmp_2[2].c_str());

        }
    }
}