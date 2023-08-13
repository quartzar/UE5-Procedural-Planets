// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleNoiseFilter.h"
// #include "NoiseSettings.h"
#include "NoiseFilterFactory.h"
#include "ShapeSettings.h"

/**
 * Class to handle the generation of the planet's shape
 */
class PROCEDURALPLANETS_API FShapeGenerator
{
public:
	FShapeGenerator() = default;
	FShapeGenerator(const FShapeSettings& ShapeSettings);

	FVector CalculatePointOnPlanet(const FVector& PointOnUnitSphere);

private:
	FShapeSettings Settings;
	// FNoiseSettings NoiseSettings;
	
	// TArray<FNoiseFilter> NoiseFilters;
	TArray<std::unique_ptr<INoiseFilter>> NoiseFilters;
};