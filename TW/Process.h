#pragma once
#include "Driver.h"
#include "offsets.h"

// 透视线程
DWORD64 GetEntityById(int Ent, DWORD64 Base)
{
	Protect(_ReturnAddress());
	DWORD64 EntityList = Base + OFFSET_ENTITYLIST;
	DWORD64 BaseEntity = memRead(EntityList, DWORD64);

	if (!BaseEntity)
		return NULL;

	return memRead(EntityList + (Ent << 5), DWORD64);
}

// 人物透视
bool IsPlayer(uintptr_t player)
{
	return (memRead(player + TOFFSET(OFFSET_NAME), uintptr_t) == 125780153691248);
}
// 人物透视
RGB Color;
float entityNewVisTime = 0;
float entityOldVisTime[100];
int visCooldownTime[100];
void Glow() {
	while (Software::run) {
		Sleep(1);
		Protect(_ReturnAddress());
		uintptr_t locPlayer = memRead(Mem::base + OFFSET_LOCAL_ENT, uintptr_t);// 本地玩家
		if (!IsPlayer(locPlayer)) continue;
		int localTeam = memRead(locPlayer + TOFFSET(OFFSET_TEAM), int);

		for (int i = 0; i < 100; i++)
		{
			// 获取敌人
			DWORD64 ent = GetEntityById(i, Mem::base);
			if (ent == 0) continue;
			DWORD64 EntityHandle = memRead(ent + TOFFSET(OFFSET_NAME), DWORD64);
			std::string Identifier = memRead(EntityHandle, std::string);
			LPCSTR IdentifierC = Identifier.c_str();

			// 是否倒地
			int entKnockedState = memRead(ent + TOFFSET(OFFSET_BLEED_OUT_STATE), int);
			if (strcmp(IdentifierC, _("player")))
			{
				if (localTeam == memRead(ent + TOFFSET(OFFSET_TEAM), int)) continue; // 跳过队友

				memWrite(ent + TOFFSET(OFFSET_GLOW_DISTANCE), FLT_MAX, float);//全局透视
				if (entKnockedState != 0) {// 倒地判断
					Color = Settings::down;
				}
				else
				{
					if (entityNewVisTime != entityOldVisTime[i]) {
						visCooldownTime[i] = 32;
						Color = Settings::visible;
						entityOldVisTime[i] = entityNewVisTime;
					}
					else {
						Color = Settings::invisible;
					}
				}

				memWrite(ent + TOFFSET(OFFSET_GLOW_ENABLE), 1, int);
				memWrite(ent + TOFFSET(OFFSET_GLOW_THROUGH_WALLS), 2, int);
				memWrite(ent + TOFFSET(OFFSET_GLOW_MODE), Settings::gm, GlowMode);
				memWrite(ent + TOFFSET(OFFSET_GLOW_COLOR), Color);
				if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1;
			}
		}
		Sleep(1);
		Unprotect(_ReturnAddress());
	}
}

// 无后线程
vec3 oldPunch = { 0.f, 0.f, 0.f };
void NoRecoil()
{
	while (Software::run)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!Settings::norecil) continue;

		uintptr_t locPlayer = memRead(Mem::base + OFFSET_LOCAL_ENT, uintptr_t);// 本地玩家
		bool isplayer = (memRead(locPlayer + OFFSET_NAME, uintptr_t) == 125780153691248);
		// check if localplayer is valid and rcs is on

		uint64_t entitylist = Mem::base + OFFSET_ENTITYLIST;
		uint64_t wephandle = memRead(locPlayer + OFFSET_WEAPON, uint64_t);
		wephandle &= 0xffff;
		uint64_t wep_entity = memRead(entitylist + (wephandle << 5), uint64_t);
		float zoom_fov = memRead(wep_entity + OFFSET_ZOOM_FOV, float);

		if (!isplayer || zoom_fov == 70) continue;

		//if (!global.enable_rcs || !isplayer || !GetAsyncKeyState(VK_RBUTTON)) continue;//|| globals.aimbotting
		vec3 viewAngles = memRead(locPlayer + OFFSET_VIEWANGLES, vec3);
		vec3 punchAngle = memRead(locPlayer + OFFSET_AIMPUNCH, vec3);
		vec3 newAngle = viewAngles + (oldPunch - punchAngle) * (Settings::intensity / 100.f);
		newAngle.Normalize(); // clamp angles
		memWrite(locPlayer + OFFSET_VIEWANGLES, newAngle, vec3); // overwrite old angles
		oldPunch = punchAngle;
	}
}