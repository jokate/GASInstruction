﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterSkillAttributeSet.h"

UABCharacterSkillAttributeSet::UABCharacterSkillAttributeSet():
	SkillRange(800.0f),
	MaxSkillRange(1200.0f),
	SkillRate(150.0f),
	MaxSkillRate(300.0f),
	SkillEnergy(100.0f),
	MaxSkillEnergy(100.0f)
{
}

void UABCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetSkillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if (Attribute == GetSkillRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSkillRate());
	}
}

