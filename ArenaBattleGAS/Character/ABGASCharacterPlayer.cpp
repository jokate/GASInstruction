// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/ABGASPlayerState.h"


// Sets default values
AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
	ASC = nullptr;
}

// Called when the game starts or when spawned
void AABGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AABGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AABGASPlayerState* GASPS = GetPlayerState<AABGASPlayerState>();

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);
		
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}
		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AABGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();
}

void AABGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 1);
	}
}

void AABGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);	
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);	
		}
	}
}


