/*

Copyright © 2019 Mashmashu Studio.
All rights reserved.

*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "EasyPopupFunctionLibrary.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EUserInput : uint8
{
	//User selected a valid input
	OnUserResponse	UMETA(DisplayName = "OnUserResponse"),

	//user canceled selection
	OnAborted		UMETA(DisplayName = "OnAborted"),

	//Still waiting for user input
	Waiting			UMETA(DisplayName = "Waiting"),
};

class FUserInputWaitAction;

UCLASS()
class EASYPOPUPS_API UEasyPopupFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	//Latent Function that will wait for user Input, InputProvider should be the object that is responsible for the Input.
	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "UserInput"), Category = "EasyPopups|InputConsumer", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo"))
	static void WaitForUserInput(UObject* WorldContextObject, UObject* InputProvider, int32& input, FLatentActionInfo LatentInfo, EUserInput& UserInput);
	
	//You MUST call this function even in case of user aborted it (To clean up references TMap).
	UFUNCTION(BlueprintCallable, Category = "EasyPopups|Inputprovider")
	static void SetUserInput(UObject* InputProvider, int32 input, EUserInput UserInput);

	//Cleans up all allocated resources and latent action.
	UFUNCTION(BlueprintCallable, Category = "EasyPopups|InputConsumer", meta = (WorldContext = "WorldContextObject"))
	static void RemoveAllAssociatedActions(UObject* WorldContextObject);

	//Internal Cleanup when Action is aborted or died.
	static void CleanUpAction(FUserInputWaitAction* action);

	//Internal Function to cleanup resources when provider is about to be deallocated.
	static void CleanUpInputProvider(UObject* inputProvider);

};
