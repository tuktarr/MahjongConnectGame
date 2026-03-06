#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"
#include "Util/Types.h"

using namespace Wanted;

class GameLevel;

class StageManager : public Actor
{
	RTTI_DECLARATIONS(StageManager, Actor)

public:
	StageManager(GameLevel* level);
	~StageManager();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// Getter
	GameState GetState() const { return m_state; }

private:
	// 상태별 업데이트 함수들 (Tick에서 호출됨)
	void UpdateReady(float deltaTime);
	void UpdatePlaying(float deltaTime);
	void UpdateStageClear(float deltaTime);
	void UpdateGameOver(float deltaTime);
private:
	GameLevel* m_level; 
	// 현재 스테이지 번호
	int m_currentStage;
	// 최대 스테이지
	int m_maxStage;

	// Type.h에 있는 enum class
	GameState m_state;

	// 스테이지 클리어 제한 타이머
	Timer m_playerTimer;

	// 상태 전환 딜레이용 타이머
	Timer m_stateTimer;

};

