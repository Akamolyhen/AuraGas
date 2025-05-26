// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WcParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WcParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WcParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WcParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* Asc, UAttributeSet* As)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitialized, please fill out BP_AuraHud"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Uninitialized, please fill out BP_AuraHud"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	const FWidgetControllerParams WidgetControllerParams(PC,PS,Asc,As);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);
	//Bind Dynamic Delegate TO Broadcast
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

void AAuraHUD::SetMenuOpen(bool MenuOpen)
{
	if (MenuOpen)
	{
		SetInputMode_UIOnly();
	}
	else
	{
		SetInputMode_GameAndUI();
	}
}


void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraHUD::SetInputMode_UIOnly() const
{
	// 创建UI独占输入模式实例
	FInputModeUIOnly InputMode;
	// 配置鼠标行为
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 允许鼠标自由移动
	// 应用输入模式
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}

void AAuraHUD::SetInputMode_GameAndUI() const
{
	GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI());
}
