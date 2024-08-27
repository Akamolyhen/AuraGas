// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

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

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
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
		InfinityEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetAsc);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	switch (InstantEffectApplicationPolicy)
	{
		case EEffectApplicationPolicy::ApplyOnOverlap:
			ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
			break;
		default:
			break;
	}
	switch (DurationEffectApplicationPolicy)
	{
		case EEffectApplicationPolicy::ApplyOnOverlap:
			ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
			break;
		default:
			break;
	}
	switch (InfinityEffectApplicationPolicy)
	{
	case EEffectApplicationPolicy::ApplyOnOverlap:
		ApplyEffectToTarget(TargetActor, InfinityGameplayEffectClass);
		break;
	default:
		break;
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	switch (InstantEffectApplicationPolicy)
	{
		case EEffectApplicationPolicy::ApplyOnEndOverlap:
			ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
			break;
		default:
			break;
	}
	switch (DurationEffectApplicationPolicy)
	{
		case EEffectApplicationPolicy::ApplyOnEndOverlap:
			ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
			break;
		default:
			break;
	}
	switch (InfinityEffectApplicationPolicy)
	{
	case EEffectApplicationPolicy::ApplyOnEndOverlap:
		ApplyEffectToTarget(TargetActor, InfinityGameplayEffectClass);
		break;
	default:
		break;
	}
	switch (InfinityEffectRemovalPolicy)
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

