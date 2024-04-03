// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
