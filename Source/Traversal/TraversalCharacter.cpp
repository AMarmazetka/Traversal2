// Copyright Epic Games, Inc. All Rights Reserved.

#include "TraversalCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TraversalGameplayTags.h"
#include "TraversalObject.h"
#include "TraversalObjects.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATraversalCharacter

ATraversalCharacter::ATraversalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATraversalCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATraversalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATraversalCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATraversalCharacter::Look);

		EnhancedInputComponent->BindAction(TraversalAction, ETriggerEvent::Started, this, &ATraversalCharacter::Travers);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATraversalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATraversalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATraversalCharacter::Travers(const FInputActionValue& Value) {
	Height = 0;
	Depth = 0;
	FindTriversalObject();
	if (Height > 0 || Depth > 0) {
		if (Height > MaxHeightVaulting && Height< MaxHeightMantling)
		{
			Mantling();
			return;
		}
		if (Depth > 0 && Depth <= MaxDepthVaulting && Height < MaxHeightVaulting) {
			Vaulting();
			return;
		}
		if (Height<MaxHeightVaulting && Depth == 0) {
			Mantling();
			return;
		}
		if (Height>MaxHeightMantling) {
			UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] VeryHeight"));
			return;
		}
		return;
	}
}

 void ATraversalCharacter::BeginPlay() {
	 Super::BeginPlay();

	 UE_LOG(LogTemp, Warning,
		 TEXT("=== BEGIN PLAY ==="));

	 if (AbilitySystemComponent) {
		UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] TrytoGiveAbility "));
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MantleAbility, 1, -1));
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(VaultAbility, 1, -1));
	 }
}

UAbilitySystemComponent* ATraversalCharacter::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

void ATraversalCharacter::Vaulting() {

	if (!AbilitySystemComponent) {
		UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] Error AbilitySystemComponent "));
		return;
	}
	AbilitySystemComponent->TryActivateAbilityByClass(VaultAbility);
}

void ATraversalCharacter::Mantling() {
	if (!AbilitySystemComponent) {
		UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] Error AbilitySystemComponent "));
		return;
	}
	FGameplayEventData EventData;
	EventData.EventMagnitude = Height;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_Mantle, EventData);
	//AbilitySystemComponent->TryActivateAbilityByClass(MantleAbility);
}

void ATraversalCharacter::FindTriversalObject() {
	//float Height = 0;
	const FVector StartPoint = GetActorLocation();
	const FVector ForwardVector = GetActorForwardVector();
	const FVector EndPoint = StartPoint + ForwardVector * DistanceInputAction;

	FHitResult HitRes;
	FCollisionQueryParams IgnorCharacter;
	IgnorCharacter.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitRes, StartPoint, EndPoint, ECC_Visibility, IgnorCharacter);

	if (!bHit || !HitRes.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] NothingHit or InvalidActor"));
		return;
	}
	AActor* HitActor = HitRes.GetActor();
	if (HitActor->Implements<UTraversalObjects>()) {
		Height = FindHeightTargetActor(HitRes);	
		Depth = FindDepthTargetActor(HitRes);
	}
}

float ATraversalCharacter::FindHeightTargetActor(FHitResult HitRes) {
	AActor* HitActor = HitRes.GetActor();
	if (HitActor->Implements<UTraversalObjects>()) {
		//== HeightPoint
		FVector TargetLocation = HitActor->GetActorLocation();
		const FVector ForwardVector = GetActorForwardVector();
		const FVector StartPoint = HitRes.ImpactPoint+ForwardVector*15.0f;
		const FVector EndPointToHeight = StartPoint + (FVector::UpVector * 500.0f);
		FHitResult HitResHeight;
		bool bHitH = GetWorld()->LineTraceSingleByChannel(HitResHeight, EndPointToHeight, StartPoint,  ECC_Visibility);
		if (!bHitH) {
			UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] HitErrrUp"));
			return 0;
		}
		const FVector HeightPoint = HitResHeight.ImpactPoint;

		//==LowPoint
		// I think about two Methods: legs of Player and low point in TraversalObjects
		const FVector EndPointToLow = StartPoint + (FVector::DownVector * 500.0f);
		FHitResult HitResLow;
		bool bHitL =  GetWorld()->LineTraceSingleByChannel(HitResLow, EndPointToLow ,StartPoint, ECC_Visibility);
		if (!bHitL) {
			UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] HitErrrDown"));
			return 0;
		}
		const FVector LowPoint = HitResLow.ImpactPoint;
		const float HeightObject=  FVector::Distance(HeightPoint, LowPoint);
		UE_LOG(LogTemp, Warning, TEXT("HeightObject = %f"), HeightObject);

		return HeightObject;
	}
	return 0;
}

float ATraversalCharacter::FindDepthTargetActor(FHitResult HitRes) {
	AActor* HitActor = HitRes.GetActor();
	if (HitActor->Implements<UTraversalObjects>()) {
		FVector ForwardVector = GetActorForwardVector();
		ForwardVector.Z = 0.f;	
		const FVector NewForwardVector= ForwardVector.GetSafeNormal();
		const FVector EndPoint = HitRes.ImpactPoint;
		const FVector StartPoint = EndPoint + (NewForwardVector *100.0f);
		FHitResult HitResDepth;
		bool bHitD = GetWorld()->LineTraceSingleByChannel(HitResDepth, StartPoint, EndPoint, ECC_Visibility);
		if (!bHitD) {
			UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] HitDepthError"));
			return 0;
		}
		const FVector DepthPoint = HitResDepth.ImpactPoint;
		const float DepthObject = FVector::Distance(EndPoint, DepthPoint);
		UE_LOG(LogTemp, Warning, TEXT("DepthObject = %f"), DepthObject);
		return DepthObject;
	}
	return 0;
}
