// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAttributeSet.h"

UCombatAttributeSet::UCombatAttributeSet() {
	InitHealth(100.f);
	InitMaxHealth(100.f);

	InitMana(100.f);
	InitMaxMana(100.f);

	InitFireResistance(0.f);
}