// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SparkMovement.h"
#include "Components/StaticMeshComponent.h"
#include "SparkPawn.generated.h"

UCLASS()
class SPARK_API ASparkPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASparkPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	USparkMovement* Movement;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebugApplySwipe;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplySwipe(const FVector& SwipeVelocity);


};
