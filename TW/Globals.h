#pragma once
#include "Struct.h"

namespace Software {
    string versionname = "2.0";//当前软件版本号
    bool run = false; // 是否运行
    string cardnum = "null"; //卡密
}

namespace Mem{
    uintptr_t pid;
    uintptr_t base = 0;
}

namespace Settings {
    // 压枪
    bool norecil;
    int intensity;

    // 颜色
    GlowMode gm = { 101,101,40,90 };// 人物辉光模式
    RGB down;//跪地颜色
    RGB visible;//显示颜色
    RGB invisible;//未显示颜色
}