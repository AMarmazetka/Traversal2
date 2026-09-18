// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "TraversalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAVERSAL_API UTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTraversalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> VaultAbility;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> MantleAbility;

	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float DistanceInputAction = 250;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float DistanceActivateAbility = 150;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float MaxHeightVaulting = 160;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float MaxDepthVaulting = 30;
	UPROPERTY(EditDefaultsOnly, Category = "ParametrsTraversal")
	float MaxHeightMantling = 260;

	float Height = 0;
	float Depth = 0;


	void GiveAbilities();
	void FindTriversalObject();
	float FindHeightTargetActor(FHitResult HitResult);
	float FindDepthTargetActor(FHitResult HitResult);
	void Traversal();


	UFUNCTION(BlueprintCallable)
	void Vaulting();
	UFUNCTION(BlueprintCallable)
	void Mantling();
};
