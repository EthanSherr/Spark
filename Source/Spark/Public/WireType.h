// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WireType.generated.h"

UENUM(BlueprintType)
enum EWireType
{
	Red UMETA(DisplayName = "Red"),
	Green UMETA(DisplayName = "Green"),
	Blue UMETA(DisplayName = "Blue")
};