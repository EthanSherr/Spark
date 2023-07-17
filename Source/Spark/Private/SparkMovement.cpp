// Fill out your copyright notice in the Description page of Project Settings.


#include "SparkMovement.h"

// Sets default values for this component's properties
USparkMovement::USparkMovement()
{
	PrimaryComponentTick.bCanEverTick = true;

	Speed = 5.0f;
	ScanRadius = 10.0f;

}


// Called when the game starts
void USparkMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USparkMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AdvanceOnWire(DeltaTime, Info))
	{
		SearchForWire(Info);
	}
}

bool USparkMovement::AdvanceOnWire(float DeltaTime, FSparkMovementInfo& OutInfo)
{
	if (!OutInfo.Valid) 
	{
		return false;
	}


	float DeltaDistance = DeltaTime * Speed * (OutInfo.Ascending ? 1 : -1);
	float NewDistance = DeltaDistance + OutInfo.Distance;

	USplineComponent* Spline = Info.Wire->GetSplineComponent();

	if (NewDistance >= Spline->GetSplineLength()) 
	{
		OutInfo.Valid = false;
		return false;
	}

	FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);
	GetOwner()->SetActorLocation(NewLocation);

	Info.Distance = NewDistance;
	UE_LOG(LogTemp, Warning, TEXT("distance %f"), Info.Distance)

	return true;
}

bool USparkMovement::SearchForWire(FSparkMovementInfo& OutInfo)
{
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(ScanRadius);

	FCollisionQueryParams CollisionQueryParams;
	if (OutInfo.Wire) {
		CollisionQueryParams.AddIgnoredActor(OutInfo.Wire);
	}

	FVector Location = GetOwner()->GetActorLocation();

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OutOverlaps,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		SphereCollisionShape,
		CollisionQueryParams
	);

	if (bDrawScanRadius)
	{
		DrawDebugSphere(GetWorld(), Location, ScanRadius, 16, FColor::Red, false, 5, 1);
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlapping %d, result %d"), OutOverlaps.Num(), bResult);

	if (bResult || OutOverlaps.Num() > 0)
	{
		for (const FOverlapResult& Hit : OutOverlaps)
		{
			AWireActor* WireActor = CastChecked<AWireActor>(Hit.GetActor());
			if (!(WireActor && WireActor->IsValidLowLevel())) 
				continue;


			if (bDrawScanRadius)
			{
				DrawDebugPoint(GetWorld(), Hit.GetComponent()->GetComponentLocation(), 5, FColor::Green, false, 5, 1);
			}

			OutInfo.Distance = 0.0f;
			OutInfo.Valid = true;
			OutInfo.Wire = WireActor;
			OutInfo.Ascending = true;
			
			return true;
		}
	}

	return false;

}