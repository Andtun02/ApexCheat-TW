#pragma once
#include "Struct.h"

namespace Software {
    string versionname = "2.0";//��ǰ����汾��
    bool run = false; // �Ƿ�����
    string cardnum = "null"; //����
}

namespace Mem{
    uintptr_t pid;
    uintptr_t base = 0;
}

namespace Settings {
    // ѹǹ
    bool norecil;
    int intensity;

    // ��ɫ
    GlowMode gm = { 101,101,40,90 };// ����Թ�ģʽ
    RGB down;//�����ɫ
    RGB visible;//��ʾ��ɫ
    RGB invisible;//δ��ʾ��ɫ
}