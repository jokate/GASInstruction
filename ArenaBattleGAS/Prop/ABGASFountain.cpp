// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASFountain.h"
#include "AbilitySystemComponent.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameplayAbilitySpec.h"
#include "Tag/ABGameplayTag.h"
#include "Abilities/GameplayAbility.h"

// Sets default values
AABGASFountain::AABGASFountain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComponent");
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovementComponent->bAutoActivate = false;
	RotatingMovementComponent->Deactivate();

	ASC->InitAbilityActorInfo(this, this);

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

// Called when the game starts or when spawned
void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));
	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	if (!ASC->HasMatchingGameplayTag(ABTAG_STATE_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}
}

