/*

Copyright © 2019 Mashmashu Studio.
All rights reserved.

*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"
#include "Engine/LatentActionManager.h"
#include "LatentActions.h"
#include "EasyPopupFunctionLibrary.h"


class IInputProvider;

//The Action:
class FUserInputWaitAction : public FPendingLatentAction
{

public:

	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget; 
	EUserInput *Result_ptr;
	int32 *Input;
	int32 UUID;

	bool bAborted;

	FUserInputWaitAction(int32 &input, const FLatentActionInfo& LatentInfo, EUserInput &UserInput)
	{
		bAborted = false;
		ExecutionFunction = LatentInfo.ExecutionFunction;
		CallbackTarget = LatentInfo.CallbackTarget.Get();
		OutputLink = LatentInfo.Linkage;
		UUID = LatentInfo.UUID;
		Result_ptr = &UserInput;
		*Result_ptr = EUserInput::Waiting;
		Input = &input;
	}

	~FUserInputWaitAction()
	{
		UEasyPopupFunctionLibrary::CleanUpAction(this);
	}

	void ReceiveUserInput(int32 input, EUserInput UserInput)
	{
		*Input = input;
		*Result_ptr = UserInput;
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if(bAborted)
		{
			*Result_ptr = EUserInput::OnAborted;
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		}	
		else
		{
			if (*Result_ptr == EUserInput::Waiting)
			{
				Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
			}
			else
			{
				Response.FinishAndTriggerIf(*Result_ptr != EUserInput::Waiting, ExecutionFunction, OutputLink, CallbackTarget);
			}
		}
	}

	virtual void NotifyActionAborted() override
	{
		bAborted = true;
		//Call - Cleanup
		UEasyPopupFunctionLibrary::CleanUpAction(this);
	}

	/*
	// Lets the latent action know that the object which originated it has been garbage collected
	// and the action is going to be destroyed (no more UpdateOperation calls will occur and
	// CallbackTarget is already NULL)
	// This is only called when the object goes away before the action is finished; perform normal
	// cleanup when responding that the action is completed in UpdateOperation
	virtual void NotifyObjectDestroyed() override
	{
		//Associated object is destroyed!!!
		//Nothing to do so far
	}
	*/

};