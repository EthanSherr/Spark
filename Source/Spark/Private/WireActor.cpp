// Fill out your copyright notice in the Description page of Project Settings.


#include "WireActor.h"

AWireActor::AWireActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetupSplineComponent();
	RootComponent = SplineComponent;
}

void AWireActor::SetupSplineComponent()
{
	SplineComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	for (int32 PointIndex = 0; PointIndex < SplineComponent->GetNumberOfSplinePoints(); PointIndex++)
	{
		SplineComponent->SetSplinePointType(PointIndex, ESplinePointType::Linear);
	}
}


void AWireActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AWireActor::BeginPlay()
{
	Super::BeginPlay();	
}



