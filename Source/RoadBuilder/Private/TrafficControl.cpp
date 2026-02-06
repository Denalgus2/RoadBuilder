// Copyright 2024. All Rights Reserved.

#include "TrafficControl.h"

ATrafficLightActor::ATrafficLightActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoleMesh"));
	PoleMesh->SetupAttachment(SceneRoot);

	StopVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("StopVolume"));
	StopVolume->SetupAttachment(SceneRoot);
	StopVolume->SetBoxExtent(FVector(50, 400, 100));
	StopVolume->SetRelativeLocation(FVector(200, 0, 50));
	StopVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	StopVolume->SetGenerateOverlapEvents(true);
	StopVolume->ShapeColor = FColor::Red;
}

void ATrafficLightActor::BeginPlay()
{
	Super::BeginPlay();
	CycleTimer = PhaseOffset;
	UpdateVisuals();
}

void ATrafficLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCycleEnabled)
	{
		UpdateCycle(DeltaTime);
	}
}

void ATrafficLightActor::SetState(ETrafficLightState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		UpdateVisuals();
		OnStateChanged.Broadcast(NewState);
	}
}

FVector ATrafficLightActor::GetStopLineCenter() const
{
	return StopVolume->GetComponentLocation();
}

FBox ATrafficLightActor::GetStopVolumeBox() const
{
	return StopVolume->Bounds.GetBox();
}

void ATrafficLightActor::UpdateCycle(float DeltaTime)
{
	CycleTimer += DeltaTime;
	float TotalCycle = GreenDuration + YellowDuration + RedDuration;
	float Phase = FMath::Fmod(CycleTimer, TotalCycle);

	ETrafficLightState DesiredState;
	if (Phase < GreenDuration)
		DesiredState = ETrafficLightState::Green;
	else if (Phase < GreenDuration + YellowDuration)
		DesiredState = ETrafficLightState::Yellow;
	else
		DesiredState = ETrafficLightState::Red;

	SetState(DesiredState);
}

void ATrafficLightActor::UpdateVisuals()
{
	// Visual updates would set material parameters on the light mesh
	// For now, just update the stop volume color for editor visibility
	switch (CurrentState)
	{
	case ETrafficLightState::Green:
		StopVolume->ShapeColor = FColor::Green;
		break;
	case ETrafficLightState::Yellow:
		StopVolume->ShapeColor = FColor::Yellow;
		break;
	case ETrafficLightState::Red:
		StopVolume->ShapeColor = FColor::Red;
		break;
	}
}

// --- Traffic Sign ---

ATrafficSignActor::ATrafficSignActor()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SignMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignMesh"));
	SignMesh->SetupAttachment(SceneRoot);

	DetectionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionVolume"));
	DetectionVolume->SetupAttachment(SceneRoot);
	DetectionVolume->SetBoxExtent(FVector(100, 400, 100));
	DetectionVolume->SetRelativeLocation(FVector(200, 0, 50));
	DetectionVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionVolume->SetGenerateOverlapEvents(true);
	DetectionVolume->ShapeColor = FColor::Orange;
}

FBox ATrafficSignActor::GetDetectionBox() const
{
	return DetectionVolume->Bounds.GetBox();
}

// --- Turn Arrow ---

ATurnArrowActor::ATurnArrowActor()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(SceneRoot);
}
