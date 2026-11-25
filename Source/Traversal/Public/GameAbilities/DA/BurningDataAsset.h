// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "BurningDataAsset.generated.h"

UCLASS(BlueprintType)
class TRAVERSAL_API UBurningDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Burning")
    TSubclassOf<UGameplayEffect> BurningEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Burning")
    float BaseBurnDuration = 6.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Burning")
    float BurnDamage = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Burning")
    float BurnBuildupThreshold = 100.0f;
};