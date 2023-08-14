// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainFace.h"


FTerrainFace::FTerrainFace(const FShapeSettings& ShapeSettings, const FShapeGenerator& InGenerator, const int& Resolution, const FVector& LocalUp)
	: ShapeSettings(ShapeSettings), ShapeGenerator(InGenerator), Resolution(Resolution), LocalUp(LocalUp)
{
	AxisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X).GetSafeNormal();
	AxisB = FVector::CrossProduct(LocalUp, AxisA);
	// this->ShapeGenerator = InGenerator;
}


FFace FTerrainFace::ConstructMesh() 
{
	FFace TerrainFace = FFace();

	TerrainFace.Vertices.SetNum(Resolution * Resolution);
	TerrainFace.Triangles.SetNum((Resolution - 1) * (Resolution - 1) * 6);
	TerrainFace.VertexNormals.SetNum(Resolution * Resolution);
	int TriIndex = 0;
	
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			const int i = x + y * Resolution;
			FVector2D Percent = FVector2D(x, y) / (Resolution - 1);
			FVector PointOnUnitCube = LocalUp + (Percent.X - .5f) * 2 * AxisA + (Percent.Y - .5f) * 2 * AxisB;
			FVector PointOnUnitSphere = PointOnUnitCube.GetSafeNormal();
			FVector PointOnPlanet = ShapeGenerator.CalculatePointOnPlanet(PointOnUnitSphere);
			TerrainFace.Vertices[i] = PointOnPlanet;

			if (x != Resolution - 1 && y != Resolution - 1)
			{
				TerrainFace.Triangles[TriIndex + 0] = i;
				TerrainFace.Triangles[TriIndex + 1] = i + Resolution;
				TerrainFace.Triangles[TriIndex + 2] = i + Resolution + 1;
				
				TerrainFace.Triangles[TriIndex + 3] = i;
				TerrainFace.Triangles[TriIndex + 4] = i + Resolution + 1;
				TerrainFace.Triangles[TriIndex + 5] = i + 1;
				TriIndex += 6;
			}
		}
	}

	int32 Index0;	int32 Index1;	int32 Index2;
	FVector Vertex0;FVector Vertex1;FVector Vertex2;
	FVector Edge1;	FVector Edge2;	FVector FaceNormal;
	// Set the vertex normals to the normal of the face they are part of
	for (int i = 0; i < TerrainFace.Triangles.Num(); i += 3)
	{
		Index0 = TerrainFace.Triangles[i];
		Index1 = TerrainFace.Triangles[i + 1];
		Index2 = TerrainFace.Triangles[i + 2];
	
		Vertex0 = TerrainFace.Vertices[Index0];
		Vertex1 = TerrainFace.Vertices[Index1];
		Vertex2 = TerrainFace.Vertices[Index2];
	
		// Calculate the normal for this triangle
		Edge1 = Vertex1 - Vertex0;
		Edge2 = Vertex0 - Vertex2;
		FaceNormal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();
	
		// Set the vertex normals for the vertices that made up the triangle
		TerrainFace.VertexNormals[Index0] = FaceNormal;
		TerrainFace.VertexNormals[Index1] = FaceNormal;
		TerrainFace.VertexNormals[Index2] = FaceNormal;
	}
	
	return TerrainFace;
}

// Normalize the vertex normals
// for (int i = 0; i < TerrainFace.Vertices.Num(); i++)
// {
// 	TerrainFace.VertexNormals[i] = TerrainFace.VertexNormals[i].GetSafeNormal();
// }