#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.h"
#include "ShapeSettings.generated.h"

USTRUCT()
struct PROCEDURALPLANETS_API FNoiseLayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Noise Layer")
	bool Enabled = true;

	UPROPERTY(EditAnywhere, Category = "Noise Layer")
	bool UseFirstLayerAsMask;

	UPROPERTY(EditAnywhere, Category = "Noise Layer")
	FNoiseSettings NoiseSettings;
};

USTRUCT()
struct PROCEDURALPLANETS_API FShapeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Shape Settings", meta=(ClampMin = "1"))
	int Resolution = 10;
	
	UPROPERTY(EditAnywhere, Category = "Shape Settings")
	int PlanetRadius = 500;

	UPROPERTY(EditAnywhere, Category = "Shape Settings")
	TArray<FNoiseLayer> NoiseLayers;
};
