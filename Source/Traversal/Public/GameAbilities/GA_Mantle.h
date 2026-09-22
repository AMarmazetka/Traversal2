// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TraversalDataAsset.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSoftObjectPtr<UTraversalDataAsset> Settings;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MantlHeight;
	UPROPERTY()
	TObjectPtr<UAnimMontage> MantleLow;
	UPROPERTY()
	float SeparatorHeight = 160;

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UFUNCTION()
	void OnAnimCompleted();

	UFUNCTION()
	void OnAnimInterrupted();

	void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void OnSettingLoaded();
};
