// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/ABCharacterNonPlayer.h"
#include "ABGASCharacterNonPlayer.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASCharacterNonPlayer : public AABCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABGASCharacterNonPlayer();

protected:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

public:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UABCharacterAttributeSet> AttributeSet;
	
};
