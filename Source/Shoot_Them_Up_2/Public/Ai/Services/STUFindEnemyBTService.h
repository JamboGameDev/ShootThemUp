// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFindEnemyBTService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOT_THEM_UP_2_API USTUFindEnemyBTService : public UBTService
{
	GENERATED_BODY()

public:
	USTUFindEnemyBTService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
