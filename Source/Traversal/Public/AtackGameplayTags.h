// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"
#include "CoreMinimal.h"

/**
 * 
 */


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Magic_FireBall);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Magic_IceBall);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Combat_Sword);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Attacking);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Burning);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Fire);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Fireball);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Damage_Fire);


class TRAVERSAL_API AtackGameplayTags
{
public:
	AtackGameplayTags();
	~AtackGameplayTags();
};



/**
 *
 */



