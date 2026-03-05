#pragma once

#include "Level/Level.h"
#include "Util/Types.h" 
#include "Math/Vector2.h"
#include "Util/Timer.h"

using namespace Wanted;

struct PathNode 
{
	int x, y, dir, turns;
	// 나를 큐에 넣은 이전 좌표
	Vector2 parent = Vector2(-1, -1);
};

// 노드 상태 정의

// 사천성 맵을 관리
class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

	// 스테이지 번호에 따라 맵 초기화
	void InitializeMap(int stageLevel);
	// 핵심 연결 알고리즘
	bool CanConnect(Vector2 start, Vector2 end);
    
	// 좌표 변환 함수
	Vector2 GridToScreen(int x, int y);
	Vector2 ScreenToGrid(Vector2 mousePos);

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 입력 로직
	void HandleInput();

	// 물리적인 그리드 맵 생성
	void CreateGrid(Vector2 size);
private:
	// 2차원 그리드 구조 (외곽 2칸 여유 포함)
	// m : mahjong
	std::vector<std::vector<NodeType>> m_map;

	// 맵의 실제 가로/세로 크기
	Vector2 m_mapSize;

	// 명시적 선택 안됨 좌표
	Vector2 InvalidPos = Vector2(-1, -1);

	// 첫 번째로 선택된 노드 좌표 (선택 안됨 : -1,-1)
	Vector2 firstSelected = InvalidPos;

	// 두 번째로 선택된 노드 좌표 (선택 안됨 : -1,-1)
	Vector2 secondSelected = InvalidPos;
	const int m_tileWidth = 4;
	const int m_tileHeight = 2;

	// 역추적 경로 (목적지에서 출발지까지)
	std::vector<Vector2> m_currentPath;

	// TimerHandle(시각적으로 경로 보여주는 타이머)
	Timer pathDisplayTimer;
};

