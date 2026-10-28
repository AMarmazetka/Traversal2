// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAttributeSet.h"
#include "GameplayEffectExtension.h"


UCombatAttributeSet::UCombatAttributeSet() {
	InitHealth(100.f);
	InitMaxHealth(100.f);

	InitMana(100.f);
	InitMaxMana(100.f);

	InitFireResistance(0.f);
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[CombatAttributeSet] Callback called. Attribute: %s"),
		*Data.EvaluatedData.Attribute.GetName()
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[CombatAttributeSet] IncomingDamage attribute: %s"),
		*GetIncomingDamageAttribute().GetName()
	);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		if (Damage <= 0.0f) {
			UE_LOG(LogTemp, Warning, TEXT("[CombatAttributeSet] Damage: %.1f | Health: %.1f / %.1f"), Damage, GetHealth(), GetMaxHealth());
			return;
		}

		const float NewHealth = FMath::Clamp(GetHealth() - Damage, 0.0f, GetMaxHealth());
		SetHealth(NewHealth);
		UE_LOG(LogTemp, Warning,TEXT("[CombatAttributeSet] Damage: %.1f | Health: %.1f / %.1f"), Damage, GetHealth(), GetMaxHealth());
	}
}