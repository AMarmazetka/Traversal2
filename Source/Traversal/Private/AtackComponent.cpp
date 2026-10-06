// Fill out your copyright notice in the Description page of Project Settings.


#include "AtackComponent.h"

// Sets default values for this component's properties
UAtackComponent::UAtackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAtackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner()) {
		if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner)) {
			AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("[AtackComponent] Error Class hasn't GAS component"));
		}
	}
	// ...
	
}


// Called every frame
void UAtackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAtackComponent::Atack() {
	UE_LOG(LogTemp, Error, TEXT("[AtackComponent] Test"));
	if (AbilitySystemComponent) {
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(FireBall, 1, -1));
		AbilitySystemComponent->TryActivateAbilityByClass(FireBall);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[AtackComponent] AbilitySystemComponent isn't inizialize"));
	}
}
