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
		FGameplayEffectAttributeCaptureDefinition HealthDef;
		FDamageStatics() {
			FireDamageBonusDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetFireDamageBonusAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
			FireResistanceDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetFireResistanceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
			HealthDef = FGameplayEffectAttributeCaptureDefinition(UCombatAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics::Get().HealthDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float TargetHealth = 0.0f;
	const float BaseDamage = Spec.GetSetByCallerMagnitude(TAG_Data_Damage, false, 0.0f);
    FAggregatorEvaluateParameters EvaluationParameters;

	const bool bHasHealth =ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics::Get().HealthDef,EvaluationParameters,TargetHealth);
	if (!bHasHealth){
		UE_LOG(LogTemp, Error,TEXT("[FireDamageExecution] Failed to capture target Health."));
		return;
	}
	if (TargetHealth <= 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("[FireDamageExecution] Target is dead. Damage ignored."));
		return;
	}

    float FireDamageBonus = 0.0f; //Default 
    float FireResistance = 0.0f;

	const bool bHasFireResistance = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics::Get().FireResistanceDef, EvaluationParameters, FireResistance);
	const bool bHasFireDamageBonus = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics::Get().FireDamageBonusDef, EvaluationParameters, FireDamageBonus);

	//*********************************************************
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[FireDamageExecution] Captured FireDamageBonus: %f"),
		FireDamageBonus
	);
	if (!bHasFireDamageBonus || !bHasFireResistance){
		UE_LOG(LogTemp, Error,TEXT("[FireDamageExecution] Failed to capture combat attributes."));
		return;
	}

	const float BonusMultiplier = 1.0f + FireDamageBonus;
	const float ResistanceMultiplier = 1.0f - FMath::Clamp(FireResistance, 0.0f, 100.0f) / 100;
	const float FinalDamage = BaseDamage * BonusMultiplier * ResistanceMultiplier;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCombatAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage));
    UE_LOG( LogTemp, Warning, TEXT("[FireDamageExecution] DamageBonus: %f | FireResistance: %f"), FireDamageBonus, FireResistance );
}