// Publisher: Fullike (https://github.com/fullike)
// Copyright 2024. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "ZoneGraphTypes.h"
#include "RoadZoneGraphExport.generated.h"

class ARoadActor;
class URoadLane;
class AZoneShape;

/**
 * Configuration for ZoneGraph export from RoadBuilder
 */
USTRUCT(BlueprintType)
struct ROADBUILDER_API FRoadZoneGraphConfig
{
	GENERATED_BODY()

	/** Width of zone lanes (in cm). Default: 400cm (4m) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	float LaneWidth = 400.0f;

	/** Number of points per meter for zone spline sampling. Default: 2 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	float PointsPerMeter = 2.0f;

	/** Minimum distance between consecutive points (in cm). Default: 50cm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	float MinPointDistance = 50.0f;

	/** Maximum distance between consecutive points (in cm). Default: 500cm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	float MaxPointDistance = 500.0f;

	/** Export only driving lanes. Default: true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	bool bExportDrivingLanesOnly = true;

	/** Generate reverse lanes for bidirectional roads. Default: true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	bool bGenerateReverseLanes = true;

	/** Tags to apply to generated zone lanes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
	FZoneGraphTagMask LaneTags;
};

/**
 * Represents a single lane exported to ZoneGraph format
 */
USTRUCT(BlueprintType)
struct ROADBUILDER_API FRoadZoneLane
{
	GENERATED_BODY()

	/** Spline points defining the lane centerline */
	UPROPERTY(BlueprintReadWrite, Category = "ZoneGraph")
	TArray<FVector> Points;

	/** Width of the lane in cm */
	UPROPERTY(BlueprintReadWrite, Category = "ZoneGraph")
	float Width = 400.0f;

	/** Tags for this lane */
	UPROPERTY(BlueprintReadWrite, Category = "ZoneGraph")
	FZoneGraphTagMask Tags;

	/** Whether this is a reverse direction lane */
	UPROPERTY(BlueprintReadWrite, Category = "ZoneGraph")
	bool bIsReverse = false;

	/** Index of the source road lane */
	UPROPERTY(BlueprintReadWrite, Category = "ZoneGraph")
	int32 SourceLaneIndex = -1;
};

/**
 * Helper class for exporting RoadBuilder roads to ZoneGraph format
 */
UCLASS()
class ROADBUILDER_API URoadZoneGraphExporter : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Export a single road actor to ZoneGraph lane format
	 * @param RoadActor The road actor to export
	 * @param Config Export configuration
	 * @param OutLanes Array of exported lanes
	 * @return true if export was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "RoadBuilder|ZoneGraph")
	static bool ExportRoadToZoneLanes(ARoadActor* RoadActor, const FRoadZoneGraphConfig& Config, TArray<FRoadZoneLane>& OutLanes);

	/**
	 * Export multiple road actors to ZoneGraph lane format
	 * @param RoadActors Array of road actors to export
	 * @param Config Export configuration
	 * @param OutLanes Array of exported lanes from all roads
	 * @return Number of successfully exported roads
	 */
	UFUNCTION(BlueprintCallable, Category = "RoadBuilder|ZoneGraph")
	static int32 ExportRoadsToZoneLanes(const TArray<ARoadActor*>& RoadActors, const FRoadZoneGraphConfig& Config, TArray<FRoadZoneLane>& OutLanes);

	/**
	 * Create a ZoneShape actor from road actors
	 * @param World World context for spawning the actor
	 * @param RoadActors Array of road actors to convert
	 * @param Config Export configuration
	 * @param SpawnLocation Location to spawn the ZoneShape
	 * @return Created ZoneShape actor, or nullptr on failure
	 */
	UFUNCTION(BlueprintCallable, Category = "RoadBuilder|ZoneGraph", meta = (WorldContext = "World"))
	static AZoneShape* CreateZoneShapeFromRoads(UWorld* World, const TArray<ARoadActor*>& RoadActors, const FRoadZoneGraphConfig& Config, const FVector& SpawnLocation = FVector::ZeroVector);

	/**
	 * Sample points along a road lane for zone graph export
	 * @param Lane The road lane to sample
	 * @param Config Export configuration
	 * @param OutPoints Array of sampled 3D points
	 * @return true if sampling was successful
	 */
	static bool SampleLanePoints(URoadLane* Lane, const FRoadZoneGraphConfig& Config, TArray<FVector>& OutPoints);

	/**
	 * Calculate appropriate lane width from road lane
	 * @param Lane The road lane
	 * @return Width in cm
	 */
	static float CalculateLaneWidth(URoadLane* Lane);

	/**
	 * Determine if a lane should be exported based on configuration
	 * @param Lane The road lane
	 * @param Config Export configuration
	 * @return true if lane should be exported
	 */
	static bool ShouldExportLane(URoadLane* Lane, const FRoadZoneGraphConfig& Config);
};
