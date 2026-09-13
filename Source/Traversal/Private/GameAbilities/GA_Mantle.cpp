// Fill out your copyright notice in the Description page of Project Settings.
#include "GameAbilities/GA_Mantle.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_Mantle::UGA_Mantle()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(TAG_Mantle);
	SetAssetTags(AssetTags);

	ActivationOwnedTags.AddTag(TAG_Mantle);
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_Mantle;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
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
	const float Height = TriggerEventData->EventMagnitude;


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