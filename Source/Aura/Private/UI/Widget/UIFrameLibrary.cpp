// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UIFrameLibrary.h"

#include "EasyPopupFunctionLibrary.h"
#include "UI/Widget/ConfirmBox.h"



TArray<TSubclassOf<UConfirmBox>> UUIFrameLibrary::ConfirmBoxTypes;
UConfirmBox* UUIFrameLibrary::CreateMessageBox(UObject* WorldContextObject, FOnConfirmEvent Event_Confirm,
	FOnCancelEvent Event_Cancel, const FString& Content, EConfirmBox_Type Type)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Error, TEXT("WorldContextObject is nullptr!"));
		return nullptr;
	}
	if (ConfirmBoxTypes.IsEmpty())
	{
		InitConfirmBoxTypes();
	}
	const TSubclassOf<UConfirmBox> ConfirmBoxClass = ConfirmBoxTypes[static_cast<uint8>(Type)];
	UWorld* World = WorldContextObject->GetWorld();
	UConfirmBox* MessageBox = CreateWidget<UConfirmBox>(World, ConfirmBoxClass);
	if (World)
	{
		MessageBox->EventDispatcher_OnConfirmEvent = Event_Confirm;
		MessageBox->EventDispatcher_OnCancelEvent = Event_Cancel;
		MessageBox->SetContent(Content);
		MessageBox->AddToViewport(99);
	}
	return MessageBox;
}

UConfirmBox* UUIFrameLibrary::CreateWaitMessageBox(UObject* WorldContextObject, const FString& Content, int32& Input,
	FLatentActionInfo LatentInfo, EUserInput& UserInput, EConfirmBox_Type Type)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Error, TEXT("WorldContextObject is nullptr!"));
		return nullptr;
	}
	if (ConfirmBoxTypes.IsEmpty())
	{
		InitConfirmBoxTypes();
	}
	const TSubclassOf<UConfirmBox> ConfirmBoxClass = ConfirmBoxTypes[static_cast<uint8>(Type)];
	UWorld* World = WorldContextObject->GetWorld();
	UConfirmBox* MessageBox = CreateWidget<UConfirmBox>(World, ConfirmBoxClass);
	if (World)
	{
		MessageBox->SetContent(Content);
		MessageBox->AddToViewport(99);
	}
	UEasyPopupFunctionLibrary::WaitForUserInput(WorldContextObject,MessageBox,Input,LatentInfo,UserInput);
	return MessageBox;
}


void UUIFrameLibrary::InitConfirmBoxTypes()
{
	TSubclassOf<UUserWidget> ConfirmBoxClassDefault =
	StaticLoadClass(UConfirmBox::StaticClass(), nullptr, 
	  TEXT("/Game/Blueprints/UI/Overlay/SubWidgets/WBP_ConfirmBox.WBP_ConfirmBox_C"));
	TSubclassOf<UUserWidget> ConfirmBoxClassNormal =
		StaticLoadClass(UConfirmBox::StaticClass(), nullptr, 
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
	ConfirmBoxTypes.Emplace(ConfirmBoxClassDefault);
	ConfirmBoxTypes.Emplace(ConfirmBoxClassNormal);
}
