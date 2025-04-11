// Shoot_Them_UP


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

void ASTUPlayerState::LogInfo()
{
	UE_LOG(LogSTUPlayerState, Display, TEXT("TeamUD: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
