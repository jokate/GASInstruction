// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABSkillDamageExecutionCalc.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Attribute/ABCharacterSkillAttributeSet.h"

void UABSkillDamageExecutionCalc::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//내부정보는 ExecutionParm에 결과는 outExecutionOutput에 들어간다.
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceActor && TargetActor)
		{
			//거리에 따른 계산.
			const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UABCharacterSkillAttributeSet::GetSkillRangeAttribute());
			//대미지를 가져온다.
			const float MaxDamage = SourceASC->GetNumericAttributeBase(UABCharacterSkillAttributeSet::GetSkillRateAttribute());
			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);
			const float InvDamageRatio = 1.0f - Distance / MaxDamageRange;
			float Damage = InvDamageRatio * MaxDamage;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UABCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}
