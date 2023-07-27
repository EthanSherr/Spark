// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SparkGameModeBase.generated.h"

class ASparkPawn;

UCLASS()
class SPARK_API ASparkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<ASparkPawn*> ActiveSparks;
	

	void GameOver();

public:
	void AddSpark(ASparkPawn* Spark);
	void RemoveSpark(ASparkPawn* Spark);
};
