// Publisher: Fullike (https://github.com/fullike)
// Copyright 2024. All Rights Reserved.

#include "RoadZoneGraphExport.h"
#include "RoadActor.h"
#include "RoadLane.h"
#include "RoadBoundary.h"
#include "ZoneGraphTypes.h"
#include "ZoneShapeComponent.h"
#include "ZoneGraphSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

bool URoadZoneGraphExporter::ExportRoadToZoneLanes(ARoadActor* RoadActor, const FRoadZoneGraphConfig& Config, TArray<FRoadZoneLane>& OutLanes)
{
	if (!RoadActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("URoadZoneGraphExporter::ExportRoadToZoneLanes - RoadActor is null"));
		return false;
	}

	// Get all lanes from the road
	TArray<URoadLane*> LeftLanes = RoadActor->GetLanes(RD_LEFT);
	TArray<URoadLane*> RightLanes = RoadActor->GetLanes(RD_RIGHT);

	int32 InitialCount = OutLanes.Num();

	// Export left lanes
	for (int32 i = 0; i < LeftLanes.Num(); i++)
	{
		URoadLane* Lane = LeftLanes[i];
		if (!Lane || !ShouldExportLane(Lane, Config))
		{
			continue;
		}

		FRoadZoneLane ZoneLane;
		ZoneLane.SourceLaneIndex = i;
		ZoneLane.bIsReverse = false;
		ZoneLane.Width = CalculateLaneWidth(Lane);
		ZoneLane.Tags = Config.LaneTags;

		if (SampleLanePoints(Lane, Config, ZoneLane.Points))
		{
			OutLanes.Add(ZoneLane);
		}
	}

	// Export right lanes
	for (int32 i = 0; i < RightLanes.Num(); i++)
	{
		URoadLane* Lane = RightLanes[i];
		if (!Lane || !ShouldExportLane(Lane, Config))
		{
			continue;
		}

		FRoadZoneLane ZoneLane;
		ZoneLane.SourceLaneIndex = i;
		ZoneLane.bIsReverse = false;
		ZoneLane.Width = CalculateLaneWidth(Lane);
		ZoneLane.Tags = Config.LaneTags;

		if (SampleLanePoints(Lane, Config, ZoneLane.Points))
		{
			OutLanes.Add(ZoneLane);
		}

		// Generate reverse lane if configured
		if (Config.bGenerateReverseLanes)
		{
			FRoadZoneLane ReverseLane = ZoneLane;
			ReverseLane.bIsReverse = true;
			
			// Reverse the points array
			Algo::Reverse(ReverseLane.Points);
			
			OutLanes.Add(ReverseLane);
		}
	}

	return OutLanes.Num() > InitialCount;
}

int32 URoadZoneGraphExporter::ExportRoadsToZoneLanes(const TArray<ARoadActor*>& RoadActors, const FRoadZoneGraphConfig& Config, TArray<FRoadZoneLane>& OutLanes)
{
	int32 SuccessCount = 0;

	for (ARoadActor* RoadActor : RoadActors)
	{
		if (ExportRoadToZoneLanes(RoadActor, Config, OutLanes))
		{
			SuccessCount++;
		}
	}

	return SuccessCount;
}

AZoneShape* URoadZoneGraphExporter::CreateZoneShapeFromRoads(UWorld* World, const TArray<ARoadActor*>& RoadActors, const FRoadZoneGraphConfig& Config, const FVector& SpawnLocation)
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("URoadZoneGraphExporter::CreateZoneShapeFromRoads - World is null"));
		return nullptr;
	}

	if (RoadActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("URoadZoneGraphExporter::CreateZoneShapeFromRoads - No road actors provided"));
		return nullptr;
	}

	// Export all roads to zone lanes
	TArray<FRoadZoneLane> ZoneLanes;
	int32 ExportedCount = ExportRoadsToZoneLanes(RoadActors, Config, ZoneLanes);

	if (ExportedCount == 0 || ZoneLanes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("URoadZoneGraphExporter::CreateZoneShapeFromRoads - No lanes exported"));
		return nullptr;
	}

	// Spawn ZoneShape actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(TEXT("ZoneShape_RoadBuilder"));
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AZoneShape* ZoneShape = World->SpawnActor<AZoneShape>(AZoneShape::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	
	if (!ZoneShape)
	{
		UE_LOG(LogTemp, Error, TEXT("URoadZoneGraphExporter::CreateZoneShapeFromRoads - Failed to spawn ZoneShape actor"));
		return nullptr;
	}

	// Get the ZoneShapeComponent
	UZoneShapeComponent* ShapeComponent = ZoneShape->GetShapeComp();
	if (!ShapeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("URoadZoneGraphExporter::CreateZoneShapeFromRoads - ZoneShape has no ShapeComponent"));
		World->DestroyActor(ZoneShape);
		return nullptr;
	}

	// Configure the zone shape with exported lanes
	TArray<FZoneShapeLaneSegment>& Lanes = ShapeComponent->GetMutableLanes();
	Lanes.Empty(ZoneLanes.Num());

	for (const FRoadZoneLane& ZoneLane : ZoneLanes)
	{
		if (ZoneLane.Points.Num() < 2)
		{
			continue; // Skip invalid lanes
		}

		FZoneShapeLaneSegment LaneSegment;
		LaneSegment.Width = ZoneLane.Width;
		LaneSegment.Tags = ZoneLane.Tags;
		
		// Set up the lane points
		LaneSegment.Points.Empty(ZoneLane.Points.Num());
		for (const FVector& Point : ZoneLane.Points)
		{
			// Convert world space to zone shape local space
			FVector LocalPoint = ZoneShape->GetTransform().InverseTransformPosition(Point);
			LaneSegment.Points.Add(FZoneShapePoint(LocalPoint));
		}

		Lanes.Add(LaneSegment);
	}

	// Update the zone shape
	ShapeComponent->UpdateShape();

	UE_LOG(LogTemp, Log, TEXT("URoadZoneGraphExporter::CreateZoneShapeFromRoads - Created ZoneShape with %d lanes from %d roads"), 
		Lanes.Num(), ExportedCount);

	return ZoneShape;
}

bool URoadZoneGraphExporter::SampleLanePoints(URoadLane* Lane, const FRoadZoneGraphConfig& Config, TArray<FVector>& OutPoints)
{
	if (!Lane)
	{
		return false;
	}

	// Get lane boundaries
	URoadBoundary* LeftBoundary = Lane->LeftBoundary;
	URoadBoundary* RightBoundary = Lane->RightBoundary;

	if (!LeftBoundary || !RightBoundary)
	{
		return false;
	}

	// Calculate the total length of the lane
	const TArray<FVector2D>& LeftPoints = LeftBoundary->Points;
	const TArray<FVector2D>& RightPoints = RightBoundary->Points;

	if (LeftPoints.Num() < 2 || RightPoints.Num() < 2)
	{
		return false;
	}

	// Calculate total length
	double TotalLength = 0.0;
	for (int32 i = 1; i < LeftPoints.Num(); i++)
	{
		TotalLength += FVector2D::Distance(LeftPoints[i - 1], LeftPoints[i]);
	}

	if (TotalLength < KINDA_SMALL_NUMBER)
	{
		return false;
	}

	// Calculate number of points based on configuration
	int32 NumPoints = FMath::CeilToInt(TotalLength * Config.PointsPerMeter / 100.0f); // Convert to meters
	NumPoints = FMath::Clamp(NumPoints, 2, 10000); // Reasonable limits

	OutPoints.Empty(NumPoints);

	// Sample points along the centerline
	for (int32 i = 0; i < NumPoints; i++)
	{
		float T = (float)i / (float)(NumPoints - 1);
		int32 SegmentIndex = FMath::Clamp(FMath::FloorToInt(T * (LeftPoints.Num() - 1)), 0, LeftPoints.Num() - 2);
		float SegmentT = (T * (LeftPoints.Num() - 1)) - SegmentIndex;

		// Interpolate left and right boundaries
		FVector2D LeftPos = FMath::Lerp(LeftPoints[SegmentIndex], LeftPoints[SegmentIndex + 1], SegmentT);
		FVector2D RightPos = FMath::Lerp(RightPoints[SegmentIndex], RightPoints[SegmentIndex + 1], SegmentT);

		// Calculate centerline position
		FVector2D CenterPos = (LeftPos + RightPos) * 0.5;

		// Get height from left boundary (assuming both boundaries have similar height profiles)
		double Height = LeftBoundary->Heights.IsValidIndex(SegmentIndex) ? 
			FMath::Lerp(LeftBoundary->Heights[SegmentIndex], 
				LeftBoundary->Heights[FMath::Min(SegmentIndex + 1, LeftBoundary->Heights.Num() - 1)], 
				SegmentT) : 0.0;

		FVector Point3D(CenterPos.X, CenterPos.Y, Height);

		// Apply distance constraints
		if (OutPoints.Num() > 0)
		{
			float Distance = FVector::Dist(OutPoints.Last(), Point3D);
			if (Distance < Config.MinPointDistance)
			{
				continue; // Skip point that's too close
			}
			if (Distance > Config.MaxPointDistance && i < NumPoints - 1)
			{
				// Point is too far, add intermediate point
				FVector MidPoint = (OutPoints.Last() + Point3D) * 0.5f;
				OutPoints.Add(MidPoint);
			}
		}

		OutPoints.Add(Point3D);
	}

	return OutPoints.Num() >= 2;
}

float URoadZoneGraphExporter::CalculateLaneWidth(URoadLane* Lane)
{
	if (!Lane || !Lane->LeftBoundary || !Lane->RightBoundary)
	{
		return 400.0f; // Default width
	}

	const TArray<FVector2D>& LeftPoints = Lane->LeftBoundary->Points;
	const TArray<FVector2D>& RightPoints = Lane->RightBoundary->Points;

	if (LeftPoints.Num() == 0 || RightPoints.Num() == 0)
	{
		return 400.0f;
	}

	// Calculate average width by sampling at multiple points
	float TotalWidth = 0.0f;
	int32 SampleCount = FMath::Min(10, FMath::Min(LeftPoints.Num(), RightPoints.Num()));
	
	for (int32 i = 0; i < SampleCount; i++)
	{
		int32 Index = i * (LeftPoints.Num() - 1) / (SampleCount - 1);
		Index = FMath::Clamp(Index, 0, FMath::Min(LeftPoints.Num() - 1, RightPoints.Num() - 1));
		
		float Width = FVector2D::Distance(LeftPoints[Index], RightPoints[Index]);
		TotalWidth += Width;
	}

	return TotalWidth / SampleCount;
}

bool URoadZoneGraphExporter::ShouldExportLane(URoadLane* Lane, const FRoadZoneGraphConfig& Config)
{
	if (!Lane)
	{
		return false;
	}

	// Check if we should only export driving lanes
	if (Config.bExportDrivingLanesOnly)
	{
		return Lane->LaneType == ELaneType::Driving;
	}

	// Export all lane types except None
	return Lane->LaneType != ELaneType::None;
}
