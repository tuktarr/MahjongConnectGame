#pragma once

#include "Level/Level.h"
#include "Types.h"
#include "Math/Vector2.h"

using namespace Wanted;

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

	// 첫 번째로 선택된 노드 좌표 (선택 안됨 : -1,-1)
	Vector2 firstSelected = Vector2(-1, -1);
};

