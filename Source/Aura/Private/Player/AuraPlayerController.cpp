// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
		return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 *	Line trace from cursor. There are several scenarios:
	 *		A. LastActor && ThisActor Both Null
	 *			Nothing To do
	 *		B. LastActor is Null But ThisActor is Valid
	 *			HighLight ThisActor
	 *		C. LastActor is Valid && ThisActor is Null
	 *			UnHighLight LastActor
	 *		D. Both Valid but LastActor != ThisActor
	 *			UnHighLight LastActor && HighLight ThisActor
	 *		E. Both Valid && Same
	 *			Nothing To do 
	 * 
	 * 
	*/
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//B
			ThisActor->HighLightActor();
		}
		else
		{
			//A
		}	
	}
	else
	{
		if (ThisActor == nullptr)
		{
			//C
			LastActor->UnHighLightActor();
		}
		else
		{
			if (ThisActor != LastActor)
			{
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
			else
			{
				//E
			}
		}
	}
}

void AAuraPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*InputTag.ToString());
}

void AAuraPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Blue,*InputTag.ToString());
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::White,*InputTag.ToString());
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext,0);	
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Look);
	AuraInputComponent->BindAbilityAction(InputConfig, this, &ThisClass::AbilityInputPressed,&ThisClass::AbilityInputReleased,&ThisClass::AbilityInputHeld);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector =InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Type::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Type::Y);
	B_Forward = InputAxisVector.Y >= 0;

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		double CalcValueForward = InputAxisVector.Y;
		double CalcValueRight = InputAxisVector.X;
		if(!B_Forward)
		{
			constexpr double BackRate = 0.48f;
			CalcValueForward *= BackRate;
			CalcValueRight *= BackRate;
		}
		ControlledPawn->AddMovementInput(ForwardDirection, CalcValueForward);
		ControlledPawn->AddMovementInput(RightDirection, CalcValueRight);
	}
}

void AAuraPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector =InputActionValue.Get<FVector2D>();
	if (GetPawn<APawn>())
	{
		AddPitchInput(InputAxisVector.Y);
		AddYawInput(InputAxisVector.X);
	}
}


