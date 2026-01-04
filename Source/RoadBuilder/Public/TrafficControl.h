// Publisher: Fullike (https://github.com/fullike)
// Copyright 2024. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "TrafficControl.generated.h"

// Enum for traffic control types
UENUM(BlueprintType)
enum class ETrafficControlType : uint8
{
	StopSign UMETA(DisplayName = "Stop Sign"),
	YieldSign UMETA(DisplayName = "Yield Sign"),
	TrafficLight UMETA(DisplayName = "Traffic Light"),
	SpeedLimit UMETA(DisplayName = "Speed Limit"),
	PedestrianCrossing UMETA(DisplayName = "Pedestrian Crossing"),
	NoParking UMETA(DisplayName = "No Parking"),
	OneWay UMETA(DisplayName = "One Way"),
	DoNotEnter UMETA(DisplayName = "Do Not Enter")
};

// Struct for traffic control placement
USTRUCT(BlueprintType)
struct FTrafficControlPlacement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Traffic Control")
	ETrafficControlType ControlType;

	UPROPERTY(EditAnywhere, Category = "Traffic Control")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Traffic Control")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Traffic Control")
	float Scale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Traffic Control")
	UObject* CustomAsset = nullptr;
};

// Struct for traffic light configuration
USTRUCT(BlueprintType)
struct FTrafficLightConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ClampMin = "1.0", ClampMax = "300.0"))
	float GreenDuration = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ClampMin = "1.0", ClampMax = "60.0"))
	float YellowDuration = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Traffic Light", meta = (ClampMin = "1.0", ClampMax = "300.0"))
	float RedDuration = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Traffic Light")
	bool bHasPedestrianSignal = false;

	UPROPERTY(EditAnywhere, Category = "Traffic Light")
	bool bHasTurnArrow = false;
};
