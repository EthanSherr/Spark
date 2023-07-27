// Fill out your copyright notice in the Description page of Project Settings.


#include "SparkPawn.h"
#include "../SparkGameModeBase.h"
#include "Kismet/GameplayStatics.h"


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
	
	ASparkGameModeBase* GM = Cast<ASparkGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM) 
	{
		GM->AddSpark(this);
	}
}

void ASparkPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Warning, TEXT("SparkPawn Begindestroy %s"), *GetName());

	ASparkGameModeBase* GM = Cast<ASparkGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM) 
	{
		GM->RemoveSpark(this);
	}
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

	Primitive->SetPhysicsLinearVelocity(Speed * SwipeDirection);
}