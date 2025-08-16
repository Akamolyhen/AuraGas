/*

Copyright � 2019 Mashmashu Studio.
All rights reserved.

*/


#include "EasyPopupFunctionLibrary.h"
#include "EasyPopups.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "UserInputWaitAction.h"



static TMap<UObject*, FLatentActionInfo> g_actionsMap;


void UEasyPopupFunctionLibrary::WaitForUserInput(UObject* WorldContextObject, UObject* InputProvider, int32& input, FLatentActionInfo LatentInfo, EUserInput& UserInput)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (IsValid(World) && IsValid(InputProvider))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FUserInputWaitAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			g_actionsMap.Emplace(InputProvider, LatentInfo);
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FUserInputWaitAction(input, LatentInfo, UserInput));
		}
	}
}

void UEasyPopupFunctionLibrary::SetUserInput(UObject* InputProvider, int32 input, EUserInput UserInput)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(InputProvider);
	if (IsValid(World) && IsValid(InputProvider))
	{
		struct FLatentActionInfo* latentInfo = g_actionsMap.Find(InputProvider);
		if (latentInfo)
		{
			FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
			FUserInputWaitAction* myAction = LatentActionManager.FindExistingAction<FUserInputWaitAction>(latentInfo->CallbackTarget, latentInfo->UUID);
			if(myAction)
			{
				myAction->ReceiveUserInput(input, UserInput);
			}
			g_actionsMap.Remove(InputProvider);
		}
	}
}

void UEasyPopupFunctionLibrary::RemoveAllAssociatedActions(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (IsValid(World))
	{
		UEasyPopupFunctionLibrary::CleanUpInputProvider(WorldContextObject); //In case the worldContextObject is actually an input provider


		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		LatentActionManager.RemoveActionsForObject(WorldContextObject); //Should automatically call the Action's d'tor that will clean up the map.
	}
}

void UEasyPopupFunctionLibrary::CleanUpAction(FUserInputWaitAction* action)
{
	UObject* keyToremoveFromMap = nullptr;
	for (auto& Elem : g_actionsMap)
	{
		const struct FLatentActionInfo& val = Elem.Value;
		int32 UUID = val.UUID;
		if( UUID == action->UUID )
		{
			keyToremoveFromMap = Elem.Key;
			break;
		}
	}
	if(keyToremoveFromMap != nullptr)
	{
		g_actionsMap.Remove(keyToremoveFromMap);
	}
}

void UEasyPopupFunctionLibrary::CleanUpInputProvider(UObject* inputProvider)
{
	UEasyPopupFunctionLibrary::SetUserInput(inputProvider, -1, EUserInput::OnAborted);
}