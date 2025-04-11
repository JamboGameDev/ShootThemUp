// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUNeedAmmoDecorator.generated.h"

class ASTUBase_Weapon;
UCLASS()
class SHOOT_THEM_UP_2_API USTUNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	USTUNeedAmmoDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ASTUBase_Weapon> WeaponType;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
