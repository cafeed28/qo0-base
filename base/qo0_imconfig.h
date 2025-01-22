#pragma once

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_ENABLE_FREETYPE

#define IM_VEC2_CLASS_EXTRA              \
	ImVec2& operator+(const float flAdd) \
	{                                    \
		x += flAdd;                      \
		y += flAdd;                      \
		return *this;                    \
	}                                    \
	ImVec2& operator-(const float flSub) \
	{                                    \
		x -= flSub;                      \
		y -= flSub;                      \
		return *this;                    \
	}

#define IM_VEC4_CLASS_EXTRA            \
	float operator[](size_t idx) const \
	{                                  \
		IM_ASSERT(idx <= 3);           \
		return (&x)[idx];              \
	}                                  \
	float& operator[](size_t idx)      \
	{                                  \
		IM_ASSERT(idx <= 3);           \
		return (&x)[idx];              \
	}
