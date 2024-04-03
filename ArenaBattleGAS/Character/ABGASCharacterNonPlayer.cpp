// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASCharacterNonPlayer.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"


// Sets default values
AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}

