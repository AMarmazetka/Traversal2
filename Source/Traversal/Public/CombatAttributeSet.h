// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CombatAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class TRAVERSAL_API UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxMana);


	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData BurnBuildup;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, BurnBuildup)

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, IncomingDamage);


	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData FireDamageBonus;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, FireDamageBonus)

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData PhysicalDamageBonus;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, PhysicalDamageBonus)




	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultMaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultMana = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultMaxMana = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultFireResistance = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultFireDamageBonus = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Initial Attributes")
	float DefaultPhysicalDamageBonus = 0.0f;

	UCombatAttributeSet();
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void InitializeAttributes();
};
