// Fill out your copyright notice in the Description page of Project Settings.


#include "SparkPawn.h"

// Sets default values
ASparkPawn::ASparkPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Movement = CreateDefaultSubobject<USparkMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void ASparkPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASparkPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASparkPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void ASparkPawn::ApplySwipe(const FVector& SwipeVelocity)
{
	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GetRootComponent());
	if (!Primitive)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Couldn't ApplySwipeImpulse : No root primitive."), *GetName());
		return;
	}

	Movement->DetachFromWire();
	float Speed = Primitive->GetPhysicsLinearVelocity().Size();
	FVector SwipeDirection = SwipeVelocity.GetSafeNormal(0.001);

	if (bDebugApplySwipe)
	{
		DrawDebugLine(
			GetWorld(), 
			GetActorLocation(), 
			GetActorLocation() + 300 * SwipeDirection, 
			FColor::Red, 
			true, 
			1000, 
			1, 
			2
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("PhysLinearSpeed: %f, SwipeNorm: %s"), Speed, *SwipeDirection.ToString());

	Primitive->SetPhysicsLinearVelocity(-Speed * SwipeDirection);
}