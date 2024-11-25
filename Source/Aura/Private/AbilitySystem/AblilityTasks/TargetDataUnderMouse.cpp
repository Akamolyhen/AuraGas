// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AblilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey)
		                      .AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		if (const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle, ActivationPredictionKey); !bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}

}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	APlayerController* Pc = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	Pc->GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
