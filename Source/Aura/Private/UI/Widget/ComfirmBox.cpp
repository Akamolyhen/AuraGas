// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ComfirmBox.h"

#include "Components/CanvasPanel.h"
#include "UI/HUD/AuraHUD.h"


void UComfirmBox::SetContent(const FString& Content)
{
	ConfirmIcon->SetBrushFromTexture(IconsArray[0]);
	ConfirmContent->SetText(FText::FromString(Content));
	ConfirmContent->SetAutoWrapText(true);
}

void UComfirmBox::NativeConstruct()
{
	Super::NativeConstruct();
	if (ConfirmButton && !ConfirmButton->OnClicked.IsBound())
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UComfirmBox::OnConfirmClicked);
	}
	if (CancelButton && !CancelButton->OnClicked.IsBound())
	{
		CancelButton->OnClicked.AddDynamic(this, &UComfirmBox::OnCancelClicked);
	}
}

void UComfirmBox::NativeDestruct()
{
	EventDispatcher_OnConfirmEvent.Clear();
	EventDispatcher_OnCancelEvent.Clear();
	Super::NativeDestruct();
}

void UComfirmBox::CallCancelEvent()
{
	RemoveFromParent();
	EventDispatcher_OnCancelEvent.ExecuteIfBound();
}

void UComfirmBox::OnConfirmClicked()
{
	RemoveFromParent();
	EventDispatcher_OnConfirmEvent.ExecuteIfBound();
}

void UComfirmBox::OnCancelClicked()
{
	RemoveFromParent();
	EventDispatcher_OnCancelEvent.ExecuteIfBound();
}
