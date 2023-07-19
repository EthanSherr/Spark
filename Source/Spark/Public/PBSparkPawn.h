// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PBSparkPawn.generated.h"

UCLASS()
class SPARK_API APBSparkPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APBSparkPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	UPhysicsConstraintComponent* PhysicsConstraintComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void SetupSplineConstraint(class AWireActor* WireActor);

	UPROPERTY()
	class AWireActor* WireActor;

	bool bIsMovingForward;

	float DistanceAlongSpline;

	float Speed;

	UPROPERTY(EditDefaultsOnly)
	float ScanRadius;

	class AWireActor* GetWireActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
