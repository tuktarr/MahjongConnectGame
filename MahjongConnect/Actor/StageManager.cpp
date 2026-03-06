#include "StageManager.h"

StageManager::StageManager(GameLevel* level)
	: m_level(level), m_currentStage(1), m_maxStage(5), m_state(GameState::Playing) // 추후에 Ready로 변경
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
}

void StageManager::UpdateReady(float deltaTime)
{
}

void StageManager::UpdatePlaying(float deltaTime)
{
}

void StageManager::UpdateStageClear(float deltaTime)
{
}

void StageManager::UpdateGameOver(float deltaTime)
{
}
