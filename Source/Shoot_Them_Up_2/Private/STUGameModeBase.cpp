// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameModeBase.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "STUPlayerState.h"
#include "STURespawnComponent.h"
#include "STUUtil.h"
#include "Player/STUCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase() 
{
   DefaultPawnClass = ASTUCharacter::StaticClass();
   PlayerControllerClass = ASTUPlayerController::StaticClass();
   HUDClass = ASTUGameHUD::StaticClass();
   PlayerStateClass = ASTUPlayerState::StaticClass();
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::StartPlay()
{
   Super::StartPlay();
   SpawnBots();
   CreateTeamsInfo();

   CurrentRound = 1;
   StartRound();
}
//----------------------------------------------------------------------------------------------------------
UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
   if (InController &&  InController->IsA<AAIController>())
   {
      return AIPawnClass;
   }
   return Super::GetDefaultPawnClassForController_Implementation(InController);
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::SpawnBots()
{
   if (!GetWorld()) return;

   for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
   {
      FActorSpawnParameters SpawnInfo;
      SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

      const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
      RestartPlayer(STUAIController);
   }
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::StartRound()
{
   RoundCountDown = GameData.RoundTime;
   GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::GameTimerUpdate()
{
   UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i, / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
   if (--RoundCountDown == 0)
   {
      GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
      if (CurrentRound + 1 <= GameData.RoundsNum)
      {
         ++CurrentRound;
         ResetPlayers();
         StartRound();
      }
      else
      {
         GameOver();
      }
   }
}
//----------------------------------------------------------------------------------------------------------
//каждый раз, когда начинается раунд, мы вызываем ResetPlayers, проходим по всем контроллерам и вызываем функцию
//ResetOnePlayer для каждого из контроллеров.
void ASTUGameModeBase::ResetPlayers()
{
   if (!GetWorld()) return;

   for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
   {
      ResetOnePlayer(It->Get());
   }
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
   if (Controller && Controller->GetPawn())
   {
      Controller->GetPawn()->Reset();
   }
   RestartPlayer(Controller);
   SetPlayerColor(Controller);
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::CreateTeamsInfo()
{
   if (!GetWorld()) return;
   int32 TeamID = 1; // Начинаем с первой команды
   
   for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // Перебираем всех контроллеров в игре
   {
      const auto Controller = It->Get(); // Получаем контроллер
      if (!Controller) continue; // Проверяем, что он существует

      const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // Приводим `PlayerState` к нужному типу
      if (!PlayerState) continue; // Проверяем, что у контроллера есть состояние игрока

      PlayerState->SetTeamID(TeamID); // Устанавливаем ID команды
      PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID)); // Назначаем цвет команды
      SetPlayerColor(Controller); 

      TeamID = TeamID == 1 ? 2 : 1; // Меняем команду на противоположную (чередование между 1 и 2)
   }
   
}
//----------------------------------------------------------------------------------------------------------
FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
   if (TeamID - 1 < GameData.TeamColor.Num()) // Проверяем, есть ли цвет для этой команды
   {
      return GameData.TeamColor[TeamID - 1]; // Возвращаем цвет команды из массива
   }
   UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
   return GameData.DefaultTeamColor;
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
   if (!Controller) return;
   const auto Character = Cast<ASTUCharacter>(Controller->GetPawn()); 
   if (!Character) return;

   const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); 
   if (!PlayerState) return;

   Character->SetPlayerColor(PlayerState->GetTeamColor());
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
   const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
   const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

   if (KillerPlayerState)
   {
      KillerPlayerState->AddKill();
   }
   
   if (VictimPlayerState)
   {
      VictimPlayerState->AddDeath();
   }

   StartRespan(VictimController);
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::LogPlayerInfo()
{
   if (!GetWorld()) return;
   
   for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
   {
      const auto Controller = It->Get(); 
      if (!Controller) continue; 

      const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
      if (!PlayerState) continue;
      
      PlayerState->LogInfo();
   }
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::StartRespan(AController* Controller)
{
   const auto RespawnAvailble = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
   if (!RespawnAvailble) return;
   
   const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
   if (!RespawnComponent) return;

   RespawnComponent->Respawn(GameData.RespawnTime);
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
   ResetOnePlayer(Controller);
}
//----------------------------------------------------------------------------------------------------------
void ASTUGameModeBase::GameOver()
{
   UE_LOG(LogSTUGameModeBase, Display, TEXT("==========GAME OVER=========="));
   LogPlayerInfo();

   for (auto Pawn: TActorRange<APawn>(GetWorld()))
   {
      if (Pawn)
      {
         Pawn->TurnOff();
         Pawn->DisableInput(nullptr);
      }
   }
}
//----------------------------------------------------------------------------------------------------------