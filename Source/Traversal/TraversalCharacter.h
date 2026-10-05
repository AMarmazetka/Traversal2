// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "TraversalComponent.h"
#include "MotionWarpingComponent.h"
#include "CombatAttributeSet.h"
#include "AtackComponent.h"
#include "TraversalCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATraversalCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TraversalAction;




	


	

public:
	ATraversalCharacter();
	virtual void BeginPlay() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Travers(const FInputActionValue& Value);


protected:

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMotionWarpingComponent* GetMotionWarpingComponent() const;
	void OnSettingLoaded();
	

public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TraversalComponent")
	TObjectPtr<UTraversalComponent> TraversalComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr <UMotionWarpingComponent> MotionWarpingComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AtackComponent")
	TObjectPtr<UCombatAttributeSet> AttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AtackComponent")
	TObjectPtr<UAtackComponent> AtackComponent;


	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


};

