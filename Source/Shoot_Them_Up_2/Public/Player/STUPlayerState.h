// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"


UCLASS()
class SHOOT_THEM_UP_2_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void SetTeamID(int32 ID) {TeamID = ID;}
	int32 GetTeamID() const {return TeamID;}

	void SetTeamColor(const FLinearColor& color) {TeamColor = color;}
	FLinearColor GetTeamColor() const {return TeamColor;}

	void AddKill() { ++KillsNum;}
	int32 GetKillsNum() const {return KillsNum;}
	
	void AddDeath() { ++DeathsNum;}
	int32 GetDeathsNum() const {return DeathsNum;}

	void LogInfo();
	
	

private:
	int32 TeamID;
	FLinearColor TeamColor;

	int32 KillsNum = 0;
	int32 DeathsNum = 0;
	
};
