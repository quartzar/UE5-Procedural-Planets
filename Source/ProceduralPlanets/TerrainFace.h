// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShapeGenerator.h"

/**
 * Class to handle the generation of the planet's shape
 */
struct FFace
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> VertexNormals;
};

class PROCEDURALPLANETS_API FTerrainFace
{
public:
	// FTerrainFace() = default;
	FTerrainFace(const FShapeSettings& ShapeSettings, const FShapeGenerator& InGenerator, const int& Resolution, const FVector& LocalUp);
	~FTerrainFace() = default;

	FFace ConstructMesh();
	
private:

	FShapeSettings ShapeSettings;
	const FShapeGenerator& ShapeGenerator;

	int Resolution;
	FVector LocalUp;
	FVector AxisA;
	FVector AxisB;
	
};