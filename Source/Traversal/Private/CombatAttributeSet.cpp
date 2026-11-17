// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAttributeSet.h"
#include "AtackGameplayTags.h"
#include "BaseEnemy.h"
#include "GameplayEffectExtension.h"


UCombatAttributeSet::UCombatAttributeSet() {
	InitHealth(100.f);
	InitMaxHealth(100.f);

	InitMana(100.f);
	InitMaxMana(100.f);

	InitFireResistance(50.f);
	InitDamageBonus(0.0f);
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCombatAttributeSet] CurrentMana: %.1f / %.1f"), GetMana(), GetMaxMana());
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		if (Damage <= 0.0f) {
			return;
		}
		float FinalDamage = Damage;

		const float NewHealth = FMath::Clamp(GetHealth() - FinalDamage, 0.0f, GetMaxHealth());
		SetHealth(NewHealth);

		UE_LOG(LogTemp, Warning,TEXT("[CombatAttributeSet] Damage: %.1f | Health: %.1f / %.1f | ResistanceFire: %.1f"), FinalDamage, GetHealth(), GetMaxHealth() , GetFireResistance());

		if (GetHealth() <= 0.0f) {
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetOwningActor())) {
				Enemy->Die();
			}
		}
	}
	else {
		return;
	}
}

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}