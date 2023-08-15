// Fill out your copyright notice in the Description page of Project Settings.


#include "ShapeGenerator.h"


FShapeGenerator::FShapeGenerator(const FShapeSettings& ShapeSettings)
	: Settings(ShapeSettings)
{
	// UE_LOG(LogTemp, Warning, TEXT("ShapeGenerator Constructor. NoiseLayers.Num: %d"), Settings.NoiseLayers.Num());

	NoiseFilters.SetNum(Settings.NoiseLayers.Num());

	for (int i = 0; i < NoiseFilters.Num(); i++)
	{
		// NoiseFilters[i] = FNoiseFilter(Settings.NoiseLayers[i].NoiseSettings);
		NoiseFilters[i] = NoiseFilterFactory::CreateNoiseFilter(Settings.NoiseLayers[i].NoiseSettings);
	}
	ElevationMinMax = FMinMax();
}


FVector FShapeGenerator::CalculatePointOnPlanet(const FVector& PointOnUnitSphere) const
{
	// UE_LOG(LogTemp, Warning, TEXT("CalculatePointOnPlanet called. NoiseFilters.Num: %d"), NoiseFilters.Num());
	
	float FirstLayerValue = 0;
	float Elevation = 0;

	if (NoiseFilters.Num() > 0 && NoiseFilters[0] != nullptr)
	{
		FirstLayerValue = NoiseFilters[0]->Evaluate(PointOnUnitSphere);
		if (Settings.NoiseLayers[0].Enabled)
		{
			Elevation = FirstLayerValue;
		}
	}

	for (int i = 1; i < NoiseFilters.Num(); i++)
	{
		if (Settings.NoiseLayers[i].Enabled && NoiseFilters[i] != nullptr)
		{
			const float Mask = (Settings.NoiseLayers[i].UseFirstLayerAsMask) ? FirstLayerValue : 1;
			Elevation += NoiseFilters[i]->Evaluate(PointOnUnitSphere) * Mask;
		}
	}

	// UE_LOG(LogTemp, Warning, TEXT("Elevation: %f"), Elevation);
	Elevation = Settings.PlanetRadius * (1 + Elevation);
	ElevationMinMax.AddValue(Elevation);
	// UE_LOG(LogTemp, Warning, TEXT("Elevation: %f, Current Min: %f, Current Max: %f"), Elevation, ElevationMinMax.GetMin(), ElevationMinMax.GetMax());


	return PointOnUnitSphere * Elevation;
}
