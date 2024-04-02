// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/ABCharacterPlayer.h"
#include "ABGASCharacterPlayer.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASCharacterPlayer : public AABCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABGASCharacterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
protected:
	void SetupGASInputComponent();

	void GASInputPressed(int32 InputId);

	void GASInputReleased(int32 InputId);
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;
};
