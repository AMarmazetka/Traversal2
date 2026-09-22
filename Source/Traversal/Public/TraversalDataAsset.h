// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystemComponent.h"
#include "TraversalDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TRAVERSAL_API UTraversalDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> VaultAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> MantleAbility;

	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float DistanceInputAction = 250;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float DistanceActivateAbility = 150;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float MaxHeightVaulting = 140;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float MaxDepthVaulting = 30;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float MaxHeightMantling = 260;

	UPROPERTY(EditDefaultsOnly, Category = "MantleAnim")
	TObjectPtr<UAnimMontage> MantlHeight;
	UPROPERTY(EditDefaultsOnly, Category = "MantleAnim")
	TObjectPtr<UAnimMontage> MantleLow;
	UPROPERTY(EditDefaultsOnly, Category = "VaultAnim")
	TObjectPtr<UAnimMontage> VaultSlow;
	UPROPERTY(EditDefaultsOnly, Category = "VaultAnim")
	TObjectPtr<UAnimMontage> VaultSpeed;
};
