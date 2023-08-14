// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleNoiseFilter.h"
// #include "NoiseSettings.h"
#include "NoiseFilterFactory.h"
#include "MinMax.h"
#include "ShapeSettings.h"

/**
 * Class to handle the generation of the planet's shape
 */
class PROCEDURALPLANETS_API FShapeGenerator
{
public:
	FShapeGenerator() = default;
	FShapeGenerator(const FShapeSettings& ShapeSettings);

	FVector CalculatePointOnPlanet(const FVector& PointOnUnitSphere) const;
	

	const FMinMax& GetElevationMinMax() const { return ElevationMinMax; }
	
private:
	FShapeSettings Settings;
	mutable FMinMax ElevationMinMax;
	// FNoiseSettings NoiseSettings;
	
	// TArray<FNoiseFilter> NoiseFilters;
	TArray<std::unique_ptr<INoiseFilter>> NoiseFilters;
};