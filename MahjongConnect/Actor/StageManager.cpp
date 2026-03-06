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
}

void StageManager::Draw()
{
}
