// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STU_AI_Controller.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class SHOOT_THEM_UP_2_API ASTU_AI_Controller : public AAIController
{
	GENERATED_BODY()

public:
	ASTU_AI_Controller();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUAIPerceptionComponent* STUAIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FName FocusOnKeyName = "EnemyActor";
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

private:
	AActor* GetFocusOnActor()const;
};
