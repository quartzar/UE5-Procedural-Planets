#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.generated.h"

UENUM()
enum class EFilterType : uint8 {
	Simple,
	Rigid
};

USTRUCT()
struct PROCEDURALPLANETS_API FSimpleNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings")
	float BaseRoughness = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings", meta = (ClampMin = "0", ClampMax = "8"))
	int NumLayers = 1;

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings")
	float Roughness = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings")
	float Persistence = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings")
	FVector Centre = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings")
	float MinValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Simple Noise Settings")
	float Strength = 1.0f;
};

USTRUCT()
struct PROCEDURALPLANETS_API FRigidNoiseSettings : public FSimpleNoiseSettings {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ridgid Noise Settings")
	float WeightMultiplier = 0.8f;
};

USTRUCT()
struct PROCEDURALPLANETS_API FNoiseSettings {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Noise Settings")
	EFilterType FilterType;

	UPROPERTY(EditAnywhere, Category = "Noise Settings", meta = (EditCondition = "FilterType == EFilterType::Simple"))
	FSimpleNoiseSettings SimpleNoiseSettings;

	UPROPERTY(EditAnywhere, Category = "Noise Settings", meta = (EditCondition = "FilterType == EFilterType::Rigid"))
	FRigidNoiseSettings RigidNoiseSettings;
};