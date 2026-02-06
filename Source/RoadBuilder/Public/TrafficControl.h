// Copyright 2024. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "RoadPreset.h"
#include "TrafficControl.generated.h"

UENUM(BlueprintType)
enum class ETrafficLightState : uint8
{
	Red			UMETA(DisplayName = "Red"),
	Yellow		UMETA(DisplayName = "Yellow"),
	Green		UMETA(DisplayName = "Green"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrafficLightStateChanged, ETrafficLightState, NewState);

UCLASS(BlueprintType)
class ROADBUILDER_API ATrafficLightActor : public AActor
{
	GENERATED_BODY()
public:
	ATrafficLightActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PoleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* StopVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	ETrafficLightState CurrentState = ETrafficLightState::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	float GreenDuration = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	float YellowDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	float RedDuration = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	float PhaseOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	bool bCycleEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Light")
	int32 GateIndex = INDEX_NONE;

	UPROPERTY(BlueprintAssignable, Category = "Traffic Light")
	FOnTrafficLightStateChanged OnStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Traffic Light")
	void SetState(ETrafficLightState NewState);

	UFUNCTION(BlueprintPure, Category = "Traffic Light")
	bool IsGreen() const { return CurrentState == ETrafficLightState::Green; }

	UFUNCTION(BlueprintPure, Category = "Traffic Light")
	bool IsRed() const { return CurrentState == ETrafficLightState::Red; }

	UFUNCTION(BlueprintPure, Category = "Traffic Light")
	bool IsLaneOpen() const { return CurrentState == ETrafficLightState::Green; }

	UFUNCTION(BlueprintPure, Category = "Traffic Light")
	FVector GetStopLineCenter() const;

	UFUNCTION(BlueprintPure, Category = "Traffic Light")
	FBox GetStopVolumeBox() const;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float CycleTimer = 0.0f;
	void UpdateCycle(float DeltaTime);
	void UpdateVisuals();
};

UCLASS(BlueprintType)
class ROADBUILDER_API ATrafficSignActor : public AActor
{
	GENERATED_BODY()
public:
	ATrafficSignActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SignMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* DetectionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign")
	ETrafficControlType SignType = ETrafficControlType::StopSign;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign")
	int32 GateIndex = INDEX_NONE;

	UFUNCTION(BlueprintPure, Category = "Sign")
	bool IsStopSign() const { return SignType == ETrafficControlType::StopSign; }

	UFUNCTION(BlueprintPure, Category = "Sign")
	bool IsYieldSign() const { return SignType == ETrafficControlType::YieldSign; }

	UFUNCTION(BlueprintPure, Category = "Sign")
	FBox GetDetectionBox() const;
};

UCLASS(BlueprintType)
class ROADBUILDER_API ATurnArrowActor : public AActor
{
	GENERATED_BODY()
public:
	ATurnArrowActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	ETurnArrowType ArrowType = ETurnArrowType::Through;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	int32 GateIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	int32 LaneIndex = INDEX_NONE;
};
