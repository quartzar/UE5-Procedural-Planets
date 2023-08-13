#pragma once

#include "CoreMinimal.h"

USTRUCT()
struct PROCEDURALPLANETS_API FColorSettings
{
	GENERATED_USTRUCT_BODY()

	// Gradient information (could be represented by an array of FLinearColor for simplicity)
	UPROPERTY(EditAnywhere, Category = "Color Settings")
	TArray<FLinearColor> GradientColors;

	// You can use a curve to interpolate colors between the gradient keys
	UPROPERTY(EditAnywhere, Category = "Color Settings")
	UCurveFloat* GradientCurve;

	// Reference to the material used for the planet
	UPROPERTY(EditAnywhere, Category = "Color Settings")
	UMaterialInterface* PlanetMaterial;
};