// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "TraversalDataAsset.h"
#include "MotionWarpingComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSoftObjectPtr<UTraversalDataAsset> Settings;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TSubclassOf<UGameplayAbility> VaultAbility;
	UPROPERTY()
	TSubclassOf<UGameplayAbility> MantleAbility;
	UPROPERTY()
	TObjectPtr <UMotionWarpingComponent> MotionWarpingComponent;
	UPROPERTY()
	float DistanceInputAction = 250;
	UPROPERTY()
	float DistanceActivateAbility = 50;
	UPROPERTY()
	float MaxHeightVaulting = 160;
	UPROPERTY()
	float MaxDepthVaulting = 30;
	UPROPERTY()
	float MaxHeightMantling = 260;

	float Height = 0;
	float Depth = 0;
	FVector WarpLocation;
	FRotator WarpRotation;



	void GiveAbilities();
	void FindTriversalObject();
	float FindHeightTargetActor(FHitResult HitResult);
	float FindDepthTargetActor(FHitResult HitResult);
	void Traversal();
	void OnSettingLoaded();
	void SetAbilitiesAndComponents();


	UFUNCTION(BlueprintCallable)
	void Vaulting();
	UFUNCTION(BlueprintCallable)
	void Mantling();
	UFUNCTION(BlueprintCallable)
	void MotionWarping(ETravelType TypeAnimation, const FVector& StartPoint, const FRotator& StartRotation); //InFutureCHange EnumType
	UFUNCTION(BlueprintCallable)
	void SetStartPosition(UPrimitiveComponent* Component);
};
