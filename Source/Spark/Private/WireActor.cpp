// Fill out your copyright notice in the Description page of Project Settings.


#include "WireActor.h"


AWireActor::AWireActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetupSplineComponent();
	SplineComponent->SetMobility(EComponentMobility::Static);
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

void AWireActor::SetupSplineMeshComponent()
{

	// Attach the Spline Mesh to the Spline Component
	for (int32 i = 0; i < SplineMeshComponents.Num(); i++) 
	{
		SplineMeshComponents[i]->DestroyComponent();
		SplineMeshComponents.RemoveAt(i);  
	}

	FVector Location, Tangent, LocationNext, TangentNext;

	int32 nEdges = SplineComponent->GetNumberOfSplinePoints() - 1;
	for (int32 i = 0; i < nEdges; i++) {

		auto SplineMeshComp = NewObject<USplineMeshComponent>(this, FName(*FString::Printf(TEXT("SplineMeshComponent %d"), i)));
		SplineMeshComp->SetStaticMesh(Mesh);
		SplineMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SplineMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMeshComp->RegisterComponent();

		SplineComponent->GetLocationAndTangentAtSplinePoint(i, Location, Tangent, ESplineCoordinateSpace::World);
		SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, LocationNext, TangentNext, ESplineCoordinateSpace::World);
		TangentNext = Tangent;
		
		SplineMeshComp->SetStartAndEnd(Location, Tangent, LocationNext, TangentNext);
		SplineMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

		SplineMeshComponents.Add(SplineMeshComp);
	}
}


void AWireActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetupSplineMeshComponent();
}

void AWireActor::BeginPlay()
{
	Super::BeginPlay();	

// TODO temp
	SetWireType(EWireType::Green);
}

USplineComponent* AWireActor::GetSplineComponent()
{
	return SplineComponent;
}

float AWireActor::FindDistanceOfNearestPointOnSpline(FVector WorldLocation)
{
	float InputKey = SplineComponent->FindInputKeyClosestToWorldLocation(WorldLocation);
	int32 SplinePoint = FMath::TruncToInt(InputKey);
	float SplinePointDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplinePoint);
	if (SplinePoint < SplineComponent->GetNumberOfSplinePoints())
	{
		float NextPointDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplinePoint + 1);
		return FMath::Lerp(SplinePointDistance, NextPointDistance, InputKey - SplinePoint);
	}
	else
	{
		float PrevPointDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplinePoint - 1);
		return FMath::Lerp(PrevPointDistance, SplinePointDistance, SplinePoint - InputKey);
	}
}

/*
	This line is causing errors - I'm not sure the issue.  
	Do enums need to be passed as TEnumAsByte? 
	Or is that just needed for return?
	Maybe there's an issue with enums being passed to blueprints - that sounds familiar.
*/

// :( not called!
void AWireActor::SetWireType(EWireType NewWireType)
{
	WireType = NewWireType;
	WireTypeChanged.Broadcast();
}

TEnumAsByte<EWireType> AWireActor::GetWireType()
{
	return WireType;
}