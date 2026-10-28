// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FireballProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// Sets default values
AFireballProjectile::AFireballProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisisonComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));

	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(12.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);


	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bShouldBounce = false;
	InitialLifeSpan = 5.0f;

	CollisionComponent->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnProjectileHit);
}

// Called when the game starts or when spawned
void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireballProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor && OtherActor != GetOwner()) {
		UE_LOG(LogTemp, Warning, TEXT("[FireballProjectile] Hit: %s"), *GetNameSafe(OtherActor));
	}
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASI) {
		UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent();
		if (TargetASC && DamageEffect) {
			FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandleEff = TargetASC->MakeOutgoingSpec(DamageEffect, 1.0f, EffectContext);
			if (SpecHandleEff.IsValid()) {
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandleEff.Data.Get());
				UE_LOG(LogTemp, Warning, TEXT("[FireballProjectile] Damage on %s"), *GetNameSafe(OtherActor));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("[FireballProjectile] TatrgetASC or DamageEffect not found"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[FireballProjectile] Tatrgetdoes not implement AbilityInterface"));
	}
	Destroy();
}


