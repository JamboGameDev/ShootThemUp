// Shoot_Them_UP


#include "Ai/Services/STUFindEnemyBTService.h"

#include "AIController.h"
#include "STUAIPerceptionComponent.h"
#include "STUUtil.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFindEnemyBTService::USTUFindEnemyBTService()
{
	NodeName = "Find Enemy";
}
//-------------------------------------------------------------------------------------------------------------
void USTUFindEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
