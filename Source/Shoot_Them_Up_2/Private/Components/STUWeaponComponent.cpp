// Shoot_Them_UP


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBase_Weapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STU_Reload_Finished_AnimNotify.h"
#include "Animations/AnimUtils.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);
	
	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}
//--------------------------------------------------------------------------------------------------------------------------

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr; 
	for(auto Weapon:Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty(); 

	Super::EndPlay(EndPlayReason);
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::SpawnWeapons()
{

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;


	for (auto OneWeaponData : WeaponData) 
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBase_Weapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnEmptyClip.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character); 
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocket);
	}
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::AttachWeaponToSocket(ASTUBase_Weapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);

}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) 
{
	if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid Weapon Index"));
				 return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{  
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocket);
		EquipAnimProgress = true;
	}

	CurrentWeapon = Weapons[WeaponIndex]; 

	const auto CurrentWeaponData = WeaponData.FilterByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass();});
	CurrentRealoadAnimMontage = CurrentWeaponData.Num() > 0 ? CurrentWeaponData[0].ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

	PlayAnimMontage(EquipAnimMontage);
}
//--------------------------------------------------------------------------------------------------------------------------

void USTUWeaponComponent::StartFire()
{
	if (!CanFire()) return; 
	CurrentWeapon->StartFire();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishNotify = AnimUtils::FindNotifyByClass <USTU_Reload_Finished_AnimNotify> (OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}

		ReloadFinishNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}

}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	EquipAnimProgress = false;
	
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	ReloadAnimProgress = false;

}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::CanFire() const 
{
	return CurrentWeapon && !EquipAnimProgress && !ReloadAnimProgress; 
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::CanEquip() const 
{
	
	return !EquipAnimProgress && !ReloadAnimProgress;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::CanReaload() const 
{
	return CurrentWeapon && !EquipAnimProgress && !ReloadAnimProgress && CurrentWeapon->CanReload(); 
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::Reload()
{
	ChangeClip();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::OnEmptyClip(ASTUBase_Weapon* AmmoEmptyWeapon)
{
	if(CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else 
	{
		for (const auto Weapon : Weapons)
		{
			if(Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
			
		}
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::ChangeClip()
{
	if (!CanReaload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimProgress = true;
	PlayAnimMontage(CurrentRealoadAnimMontage);
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if(CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBase_Weapon> WeaponType, int32 ClipsAmount)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBase_Weapon> WeaponType)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
