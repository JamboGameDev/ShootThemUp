// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFireService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOT_THEM_UP_2_API USTUFireService : public UBTService
{
	GENERATED_BODY()

public:
	USTUFireService();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;
	
};
