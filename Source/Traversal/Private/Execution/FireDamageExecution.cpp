// Fill out your copyright notice in the Description page of Project Settings.

#include "Execution/FireDamageExecution.h"
#include "CombatAttributeSet.h"
#include "GameplayEffect.h"
#include "AtackGameplayTags.h"
#include "GameplayEffectExtension.h"



namespace FireDamageStatics {
	struct FFireDamageStatics {
		FGameplayEffectAttributeCaptureDefinition DamageBonusDef;
		FGameplayEffectAttributeCaptureDefinition FireResistanceDef;
		FFireDamageStatics() {
			DamageBonusDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetDamageBonusAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
			FireResistanceDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetFireResistanceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		}
	};

	static const FFireDamageStatics& Get() {
		static FFireDamageStatics Statics;
		return Statics;
	}
}

UFireDamageExecution::UFireDamageExecution() {
	RelevantAttributesToCapture.Add(FireDamageStatics::Get().DamageBonusDef);
	RelevantAttributesToCapture.Add(FireDamageStatics::Get().FireResistanceDef);
}

void UFireDamageExecution::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const float BaseDamage = Spec.GetSetByCallerMagnitude(TAG_Data_Damage, false, 0.0f);
    FAggregatorEvaluateParameters EvaluationParameters;

    float DamageBonus = 0.3f;
    float FireResistance = 50.0f;

	const float BonusMultiplier = 1.0f + DamageBonus;
	const float ResistanceMultiplier = 1.0f - FMath::Clamp(FireResistance, 0.0f, 100.0f) / 100;
	const float FinalDamage = BaseDamage * BonusMultiplier * ResistanceMultiplier;

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( FireDamageStatics::Get().DamageBonusDef, EvaluationParameters, DamageBonus );
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( FireDamageStatics::Get().FireResistanceDef, EvaluationParameters, FireResistance );

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCombatAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage));


    UE_LOG( LogTemp, Warning, TEXT("[FireDamageExecution] DamageBonus: %f | FireResistance: %f"), DamageBonus, FireResistance );
}