// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "WireActor.generated.h"

UCLASS()
class SPARK_API AWireActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWireActor();

	USplineComponent* GetSplineComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	virtual void OnConstruction(const FTransform& Transform) override;

	// Component setup
	void SetupSplineComponent();
	void SetupSplineMeshComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Spline")
	UStaticMesh* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	TArray<USplineMeshComponent*> SplineMeshComponents;
};
