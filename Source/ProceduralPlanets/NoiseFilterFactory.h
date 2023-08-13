#pragma once

#include "CoreMinimal.h"
#include "INoiseFilter.h"
#include "SimpleNoiseFilter.h"
#include "RigidNoiseFilter.h"
#include "NoiseSettings.h"
#include <memory>

class NoiseFilterFactory
{
public:
	static std::unique_ptr<INoiseFilter> CreateNoiseFilter(const FNoiseSettings& Settings)
	{
		switch (Settings.FilterType)
		{
			case EFilterType::Simple:
				return std::make_unique<FSimpleNoiseFilter>(Settings.SimpleNoiseSettings);
			case EFilterType::Rigid:
				return std::make_unique<FRigidNoiseFilter>(Settings.RigidNoiseSettings);
		}
		return nullptr; // or throw an exception if unsupported types should be an error
	}
};