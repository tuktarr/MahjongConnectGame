#include "GameLevel.h"
#include "Render/Renderer.h"
#include <algorithm>
#include <random>
#include "Core/Input.h"

GameLevel::GameLevel()
{
    InitializeMap(1);
}

GameLevel::~GameLevel()
{
}

void GameLevel::InitializeMap(int stageLevel)
{
	// 스테이지에 따른 사이즈 결정
	Vector2 playSize;
    switch (stageLevel)
    {
    case 1:
        playSize = Vector2(3, 3);
        break;
    case 2:
        playSize = Vector2(4, 4);
        break;
    case 3:
        playSize = Vector2(5, 5);
        break;
    case 4:
        playSize = Vector2(6, 6);
        break;
    case 5:
        playSize = Vector2(8, 8);
        break;
    default:
        playSize = Vector2(3, 3);
        break;
    }

    // 물리적 빈 그리드 생성
	CreateGrid(playSize);

    // 배치할 아이템 리스트 준비
    std::vector<Vector2> validPositions;

    // Grid 영역 순회 ( 외곽은 어처피 empty -> 초기에 empty로 초기화, 0번인덱스 순회 안함)
    for (int y = 1; y <= playSize.y; ++y)
    {
        for (int x = 1; x <= playSize.x; ++x)
        {
            // Vector.x * Vector.y 의 값이 홀수인 스테이지인 경우 중앙은 비워둠
            bool isOddMap = (playSize.x * playSize.y) % 2 == 1;
            Vector2 center(playSize.x / 2 + 1, playSize.y / 2 + 1);

            if (isOddMap && x == center.x && y == center.y)
            {
                continue;
            }

            validPositions.emplace_back(x,y);
        }
    }
    // 무작위 씨앗 준비
    std::random_device randomDevice;
    // 씨앗을 '엄청나게 빠른 숫자 생성기'에 넣음
    std::mt19937 shuffleEngine(randomDevice());
    // 생성기의 힘을 빌려 아이템을 섞음
    std::shuffle(validPositions.begin(), validPositions.end(), shuffleEngine);

    // 아이템 리스트에 짝을 맞춰 종류 할당
    int pairCount = static_cast<int>(validPositions.size() / 2);
    // 실제 사용할 패의 종류 개수
    int typeRange = static_cast<int>(NodeType::Max) - 1;

    for (int i = 0; i < pairCount; ++i)
    {
        // 타입 패 개수 초과 시, 순환하며 할당
        NodeType type = static_cast<NodeType>((i % typeRange) + 1);
        
        // 리스트에 두 칸씩 같은 타입을 채움.
        Vector2 pos1 = validPositions[i * 2];
        Vector2 pos2 = validPositions[i * 2 + 1];

        m_map[pos1.y][pos1.x] = type;
        m_map[pos2.y][pos2.x] = type;
    }
}

void GameLevel::Tick(float deltaTime)
{
}

void GameLevel::Draw()
{
    Level::Draw();

    // 타일 및 여백 설정
    const int tileWidth = 4;
    const int tileHeight = 2;

    // 화면 중앙 정렬
    Vector2 startPos(
        (Renderer::Get().GetScreenSize().x - (m_mapSize.x * tileWidth)) / 2,
        (Renderer::Get().GetScreenSize().y - (m_mapSize.y * tileHeight)) / 2
    );

    // 맵 순회하며 그리기
    for (int y = 0; y < m_mapSize.y; ++y)
    {
        for (int x = 0; x < m_mapSize.x; ++x)
        {
            NodeType type = m_map[y][x];
            // 그리드 좌표를 Vector2로 변환 (콘솔좌표계)
            // 인덱스 표현을 위해 tileWidth,tileHeight로 곱함
            Vector2 drawPos(startPos.x + (x * tileWidth), startPos.y + (y * tileHeight));

            // 선택된(Hovered) 노드인지 확인
            bool isFirstSelected = (firstSelected == Vector2(x, y));

            // 빈 공간인지 확인
            if (type == NodeType::Empty)
            {
                // 아무것도 그리지 않기
                continue;
            }

            // NodeType에 따른 문자 표현
            std::string tileText;
            Color contentColor = Color::White;

            switch (type)
            {
            case NodeType::Type1:
                tileText = "[A]"; 
                contentColor = Color::Red; 
                break;
            case NodeType::Type2:
                tileText = "[B]";
                contentColor = Color::Green;
                break;
            case NodeType::Type3: 
                tileText = "[C]";
                contentColor = Color::Blue;
                break;
            case NodeType::Type4:
                tileText = "[D]";
                contentColor = Color::Magenta;
                break;
            case NodeType::Type5:
                tileText = "[E]";
                contentColor = Color::Cyan; 
                break;
            default: tileText = "[?]"; break;
            }

            // 선택된 경우 색상을 반전시키거나 강조
            if (isFirstSelected)
            {
                contentColor = Color::Yellow;
            }

            // 그리기 제출
            Renderer::Get().Submit(tileText, drawPos, contentColor, 5);
        }
    }
}

void GameLevel::HandleInput()
{
    // 마우스 왼쪽 버튼 클릭 시점에만 실행
    if (Input::Get().GetButtonDown(VK_LBUTTON))
    {
        // 마우스 현재 콘솔 좌표 가져오기
        Vector2 mousePos = Input::Get().GetMousePosition();

        // Draw함수와 동일한 정렬 기준값 계산
        const int tileWidth = 4;
        const int tileHeight = 2;
        Vector2 startPos(
            (Renderer::Get().GetScreenSize().x - (m_mapSize.x * tileWidth)) / 2,
            (Renderer::Get().GetScreenSize().y - (m_mapSize.y * tileHeight)) / 2
        );

        // 스크린 좌표 -> 그리드 인덱스 변환
        // 기준점을 맞추기 위해서 startPos만큼 줄임 (시작인덱스만큼 뺌)
        int gridX = static_cast<int>(mousePos.x - startPos.x) / tileWidth;
        int gridY = static_cast<int>(mousePos.y - startPos.y) / tileHeight;

        // 유효범위 검사 (맵 안을 클릭했는지?)
        // 유효한 조건
        if (gridX >= 0 && gridX < m_mapSize.x && gridY >= 0 && gridY < m_mapSize.y)
        {
            // TODO : 빈칸 클릭 시, 나중에 호버된 인덱스들 하나도 없게 만들기
            if (m_map[gridY][gridX] == NodeType::Empty)
            {
                return;
            }

            // 이미 선택된 곳을 선택한다면 Hover 해제
            // 아무것도 선택하지 않은 상태를 Vector2(-1,-1)로
            if (firstSelected == Vector2(gridX, gridY))
            {
                firstSelected = Vector2(-1, -1);
            }
            else
            {
                // 아무것도 선택되지 않은 상태라면 마우스 찍은 곳에 Hover
                if (firstSelected == Vector2(-1, -1))
                {
                    firstSelected = Vector2(gridX, gridY);
                }
                // TODO : 두 번째로 찍은 곳에 대해 BFS, A* 알고리즘 실행
                else
                {
                    
                }
            }
        }
    }
}

void GameLevel::CreateGrid(Vector2 size)
{
	// 실제 사용할 전체 크기 계산 (외곽 포함)
	m_mapSize = size + Vector2(2, 2);

	// 세로 크기 결정
	m_map.resize(m_mapSize.y);

	// 세로 줄 마다 가로 크기 결정, 가로줄 Empty로 채움
	for (int i = 0; i < m_mapSize.y; ++i)
	{
		m_map[i].assign(m_mapSize.x, NodeType::Empty);
	}
}
