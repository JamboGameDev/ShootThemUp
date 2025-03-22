// Shoot_Them_UP


#include "Components/STUAIPerceptionComponent.h"

#include "AIController.h"
#include "STUHealthComponent.h"
#include "STUUtil.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy()
{
	TArray<AActor*> PerciveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerciveActors);
	if (PerciveActors.Num() == 0) return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for (const auto PerciveActor : PerciveActors)
	{
		const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PerciveActor);

		const auto PercievePawn = Cast<APawn>(PerciveActor);
		const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);
		
		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = (PerciveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerciveActor;
			}
		}
	}
	return BestPawn;
}
