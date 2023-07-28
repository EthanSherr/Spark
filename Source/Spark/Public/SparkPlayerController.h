// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "SparkPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SPARK_API ASparkPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASparkPlayerController();

protected:
	virtual void SetupInputComponent() override;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	// Swipe config
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float SwipeSweepRadius;
	
	TMap<FString, bool> CanSwipe;

// input actions

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* SparkMappingContext;

// mouse handlers
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLeftDownAction;

	void HandleMouseMove(const FInputActionValue& Value);
	void HandleMouseLeftDown(const FInputActionValue& Value);

// touch handlers
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchPressedAction;

	void HandleTouchPressedAction();
	void HandleTouchReleasedAction();

	void ShowMouseCursor(bool bShow);

// Swipe logic
	bool GetSparkPlaneVectorIntersect(FVector WorldLocation, FVector WorldDirection, FVector& IntersectionPoint);

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugSwipe;
	void ScanSwipeSparkDeep(float DeltaTime);

	FVector IntersectionPoint;

// need to know how deep in the scene the spark is for the ScanSwipeSparkDeep functionality
	void SetupSparkX();
	float SparkX;

};
