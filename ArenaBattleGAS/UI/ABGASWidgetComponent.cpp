// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASWidgetComponent.h"
#include "UI/ABGASUserWidget.h"

// Sets default values for this component's properties
UABGASWidgetComponent::UABGASWidgetComponent()
{

}

void UABGASWidgetComponent::InitWidget()
{
    //위젯이 생성되어 들어왔을 시, 관런 GAS 위젯일 경우, 실행하도록 추가.
	Super::InitWidget();

	UABGASUserWidget* GASUserWidget = Cast<UABGASUserWidget>(GetWidget());

	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}


