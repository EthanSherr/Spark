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

	SetupSparkX();
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

void ASparkPlayerController::SetupSparkX()
{
	UE_LOG(LogTemp, Warning, TEXT("SetupSparkX"))
	SparkX = -300.0f;
}

void ASparkPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ScanSwipeSpark(DeltaTime);

	ScanSwipeSparkDeep(DeltaTime);
}

void ASparkPlayerController::ScanSwipeSparkDeep(float DeltaTime)
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(this->GetLocalPlayer());
	if (!LocalPlayer || !LocalPlayer->ViewportClient) {
		return;
	}

	FVector2D CurPosition;
	LocalPlayer->ViewportClient->GetMousePosition(CurPosition);

	if (CurPosition.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Screen position is 0,0 let's return early"))
		return;
	}

	FVector WorldLocation, WorldDirection;
	DeprojectScreenPositionToWorld(CurPosition.X, CurPosition.Y, WorldLocation, WorldDirection);


	FVector NextIntersectionPoint;
	if (!GetSparkPlaneVectorIntersect(WorldLocation, WorldDirection, NextIntersectionPoint)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ScanSwipeSparkDeep failed"));
		return;
	}
	
	if (!IntersectionPoint.IsZero()) 
	{
		// trace from NextIntersectionPoint - IntersectionPoint
		if (bDebugSwipe)
		{
			DrawDebugPoint(GetWorld(), NextIntersectionPoint, 3, FColor::Green, false, 0.5, 2);
			DrawDebugLine(GetWorld(), IntersectionPoint, NextIntersectionPoint, FColor::Green, false, 0.5, 2);
		}

		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionParams;

		bool bHit = GetWorld()->LineTraceMultiByChannel(
			HitResults,
			WorldLocation,
			WorldLocation + WorldDirection * SwipeTraceLength,
			ECC_Visibility,
			CollisionParams
		);

		TMap<FString, bool> NewCanSwipe;

		FVector SwipeDir = (NextIntersectionPoint - IntersectionPoint).GetSafeNormal();
		for (auto Result : HitResults) 
		{
			ASparkPawn* Spark = Cast<ASparkPawn>(Result.GetActor());
			if (!Spark || CanSwipe.Contains(Spark->GetName()))
			{
				continue;
			}

			NewCanSwipe.Add(Spark->GetName(), false);
			Spark->ApplySwipe(SwipeDir);

			if (bDebugSwipe)
			{
				DrawDebugPoint(GetWorld(), Result.Location, 5, FColor::Red, false, 2, 2);
			}
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Result.GetActor()->GetName());
		}

		CanSwipe = NewCanSwipe;
	}

	IntersectionPoint = NextIntersectionPoint;
}

bool ASparkPlayerController::GetSparkPlaneVectorIntersect(FVector WorldLocation, FVector WorldDirection, FVector& OutIntersectionPoint)
{
	// Define the plane location along the x-axis (-300)
    const float PlaneX = SparkX;

    // Check if the ray is parallel to the plane (dot product of direction and plane normal)
    FVector PlaneNormal(1.0f, 0.0f, 0.0f); // Plane normal is along the x-axis
    float DotProduct = FVector::DotProduct(WorldDirection, PlaneNormal);

    if (FMath::IsNearlyZero(DotProduct))
    {
        // Ray is parallel to the plane, so there's no intersection
		UE_LOG(LogTemp, Warning, TEXT("Ray is parallel to plane, no intersection"))
        return false;
    }

    // Calculate the t parameter for the ray equation: IntersectionPoint = WorldLocation + t * WorldDirection
    float t = (PlaneX - WorldLocation.X) / DotProduct;

    // Ensure the intersection point is in front of the ray's origin (WorldLocation)
    if (t < 0)
    {
		UE_LOG(LogTemp, Warning, TEXT(" Ensure the intersection point is in front of the ray's origin (WorldLocation)"))
        return false;
    }

    // Calculate the intersection point using the ray equation
    OutIntersectionPoint = WorldLocation + t * WorldDirection;
	return true;
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