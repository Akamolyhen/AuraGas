// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ConfirmBox.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFrameLibrary.generated.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EConfirmBox_Type : uint8
{
	ConfirmBox_Type_Default,
	ConfirmBox_Type_Normal
};
UCLASS()
class AURA_API UUIFrameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI", meta = (WorldContext = "WorldContextObject"))
	static UConfirmBox* CreateMessageBox(UObject* WorldContextObject, FOnConfirmEvent Event_Confirm, FOnCancelEvent Event_Cancel, const FString& Content,
	                                                    EConfirmBox_Type Type =
		                                                    EConfirmBox_Type::ConfirmBox_Type_Default);

	static void InitConfirmBoxTypes();
	static TArray<TSubclassOf<UConfirmBox>> ConfirmBoxTypes;
};
