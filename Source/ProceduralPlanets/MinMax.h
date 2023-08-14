#pragma once

class FMinMax
{
private:
	float Min;
	float Max;

public:
	FMinMax();

	float GetMin() const;
	float GetMax() const;

	void AddValue(float V);
};