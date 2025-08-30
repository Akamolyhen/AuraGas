// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include <iostream>

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;//运动将被限制在一个平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;//当前为true and 平面约束 -> 组件在首次附加时捕捉到平面

	//禁止将控制器的Rotation应用到Character上
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	std::cout << "Hello World!" << std::endl;

	// 检查当前编译环境是否符合条件
	#if defined(_MSC_VER)
		std::cout << "检测到MSVC编译器，_MSC_VER = " << _MSC_VER << std::endl;
	
	#if defined(__clang__)
		std::cout << "注意：当前使用的是Clang模拟MSVC模式" << std::endl;
	#else
		std::cout << "当前使用的是原生MSVC模式" << std::endl;
	
	#if _MSC_VER < 1941
		std::cout << "当前环境不满足要求：MSVC版本低于14.41" << std::endl;
	#else
		std::cout << "当前环境满足要求：MSVC版本 >= 14.41" << std::endl;
	#endif
	#endif
	#else
		std::cout << "未检测到MSVC编译器" << std::endl;
	#endif
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	//确保 OwnerActor 是bind在 Controlled playerState 上的
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHud = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHud->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
