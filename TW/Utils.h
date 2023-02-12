#pragma once
#include <string>
#include <random>

#include "MemProtect.h"
#include "crypt.h"

// 随机字符串
std::string RandomString(const int len)
{
	Protect(_ReturnAddress());
	const std::string alpha_numeric(_("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890@#%"));

	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

	std::string str(len, 0);
	for (auto& it : str)
		it = alpha_numeric[distribution(generator)];

	return str;
	Unprotect(_ReturnAddress());
}

namespace Utils {
	bool RenameFile(std::string& path)
	{
		Protect(_ReturnAddress());
		std::string newPath = (RandomString(24) + ".exe");
		SetConsoleTitleA(RandomString(24).c_str());
		if (std::rename(path.c_str(), newPath.c_str())) return false;

		path = newPath;
		printf("[+] 文件重命名\n");
		return true;
		Unprotect(_ReturnAddress());
	}
}