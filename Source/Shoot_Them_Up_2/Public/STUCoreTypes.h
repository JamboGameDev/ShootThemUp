#pragma once

#include "STUCoreTypes.generated.h"

// Weapon

class ASTUBase_Weapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnclipEmptySignature, ASTUBase_Weapon*);

//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};
//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUBase_Weapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;

};
//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UTexture2D* MainIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UTexture2D* CrossHair;

	

};
//--------------------------------------------------------------------------------------------------------------------------
// Health
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);
//--------------------------------------------------------------------------------------------------------------------------

// VFX
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FVector Size = FVector(10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float FadeOutTime = 0.7f;

};
//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FDecalData DecalData;

};

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
	int32 PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RoundsNum = 4;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "300"))
	int32 RoundTime = 10; //in second

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DefaultTeamColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FLinearColor> TeamColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
	int32 RespawnTime = 5; //in second
	
	
};