// Shoot_Them_UP


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFxComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); 
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);


	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.5f;

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFxComponent>("WeaponFXComponent");

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	//if (!MovementComponent->InitialSpeed) return;

	check(MovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);  
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit); 
	SetLifeSpan(LifeSeconds);

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	//Make Damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                   DamageAmount,
	                                   GetActorLocation(),
	                                   DamageRadius,
	                                   UDamageType::StaticClass(),
	                                  {GetOwner()},
	                                   this,
	                                   GetController(),
	                                   DoFullDamage);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	WeaponFXComponent->PlayImpactFx(Hit);

	Destroy();
}
//--------------------------------------------------------------------------------------------------------------------------
AController* ASTUProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner()); 
	return Pawn ? Pawn->GetController() : nullptr;

}
