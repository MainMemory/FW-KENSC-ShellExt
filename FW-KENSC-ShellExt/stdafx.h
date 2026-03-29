// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files:
#include <windows.h>

#include <shlwapi.h>

#include <array>

// Windows is incredibly stupid and has dwOffset defined as an int, causing 'narrowing initializer' errors in C++11.
// This is worked-around with a converter.
typedef struct {
  const IID *piid;
  DWORD dwOffset;
} CorrectQITAB;

template<std::size_t S>
constexpr inline auto MakeQITAB(const CorrectQITAB (&arguments)[S])
{
	std::array<QITAB, S> qitabs;

	for (std::size_t i = 0; i < S; ++i)
	{
		qitabs[i].piid = arguments[i].piid;
		qitabs[i].dwOffset = arguments[i].dwOffset;
	}

	return qitabs;
}

// TODO: reference additional headers your program requires here
