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

