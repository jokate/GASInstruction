// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASItemBox.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/ABCollision.h"

// Sets default values
AABGASItemBox::AABGASItemBox()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));

	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);		
	}

	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AABGASItemBox::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASItemBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	InvokeGameplayCue(OtherActor);
	ApplyEffectToTarget(OtherActor);
	
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	SetLifeSpan(2.0f);
}

void AABGASItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this, this);
}

void AABGASItemBox::ApplyEffectToTarget(AActor* Target)
{
	//Target에다가 영향을 줘볼 것임.
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);

		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			/*
			* FActiveGameplayEffectHandle UAbilitySystemComponent::BP_ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> GameplayEffectClass, UAbilitySystemComponent* Target, float Level, FGameplayEffectContextHandle Context)
			  {
				if (Target == nullptr)
				{
					ABILITY_LOG(Log, TEXT("UAbilitySystemComponent::BP_ApplyGameplayEffectToTarget called with null Target. %s. Context: %s"), *GetFullName(), *Context.ToString());
					return FActiveGameplayEffectHandle();
				}

				if (GameplayEffectClass == nullptr)
				{
					ABILITY_LOG(Error, TEXT("UAbilitySystemComponent::BP_ApplyGameplayEffectToTarget called with null GameplayEffectClass. %s. Context: %s"), *GetFullName(), *Context.ToString());
					return FActiveGameplayEffectHandle();
				}

				UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
				실제적으로 결국 나오는 것을 확인이 가능함.
				return ApplyGameplayEffectToTarget(GameplayEffect, Target, Level, Context);	
				}
			 */
		}
	}
}

void AABGASItemBox::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}
