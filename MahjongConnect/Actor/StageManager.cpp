#include "StageManager.h"
#include "Level/GameLevel.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
StageManager::StageManager(GameLevel* level)
	: m_level(level), m_currentStage(1), m_maxStage(5), m_state(GameState::Ready)
{

}

StageManager::~StageManager()
{
}

void StageManager::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	switch (m_state)
	{
	// Open 버튼 Rendering
	case GameState::Ready:
		UpdateReady(deltaTime);
		break;
	// Stage에 맞는 타일들 배치
	case GameState::Playing:
		UpdatePlaying(deltaTime);
		break;
	// Stage에 남아 있는 타일이 없으면 StageClear 하고 다음 스테이지로 이동
	case GameState::StageClear:
		UpdateStageClear(deltaTime);
		break;
	// Stage에 교착상태가 발생해서 진행불가상태이거나 제한 시간초과
	case GameState::GameOver:
		UpdateGameOver(deltaTime);
		break;
	}

}

void StageManager::Draw()
{
	super::Draw();

	// 화면 정중앙 좌표 계산
	Vector2 screenSize = Renderer::Get().GetScreenSize();
	Vector2 centerPos = Vector2(screenSize.x / 2 - 5, screenSize.y / 2);

	// 현재 상태에 따라 다른 UI 출력
	switch (m_state)
	{
		case GameState::Ready:
		{
			Renderer::Get().Submit("[ O P E N ]", centerPos, Color::Green, 33);
			break;
		}
		case GameState::Playing:
		{
			// 현재 남은 시간, 현재 스테이지 그리기
			std::string stageText = "STAGE " + std::to_string(m_currentStage);
		
			// 남은 시간 소수점 떼고 정수로 변환하여 출력
			int remainTime = static_cast<int>(m_playerTimer.GetRemainTime());
			std::string timeText = "TIME: " + std::to_string(remainTime) + " sec";

			// 화면 좌측 상단 쪽에 UI 배치
			Renderer::Get().Submit(stageText, Vector2(2, 1), Color::Yellow, 33);
			Renderer::Get().Submit(timeText, Vector2(2, 3), Color::Yellow, 33);
			break;
		}
		case GameState::StageClear:
		{
			Renderer::Get().Submit("STAGE CLEAR!", centerPos, Color::Green, 33);
			break;
		}
		case GameState::GameOver:
		{
			Renderer::Get().Submit("GAME OVER...", centerPos, Color::Red, 33);
			break;
		}
	}
}

void StageManager::UpdateReady(float deltaTime)
{
	// Open 버튼 렌더링
	if (Input::Get().GetButtonDown(VK_LBUTTON))
	{
		// 타일 배치
		m_level->InitializeMap(m_currentStage);
	
		// 타이머 리셋
		// TODO : 나중에 랭킹 시스템을 위해서 Timer의 시간을 저장해야함
		m_playerTimer.Reset();
		m_playerTimer.SetTargetTime(60.0f);

		// 버튼 누르면 Playing 상태로 넘어감
		m_state = GameState::Playing;
	}
}

void StageManager::UpdatePlaying(float deltaTime)
{
	// TODO : 타임아웃 타이머 진행
	m_playerTimer.Tick(deltaTime);
	
	// 타일 모두 제거 시, StageClear 상태로
	if (m_level->GetRemainPairs() <= 0)
	{
		m_state = GameState::StageClear;
		m_stateTimer.Reset();
		m_stateTimer.SetTargetTime(3.0f);
		return;
	}

	// 제한 시간 초과 시, GameOver 상태로
	if (m_playerTimer.IsTimeOut())
	{
		m_state = GameState::GameOver;
		m_stateTimer.Reset();
		m_stateTimer.SetTargetTime(3.0f);
		return;
	}

	// TODO : 교착 상태 발생 시, GameOver 상태로

}

void StageManager::UpdateStageClear(float deltaTime)
{
	// 지정된 시간 동안 대기
	m_stateTimer.Tick(deltaTime);

	// 스테이지 클리어 시, m_currentStage++
	if (m_stateTimer.IsTimeOut())
	{
		// 스테이지 증가
		m_currentStage++;

		// 만약 마지막 스테이지를 깼다면 엔딩 처리
		if (m_currentStage > m_maxStage)
		{
			m_state = GameState::GameOver;
			m_stateTimer.Reset();
			m_stateTimer.SetTargetTime(3.0f);
		}
		else
		{
			// 다음 스테이지 준비 상태로 돌입
			m_state = GameState::Ready;
		}
	}
	
}

void StageManager::UpdateGameOver(float deltaTime)
{
	m_stateTimer.Tick(deltaTime);

	if (m_stateTimer.IsTimeOut())
	{
		// 데이터 초기화
		m_currentStage = 1;

		// 임시 사용
		m_state = GameState::Ready;
		// 게임 시작 메뉴로 이동
		// TODO : 게임 시작 메뉴 구현
	}
}
