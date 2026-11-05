// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "ProjectileAbilityDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TRAVERSAL_API UProjectileAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<UGameplayEffect> DamageEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float Damage = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float ProjectileSpeed = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float ProjectileLifetime = 5.0f;
};
