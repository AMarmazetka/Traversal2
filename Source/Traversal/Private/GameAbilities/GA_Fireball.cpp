// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/GA_Fireball.h"

void UGA_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		UE_LOG(LogTemp, Warning, TEXT("[UGA_FireBall] Error Commit"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGA_Fireball] Action"));
	EndAbility(Handle, ActorInfo,ActivationInfo,false,false);
	return;
}

