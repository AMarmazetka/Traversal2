// Fill out your copyright notice in the Description page of Project Settings.
#include "GameAbilities/GA_Mantle.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_Mantle::UGA_Mantle()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(TAG_Ability_Traversal_Mantle);
	SetAssetTags(AssetTags);

	ActivationOwnedTags.AddTag(TAG_State_Traversal_Mantle);
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_Ability_Traversal_Mantle;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	BlockAbilitiesWithTag.AddTag(TAG_State_Traversal_Mantle);
	BlockAbilitiesWithTag.AddTag(TAG_State_Traversal_Vault);
	ActivationBlockedTags.AddTag(TAG_State_Traversal_Mantle);
	ActivationBlockedTags.AddTag(TAG_State_Traversal_Vault);
	AbilityTriggers.Add(TriggerData);


}

void UGA_Mantle::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
	Super::OnAvatarSet(ActorInfo, Spec);
	if (Settings.IsNull()) {
		return;
	}
	FStreamableManager& Stream = UAssetManager::GetStreamableManager();
	Stream.RequestAsyncLoad(Settings.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UGA_Mantle::OnSettingLoaded));
}

void UGA_Mantle::OnSettingLoaded() {
	if (Settings.IsValid()) { 
		UTraversalDataAsset* Setting = Settings.Get();
		SeparatorHeight = Setting->MaxHeightVaulting;
		MantlHeight = Setting->MantlHeight;
		MantleLow = Setting-> MantleLow;
	}
}

void UGA_Mantle::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		UE_LOG(LogTemp, Warning, TEXT("[UGA_Mantle] Error Commit"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character || !MantlHeight || !MantleLow)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UGA_Mantle] Error Cast"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAnimMontage* MontagePlay = MantlHeight;
	float Height;
	if (TriggerEventData) {
		Height = TriggerEventData->EventMagnitude;
	}

	if (Height >= SeparatorHeight) {
		MontagePlay = MantlHeight;
	}
	if (Height < SeparatorHeight) {
		MontagePlay = MantleLow;
	}
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	UAbilityTask_PlayMontageAndWait* AnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontagePlay, 1.0f, NAME_None, true, 1.0f, 0.0f);
	
	AnimTask->OnCompleted.AddDynamic(this, &UGA_Mantle::OnAnimCompleted);
	AnimTask->OnInterrupted.AddDynamic(this, &UGA_Mantle::OnAnimInterrupted);
	AnimTask->OnCancelled.AddDynamic(this, &UGA_Mantle::OnAnimInterrupted);
	AnimTask->ReadyForActivation();

}

void UGA_Mantle::OnAnimCompleted()
{
	if (Character) {
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Mantle::OnAnimInterrupted()
{
	if (Character) {
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}