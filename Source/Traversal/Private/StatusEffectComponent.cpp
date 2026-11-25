// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "AtackGameplayTags.h"

// Sets default values for this component's properties
UStatusEffectComponent::UStatusEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatusEffectComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* InASC) {
	if (!InASC) {
		UE_LOG(LogTemp, Error, TEXT("[StatusEffectComponent] AbilitySystemComponent is null"));
		return;
	}
	AbilitySystemComponent = InASC;
	CombatAttributeSet = const_cast<UCombatAttributeSet*>(InASC->GetSet<UCombatAttributeSet>());
	if (!CombatAttributeSet) {
		UE_LOG(LogTemp, Error, TEXT("[StatusEffectComponent] CombatAttributeSet is null"));
		return;
	}
	InASC->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetBurnBuildupAttribute()).AddUObject(this, &UStatusEffectComponent::OnBurnBuildupChanged);
	UE_LOG(LogTemp, Warning, TEXT("[StatusEffectComponent] Initialized with ASC and AttributeSet"));

}

void UStatusEffectComponent::TryApplyBurning() {
	if (!AbilitySystemComponent.IsValid() || !CombatAttributeSet || !BurningData->BurningEffect)
	{
		return;
	}

	const bool bAlreadyBurning = AbilitySystemComponent->HasMatchingGameplayTag(TAG_Status_Burning);
	if (bAlreadyBurning && !bRefreshBurningDurationOnReapply)
	{
		AbilitySystemComponent->SetNumericAttributeBase(UCombatAttributeSet::GetBurnBuildupAttribute(), 0.0f);
		return;
	}
	const float FireResistence = CombatAttributeSet->GetFireResistance();
	const float IgniteChance = 1.0f - FMath::Clamp(FireResistence, 0.0f, 100.0f) / 100.0f;
	const float RandomValue = FMath::FRand();

	if (RandomValue > IgniteChance) {
		UE_LOG(LogTemp, Warning, TEXT("[StatusEffectComponent] Ignition failed"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[StatusEffectComponent] Ignition successful Ignite chance: %.2f | Random: %.2f"), IgniteChance, RandomValue);


	const float BaseBurnDuration = BurningData->BaseBurnDuration;
	const float BurnDuration = BaseBurnDuration * IgniteChance;
	if (BurnDuration <= 0.0f) {
		return;
	}
	
	
	UAbilitySystemComponent* SourceASC = BurnSourceASC.Get();
	if (!SourceASC) {
		UE_LOG(LogTemp, Error, TEXT("[StatusEffectComponent] Burn source ASC is invalid"));
		return;
	}

	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetOwner());
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(BurningData->BurningEffect, 1.0f, EffectContext);
	
	if (!SpecHandle.IsValid()) {
		return;
	}
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, BurningData->BurnDamage);
	SpecHandle.Data->SetDuration(BurnDuration, true);
	FActiveGameplayEffectHandle BurningHandle =AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	UE_LOG(LogTemp, Warning, TEXT("[StatusEffectComponent] Applying Burning. Duration: %.2f | Source: %s"), BurnDuration, *GetNameSafe(SourceASC->GetAvatarActor()));

	AbilitySystemComponent->SetNumericAttributeBase(UCombatAttributeSet::GetBurnBuildupAttribute(), 0.0f);
	if (BurningHandle.IsValid()) {
		AbilitySystemComponent->SetNumericAttributeBase(UCombatAttributeSet::GetBurnBuildupAttribute(), 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("[StatusEffectComponent] Brning reset to 0"));
	}
}

void UStatusEffectComponent::OnBurnBuildupChanged(const FOnAttributeChangeData& Data) {
	if (!BurningData){
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[StatusEffectComponent] BurnBuildup changed: %.1f -> %.1f"), Data.OldValue, Data.NewValue);
	if (Data.NewValue >= BurningData->BurnBuildupThreshold) {
		TryApplyBurning();
	}
}

void UStatusEffectComponent::SetBurnSourceASC(UAbilitySystemComponent* InSourceASC){
	BurnSourceASC = InSourceASC;
}