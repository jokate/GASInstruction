// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABGASUserWidget.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "ABGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASHpBarUserWidget : public UABGASUserWidget
{
	GENERATED_BODY()
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHpBar();

protected:
	//현재 제공하고 있는 UI의 이름에 맞게 변수로 맞춰주면 관리에 편하기에 다음과 같이 선언.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;
	
	float CurrentHealth = 0.f;

	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;

	FLinearColor InvinsibleColor = FLinearColor::Blue;
};