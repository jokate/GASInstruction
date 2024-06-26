﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/ABGASPlayerState.h"
#include "UI/ABGASWidgetComponent.h"
#include "UI/ABGASUserWidget.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Tag/ABGameplayTag.h"

// Sets default values
AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
	ASC = nullptr;

	HpBar = CreateDefaultSubobject<UABGASWidgetComponent>("HpBar");
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hellhammer/SK_Blunt_HellHammer.SK_Blunt_HellHammer'"));

	if (WeaponMeshRef.Object)
	{
		WeaponMesh = WeaponMeshRef.Object;
	}
	
	WeaponRange = 75.0f;
	WeaponDamage = 100.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_SkillAttack.AM_SkillAttack'"));

	if (SkillActionMontageRef.Object)
	{
		SkillActionMontage = SkillActionMontageRef.Object;
	}
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

		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AABGASCharacterPlayer::EquipWeapon);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AABGASCharacterPlayer::UnEquipWeapon);
		const UABCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UABCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		}
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
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 2);
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

void AABGASCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}

void AABGASCharacterPlayer::UnEquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(nullptr);

		//탈착하는 것.
		FGameplayAbilitySpec* SkillAbilitySpec = ASC->FindAbilitySpecFromClass(SkillAbilityClass);

		if (SkillAbilitySpec)
		{
			ASC->ClearAbility(SkillAbilitySpec->Handle);
		}
		
		//여기서 어트리뷰트를 바꿀 거임.
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponDamage);
	}
}

void AABGASCharacterPlayer::EquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(WeaponMesh);

		FGameplayAbilitySpec SkillAbilitySpec(SkillAbilityClass);
		SkillAbilitySpec.InputID = 2;

		//현재 어빌리티가 부여되어있지 않은 경우에?
		if (!ASC->FindAbilitySpecFromClass(SkillAbilityClass))
		{
			ASC->GiveAbility(SkillAbilitySpec);
		}
		
		//여기서 어트리뷰트를 바꿀 거임.
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange + WeaponRange);
		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate + WeaponDamage);
	}
}
