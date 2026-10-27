// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Fireball.generated.h"

/**
 * 
 */

class AFireballProjectile;
UCLASS()
class TRAVERSAL_API UGA_Fireball : public UGameplayAbility
{
	GENERATED_BODY()



public:
	UGA_Fireball();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	TSubclassOf<AFireballProjectile> ProjectileClass;
};
