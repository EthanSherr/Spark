// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayArea.h"
#include "SparkPawn.h"

// Sets default values
APlayArea::APlayArea()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayAreaBoxComponent"));
	RootComponent = BoxComponent;

	BoxComponent->InitBoxExtent(FVector(500.0f, 500.0f, 500.0f));

}

void APlayArea::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APlayArea::OnPlayAreaEndOverlap);

}


void APlayArea::OnPlayAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap end %s"), *OtherActor->GetName())
	ASparkPawn* SparkPawn = Cast<ASparkPawn>(OtherActor);
	if (SparkPawn)
	{
		SparkPawn->Destroy();
	}
}