// Publisher: Fullike (https://github.com/fullike)
// Copyright 2024. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "TrafficControl.h"
#include "IntersectionManager.generated.h"

// Enum for intersection types
UENUM(BlueprintType)
enum class EIntersectionType : uint8
{
	FourWay UMETA(DisplayName = "Four-Way"),
	ThreeWay UMETA(DisplayName = "Three-Way (T-Junction)"),
	Roundabout UMETA(DisplayName = "Roundabout"),
	Custom UMETA(DisplayName = "Custom")
};

// Struct for intersection configuration
USTRUCT(BlueprintType)
struct FIntersectionConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Intersection")
	EIntersectionType IntersectionType = EIntersectionType::FourWay;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	bool bHasTrafficLights = true;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	bool bHasStopSigns = false;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	bool bHasYieldSigns = false;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	bool bHasCrosswalks = true;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	float CrosswalkWidth = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	FTrafficLightConfig TrafficLightConfig;
};
