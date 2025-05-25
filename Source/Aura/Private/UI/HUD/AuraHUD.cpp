// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

AAuraHUD::AAuraHUD()
{
	TSubclassOf<UUserWidget> ConfirmBoxClassDefault =
		StaticLoadClass(UUserWidget::StaticClass(), nullptr, 
		  TEXT("/Game/Blueprints/UI/Overlay/SubWidgets/WBP_ConfirmBox.WBP_ConfirmBox_C"));
	TSubclassOf<UUserWidget> ConfirmBoxClassNormal =
		StaticLoadClass(UUserWidget::StaticClass(), nullptr, 
		TEXT("/Game/Blueprints/UI/Overlay/SubWidgets/WBP_ConfirmBox1.WBP_ConfirmBox1_C"));
	// 检查类是否成功加载
	if (!ConfirmBoxClassDefault)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load ConfirmBoxClassDefault!"));
	}
	
	if (!ConfirmBoxClassNormal)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load ConfirmBoxClassNormal!"));
	}
	ConfirmBoxClassArray.Emplace(ConfirmBoxClassDefault);
	ConfirmBoxClassArray.Emplace(ConfirmBoxClassNormal);
}

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

UComfirmBox* AAuraHUD::ShowConfirmBox(const FString& Content, EConfirmBox_Type Type)
{
	if (ConfirmBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfirmBox is already initialized"));
		return nullptr;
	}
	checkf(!ConfirmBoxClassArray.IsEmpty(), TEXT("ConfirmBox Class Uninitialized, please fill out BP_AuraHud"));
	const TSubclassOf<UUserWidget> ConfirmBoxClass = ConfirmBoxClassArray[static_cast<uint8>(Type)];
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ConfirmBoxClass);
	ConfirmBox = Cast<UComfirmBox>(Widget);
	if (ConfirmBox)
	{
    
		// 设置输入模式为UI独占
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			// 创建UI独占输入模式实例
			FInputModeUIOnly InputMode;
			//  
			// // 可选：指定焦点控件（如确认框的根Widget）
			InputMode.SetWidgetToFocus(ConfirmBox->TakeWidget());
    
			// 配置鼠标行为
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 允许鼠标自由移动
    
			// 应用输入模式
			PC->SetInputMode(InputMode);
		}
		ConfirmBox->SetContent(Content);
		ConfirmBox->AddToViewport(999);
	}
	return ConfirmBox;
}

void AAuraHUD::RemoveConfirmBox()
{
	if (ConfirmBox)
	{
		ConfirmBox->RemoveFromParent();
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameAndUI());
		}
		ConfirmBox = nullptr;
	}	
}



void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}
