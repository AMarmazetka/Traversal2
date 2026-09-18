// Fill out your copyright notice in the Description page of Project Settings.

#include "TraversalComponent.h"
#include "TraversalObjects.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TraversalGameplayTags.h"



// Sets default values for this component's properties
UTraversalComponent::UTraversalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	// ...
}


// Called when the game starts
void UTraversalComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* Owner = GetOwner()) {
		if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner)) {
			AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
			GiveAbilities();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("[TraversalComponent] Error Class hasn't GAS component"));
		}
	
	}

	// ...
	
}


// Called every frame
void UTraversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UTraversalComponent::GiveAbilities() {
	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MantleAbility, 1, -1));
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(VaultAbility, 1, -1));
	}
}

void UTraversalComponent::FindTriversalObject() {
	//float Height = 0;
	const FVector StartPoint = GetOwner()->GetActorLocation();
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();
	const FVector EndPoint = StartPoint + ForwardVector * DistanceInputAction;

	FHitResult HitRes;
	FCollisionQueryParams IgnorCharacter;
	IgnorCharacter.AddIgnoredActor(GetOwner());
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

float UTraversalComponent::FindHeightTargetActor(FHitResult HitRes) {
	AActor* HitActor = HitRes.GetActor();
	if (HitActor->Implements<UTraversalObjects>()) {
		//== HeightPoint
		FVector TargetLocation = HitActor->GetActorLocation();
		const FVector ForwardVector = GetOwner()->GetActorForwardVector();
		const FVector StartPoint = HitRes.ImpactPoint + ForwardVector * 15.0f;
		const FVector EndPointToHeight = StartPoint + (FVector::UpVector * 200.0f);
		FHitResult HitResHeight;
		bool bHitH = GetWorld()->LineTraceSingleByChannel(HitResHeight, EndPointToHeight, StartPoint, ECC_Visibility);
		if (!bHitH) {
			UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] HitErrrUp"));
			return 0;
		}
		const FVector HeightPoint = HitResHeight.ImpactPoint;

		//==LowPoint
		// I think about two Methods: legs of Player and low point in TraversalObjects
		const FVector EndPointToLow = StartPoint + (FVector::DownVector * 200.0f);
		FHitResult HitResLow;
		bool bHitL = GetWorld()->LineTraceSingleByChannel(HitResLow, EndPointToLow, StartPoint, ECC_Visibility);
		if (!bHitL) {
			UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] HitErrrDown"));
			return 0;
		}
		const FVector LowPoint = HitResLow.ImpactPoint;
		const float HeightObject = FVector::Distance(HeightPoint, LowPoint);
		UE_LOG(LogTemp, Warning, TEXT("HeightObject = %f"), HeightObject);

		return HeightObject;
	}
	return 0;
}

float UTraversalComponent::FindDepthTargetActor(FHitResult HitRes) {
	AActor* HitActor = HitRes.GetActor();
	if (HitActor->Implements<UTraversalObjects>()) {
		FVector ForwardVector = GetOwner()->GetActorForwardVector();
		ForwardVector.Z = 0.f;
		const FVector NewForwardVector = ForwardVector.GetSafeNormal();
		const FVector EndPoint = HitRes.ImpactPoint;
		const FVector StartPoint = EndPoint + (NewForwardVector * 100.0f);
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

void UTraversalComponent::Vaulting() {
	if (!AbilitySystemComponent) {
		UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] Error AbilitySystemComponent "));
		return;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("[TraversalComponent] VaultAbility = %s"),
		*GetNameSafe(VaultAbility));

	UE_LOG(LogTemp, Warning,
		TEXT("[TraversalComponent] MantleAbility = %s"),
		*GetNameSafe(MantleAbility));
	AbilitySystemComponent->TryActivateAbilityByClass(VaultAbility);
}

void UTraversalComponent::Mantling() {
	if (!AbilitySystemComponent) {
		UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] Error AbilitySystemComponent "));
		return;
	}
	FGameplayEventData EventData;
	EventData.EventMagnitude = Height;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), TAG_Ability_Traversal_Mantle, EventData);
	//AbilitySystemComponent->TryActivateAbilityByClass(MantleAbility);
}

void UTraversalComponent::Traversal() {
	Height = 0;
	Depth = 0;
		FindTriversalObject();
		if (Height > 0 || Depth > 0) {
			if (Height > MaxHeightVaulting && Height < MaxHeightMantling)
			{
				Mantling();
				return;
			}
			if (Depth > 0 && Depth <= MaxDepthVaulting && Height < MaxHeightVaulting) {
				Vaulting();
				return;
			}
			if (Height < MaxHeightVaulting && Depth == 0) {
				Mantling();
				return;
			}
			if (Height > MaxHeightMantling) {
				UE_LOG(LogTemp, Warning, TEXT("[TraversalCharacter] VeryHeight"));
				return;
			}
			return;
		}
}