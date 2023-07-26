// Fill out your copyright notice in the Description page of Project Settings.


#include "SparkPlayerController.h"
#include "Components/InputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "SparkPawn.h"

ASparkPlayerController::ASparkPlayerController()
{
	bIsSwiping = false;
	bIsMouseDown = false;

	MinSwipeDistanceThreshold = 50.0f;
	MinSwipeTimeThreshold = 0.2f;

	PrimaryActorTick.bCanEverTick = true;
}

void ASparkPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(this->GetLocalPlayer());
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		// Get the mouse position in the viewport
		FVector2D CurPosition;
		LocalPlayer->ViewportClient->GetMousePosition(CurPosition);
		
		FVector WorldLocation, WorldDirection;
		DeprojectScreenPositionToWorld(CurPosition.X, CurPosition.Y, WorldLocation, WorldDirection);

		SwipeWorldVelocity = (SwipeWorldLocation - WorldLocation) / DeltaTime;
		SwipeWorldLocation = WorldLocation;

		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionParams;

		bool bHit = GetWorld()->LineTraceMultiByChannel(
			HitResults,
			WorldLocation,
			WorldLocation + WorldDirection * SwipeTraceLength,
			ECC_Visibility,
			CollisionParams
		);

		if (bDrawSwipeDebug)
		{
			DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + WorldDirection * SwipeTraceLength, FColor::Green, true, 1000.0f, 1, 1);
		}

		TMap<FString, bool> NewCanSwipe;

		for (auto Result : HitResults) 
		{
			ASparkPawn* Spark = Cast<ASparkPawn>(Result.GetActor());
			if (!Spark || CanSwipe.Contains(Spark->GetName()))
			{
				continue;
			}

			NewCanSwipe.Add(Spark->GetName(), false);
			Spark->ApplySwipe(SwipeWorldVelocity);

			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Result.GetActor()->GetName());
		}

		CanSwipe = NewCanSwipe;
	}
}

void ASparkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowMouseCursor(true);

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());
	if (!InputSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to bind input: input UEnhancedInputLocalPlayerSubsystem* InputSystem is null"));
		return;
	}

	if (!SparkMappingContext)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to bind input : SparkMappingContext is null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("SUCCESS SPARK MAPPING!"));
	InputSystem->AddMappingContext(SparkMappingContext, 1);
}

void ASparkPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent"));

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Unable to setup input : input component is not an UEnhancedInputComponent"), *GetName());
		return;
	}

	EnhancedInputComponent->BindAction(MoveMouseAction, ETriggerEvent::Triggered, this, &ASparkPlayerController::HandleMouseMove);
	EnhancedInputComponent->BindAction(MouseLeftDownAction, ETriggerEvent::Triggered, this, &ASparkPlayerController::HandleMouseLeftDown);

	EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Triggered, this, &ASparkPlayerController::HandleTouchAction);
}

void ASparkPlayerController::HandleTouchAction()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleTouchAction! yay"))
}

void ASparkPlayerController::HandleMouseMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();


	UE_LOG(LogTemp, Warning, TEXT("HANDLE MOUSE MOVE %s"), *MovementVector.ToString())
}

void ASparkPlayerController::HandleMouseLeftDown(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleMouseLeftDown %d"), Value.Get<bool>());
}

void ASparkPlayerController::ShowMouseCursor(bool bShow)
{
	bShowMouseCursor = bShow;
	bEnableClickEvents = bShow;


	if (bShow)
	{
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		SetInputMode(inputMode);
	} 
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}