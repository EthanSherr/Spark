// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SparkMovement.generated.h"

class AWireActor;

USTRUCT(BlueprintType)
struct FSparkMovementInfo
{
	GENERATED_BODY()

	UPROPERTY()
	bool Ascending;

	UPROPERTY()
	bool Valid;

	UPROPERTY()
	float Distance;

	UPROPERTY()
	AWireActor* Wire;

	UPROPERTY()
	AWireActor* PreviousWire;

	FSparkMovementInfo()
	{
		Valid = false;
		Ascending = true;
		Wire = NULL;
		PreviousWire = NULL;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARK_API USparkMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USparkMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float ScanRadius;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawScanRadius;

	UPROPERTY()
	FSparkMovementInfo Info;

	bool AdvanceOnWire(float DeltaTime, FSparkMovementInfo& OutInfo);
	
	bool SearchAndAttachToWires(FSparkMovementInfo& OutInfo);
	bool ScanForWires(TArray<AWireActor*>& OutWires, bool& bScanContainedPreviousWire);
	void AttachToWire(AWireActor* Wire);

	void SetSimulatePhysics(bool bSimulate);

	UPrimitiveComponent* GetPrimitiveComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DetachFromWire();
	
};
