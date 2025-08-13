// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ConfirmBox.h"

#include "Components/CanvasPanel.h"
#include "UI/HUD/AuraHUD.h"


void UConfirmBox::SetContent(const FString& Content)
{
	ConfirmIcon->SetBrushFromTexture(IconsArray[0]);
	ConfirmContent->SetText(FText::FromString(Content));
	ConfirmContent->SetAutoWrapText(true);
}

void UConfirmBox::NativeConstruct()
{
	Super::NativeConstruct();
	if (ConfirmButton && !ConfirmButton->OnClicked.IsBound())
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UConfirmBox::OnConfirmClicked);
	}
	if (CancelButton && !CancelButton->OnClicked.IsBound())
	{
		CancelButton->OnClicked.AddDynamic(this, &UConfirmBox::OnCancelClicked);
	}
}

void UConfirmBox::NativeDestruct()
{
	EventDispatcher_OnConfirmEvent.Clear();
	EventDispatcher_OnCancelEvent.Clear();
	Super::NativeDestruct();
}

void UConfirmBox::CallCancelEvent()
{
	RemoveFromParent();
	EventDispatcher_OnCancelEvent.ExecuteIfBound();
}

void UConfirmBox::OnConfirmClicked()
{
	RemoveFromParent();
	EventDispatcher_OnConfirmEvent.ExecuteIfBound();
}

void UConfirmBox::OnCancelClicked()
{
	RemoveFromParent();
	EventDispatcher_OnCancelEvent.ExecuteIfBound();
}
