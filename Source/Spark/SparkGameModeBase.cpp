// Copyright Epic Games, Inc. All Rights Reserved.


#include "SparkGameModeBase.h"
#include "SparkPawn.h"
#include "Kismet/GameplayStatics.h"


void ASparkGameModeBase::GameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Game over, restart curent level or something"))
    UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetMapName()));
}

void ASparkGameModeBase::AddSpark(ASparkPawn* Spark)
{
    ActiveSparks.Add(Spark);
}

void ASparkGameModeBase::RemoveSpark(ASparkPawn* Spark)
{
    ActiveSparks.Remove(Spark);
    if (ActiveSparks.Num() == 0)
    {
        GameOver();
    }
}