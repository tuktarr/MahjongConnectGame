#pragma once
#include <Windows.h>
#include "Common/Common.h"
#include "Math/Vector2.h"

namespace Wanted
{
	class WANTED_API Input
	{
		friend class Engine;

		struct KeyState
		{
			// 현재 키가 눌렸는지.
			bool isKeyDown = false;

			// 이전에 키가 눌렸는지.
			bool wasKeyDown = false;
		};

	private:
		Input();
		~Input();

	public:
		// 입력 확인 함수.
		// 이전에 입력이 안됐는데, 현재 입력이 됐으면 1번 호출.
		bool GetButtonDown(int buttonCode);

		// 이전에 입력이 됐는데, 현재 입력이 취소됐으면 1번 호출.
		bool GetButtonUp(int buttonCode);

		// 현재 눌려있으면 반복 호출.
		bool GetButton(int buttonCode);

		// 전역적으로 접근하는 함수.
		static Input& Get();

		// 마우스 커서 위치 확인하는 함수
		inline Vector2 GetMousePosition() const { return mousePosition; }


	private:
		// 입력 처리 함수.
		void ProcessInput();

		// 기존 입력 저장 함수.
		void SavePreviousInputStates();

	private:
		// 키 상태 저장용 배열.
		KeyState keyStates[256] = { };
		
		// 마우스 좌표 (x,y)
		Vector2 mousePosition;

		// 전역적으로 접근하도록 만들기 위한 전역 변수.
		static Input* instance;

		HANDLE mouseInputHandle; // 핸들 멤버 변수 저장
	};
}
