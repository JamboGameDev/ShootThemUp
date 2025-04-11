// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUPlayerController.h"

#include "STURespawnComponent.h"
#include "GameFramework/GameModeBase.h"
//---------------------------------------------------------------------------------------------------------
ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}
//---------------------------------------------------------------------------------------------------------
void ASTUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}
//---------------------------------------------------------------------------------------------------------
void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("Pause", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
}
//---------------------------------------------------------------------------------------------------------
void ASTUPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	
	GetWorld()->GetAuthGameMode()->SetPause(this);
}
//---------------------------------------------------------------------------------------------------------