// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "TraversalGameplayTags.h"
#include "GA_Vault.generated.h"

/**
 * 
 */
UCLASS()
class TRAVERSAL_API UGA_Vault : public UGameplayAbility
{
	GENERATED_BODY()
	
public: 
	UGA_Vault();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "VaultAnim")
	TObjectPtr<UAnimMontage> VaultSlow;
	UPROPERTY(EditDefaultsOnly, Category = "VaultAnim")
	TObjectPtr<UAnimMontage> VaultSpeed;
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UFUNCTION()
	void OnAnimCompleted();

	UFUNCTION()
	void OnAnimInterrupted();

};
