// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "StatusEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "CombatAttributeSet.h"
#include "AtackComponent.h"
#include "BaseEnemy.generated.h"

class UGameplayEffect;

UCLASS()
class TRAVERSAL_API ABaseEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION()
	void Die();
	UFUNCTION()
	bool IsDead();



public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAtackComponent> AtackComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatusEffectComponent> StatusEffectComponent;

	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> AttributeSet;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Burning")
	TSubclassOf<UGameplayEffect> BurningEffect;

protected:
	bool bIsDead = false;

};
