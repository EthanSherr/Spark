// Fill out your copyright notice in the Description page of Project Settings.


#include "PBSparkPawn.h"
#include "WireActor.h"

// Sets default values
APBSparkPawn::APBSparkPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = StaticMesh;

	PhysicsConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraintComponent->SetupAttachment(StaticMesh);

	Speed = 100.0f;
	ScanRadius = 200.0f;
}

// Called when the game starts or when spawned
void APBSparkPawn::BeginPlay()
{
	Super::BeginPlay();
	
	bIsMovingForward = false;
	DistanceAlongSpline = 0.f;
}

// Other Setup
void APBSparkPawn::SetupSplineConstraint(AWireActor* NewWireActor)
{
	if (!StaticMesh || !NewWireActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Inproper SetupSplineConstraint call, no static mesh or no wire actor."))
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("APBSparkPawn got WireActor (%s) setup "), *NewWireActor->GetName())
	WireActor = NewWireActor;

	USplineComponent* SplineComponent = WireActor->GetSplineComponent();
	FVector StartPosition = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	PhysicsConstraintComponent->SetConstrainedComponents(StaticMesh, NAME_None, SplineComponent, NAME_None);
	PhysicsConstraintComponent->SetWorldLocation(StartPosition);
	//PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	//PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	//PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	//PhysicsConstraintComponent->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//PhysicsConstraintComponent->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	//PhysicsConstraintComponent->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

}


// Called every frame
void APBSparkPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!WireActor)
	{
		AWireActor* W = GetWireActor();
		if (W) {			
			SetupSplineConstraint(W);
		}
	}

	// Update the distance along the spline based on the ball's current speed
	float DeltaDistance = DeltaTime * Speed * (bIsMovingForward ? 1.f : -1.f);
	DistanceAlongSpline += DeltaDistance;

	if (!WireActor) 
	{
		return;
	}

	USplineComponent* SplineComponent = WireActor->GetSplineComponent();

	// Snap to dist!
	if (DistanceAlongSpline >= SplineComponent->GetSplineLength())
	{
		bIsMovingForward = false;
		DistanceAlongSpline = SplineComponent->GetSplineLength();
	}
	else if (DistanceAlongSpline <= 0.f)
	{
		bIsMovingForward = true;
		DistanceAlongSpline = 0.f;
	}

	// Calculate the position along the spline where the ball should be constrained
	FVector TargetPosition = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	UE_LOG(LogTemp, Warning, TEXT("%s: DistanceAlongSplince %f"), *GetName(), DistanceAlongSpline)

	// Move the ball using the physics constraint
	PhysicsConstraintComponent->SetWorldLocation(TargetPosition);

}

// Called to bind functionality to input
void APBSparkPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AWireActor* APBSparkPawn::GetWireActor()
{
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

	UE_LOG(LogTemp, Warning, TEXT("Overlapping %d, result %d"), OutOverlaps.Num(), bResult);

	if (bResult || OutOverlaps.Num() > 0)
	{
		for (const FOverlapResult& Hit : OutOverlaps)
		{
			AWireActor* W = CastChecked<AWireActor>(Hit.GetActor());
			if (!(W && W->IsValidLowLevel()))
				continue;

			return W;
		}
	}

	return NULL;
}

