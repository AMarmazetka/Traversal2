// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "TraversalGameplayTags.h"
#include "GA_Mantle.generated.h"


/**
 * 
 */
UCLASS()
class TRAVERSAL_API UGA_Mantle : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Mantle();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "MantleAnim")
	TObjectPtr<UAnimMontage> MantlHeight;
	UPROPERTY(EditDefaultsOnly, Category = "MantleAnim")
	TObjectPtr<UAnimMontage> MantleLow;
	UPROPERTY(EditDefaultsOnly, Category = "MantleAnim")
	float SeparatorHeight = 160;

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UFUNCTION()
	void OnAnimCompleted();

	UFUNCTION()
	void OnAnimInterrupted();
};
