// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUCharacter.generated.h"


class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOT_THEM_UP_2_API ASTUCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTUCharacter(const FObjectInitializer& ObjInit);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	void SetPlayerColor(FLinearColor Color);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f); //the maximum speed at which damage will be dealt

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10, 100); //damage value depending on speed

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color";
	
	virtual void BeginPlay() override;
	virtual void OnDeath();
	void OnHealthChanged(float Health, float HealthDelta);

private:
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

};
