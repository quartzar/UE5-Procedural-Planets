#include "MinMax.h"

#include <cfloat>

FMinMax::FMinMax()
	: Min(FLT_MAX), Max(FLT_MIN)
{}

float FMinMax::GetMin() const 
{
	return Min;
}

float FMinMax::GetMax() const 
{
	return Max;
}

void FMinMax::AddValue(float V)
{
	if (V > Max)
	{
		Max = V;
	}
	if (V < Min)
	{
		Min = V;
	}
}