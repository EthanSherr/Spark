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

	void OnSwipeStarted(ETouchIndex::Type FingerIndex, FVector Vector);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;


	// Swipe config
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinSwipeDistanceThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinSwipeTimeThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float SwipeTraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDrawSwipeDebug;

	// Swipe state
	bool bIsSwiping;
	bool bIsMouseDown;

	
	TMap<FString, bool> CanSwipe;

	FVector SwipeWorldLocation;
	FVector SwipeWorldVelocity;


	// input actions

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* SparkMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveMouseAction;

	void HandleMouseMove(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLeftDownAction;

	void HandleMouseLeftDown(const FInputActionValue& Value);

	void ShowMouseCursor(bool bShow);

};
