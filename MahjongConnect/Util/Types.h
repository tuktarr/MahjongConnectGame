#pragma once
namespace Wanted
{
	// 노드(사천성 패)의 종류
	enum class NodeType
	{
		Empty = 0,  // 빈 공간 (길)
		Type1,		// 아직 이름 미정
		Type2,		
		Type3,
		Type4,
		Type5,
		Type6,
		Max
	};

	// 게임 상태
	enum class GameState
	{
		Ready,		// 스테이지 시작 전 (Open 창)
		Playing,	// 게임 진행 중
		GameOver,	// 시간 초과
		StageClear	// 모든 노드 제거 완료
	};

	struct ParentInfo
	{
		int px = -1;    // 부모의 X 좌표
		int py = -1;    // 부모의 Y 좌표
		int pdir = -1;  // 부모가 바라보던 방향 (역추적의 핵심 키)

		ParentInfo() {}
		ParentInfo(int _x, int _y, int _d) : px(_x), py(_y), pdir(_d) {}
	};

}
