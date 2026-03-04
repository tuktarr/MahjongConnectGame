#pragma once

#include "Common/Common.h"

namespace Wanted
{
	// 전방 선언.
	class Input;
	class Renderer;

	// Main game engine class.
	class WANTED_API Engine
	{
		// 엔진 설정 구조체
		struct EngineSetting
		{
			// 프레임 속도
			float framerate = 0.0f;
			
			// 화면 너비
			int width = 0;

			// 화면 높이
			int height = 0;
		};

	public:
		// 소멸자 virtual -> 호출순서, 소멸순서 생각 ! 부모소멸자 호출되고 자손 소멸자도 호출하기 위해서
		Engine();
		virtual ~Engine();

		// 엔진 루프(게임 루프).
		void Run();

		// 엔진 종료 함수.
		void QuitEngine();

		// 새 레벨을 추가(설정)하는 함수.
		void SetNewLevel(class Level* newLevel);

		// 전역 접근 함수.
		static Engine& Get();

		// 화면 너비 반환 함수
		inline int GetWidth() const { return setting.width; }
		
		// 화면 높이 반환 함수
		inline int GetHeight() const { return setting.height; }

	protected:

		// 정리 함수
		void Shutdown();

		// 설정 파일 로드 함수
		void LoadSetting();

		// 게임 플레이 시작 함수.
		// Unity의 경우: Start/Awake.
		void BeginPlay();

		// 업데이트 함수.
		void Tick(float deltaTime);

		// 그리기 함수. (Draw/Render).
		void Draw();

	protected:
		// 엔진 종료 플래그.
		bool isQuit = false;

		// 엔진 설정 값
		EngineSetting setting;
		
		// 입력 관리자.
		Input* input = nullptr;

		// 렌더러 객체
		Renderer* renderer = nullptr;

		// 메인 레벨.
		class Level* mainLevel = nullptr;

		// 다음 프레임에 교체될 예약 레벨
		class Level* pendingLevel = nullptr;

		// 실제 레벨 교체를 안전하게 처리하는 함수
		void ProcessLevelChange();

		// 전역 변수.
		static Engine* instance;
	};
}
