// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGA_Attack.h"

#include "ArenaBattleGAS.h"
#include "Character/ABCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ABComboActionData.h"

UABGA_Attack::UABGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	CurrentComboData = ABCharacter->GetComboActionData();
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage(), 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

void UABGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UABGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	CurrentCombo = 0;
	CurrentComboData = nullptr;
	HasNextComboInput = false;
}

void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	//Timer가 유효하지 않은 경우 (즉 마지막까지 도달한 경우에는 추가적으로 동작할 이유는 없음.
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}
}

void UABGA_Attack::OnCompleteCallback()
{
	//명시적으로 Ability가 끝났음을 알림
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UABGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UABGA_Attack::StartComboTimer()
{
	//배열에 따라서 현재 콤보에 대해서 - 1
	int32 ComboIndex = CurrentCombo - 1;
	//혹시 제대로 된 Index인지에 대한 검증
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	//입력까지 기다리는 시간
	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;

	//대기 시간이 있는 경우에는 Timer을 돌려서 제대로 된 Input이 있는지를 검증한다.
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UABGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UABGA_Attack::CheckComboInput()
{
	//제한 시간이 되었다면 Timer에 대해서 Invalidate를 하고
	ComboTimerHandle.Invalidate();

	//만약 이전에 입력이 들어온 경우에는 다음으로 넘어가준다.=
	if (HasNextComboInput)
	{
		//근데 이 함수는 좀 생소하긴 한데 몽타쥬를 점프시키는 함수
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
