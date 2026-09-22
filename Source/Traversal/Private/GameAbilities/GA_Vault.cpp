// Fill out your copyright notice in the Description page of Project Settings.

#include "GameAbilities/GA_Vault.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_Vault::UGA_Vault()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(TAG_Ability_Traversal_Vault);
	SetAssetTags(AssetTags);

	ActivationOwnedTags.AddTag(TAG_State_Traversal_Vault);
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_Ability_Traversal_Vault;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	BlockAbilitiesWithTag.AddTag(TAG_State_Traversal_Mantle);
	BlockAbilitiesWithTag.AddTag(TAG_State_Traversal_Vault);
	ActivationBlockedTags.AddTag(TAG_State_Traversal_Mantle);
	ActivationBlockedTags.AddTag(TAG_State_Traversal_Vault);

}

void UGA_Vault::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
	Super::OnAvatarSet(ActorInfo, Spec);
	if (Settings.IsNull()) {
		return;
	}
	FStreamableManager& Stream = UAssetManager::GetStreamableManager();
	Stream.RequestAsyncLoad(Settings.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UGA_Vault::OnSettingLoaded));
}

void UGA_Vault::OnSettingLoaded() {
	if (Settings.IsValid()) { 
		UTraversalDataAsset* Setting = Settings.Get();
		VaultSlow = Setting->VaultSlow;
		VaultSpeed = Setting->VaultSpeed;
	}
}

void UGA_Vault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		UE_LOG(LogTemp, Warning, TEXT("[UGA_Vault] Error Commit"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character || !VaultSlow || !VaultSpeed)
	{

		UE_LOG(LogTemp, Warning, TEXT("[UGA_Vault] Error Cast"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UAnimMontage* MontagePlay = VaultSlow;
	float Speed = Character->GetCharacterMovement()->Velocity.Size();
	if (Speed <= 495) {
		MontagePlay = VaultSlow;
		UE_LOG(LogTemp, Warning, TEXT("[UGA_Vault] SlowWalkSpeed %f"), Speed);
	}
	if (Speed > 495) {
		MontagePlay = VaultSpeed;
		UE_LOG(LogTemp, Warning, TEXT("[UGA_Vault] SpeedWalkSpeed %f"), Speed);
	}
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		UAbilityTask_PlayMontageAndWait* AnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontagePlay, 1.0f, NAME_None, true, 1.0f, 0.0f);

		AnimTask->OnCompleted.AddDynamic(this, &UGA_Vault::OnAnimCompleted);
		AnimTask->OnInterrupted.AddDynamic(this, &UGA_Vault::OnAnimInterrupted);
		AnimTask->OnCancelled.AddDynamic(this, &UGA_Vault::OnAnimInterrupted);

		AnimTask->ReadyForActivation();
		
	


}

void UGA_Vault::OnAnimCompleted()
{
	if (Character) {
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Vault::OnAnimInterrupted()
{
	if (Character) {
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}