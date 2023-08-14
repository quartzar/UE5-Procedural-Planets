// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealtimeMeshSimple.h"
#include "RealtimeMeshActor.h"
// #include "ColourSettings.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"
#include "TerrainFace.h"
#include "ProceduralPlanet.generated.h"


UCLASS()
class PROCEDURALPLANETS_API AProceduralPlanet : public ARealtimeMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralPlanet();

	virtual void OnGenerateMesh_Implementation() override;

	void InitialiseMesh();
	
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

	// UPROPERTY(EditAnywhere, Category = "Planet Settings")
	// FColorSettings ColorSettings;
	
	UPROPERTY(VisibleAnywhere, Category = "Procedural Settings")
	UMaterialInstanceDynamic* PlanetMaterial;

private:
	FRealtimeMeshSectionKey StaticSectionKey;

	FShapeGenerator ShapeGenerator;
	
	const FVector Directions[6] = {FVector::UpVector, FVector::DownVector, FVector::LeftVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector};

	bool bMeshInitialised;
	
};
