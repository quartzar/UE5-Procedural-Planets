// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealtimeMeshSimple.h"
#include "RealtimeMeshActor.h"
// #include "NoiseFilter.h"
// #include "NoiseSettings.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"
#include "TerrainFace.h"
#include "ProceduralPlanet.generated.h"

// Define the TerrainFace struct
// struct FTerrainFace
// {
// 	TArray<FVector> Vertices;
// 	TArray<int32> Triangles;
// 	TArray<FVector> VertexNormals;
// };

UCLASS()
class PROCEDURALPLANETS_API AProceduralPlanet : public ARealtimeMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralPlanet();

	virtual void OnGenerateMesh_Implementation() override;

	void InitialiseMesh();
	// FTerrainFace GenerateMesh(FVector LocalUp, FVector AxisA, FVector AxisB);
	// void ClearMesh();
	void UpdateSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	URealtimeMeshSimple* RealtimeMesh;

	UPROPERTY(EditAnywhere, Category = "Procedural Settings")
	FShapeSettings ShapeSettings;

	// UPROPERTY(EditAnywhere, Category = "Procedural Settings")
	// FNoiseSettings NoiseSettings;

	// UPROPERTY(EditAnywhere, Category = "Procedural Settings")
	// FNoiseLayer NoiseLayer;

	// UPROPERTY(EditAnywhere, Category = "Procedural Settings", Interp, meta=(ClampMin = "1"))
	// int Resolution = 10;

	// UPROPERTY(EditAnywhere, Category = "Procedural Settings", Interp)
	// float PlanetScale = 100.f;

	

private:
	// Create Vector3 for localUp, AxisA and AxisB
	// FVector LocalUp;
	// FVector AxisA;
	// FVector AxisB;

	// FRealtimeMeshSimpleMeshData MeshData;
	FRealtimeMeshSectionKey StaticSectionKey;
	// FRealtimeMeshSimpleMeshData MeshData;

	FShapeGenerator ShapeGenerator;
	// TArray<FFace> TerrainFaces;
	// FFace* TerrainFaces;
	
	const FVector Directions[6] = {FVector::UpVector, FVector::DownVector, FVector::LeftVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector};

	bool bMeshInitialised;
	
	// Arrays to hold the vertices, triangles, normals, and colors of the mesh
	// TArray<FVector> Vertices;
	// TArray<int32> Triangles;
	// TArray<FVector> VertexNormals;
	// TArray<FLinearColor> VertexColors;
	

};
