// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayArea.generated.h"

UCLASS()
class SPARK_API APlayArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayArea();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Config")
	UBoxComponent* BoxComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
