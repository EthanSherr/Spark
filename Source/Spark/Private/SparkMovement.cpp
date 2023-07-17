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

	if (Info.Valid)
	{
		float DeltaLength = DeltaTime * Speed;
		UE_LOG(LogTemp, Warning, TEXT("Info is valid!"));
		USplineComponent* Spline = Info.Wire->GetSplineComponent();
		Spline->GetLocationAtDistanceAlongSpline(DeltaLength + (Info.Distance * Info.Ascending ? -1 : 1), ESplineCoordinateSpace::World);
	}
	else
	{
		SearchForWire(Info);
	}
}

void USparkMovement::SearchForWire(FSparkMovementInfo& OutInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("TEMP TODO - GOTTA SEARCH FOR A WIRE!"))
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(ScanRadius);

	FCollisionQueryParams CollisionQueryParams;
	//CollisionQueryParams.bTraceComplex = true;
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
		auto Hit = OutOverlaps[0];

		if (Hit.GetActor()->IsValidLowLevel()) {
			UE_LOG(LogTemp, Warning, TEXT("Hit hit hit %s %s %s"), 
				*Hit.GetActor()->GetName(), 
				*Hit.GetComponent()->GetName(), 
				*Hit.GetComponent()->GetComponentLocation().ToString()
			);
			
			if (bDrawScanRadius) 
			{
				DrawDebugPoint(GetWorld(), Hit.GetComponent()->GetComponentLocation(), 5, FColor::Green, false, 5, 1);
			}
		}
	}

}