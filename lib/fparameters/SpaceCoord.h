#pragma once

#include "DimensionCoord.h"
#include <vector>

#include <cstddef>

class ParamSpace;

class SpaceCoord {
public:
	std::vector<DimensionCoord> dims;

	SpaceCoord(const ParamSpace& ps);
	explicit SpaceCoord(const size_t dimensions);
	SpaceCoord(std::initializer_list<DimensionCoord> dims);

	const DimensionCoord& operator[](const size_t& index) const;

	DimensionCoord& operator[](const size_t& index);
};

