// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tag/ABGameplayTag.h"

void UABGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		//ASC에서 특정 어트리뷰트 값을 받아와서 업데이팅을 해주면 편함.
		//다만 이거를 Attribute에 대한 이벤트를 가져오는 것이 굉장히 편하기 때문에...
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnMaxHealthChanged);
		ASC->RegisterGameplayTagEvent(ABTAG_Character_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UABGASHpBarUserWidget::OnInvinsibleTagChanged);
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		
		//델리게이트 바인딩 후에, 현재 Attribute에 대해서 업데이팅을 해줘야 한다.
		const UABCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UABCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			//그래서 현재 값을 가져옴.
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UABGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	//값에 대한 변동이 일어난 경우 ChangeData에 NewValue로 넘어온다.
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	//실제로 InvincibleTag가 추가가 되었다고 한다면?
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvinsibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		//만약에 Tag가 빠진 경우 (0이면 없어진 것이니까)
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
} 

void UABGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
