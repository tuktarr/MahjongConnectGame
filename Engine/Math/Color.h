#pragma once

#include "Common/Common.h"
#include <Windows.h>

namespace Wanted
{
	// 콘솔에 텍스트 색상 등을 지정할 때, 사용함
	// 색상 열거형
	// 보통 RGB 순서 -> MS만 BGR
	enum class WANTED_API Color : unsigned short
	{
        Black = 0,

        // === 어두운 색 (기본) ===
        DarkBlue = FOREGROUND_BLUE,
        DarkGreen = FOREGROUND_GREEN,
        DarkRed = FOREGROUND_RED,
        DarkCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
        Brown = FOREGROUND_RED | FOREGROUND_GREEN,      // 어두운 노랑 -> 갈색(구리색 대용)
        Gray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // 어두운 흰색 -> 회색/은색(Silver)

        // === 밝은 색 (Intensity 포함) ===
        DarkGray = FOREGROUND_INTENSITY,                   // 검정 + 밝음 -> 짙은 회색(Coal)
        Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // 하늘색(Diamond)
        Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
        Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,  // 밝은 노랑(Gold)
        White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY // 밝은 흰색(Platinum)
	}; 
}