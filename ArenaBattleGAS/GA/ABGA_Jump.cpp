// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGA_Jump.h"

#include "AT/ABAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UABGA_Jump::UABGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*UABAT_JumpAndWaitForLanding* JumpAndWaitForLanding = UABAT_JumpAndWaitForLanding::CreateTask(this);
	JumpAndWaitForLanding->OnComplete.AddDynamic(this, &UABGA_Jump::OnLandedCallback);
	JumpAndWaitForLanding->ReadyForActivation();*/
}

void UABGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}


bool UABGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;	
	}

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	return (Character && Character->CanJump());
}

void UABGA_Jump::OnLandedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
