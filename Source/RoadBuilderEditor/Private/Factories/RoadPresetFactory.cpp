// Copyright 2024. All Rights Reserved.

#include "Factories/RoadPresetFactory.h"
#include "RoadPreset.h"

#define LOCTEXT_NAMESPACE "RoadBuilder"

URoadPresetFactory::URoadPresetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bText = false;
	bCreateNew = true;
	bEditAfterNew = true;
	bEditorImport = false;
	SupportedClass = URoadPreset::StaticClass();
}

UObject* URoadPresetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	URoadPreset* Object = NewObject<URoadPreset>(InParent, InClass, InName, Flags | RF_Transactional);
	return Object;
}

FText FRoadPresetTypeActions::GetName() const
{
	return LOCTEXT("FRoadPresetTypeActionsName", "Road Preset");
}

UClass* FRoadPresetTypeActions::GetSupportedClass() const
{
	return URoadPreset::StaticClass();
}

#undef LOCTEXT_NAMESPACE
