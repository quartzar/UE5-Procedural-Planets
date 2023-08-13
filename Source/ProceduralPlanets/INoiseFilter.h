#pragma once

#include "CoreMinimal.h"

class INoiseFilter {
public:
	virtual float Evaluate(const FVector& Point) const = 0;
	virtual ~INoiseFilter() = default;
};