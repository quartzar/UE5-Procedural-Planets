// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralPlanet.h"


// Sets default values
AProceduralPlanet::AProceduralPlanet()
	: ShapeSettings(FShapeSettings())//, TerrainFace(ShapeSettings)//ShapeGenerator(ShapeSettings)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create DefaultSceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	bMeshInitialised = false;
	

	// Initialize ShapeSettings and NoiseSettings with default values
	// ShapeSettings = FShapeSettings();
	// NoiseSettings = FNoiseSettings();
	
}


void AProceduralPlanet::OnGenerateMesh_Implementation()
{
	Super::OnGenerateMesh_Implementation();


	bMeshInitialised = false;
	UpdateSettings();
	InitialiseMesh();
}


// Create initial mesh and SectionKey
void AProceduralPlanet::InitialiseMesh() 
{
	// ClearMesh();

	// if (!bMeshInitialised)
	// 	RealtimeMesh = GetRealtimeMeshComponent()->InitializeRealtimeMesh<URealtimeMeshSimple>();
	
	// TerrainFaces.SetNum(6);

	TArray<FFace> TerrainFaces;
	// FFace* TerrainFaces = new FFace[6];
	
	for (int i = 0; i < 6; i++)
	{
		// FVector LocalUp = Directions[i];
		// FVector AxisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
		// FVector AxisB = FVector::CrossProduct(LocalUp, AxisA);

		// Generate the mesh for each face
		// TerrainFaces.Add(GenerateMesh(LocalUp, AxisA, AxisB));
		// TerrainFaces[i] = FTerrainFace(ShapeSettings, ShapeSettings.Resolution, Directions[i]).ConstructMesh();
		TerrainFaces.Add(FTerrainFace(ShapeSettings, ShapeSettings.Resolution, Directions[i]).ConstructMesh());
	}


	FRealtimeMeshSimpleMeshData MeshData;
	
	// Combine the data from each face
	for	(int i = 0; i < 6; i++)
	{
		const int32 VertexOffset = MeshData.Positions.Num();
		MeshData.Positions.Append(TerrainFaces[i].Vertices);
		MeshData.Normals.Append(TerrainFaces[i].VertexNormals);

		for (const int32 TriIndex : TerrainFaces[i].Triangles)
		{
			MeshData.Triangles.Add(TriIndex + VertexOffset);
		}
	}
	
	
	
	// for (const FFace& TerrainFace : TerrainFaces)
	// {
	// 	const int32 VertexOffset = MeshData.Positions.Num();
	// 	MeshData.Positions.Append(TerrainFace.Vertices);
	// 	MeshData.Normals.Append(TerrainFace.VertexNormals);
	//
	// 	for (const int32 TriIndex : TerrainFace.Triangles)
	// 	{
	// 		MeshData.Triangles.Add(TriIndex + VertexOffset);
	// 	}
	// }
	
	
	// MeshData.Positions = Vertices;
	// MeshData.Triangles = Triangles;
	// MeshData.Normals = VertexNormals;
	
	// const int MeshDataArraySize = MeshData.Positions.Num();
	// // MeshData.Normals.SetNumZeroed(MeshDataArraySize);
	// MeshData.LinearColors.SetNumZeroed(MeshDataArraySize);
	// MeshData.Tangents.SetNumZeroed(MeshDataArraySize);
	// MeshData.UV0.SetNumZeroed(MeshDataArraySize);

	// If StaticSectionKey has not yet been created, create it
	if (!bMeshInitialised)
	{
		RealtimeMesh = GetRealtimeMeshComponent()->InitializeRealtimeMesh<URealtimeMeshSimple>();

		// Enable async collision cooking
		FRealtimeMeshCollisionConfiguration CollisionSettings;
		CollisionSettings.bUseAsyncCook = true;
		RealtimeMesh->SetCollisionConfig(CollisionSettings);
		
		StaticSectionKey = RealtimeMesh->CreateMeshSection(0,
			FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Static, 0),
			MeshData, false);
		RealtimeMesh->SetupMaterialSlot(0, "Material, Material");
		bMeshInitialised = true;
	}
	else
	{
		RealtimeMesh->UpdateSectionMesh(StaticSectionKey, MeshData);
	}

	MeshData.Positions.Empty();
	MeshData.Triangles.Empty();
	MeshData.Normals.Empty();

	// delete [] TerrainFaces;
	TerrainFaces.Empty();
}





// Cube-sphere mesh generation function
// FTerrainFace AProceduralPlanet::GenerateMesh(FVector LocalUp, FVector AxisA, FVector AxisB) 
// {
// 	int Resolution = ShapeSettings.Resolution;
// 	FTerrainFace TerrainFace = FTerrainFace();
//
// 	TerrainFace.Vertices.SetNum(Resolution * Resolution);
// 	TerrainFace.Triangles.SetNum((Resolution - 1) * (Resolution - 1) * 6);
// 	TerrainFace.VertexNormals.SetNum(Resolution * Resolution);
// 	int TriIndex = 0;
// 	
// 	for (int y = 0; y < Resolution; y++)
// 	{
// 		for (int x = 0; x < Resolution; x++)
// 		{
// 			const int i = x + y * Resolution;
// 			FVector2D Percent = FVector2D(x, y) / (Resolution - 1);
// 			FVector PointOnUnitCube = LocalUp + (Percent.X - .5f) * 2 * AxisA + (Percent.Y - .5f) * 2 * AxisB;
// 			FVector PointOnUnitSphere = PointOnUnitCube.GetSafeNormal();
// 			FVector PointOnPlanet = ShapeGenerator.CalculatePointOnPlanet(PointOnUnitSphere);
// 			TerrainFace.Vertices[i] = PointOnPlanet;
//
// 			if (x != Resolution - 1 && y != Resolution - 1)
// 			{
// 				TerrainFace.Triangles[TriIndex + 0] = i;
// 				TerrainFace.Triangles[TriIndex + 1] = i + Resolution;
// 				TerrainFace.Triangles[TriIndex + 2] = i + Resolution + 1;
// 				
// 				TerrainFace.Triangles[TriIndex + 3] = i;
// 				TerrainFace.Triangles[TriIndex + 4] = i + Resolution + 1;
// 				TerrainFace.Triangles[TriIndex + 5] = i + 1;
// 				TriIndex += 6;
// 			}
// 			
// 		}
// 	}
//
// 	int32 Index0;
// 	int32 Index1;
// 	int32 Index2;
// 	FVector Vertex0;
// 	FVector Vertex1;
// 	FVector Vertex2;
// 	FVector Edge1;
// 	FVector Edge2;
// 	FVector FaceNormal;
// 	// Set the vertex normals to the normal of the face they are part of
// 	for (int i = 0; i < TerrainFace.Triangles.Num(); i += 3)
// 	{
// 		Index0 = TerrainFace.Triangles[i];
// 		Index1 = TerrainFace.Triangles[i + 1];
// 		Index2 = TerrainFace.Triangles[i + 2];
// 	
// 		Vertex0 = TerrainFace.Vertices[Index0];
// 		Vertex1 = TerrainFace.Vertices[Index1];
// 		Vertex2 = TerrainFace.Vertices[Index2];
// 	
// 		// Calculate the normal for this triangle
// 		Edge1 = Vertex1 - Vertex0;
// 		Edge2 = Vertex0 - Vertex2;
// 		FaceNormal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();
// 	
// 		// Set the vertex normals for the vertices that made up the triangle
// 		TerrainFace.VertexNormals[Index0] = FaceNormal;
// 		TerrainFace.VertexNormals[Index1] = FaceNormal;
// 		TerrainFace.VertexNormals[Index2] = FaceNormal;
// 	}
//
// 	// Normalize the vertex normals
// 	// for (int i = 0; i < TerrainFace.Vertices.Num(); i++)
// 	// {
// 	// 	TerrainFace.VertexNormals[i] = TerrainFace.VertexNormals[i].GetSafeNormal();
// 	// }
// 	
// 	return TerrainFace;
// }

// Clear previous mesh data from MeshData
// void AProceduralPlanet::ClearMesh()
// {
// 	Vertices.Empty();
// 	Triangles.Empty();
// 	VertexNormals.Empty();
// }

void AProceduralPlanet::UpdateSettings()
{
	ShapeGenerator = FShapeGenerator(ShapeSettings);
}


#if WITH_EDITOR
// Checks for parameter changes in the editor
void AProceduralPlanet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// Log the name of the property that was changed
	// UE_LOG(LogTemp, Warning, TEXT("Property Changed: %s"), *PropertyChangedEvent.Property->GetName());
	UpdateSettings();
	InitialiseMesh();
	
}
#endif


// Called when the game starts or when spawned
void AProceduralPlanet::BeginPlay()
{
	Super::BeginPlay();

	// Create RealtimeMesh
	InitialiseMesh(); // Generate initial mesh
	
}

// Called every frame
void AProceduralPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//-----------------------------------------------------------------------------------------------
// // Old Functions

// void AProceduralPlanet::GenerateMesh()	// Basic square for testing
// {
// 	// Loop to create vertices and normals for the grid
// 	for (int y = 0; y <= GridSize; y++)
// 	{
// 		for (int x = 0; x <= GridSize; x++)
// 		{
// 			Vertices.Add(FVector(x, y, 0)); // Add vertex
// 			VertexNormals.Add(FVector(0, 0, 1)); // Add normal
// 			// VertexColors.Add(FLinearColor::White); // Add white color
// 		}
// 	}
//
// 	// Loop to define the triangles for the grid
// 	for (int y = 0; y < GridSize; y++)
// 	{
// 		for (int x = 0; x < GridSize; x++)
// 		{
// 			int Index = y * (GridSize + 1) + x;
// 			Triangles.Add(Index);
// 			Triangles.Add(Index + GridSize + 2);
// 			Triangles.Add(Index + 1);
// 			Triangles.Add(Index);
// 			Triangles.Add(Index + GridSize + 1);
// 			Triangles.Add(Index + GridSize + 2);
// 		}
// 	}
// }


// void AProceduralPlanet::OnGenerateMesh_Implementation()
// {
// 	Super::OnGenerateMesh_Implementation();
//
// 	GenerateMesh();	// Generate the mesh
// 	
// 	RealtimeMesh = GetRealtimeMeshComponent()->InitializeRealtimeMesh<URealtimeMeshSimple>();
// 	FRealtimeMeshSimpleMeshData MeshData;
// 	
// 	MeshData.Positions = Vertices;
// 	MeshData.Triangles = Triangles;
// 	MeshData.Normals = VertexNormals;
// 	
// 	const int MeshDataArraySize = MeshData.Positions.Num();
// 	
// 	// Zero out uninitialised arrays
// 	MeshData.LinearColors.SetNumZeroed(MeshDataArraySize);
// 	MeshData.Tangents.SetNumZeroed(MeshDataArraySize);
// 	MeshData.UV0.SetNumZeroed(MeshDataArraySize);
// 	
// 	FRealtimeMeshSectionKey StaticSectionKey = RealtimeMesh->CreateMeshSection(
// 		0, FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Dynamic, 0), MeshData, true);
// 	RealtimeMesh->SetupMaterialSlot(0, "Material, Material");
// 	
// }


// Basic square for testing
// void AProceduralPlanet::GenerateMesh()	
// {
// 	// Loop to create vertices and normals for the grid
// 	for (int y = 0; y <= GridSize; y++)
// 	{
// 		for (int x = 0; x <= GridSize; x++)
// 		{
// 			MeshData.Positions.Add(FVector(x, y, 0)); // Add vertex
// 			MeshData.Normals.Add(FVector(0, 0, 1)); // Add normal
// 			// VertexColors.Add(FLinearColor::White); // Add white color
// 		}
// 	}
//
// 	// Loop to define the triangles for the grid
// 	for (int y = 0; y < GridSize; y++)
// 	{
// 		for (int x = 0; x < GridSize; x++)
// 		{
// 			int Index = y * (GridSize + 1) + x;
// 			MeshData.Triangles.Add(Index);
// 			MeshData.Triangles.Add(Index + GridSize + 2);
// 			MeshData.Triangles.Add(Index + 1);
// 			MeshData.Triangles.Add(Index);
// 			MeshData.Triangles.Add(Index + GridSize + 1);
// 			MeshData.Triangles.Add(Index + GridSize + 2);
// 		}
// 	}
// }