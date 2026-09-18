// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"
#include "CoreMinimal.h"

/**
 * 
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Traversal_Mantle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Traversal_Vault);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Traversal_Mantle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Traversal_Vault);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Traversal_InTraversal);

class TRAVERSAL_API TraversalGameplayTags
{
public:
	TraversalGameplayTags();
	~TraversalGameplayTags();
};
