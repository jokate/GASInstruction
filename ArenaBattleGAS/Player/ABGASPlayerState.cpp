// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASPlayerState.h"

#include "AbilitySystemComponent.h"

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
