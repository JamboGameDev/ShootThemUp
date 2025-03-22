// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBase_Weapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOT_THEM_UP_2_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBase_Weapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ASTUBase_Weapon> WeaponType);
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "Weapon_Socet";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocket = "Armory_Socket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	ASTUBase_Weapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<ASTUBase_Weapon*> Weapons;

	int32 CurrentWeaponIndex = 0;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CanFire() const;
	bool CanEquip() const;
	
	void EquipWeapon(int32 WeaponIndex);

private:
	

	UPROPERTY()
	UAnimMontage* CurrentRealoadAnimMontage = nullptr;
	
	bool EquipAnimProgress = false;
	bool ReloadAnimProgress = false;


	void InitAnimations();
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTUBase_Weapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void PlayAnimMontage(UAnimMontage* Animation);

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	
	bool CanReaload() const;

	void OnEmptyClip(ASTUBase_Weapon* AmmoEmptyWeapon);
	void ChangeClip();

};
