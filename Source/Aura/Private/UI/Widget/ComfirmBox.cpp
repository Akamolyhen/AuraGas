// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ComfirmBox.h"

#include "Components/CanvasPanel.h"


void UComfirmBox::SetContent(const FString& Content)
{
	ConfirmContent->SetText(FText::FromString(Content));
}

void UComfirmBox::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("UComfirmBox::NativeConstruct"));
	Super::NativeConstruct();
	if (ConfirmButton && !ConfirmButton->OnClicked.IsBound())
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UComfirmBox::OnConfirmClicked);
	}
	if (CancelButton && !CancelButton->OnClicked.IsBound())
	{
		CancelButton->OnClicked.AddDynamic(this, &UComfirmBox::OnCancelClicked);
	}
	if (ConfirmIcon && IconTexture.IsValid())
	{
		ConfirmIcon->SetBrushFromTexture(IconTexture.Get());
	}
	else if (ConfirmIcon && IconTexture.IsPending())
	{
		// 同步加载（仅在蓝图显式调用时使用）
		if (UTexture2D* Icon = IconTexture.LoadSynchronous())
		{
			ConfirmIcon->SetBrushFromTexture(Icon);
		}
	}
}

void UComfirmBox::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT("UComfirmBox::NativeDestruct"));
	EventDispatcher_OnConfirmEvent.Clear();
	EventDispatcher_OnCancelEvent.Clear();
	Super::NativeDestruct();
}

void UComfirmBox::CallCancelEvent()
{
	EventDispatcher_OnCancelEvent.Broadcast();
}

void UComfirmBox::OnConfirmClicked()
{
	EventDispatcher_OnConfirmEvent.Broadcast();
}

void UComfirmBox::OnCancelClicked()
{
	EventDispatcher_OnCancelEvent.Broadcast();
}
