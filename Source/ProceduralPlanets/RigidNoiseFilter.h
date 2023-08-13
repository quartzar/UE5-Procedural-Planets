// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.h"
#include "INoiseFilter.h"
#include "ThirdParty/FastNoiseLite.h"

/**
 * Class to encapsulate noise filtering logic
 */
class PROCEDURALPLANETS_API FRigidNoiseFilter : public INoiseFilter
{
public:
	FRigidNoiseFilter() = default;
	FRigidNoiseFilter(const FRigidNoiseSettings& InSettings);

	// Evaluates the noise at a given 3D point.
	virtual float Evaluate(const FVector& Point) const override;

private:
	FRigidNoiseSettings Settings;
	FastNoiseLite Noise;
};
