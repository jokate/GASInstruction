// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"


UABGA_Rotate::UABGA_Rotate()
{
	AbilityTags.AddTag(ABTAG_ACTOR_ROTATE);
	ActivationOwnedTags.AddTag(ABTAG_STATE_ISROTATING);
}

void UABGA_Rotate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	AActor* Avatar = ActorInfo->AvatarActor.Get();

	if (Avatar)
	{
		URotatingMovementComponent* RotatingMovementComponent = Cast<URotatingMovementComponent>(Avatar->GetComponentByClass(URotatingMovementComponent::StaticClass()));

		if (RotatingMovementComponent)
		{
			RotatingMovementComponent->Deactivate();
		}
	}
}

void UABGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = ActorInfo->AvatarActor.Get();

	if (Avatar)
	{
		URotatingMovementComponent* RotatingMovementComponent = Cast<URotatingMovementComponent>(Avatar->GetComponentByClass(URotatingMovementComponent::StaticClass()));

		if (RotatingMovementComponent)
		{
			RotatingMovementComponent->Activate(true);
		}
	}
}
