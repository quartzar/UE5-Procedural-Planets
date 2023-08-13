// Fill out your copyright notice in the Description page of Project Settings.


#include "RigidNoiseFilter.h"


FRigidNoiseFilter::FRigidNoiseFilter(const FRigidNoiseSettings& InSettings)
	: Settings(InSettings)
{
	// UE_LOG(LogTemp, Warning, TEXT("NoiseFilter Constructor. BaseRoughness: %f, NumLayers: %d"), Settings.BaseRoughness, Settings.NumLayers);
	Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
}


float FRigidNoiseFilter::Evaluate(const FVector& Point) const
{
	float NoiseValue = 0.0f;
	float Frequency = Settings.BaseRoughness;
	float Amplitude = 1.0f;
	float Weight = 1.0f;

	for (int i = 0; i < Settings.NumLayers; i++)
	{
		float V = 1 - FMath::Abs(Noise.GetNoise(Point.X * Frequency + Settings.Centre.X,
												Point.Y * Frequency + Settings.Centre.Y,
												Point.Z * Frequency + Settings.Centre.Z));
		V *= V;
		V *= Weight;
		Weight = FMath::Clamp(V * Settings.WeightMultiplier, 0.0f, 1.0f);

		NoiseValue += V * Amplitude;
		Frequency *= Settings.Roughness;
		Amplitude *= Settings.Persistence;
	}

	NoiseValue = FMath::Max(0.0f, NoiseValue - Settings.MinValue);
	return NoiseValue * Settings.Strength;
}


