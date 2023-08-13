// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.h"
#include "INoiseFilter.h"
#include "ThirdParty/FastNoiseLite.h"

/**
 * Class to encapsulate noise filtering logic
 */
class PROCEDURALPLANETS_API FSimpleNoiseFilter : public INoiseFilter
{
public:
	FSimpleNoiseFilter() = default;
	FSimpleNoiseFilter(const FSimpleNoiseSettings& InSettings);

	// Evaluates the noise at a given 3D point.
	virtual float Evaluate(const FVector& Point) const override;

private:
	FSimpleNoiseSettings Settings;

	FastNoiseLite Noise;
};
