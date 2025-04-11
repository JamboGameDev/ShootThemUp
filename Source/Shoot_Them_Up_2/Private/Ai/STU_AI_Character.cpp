// Shoot_Them_UP


#include "Ai/STU_AI_Character.h"

#include "BrainComponent.h"
#include "Ai/STU_AI_Controller.h"
#include "Components/STUAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTU_AI_Character::ASTU_AI_Character(const FObjectInitializer& ObjInit)
:Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ASTU_AI_Controller::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}
//--------------------------------------------------------------------------------------------------------------
void ASTU_AI_Character::OnDeath()
{
	Super::OnDeath();

	const auto STUController = Cast<ASTU_AI_Controller>(Controller);
	if (STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();
	}
}
