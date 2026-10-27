// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/GA_Fireball.h"
#include "AbilitySystemComponent.h"
#include "Projectile/FireballProjectile.h"

UGA_Fireball::UGA_Fireball(){
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGA_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!ProjectileClass) {
		UE_LOG(LogTemp,Error,TEXT("[GA_Fireball] ProjectileClass is not assigned"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		UE_LOG(LogTemp, Warning, TEXT("[GA_FireBall] Error Commit"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	
	const FVector SpawnLocation = AvatarActor->GetActorLocation() + AvatarActor->GetActorForwardVector() * 100.0f + FVector(0.0f, 0.0f, 50.0f);
	const FRotator SpawnRotation = AvatarActor->GetActorRotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = AvatarActor;
	SpawnParameters.Instigator = Cast<APawn>(AvatarActor);
	AFireballProjectile* Projectile = AvatarActor->GetWorld()->SpawnActor<AFireballProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);

	if (!Projectile) {
		UE_LOG(LogTemp, Error, TEXT("[GA_Fireball] Failed to spawn projectile"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[GA_Fireball] Fireball spawned"));
	EndAbility(Handle, ActorInfo,ActivationInfo,true,false);
	return;
}

