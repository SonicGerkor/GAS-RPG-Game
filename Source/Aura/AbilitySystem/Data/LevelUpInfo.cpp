// Copyright Gerkor Games

#include "LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		if (LevelInfo.Num() - 1 <= Level) return Level;

		if (XP >= LevelInfo[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
