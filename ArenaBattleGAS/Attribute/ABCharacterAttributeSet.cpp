// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterAttributeSet.h"

#include "ArenaBattleGAS.h"
#include "GameplayEffectExtension.h"
#include "Tag/ABGameplayTag.h"

UABCharacterAttributeSet::UABCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.0f),
	AttackRate(30.0f),
	MaxAttackRadius(150.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Damage(0.f)
{
	InitHealth(GetMaxHealth());
}

void UABCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UABCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	//여기서 데미지가 들어왔는데,
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f) 
		{
			// 타겟으로 삼는 놈이 무적태그를 쥐고 있으면?
			if (Data.Target.HasMatchingGameplayTag(ABTAG_Character_INVINSIBLE))
			{
				//무력화 시킨다.
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;
}

void UABCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinHealth = 0.f;
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(),  MinHealth, GetMaxHealth()));
		SetDamage(0);
	}

	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		//수동으로 태그를 넣어준다.
		Data.Target.AddLooseGameplayTag(ABTAG_Character_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}

/*
void UABCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}
*/
