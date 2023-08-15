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

	// FString MaterialPath = "/Game/ProceduralPlanets/Materials/M_Planet_Inst";
	
	

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
	// Create the data for each face
	TArray<FFace> TerrainFaces;
	for (int i = 0; i < 6; i++)
	{
		TerrainFaces.Add(FTerrainFace(ShapeSettings, ShapeGenerator, ShapeSettings.Resolution, Directions[i]).ConstructMesh());
		// UE_LOG(LogTemp, Warning, TEXT("Face %d: Vertices: %d, Triangles: %d"), i, TerrainFaces[i].Vertices.Num(), TerrainFaces[i].Triangles.Num());
	}
	
	// Combine the data from each face
	FRealtimeMeshSimpleMeshData MeshData;
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
	

	// If StaticSectionKey has not yet been created, create it
	if (!bMeshInitialised)
	{
		FString MaterialPath = "/Game/ProceduralPlanets/Materials/M_Planet_Inst.M_Planet_Inst";
		if (UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *MaterialPath); IsValid(Material))
		{
			this->PlanetMaterial = UMaterialInstanceDynamic::Create(Material, this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Material not found at path: %s"), *MaterialPath);
		}
		
		
		RealtimeMesh = GetRealtimeMeshComponent()->InitializeRealtimeMesh<URealtimeMeshSimple>();

		// Enable async collision cooking
		FRealtimeMeshCollisionConfiguration CollisionSettings;
		CollisionSettings.bUseAsyncCook = true;
		RealtimeMesh->SetCollisionConfig(CollisionSettings);
		
		StaticSectionKey = RealtimeMesh->CreateMeshSection(0,
			FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Static, 0),
			MeshData, false);
		// RealtimeMesh->SetupMaterialSlot(0, "Material, Material");
		RealtimeMesh->SetupMaterialSlot(0, FName("Material"), PlanetMaterial);
		bMeshInitialised = true;
	}
	else
	{
		RealtimeMesh->UpdateSectionMesh(StaticSectionKey, MeshData);
	}

	const FMinMax& ElevationRange = ShapeGenerator.GetElevationMinMax();
	PlanetMaterial->SetScalarParameterValue("ElevationMin", ElevationRange.GetMin());
	// UE_LOG(LogTemp, Warning ,TEXT("ElevationMin: %f"), ElevationRange.GetMin());
	// UE_LOG(LogTemp, Warning, TEXT("ElevationMax: %f"), ElevationRange.GetMax());
	PlanetMaterial->SetScalarParameterValue("ElevationMax", ElevationRange.GetMax());
	PlanetMaterial->SetScalarParameterValue("Radius", ShapeSettings.PlanetRadius);
	
	MeshData.Positions.Empty();
	MeshData.Triangles.Empty();
	MeshData.Normals.Empty();

	// delete [] TerrainFaces;
	TerrainFaces.Empty();
}


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
	UE_LOG(LogTemp, Warning, TEXT("Property Changed: %s"), *PropertyChangedEvent.Property->GetName());
	// UpdateSettings();

	// Get the name of the property that was changed
	// const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	// // If we changed bGenerateAllFaces, set bMeshInitialised to false
	// if (PropertyName == GET_MEMBER_NAME_CHECKED(FShapeSettings, bGenerateAllFaces))
	// {
	// 	bMeshInitialised = false;
	// }
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