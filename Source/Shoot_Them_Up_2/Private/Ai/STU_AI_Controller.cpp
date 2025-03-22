// Shoot_Them_UP


#include "Ai/STU_AI_Controller.h"
#include "STUAIPerceptionComponent.h"
#include "Ai/STU_AI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTU_AI_Controller::ASTU_AI_Controller()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);

	bWantsPlayerState = true;
}
//-------------------------------------------------------------------------------------------------------------
void ASTU_AI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUCharacter = Cast<ASTU_AI_Character>(InPawn);
	if(STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTree);
	}
}
//-------------------------------------------------------------------------------------------------------------
void ASTU_AI_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}
//-------------------------------------------------------------------------------------------------------------
AActor* ASTU_AI_Controller::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
//-------------------------------------------------------------------------------------------------------------
