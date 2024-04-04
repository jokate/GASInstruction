// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UABGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UABGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
