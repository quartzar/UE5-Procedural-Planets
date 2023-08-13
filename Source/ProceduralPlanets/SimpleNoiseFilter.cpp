// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleNoiseFilter.h"


FSimpleNoiseFilter::FSimpleNoiseFilter(const FSimpleNoiseSettings& InSettings)
	: Settings(InSettings)
{
	// UE_LOG(LogTemp, Warning, TEXT("NoiseFilter Constructor. BaseRoughness: %f, NumLayers: %d"), Settings.BaseRoughness, Settings.NumLayers);
	Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}


float FSimpleNoiseFilter::Evaluate(const FVector& Point) const
{
	float NoiseValue = 0.0f;
	float Frequency = Settings.BaseRoughness;
	float Amplitude = 1.0f;

	for (int i = 0; i < Settings.NumLayers; i++)
	{
		const float V = Noise.GetNoise(Point.X * Frequency + Settings.Centre.X,
		                               Point.Y * Frequency + Settings.Centre.Y,
		                               Point.Z * Frequency + Settings.Centre.Z);
		// UE_LOG(LogTemp, Warning, TEXT("V: %f"), V);
		NoiseValue += (V + 1) * 0.5f * Amplitude;
		Frequency *= Settings.Roughness;
		Amplitude *= Settings.Persistence;
	}
	// Log Noise Value
	// UE_LOG(LogTemp, Warning, TEXT("Noise Value: %f"), NoiseValue);
	NoiseValue = FMath::Max(0.0f, NoiseValue - Settings.MinValue);
	// Log Strength
	// UE_LOG(LogTemp, Warning, TEXT("Strength: %f"), Settings.Strength);
	return NoiseValue * Settings.Strength;
}


