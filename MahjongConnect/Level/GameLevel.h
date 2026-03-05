#pragma once

#include "Level/Level.h"
#include "Util/Types.h" 
#include "Math/Vector2.h"
#include "Util/Timer.h"

using namespace Wanted;

// (A* / BFS 탐색용 구조체)
struct PathNode 
{
	int x, y, dir, turns;
	// 나를 큐에 넣은 이전 좌표
	Vector2 parent = Vector2(-1, -1);
};

// A* 알고리즘에서 먼저 탐색할 경로에 대한 기준
struct NodeComparer
{
	Vector2 targetPos;
	NodeComparer(Vector2 target) : targetPos(target) {}

	bool operator() (const PathNode& a, const PathNode& b)
	{
		// G-Score : 꺾임 횟수
		int g_a = a.turns * 10;
		int g_b = b.turns * 10;

		// H-Score : 목적지까지 직선 거리 (유동적으로 계속 변함)
		int h_a = abs(a.x - targetPos.x) + abs(a.y - targetPos.y);
		int h_b = abs(b.x - targetPos.x) + abs(b.y - targetPos.y);

		// F-Score = G + H
		int f_a = g_a + h_a;
		int f_b = g_b + h_b;

		// 점수가 낮은 것이 우선순위가 높아야 하므로 >
		// 꺾임 횟수 적은 것을 최우선으로 탐색 pq에서 front를 안하고 top을 함
		if (f_a != f_b)
		{
			return f_a > f_b;
		}
		
		// 점수가 같다면 꺾임이 더 적은 쪽을 우선
		return a.turns > b.turns;
	}
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
	
	// 좌표들 사이의 방향을 보고 적절한 선 문자를 반환하는 함수
	std::string GetPathChar(Vector2 prev, Vector2 curr, Vector2 next);

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

