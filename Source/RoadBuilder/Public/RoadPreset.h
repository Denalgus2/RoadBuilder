// Copyright 2024. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "RoadActor.h"
#include "LaneShape.h"
#include "LaneMarkStyle.h"
#include "RoadProps.h"
#include "RoadPreset.generated.h"

UENUM(BlueprintType)
enum class ERoadPresetType : uint8
{
	Street		UMETA(DisplayName = "Street"),
	Highway		UMETA(DisplayName = "Highway"),
	Elevated	UMETA(DisplayName = "Elevated"),
	Ramp		UMETA(DisplayName = "Ramp"),
};

UENUM(BlueprintType)
enum class ELineColor : uint8
{
	White	UMETA(DisplayName = "White"),
	Yellow	UMETA(DisplayName = "Yellow"),
};

UENUM(BlueprintType)
enum class ETurnArrowType : uint8
{
	None,
	Left,
	Through,
	Right,
	LeftThrough,
	ThroughRight,
	LeftRight,
	LeftThroughRight,
	UTurn,
};

UENUM(BlueprintType)
enum class ETrafficControlType : uint8
{
	None			UMETA(DisplayName = "None"),
	StopSign		UMETA(DisplayName = "Stop Sign"),
	YieldSign		UMETA(DisplayName = "Yield Sign"),
	TrafficLight	UMETA(DisplayName = "Traffic Light"),
};

UCLASS(BlueprintType)
class ROADBUILDER_API URoadPreset : public UObject
{
	GENERATED_BODY()
public:
	URoadPreset();

	UPROPERTY(EditAnywhere, Category = "Road Type")
	ERoadPresetType RoadType = ERoadPresetType::Street;

	UPROPERTY(EditAnywhere, Category = "Lanes", meta = (ClampMin = 1, ClampMax = 6))
	int32 LanesPerSide = 2;

	UPROPERTY(EditAnywhere, Category = "Lanes")
	double LaneWidth = 350.0;

	UPROPERTY(EditAnywhere, Category = "Sidewalks")
	bool bHasSidewalks = true;

	UPROPERTY(EditAnywhere, Category = "Sidewalks", meta = (EditCondition = "bHasSidewalks"))
	double SidewalkWidth = 200.0;

	UPROPERTY(EditAnywhere, Category = "Shoulder", meta = (EditCondition = "RoadType==ERoadPresetType::Highway||RoadType==ERoadPresetType::Elevated"))
	bool bHasShoulder = false;

	UPROPERTY(EditAnywhere, Category = "Shoulder", meta = (EditCondition = "bHasShoulder"))
	double ShoulderWidth = 150.0;

	UPROPERTY(EditAnywhere, Category = "Median")
	bool bHasMedian = false;

	UPROPERTY(EditAnywhere, Category = "Median", meta = (EditCondition = "bHasMedian"))
	double MedianWidth = 200.0;

	UPROPERTY(EditAnywhere, Category = "Surface")
	UMaterialInterface* RoadMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Surface")
	UMaterialInterface* SidewalkMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Surface")
	UMaterialInterface* CurbMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Markings")
	ELineColor CenterLineColor = ELineColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "Markings")
	ELineColor LaneLineColor = ELineColor::White;

	UPROPERTY(EditAnywhere, Category = "Markings")
	UMaterialInterface* WhiteLineMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Markings")
	UMaterialInterface* YellowLineMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	ETrafficControlType TrafficControl = ETrafficControlType::None;

	UPROPERTY(EditAnywhere, Category = "Intersection")
	bool bAutoTurnArrows = true;

	UPROPERTY(EditAnywhere, Category = "Intersection", meta = (EditCondition = "bAutoTurnArrows"))
	UStaticMesh* TurnArrowMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Props")
	bool bHasStreetLights = false;

	UPROPERTY(EditAnywhere, Category = "Props", meta = (EditCondition = "bHasStreetLights"))
	UStaticMesh* StreetLightMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Props", meta = (EditCondition = "bHasStreetLights"))
	double StreetLightSpacing = 2000.0;

	UPROPERTY(EditAnywhere, Category = "Ground")
	bool bHasGround = false;

	URoadStyle* GenerateRoadStyle();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	ULaneShape* CreateDrivingShape();
	ULaneShape* CreateSidewalkShape();
	ULaneShape* CreateShoulderShape();
	ULaneShape* CreateMedianShape();
	ULaneMarkStyle* CreateLaneMarkStyle(ELineColor Color, ELaneMarkType Type);
	URoadProps* CreateStreetLightProps();

	UPROPERTY(Transient)
	URoadStyle* CachedStyle = nullptr;
};
