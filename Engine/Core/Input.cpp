#include "Input.h"
#include <Windows.h>
#include <iostream>
#include <cassert>
#include "Util/Util.h"
#include "Engine/Engine.h"


namespace Wanted
{
	Input* Input::instance = nullptr;

	Input::Input()
		: mouseInputHandle(nullptr)
	{
		// 객체가 초기화되면 자기 자신의 주소를 저장.
		instance = this;

		// 입력 핸들 얻어오기 & 마우스 입력 활성화 설정.
		mouseInputHandle = GetStdHandle(STD_INPUT_HANDLE);

		// 마우스 이벤트 활성화
		DWORD mode = 0;
		
		// 기존 설정 가져오기
		GetConsoleMode(mouseInputHandle, &mode);

		// 기존 설정에 마우스 기능 추가 (OR 연산)
		mode |= (ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT);

		// 3. 빠른 편집 모드 제거 (AND NOT 연산)
		mode &= ~ENABLE_QUICK_EDIT_MODE;


		if (SetConsoleMode(mouseInputHandle, mode) == FALSE)
		{
			MessageBoxA(nullptr, "Failed to set console mode", "Error", MB_OK);
			__debugbreak();
		}
	}
	
	Input::~Input()
	{
		if (instance == this)
		{
			instance = nullptr;
		}
	}

	bool Input::GetButtonDown(int buttonCode)
	{
		assert(buttonCode >= 0 && buttonCode < 256);

		return keyStates[buttonCode].isKeyDown
			&& !keyStates[buttonCode].wasKeyDown;
	}
	bool Input::GetButtonUp(int buttonCode)
	{
		assert(buttonCode >= 0 && buttonCode < 256);

		return !keyStates[buttonCode].isKeyDown
			&& keyStates[buttonCode].wasKeyDown;
	}
	bool Input::GetButton(int buttonCode)
	{
		assert(buttonCode >= 0 && buttonCode < 256);

		return keyStates[buttonCode].isKeyDown;
	}

	Input& Input::Get()
	{
		if (!instance)
		{
			std::cout << "Error: Input::Get(). instance is null\n";

			__debugbreak();
		}
		return *instance;
	}

	void Input::ProcessInput()
	{

		// 대기 중인 이벤트 개수 확인
		DWORD eventCount = 0;
		GetNumberOfConsoleInputEvents(mouseInputHandle, &eventCount);

		if (eventCount == 0)
		{
			return;
		}

		// 이벤트를 읽어올 버퍼
		static const int recordLimit = 128;
		INPUT_RECORD records[recordLimit];
		DWORD eventsRead = 0;
		
		if (ReadConsoleInput(mouseInputHandle, records, recordLimit, &eventsRead))
		{
			for (DWORD i = 0; i < eventsRead; ++i)
			{
				INPUT_RECORD& record = records[i];

				switch (record.EventType)
				{
					// 키 이벤트인 경우.
				case KEY_EVENT:
				{
					// 키보드 상태 업데이트
										// wVirtualKeyCode는 VK_ESCAPE, VK_SPACE 등 가상 키 코드입니다.
					WORD keyParams = record.Event.KeyEvent.wVirtualKeyCode;

					if (keyParams < 256) // 배열 범위 안전 장치
					{
						keyStates[keyParams].isKeyDown = record.Event.KeyEvent.bKeyDown;
					}
				}
				break;

				// 마우스 이벤트 처리.
				case MOUSE_EVENT:
				{
					//// [디버깅] 원본 좌표 확인
					//int rawX = record.Event.MouseEvent.dwMousePosition.X;
					//int rawY = record.Event.MouseEvent.dwMousePosition.Y;

					//// [디버깅] 버튼 상태 확인 (비트 마스크)
					//DWORD buttonState = record.Event.MouseEvent.dwButtonState;
					//bool isLeftDown = (buttonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

					// 마우스 x 위치 설정.
					mousePosition.x = record.Event.MouseEvent.dwMousePosition.X;
					mousePosition.y = record.Event.MouseEvent.dwMousePosition.Y;

					// x좌표 조정 (마우스 커서가 화면 안에 고정되도록).
					mousePosition.x
						= Util::Clamp<int>(mousePosition.x, 0, Engine::Get().GetWidth() - 1);

					// y좌표 조정.
					mousePosition.y
						= Util::Clamp<int>(mousePosition.y, 0, Engine::Get().GetHeight() - 1);

					// 마우스 클릭 여부 상태 저장.
					bool isLeftPressed = (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

					keyStates[VK_LBUTTON].isKeyDown = isLeftPressed;

					// [보완] 만약 더블 클릭도 인식하고 싶다면:
					if (record.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
					{
						keyStates[VK_LBUTTON].isKeyDown = true;
					}
				}
				break;
				}
			}
		}
	}
	
	void Input::SavePreviousInputStates()
	{
		// 현재 입력 값을 이전 입력 값으로 저장.
		for (int ix = 0; ix < 256; ++ix)
		{
			keyStates[ix].wasKeyDown
				= keyStates[ix].isKeyDown;
		}
	}
}
