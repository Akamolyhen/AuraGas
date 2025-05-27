// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ComfirmBox.generated.h"


DECLARE_DYNAMIC_DELEGATE(FOnConfirmEvent);
DECLARE_DYNAMIC_DELEGATE(FOnCancelEvent);
/**
 * 
 */
UCLASS()
class AURA_API UComfirmBox : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnConfirmEvent EventDispatcher_OnConfirmEvent;

	FOnCancelEvent EventDispatcher_OnCancelEvent;
	UFUNCTION(BlueprintCallable)
	void SetContent(const FString& Content);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


	
	UFUNCTION(BlueprintCallable)
	void CallCancelEvent();

	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D*> IconsArray;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ConfirmIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConfirmContent;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UFUNCTION()
	void OnConfirmClicked();
	UFUNCTION()
	void OnCancelClicked();

	
};
