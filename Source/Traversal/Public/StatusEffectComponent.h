// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "CombatAttributeSet.h"
#include "StatusEffectComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAVERSAL_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectComponent();
	void InitializeWithAbilitySystem(UAbilitySystemComponent* InACS);
	void SetBurnSourceASC(UAbilitySystemComponent* InSourceASC);
	void TryApplyBurning();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OnBurnBuildupChanged(const FOnAttributeChangeData& Data);

	TWeakObjectPtr <UAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UAbilitySystemComponent> BurnSourceASC;


	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> CombatAttributeSet;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Status Effects")
	TSubclassOf<UGameplayEffect> BurningEffect;

		
};
