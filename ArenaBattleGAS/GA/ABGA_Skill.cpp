// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGA_Skill.h"
#include "Character/ABGASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
UABGA_Skill::UABGA_Skill()
{
}

void UABGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 해당 어빌리티가 수행되면 타겟으로 두는 캐릭터에게 Montage를 플레이 해줘야 한다.
	AABGASCharacterPlayer* TargetCharacterPlayer = CastChecked<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if (!TargetCharacterPlayer)
	{
		return;
	}
	
	ActiveSkillActionMontage = TargetCharacterPlayer->GetSkillActionMontage();

	if (!ActiveSkillActionMontage)
	{
		return;
	}
	// 움직임을 봉쇄해야 한다. (스킬이 재생되는 경우에는 없애주겠다는 것.)
	TargetCharacterPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);

	PlayMontageTask->OnCompleted.AddDynamic(this, &UABGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UABGA_Skill::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();
}


void UABGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//종료시에는 당연한 것이겠지만 움직임을 활성화 시켜야 한다.
	AABGASCharacterPlayer* TargetCharacterPlayer = CastChecked<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (TargetCharacterPlayer)
	{
		// 움직임을 복원한다. 
		TargetCharacterPlayer->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UABGA_Skill::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_Skill::OnInterruptedCallback()
{	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
