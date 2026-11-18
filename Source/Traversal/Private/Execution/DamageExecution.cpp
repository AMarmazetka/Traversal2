// Fill out your copyright notice in the Description page of Project Settings.

#include "Execution/DamageExecution.h"
#include "CombatAttributeSet.h"
#include "GameplayEffect.h"
#include "AtackGameplayTags.h"
#include "GameplayEffectExtension.h"



namespace DamageStatics {
	struct FDamageStatics {
		FGameplayEffectAttributeCaptureDefinition FireDamageBonusDef;
		FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
		FDamageStatics() {
			FireDamageBonusDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetFireDamageBonusAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
			FireResistanceDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetFireResistanceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		}
	};

	static const FDamageStatics& Get() {
		static FDamageStatics Statics;
		return Statics;
	}
}

UDamageExecution::UDamageExecution() {
	RelevantAttributesToCapture.Add(DamageStatics::Get().FireDamageBonusDef);
	RelevantAttributesToCapture.Add(DamageStatics::Get().FireResistanceDef);
}

void UDamageExecution::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const float BaseDamage = Spec.GetSetByCallerMagnitude(TAG_Data_Damage, false, 0.0f);
    FAggregatorEvaluateParameters EvaluationParameters;

    float FireDamageBonus = 0.3f;
    float FireResistance = 50.0f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics::Get().FireDamageBonusDef, EvaluationParameters, FireDamageBonus);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics::Get().FireResistanceDef, EvaluationParameters, FireResistance);

	const float BonusMultiplier = 1.0f + FireDamageBonus;
	const float ResistanceMultiplier = 1.0f - FMath::Clamp(FireResistance, 0.0f, 100.0f) / 100;
	const float FinalDamage = BaseDamage * BonusMultiplier * ResistanceMultiplier;



	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCombatAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage));


    UE_LOG( LogTemp, Warning, TEXT("[FireDamageExecution] DamageBonus: %f | FireResistance: %f"), FireDamageBonus, FireResistance );
}