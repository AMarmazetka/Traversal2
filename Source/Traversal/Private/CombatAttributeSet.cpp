// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAttributeSet.h"
#include "AtackGameplayTags.h"
#include "GameplayEffectExtension.h"


UCombatAttributeSet::UCombatAttributeSet() {
	InitHealth(100.f);
	InitMaxHealth(100.f);

	InitMana(100.f);
	InitMaxMana(100.f);

	InitFireResistance(50.f);
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		if (Damage <= 0.0f) {
			UE_LOG(LogTemp, Warning, TEXT("[CombatAttributeSet] Damage: %.1f | Health: %.1f / %.1f"), Damage, GetHealth(), GetMaxHealth());
			return;
		}
		float FinalDamage = Damage;
		FGameplayTagContainer EffectTags;
		Data.EffectSpec.GetAllAssetTags(EffectTags);
		if (EffectTags.HasTag(TAG_Damage_Fire)) {
			const float FireResistance2 = FMath::Clamp(GetFireResistance(), 0.0f, 100.0f);
			FinalDamage = FinalDamage * (1.0f - FireResistance2 / 100.0f);
		}
		const float NewHealth = FMath::Clamp(GetHealth() - FinalDamage, 0.0f, GetMaxHealth());
		SetHealth(NewHealth);
		UE_LOG(LogTemp, Warning,TEXT("[CombatAttributeSet] Damage: %.1f | Health: %.1f / %.1f | ResistanceFire: %.1f"), FinalDamage, GetHealth(), GetMaxHealth() , GetFireResistance());
	}
	else {
		return;
	}
}