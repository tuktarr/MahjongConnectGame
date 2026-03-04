#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <time.h>

using namespace Wanted;

// 헬퍼(Helper) 기능 제공
namespace Util
{
	// 커서 끄기
	inline void TurnOffCursor()
	{
		// 커서 끄기
		CONSOLE_CURSOR_INFO info = {};
		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = false;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}

	// 커서 켜기
	inline void TurnOnCursor()
	{
		// 커서 끄기
		CONSOLE_CURSOR_INFO info = {};
		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = true;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}

	// 어떤 값을 두 수 사이로 고정할 때 사용하는 함수.
	template<typename T>
	T Clamp(T value, T min, T max)
	{
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}

		return value;
	}
	

}

// 메모리 정리 함수
template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

template<typename T>
void SafeDeleteArray(T*& t)
{
	if (t)
	{
		delete[] t;
		t = nullptr;
	}
}

