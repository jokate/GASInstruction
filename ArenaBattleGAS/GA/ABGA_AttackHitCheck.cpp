// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGA_AttackHitCheck.h"

#include "ArenaBattleGAS.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AT/ABAT_Trace.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "TA/ABTA_Trace.h"
#include "Tag/ABGameplayTag.h"

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	ABGAS_LOG(LogABGAS, Log, TEXT("%s"), *TriggerEventData->EventTag.GetTagName().ToString());
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;
	
	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, TargetActorClass);

	AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UABGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	//제대로 받았는지에 관해서 로그를 찍어볼 것임.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected"), *HitResult.GetActor()->GetName());
		
		
		//현재 SourceActor의 ASC를 가져온다.
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		if (!SourceASC || !TargetASC)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("ASC NOT FOUND"))
			return;
		}

		const UABCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UABCharacterAttributeSet>();
		/*UABCharacterAttributeSet* TargetAttribute = const_cast<UABCharacterAttributeSet*>(TargetASC->GetSet<UABCharacterAttributeSet>());

		if (!SourceAttribute || !TargetAttribute)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("ASC NOT FOUND"))
			return;
		}

		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);*/

		//이제는 이펙트를 이용해서 호출할 예정임.
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		
		if (EffectSpecHandle.IsValid())
		{
			//상대방에게 해당 이펙트를 발현할 수 있음.
			//EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			//이펙트에서 Cue를 가지고 넣어주기는 할 건데..
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}

		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);

		if (BuffEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		//여기서는 실제적으로 Data의 값이 Actors로 들어온다는 사실을 알아야 함.
		//사실상 왠만치 거의 똑같다 보면 된다.
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}
	}
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
