#ifndef UTILITY_H
#define UTILITY_H

#include "header.h"
#include "templates.h"
#include <utility>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

using Vector2i = std::pair<int, int>;
using BoundingBox2i = std::pair<Vector2i, Vector2i>;

namespace utility
{
	constexpr std::size_t NUMBER_OF_CREATURES = 128;
	constexpr unsigned int DEFAULT_SPEED = 1;
	constexpr unsigned int DEFAULT_AGGRO_RANGE = 150;
	inline bool contains(const BoundingBox2i& box, const Vector2i& point)
	{
		bool x_aligned = box.first.first <= point.first && box.second.first >= point.first;
		bool y_aligned = box.first.second <= point.second && box.second.second >= point.second;
		return x_aligned && y_aligned;
	}

	inline bool intersects(const BoundingBox2i& box1, const BoundingBox2i& box2)
	{
		return box1.first.first < box2.second.first && box1.second.first > box2.first.first && box1.first.second > box2.first.second && box1.second.second < box2.second.second;
	}
	
	inline void draw_bounding_box(const BoundingBox2i& box, BaseEngine* engine, unsigned int colour = 0xffff00)
	{
		engine->DrawForegroundRectangle(box.first.first, box.first.second, box.second.first, box.second.second, colour);
		//StoreLastScreenPositionForUndraw();
	}

	/// Split string by non-character delimiter
	inline std::vector<std::string> split_string(const std::string& string, const std::string& delimiter)
	{
		std::vector<std::string> v;
		// Start of an element.
		std::size_t element_start = 0;
		// We start searching from the end of the previous element, which
		// initially is the start of the string.
		std::size_t element_end = 0;
		// Find the first non-delim, i.e. the start of an element, after the end of the previous element.
		while ((element_start = string.find_first_not_of(delimiter, element_end)) != std::string::npos)
		{
			// Find the first delem, i.e. the end of the element (or if this fails it is the end of the string).
			element_end = string.find_first_of(delimiter, element_start);
			// Add it.
			v.emplace_back(string, element_start, element_end == std::string::npos ? std::string::npos : element_end - element_start);
		}
		// When there are no more non-spaces, we are done.
		return v;
	}
	/// Not C++17 so no std::clamp. Just going to write it myself.
	/// No constexpr-if because no C++17, just deal with runtime if.
	template<typename T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		if (v < lo)
			return lo;
		else if (v > hi)
			return hi;
		else
			return v;
	}
}

#endif

