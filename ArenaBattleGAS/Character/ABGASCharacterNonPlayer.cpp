// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASCharacterNonPlayer.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "UI/ABGASWidgetComponent.h"


// Sets default values
AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>("AttributeSet");
	
	HpBar = CreateDefaultSubobject<UABGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	Level = 1.0f;
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	//여기에 이펙트를 적용하는 코드를 넣어야 한다.
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	//SpecHandle이 생성이 된다.
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);

	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AABGASCharacterNonPlayer::OnOutOfHealth()
{
	SetDead();
}

