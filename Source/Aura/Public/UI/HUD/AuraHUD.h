// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/ComfirmBox.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;
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
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	AAuraHUD();
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WcParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WcParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* Asc, UAttributeSet* As);

	UFUNCTION(BlueprintCallable)
	UComfirmBox* ShowConfirmBox(const FString& Content, EConfirmBox_Type Type = EConfirmBox_Type::ConfirmBox_Type_Default);

	UFUNCTION(BlueprintCallable)
	void RemoveConfirmBox();
	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController; 

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UComfirmBox> ConfirmBox;
	UPROPERTY(VisibleAnywhere, Category="ConfirmBox")
    TArray< TSubclassOf<UUserWidget>> ConfirmBoxClassArray;
};
