// Fill out your copyright notice in the Description page of Project Settings.


#include "SparkMovement.h"
#include "Components/PrimitiveComponent.h"
#include "WireActor.h"

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

	if (!Info.Valid) 
	{
		SearchAndAttachToWires(Info);
	}
	
	AdvanceOnWire(DeltaTime, Info);
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

	if (NewDistance > Spline->GetSplineLength() || NewDistance < 0)
	{
		DetachFromWire();
		return false;
	}

	FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::World);

	UPrimitiveComponent* Prim = GetPrimitiveComponent();
	if (Prim) 
	{
		Prim->SetPhysicsLinearVelocity((NewLocation - GetOwner()->GetActorLocation()) / DeltaTime);
	}
	GetOwner()->SetActorLocation(NewLocation);

	Info.Distance = NewDistance;

	return true;
}

bool USparkMovement::SearchAndAttachToWires(FSparkMovementInfo& OutInfo)
{
	TArray<AWireActor*> Wires;

	bool bScanContainedPreviousWire;
	if (ScanForWires(Wires, bScanContainedPreviousWire)) 
	{
		AttachToWire(Wires[0]);
		return true;
	}
	else
	if (!bScanContainedPreviousWire)
	{
		Info.PreviousWire = NULL;
	}
	return false;
}

bool USparkMovement::ScanForWires(TArray<AWireActor*>& OutWires, bool& bScanContainedPreviousWire) {
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(ScanRadius);
	FCollisionQueryParams CollisionQueryParams;

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

	bScanContainedPreviousWire = false;
	for (const FOverlapResult& Hit : OutOverlaps)
	{
		AWireActor* WireActor = CastChecked<AWireActor>(Hit.GetActor());
		if (!(WireActor && WireActor->IsValidLowLevel()))
		{
			continue;
		}

		if (WireActor == Info.PreviousWire)
		{
			bScanContainedPreviousWire = true;
			continue;
		}


		if (bDrawScanRadius)
		{
			DrawDebugPoint(GetWorld(), Hit.GetComponent()->GetComponentLocation(), 5, FColor::Green, false, 5, 1);
		}

		OutWires.Add(WireActor);
	}

	return OutWires.Num() > 0;

}

void USparkMovement::AttachToWire(AWireActor* Wire)
{
	if (!Wire) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s unable to attach to wire: Null wire"), *GetOwner()->GetName());
		return;
	}

	float Distance = Wire->FindDistanceOfNearestPointOnSpline(GetOwner()->GetActorLocation());

	FVector Location = Wire->GetSplineComponent()->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector Tangent = Wire->GetSplineComponent()->GetTangentAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	UPrimitiveComponent* Prim = GetPrimitiveComponent();
	if (!Prim) {
		UE_LOG(LogTemp, Error, TEXT("%s Unable to determine Ascending: root is not a primitive"));
		return;
	}
	
	bool Ascending = true;

	if (Tangent.Dot(Prim->GetPhysicsLinearVelocity()) < 0) {
		Ascending = false;
	}
	
	SetSimulatePhysics(false);
	Info.Wire = Wire;
	Info.Ascending = Ascending;
	Info.Distance = Distance;
	Info.Valid = true;
}

void USparkMovement::SetSimulatePhysics(bool bSimulate)
{
	UPrimitiveComponent* Prim = GetPrimitiveComponent();
	if (!Prim)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s could SetSimulatePhysics: root is not primitive"), *GetOwner()->GetName());
		return;
	}

	Prim->SetSimulatePhysics(bSimulate);
}

UPrimitiveComponent* USparkMovement::GetPrimitiveComponent()
{
	return Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
}

void USparkMovement::DetachFromWire()
{
	Info.PreviousWire = Info.Wire;
	Info.Valid = false;
	SetSimulatePhysics(true);
}