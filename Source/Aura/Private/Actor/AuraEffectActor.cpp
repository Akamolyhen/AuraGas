// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, bool InfinityRemove)
{
	// Way 1
	/*if (const IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(Target))
	{
		UAbilitySystemComponent* TargetAsc = AscInterface->GetAbilitySystemComponent();
	}*/
	// Way 2
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAsc == nullptr)
		return;
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAsc->MakeOutgoingSpec(
		GameplayEffectClass, 1.f, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	if (EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite &&
		InfinityRemove)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetAsc);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for(auto& GameplayEffect : AppliesGamePlayEffects)
	{
		switch (GameplayEffect.InstantEffectApplicationPolicy)
		{
		case EEffectApplicationPolicy::ApplyOnOverlap:
			ApplyEffectToTarget(TargetActor, GameplayEffect.InstantGameplayEffectClass);
			break;
		default:
			break;
		}
		switch (GameplayEffect.DurationEffectApplicationPolicy)
		{
		case EEffectApplicationPolicy::ApplyOnOverlap:
			ApplyEffectToTarget(TargetActor, GameplayEffect.DurationGameplayEffectClass);
			break;
		default:
			break;
		}
		switch (GameplayEffect.InfinityEffectApplicationPolicy)
		{
		case EEffectApplicationPolicy::ApplyOnOverlap:
			ApplyEffectToTarget(TargetActor, GameplayEffect.InfinityGameplayEffectClass,
			                    GameplayEffect.InfinityEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap);
			break;
		default:
			break;
		}		
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for(auto& GameplayEffect : AppliesGamePlayEffects)
	{
		switch (GameplayEffect.InstantEffectApplicationPolicy)
		{
		case EEffectApplicationPolicy::ApplyOnEndOverlap:
			ApplyEffectToTarget(TargetActor, GameplayEffect.InstantGameplayEffectClass);
			break;
		default:
			break;
		}
		switch (GameplayEffect.DurationEffectApplicationPolicy)
		{
		case EEffectApplicationPolicy::ApplyOnEndOverlap:
			ApplyEffectToTarget(TargetActor, GameplayEffect.DurationGameplayEffectClass);
			break;
		default:
			break;
		}
		switch (GameplayEffect.InfinityEffectApplicationPolicy)
		{
		case EEffectApplicationPolicy::ApplyOnEndOverlap:
			ApplyEffectToTarget(TargetActor, GameplayEffect.InfinityGameplayEffectClass);
			break;
		default:
			break;
		}
		switch (GameplayEffect.InfinityEffectRemovalPolicy)
		{
		case EEffectRemovalPolicy::RemoveOnEndOverlap:
			{
				UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
				if(!IsValid(TargetAsc)) return;
				TArray<FActiveGameplayEffectHandle> HandlesRemove;
				for(auto HandlePair : ActiveEffectHandles)
				{
					if (TargetAsc == HandlePair.Value)
					{
						TargetAsc->RemoveActiveGameplayEffect(HandlePair.Key,1);
						HandlesRemove.Add(HandlePair.Key);
					}
				}
				for(auto& Handle : HandlesRemove)
				{
					ActiveEffectHandles.FindAndRemoveChecked(Handle);
				}
				break;			
			}
		default:
			break;
		}	
	}
}

