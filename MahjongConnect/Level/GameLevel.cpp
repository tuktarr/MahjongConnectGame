#include "GameLevel.h"
#include "Render/Renderer.h"
#include <algorithm>
#include <random>
#include <queue>
#include "Core/Input.h"

GameLevel::GameLevel()
    : pathDisplayTimer(15.0f)
{
    // 콘솔모드 변경
    SetConsoleOutputCP(437);
    InitializeMap(1);
}

GameLevel::~GameLevel()
{
    SetConsoleOutputCP(949);
}

void GameLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);
    HandleInput();

    // 타이머가 진행 중이라면 업데이트 (IsTimerOut이 아닐 때만)
    if (!pathDisplayTimer.IsTimeOut())
    {
        pathDisplayTimer.Tick(deltaTime);
    }

    // 이펙트 업데이트
    for (auto it = m_effects.begin(); it != m_effects.end();)
    {
        it->timer.Tick(deltaTime);

        // 속도만큼 위치 이동
        it->pos.x += it->velocity.x * deltaTime;
        it->pos.y += it->velocity.y * deltaTime;

        // 중력 가속도 적용
        it->velocity.y += 9.8f * deltaTime;

        // 시간이 지날수록 모양 변화
        if (it->timer.GetProgress() > 0.7f) it->icon = ".";

        if (it->timer.IsTimeOut())
        {
            it = m_effects.erase(it);
        }
        else
        {
            ++it;
        }
    }
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

bool GameLevel::CanConnect(Vector2 start, Vector2 end)
{
    // 이전 경로 초기화
    m_currentPath.clear(); 

    // 방향 : 상(0) 하(1) 좌(2) 우(3)
    int dx[] = { 0,0,-1,1 };
    int dy[] = { -1,1,0,0 };

    // 해당 좌표에서의 최소 꺾임 횟수 기록
    std::vector<std::vector<int>> minTurns(m_mapSize.y, std::vector<int>(m_mapSize.x, 50));
    // 부모 노드를 추적하기 위한 맵
    // vector<vector<Vector2>> -> [][] 칸에 있는 내용물이 Vector2이다.
    std::vector<std::vector<Vector2>> parentMap(m_mapSize.y, std::vector<Vector2>(m_mapSize.x, InvalidPos));

    // A* 탐색을 위한 우선순위 큐
    // 명시적 객체 선언
    NodeComparer comparer(end); // 생성자 인자 선언
    std::priority_queue<PathNode, std::vector<PathNode>,NodeComparer> pq(comparer);
    pq.push({ start.x,start.y, -1, 0, InvalidPos });
    minTurns[start.y][start.x] = 0;

    // 목적지 큐 뽑아내기
    while (!pq.empty())
    {
        PathNode curr = pq.top();
        pq.pop();

        // 목표 도달 시 경로 역추적
        if (curr.x == end.x && curr.y == end.y)
        {
            Vector2 backTrace = Vector2(curr.x, curr.y);
            while (backTrace != InvalidPos)
            {
                m_currentPath.push_back(backTrace);
                backTrace = parentMap[backTrace.y][backTrace.x]; // 부모 좌표로 이동
            }
            return true;
        }

        // 4방향
        for (int dir = 0; dir < 4; dir++)
        {
            int nx = curr.x + dx[dir];
            int ny = curr.y + dy[dir];

            if (nx >= 0 && nx < m_mapSize.x && ny >= 0 && ny < m_mapSize.y)
            {
                // 꺾임 횟수 계산
                int totalTurns = curr.turns;
                if (curr.dir != -1 && curr.dir != dir)
                {
                    totalTurns++;
                }

                // 최대 꺾임 횟수 2회
                if (totalTurns > 2)
                {
                    continue;
                }

                // 이동 가능한지 확인
                if ((nx == end.x && ny == end.y) || m_map[ny][nx] == NodeType::Empty)
                {
                    if (totalTurns <= minTurns[ny][nx])
                    {
                        minTurns[ny][nx] = totalTurns;
                        parentMap[ny][nx] = Vector2(curr.x, curr.y);
                        pq.push({ nx,ny,dir,totalTurns, Vector2(curr.x,curr.y)});
                    }
                }
            }
        }


    }
    return false;
}

Vector2 GameLevel::GridToScreen(int x, int y)
{
    Vector2 startPos(
        (Renderer::Get().GetScreenSize().x - (m_mapSize.x * m_tileWidth)) / 2,
        (Renderer::Get().GetScreenSize().y - (m_mapSize.y * m_tileHeight)) / 2
    );

    int ScreenX = (startPos.x + (x * m_tileWidth));
    int ScreenY = (startPos.y + (y * m_tileHeight));

    return Vector2(ScreenX, ScreenY);
}

Vector2 GameLevel::ScreenToGrid(Vector2 mousePos)
{
    Vector2 startPos(
        (Renderer::Get().GetScreenSize().x - (m_mapSize.x * m_tileWidth)) / 2,
        (Renderer::Get().GetScreenSize().y - (m_mapSize.y * m_tileHeight)) / 2
    );

    int gridX = (mousePos.x - startPos.x) / m_tileWidth;
    int gridY = (mousePos.y - startPos.y) / m_tileHeight;

    return Vector2(gridX, gridY);
}

std::string GameLevel::GetPathChar(Vector2 prev, Vector2 curr, Vector2 next)
{
    bool up = (prev.y < curr.y || next.y < curr.y);
    bool down = (prev.y > curr.y || next.y > curr.y);
    bool left = (prev.x < curr.x || next.x < curr.x);
    bool right = (prev.x > curr.x || next.x > curr.x);

    // CP949 16진수: ─(\xA6\xA1), │(\xA6\xA2), ┌(\xA6\xA3), ┐(\xA6\xA4), ┘(\xA6\xA5), └(\xA6\xA6)
    // 모든 리턴은 정확히 4바이트여야 그리드가 안밀림

    //// 세로 직선
    //if (up && down)
    //{
    //    return "    \xA6\xA2    ";
    //}
    //// 가로 직선
    //if (left && right) 
    //    return "\xA6\xA1\xA6\xA1\xA6\xA1\xA6\xA1\xA6\xA1";

    //// 코너 (오른쪽 타일과 연결: └, ┌) -> 뒤쪽을 가로선으로 채움
    //if (up && right)
    //{
    //    return "    \xA6\xA6\xA6\xA1\xA6\xA1";// └─
    //}
    //if (down && right) 
    //{
    //    return "    \xA6\xA3\xA6\xA1\xA6\xA1";// ┌─
    //}
    //// 코너 (왼쪽 타일과 연결: ┘, ┐) -> 앞쪽을 가로선으로 채움
    //if (up && left)
    //{
    //    return "\xA6\xA1\xA6\xA1\xA6\xA5    ";// ─┘
    //}
    //if (down && left)
    //{
    //    return "\xA6\xA1\xA6\xA1\xA6\xA4    ";// ─┐
    //}
    //return "          ";
    //if (up && down) return "    |     ";          // 세로 (공백3 + | + 공백4 = 8칸)
    //if (left && right) return "----------";       // 가로 (- 8개 = 8칸)
    //if (up && right) return "   +------";         // 코너 (└─ 대용)
    //if (down && right) return "   +------";        // 코너 (┌─ 대용)
    //if (up && left) return "------+   ";          // 코너 (─┘ 대용)
    //if (down && left) return "------+   ";        // 코너 (─┐ 대용)
    char v = (char)179;
    char h = (char)196;
    char ur = (char)192;
    char dr = (char)218;
    char ul = (char)217;
    char dl = (char)191;

    if (up && down) return std::string("    ") + v + std::string("     ");
    if (left && right) return std::string(10, h);
    if (up && right) return std::string("    ") + ur + std::string(5, h);
    if (down && right) return std::string("    ") + dr + std::string(5, h);
    if (up && left) return std::string(4, h) + ul + std::string("     ");
    if (down && left) return std::string(4, h) + dl + std::string("     ");

    return "          ";
}


void GameLevel::Draw()
{
    Level::Draw();

    // 맵 순회하며 그리기
    for (int y = 0; y < m_mapSize.y; ++y)
    {
        for (int x = 0; x < m_mapSize.x; ++x)
        {
            NodeType type = m_map[y][x];

            // 선택된(Hovered) 노드인지 확인
            bool isFirstSelected = (firstSelected == Vector2(x, y));

            // 좌표 변환 후, draw
            Vector2 drawPos = GridToScreen(x, y);

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
            default: tileText = "[ ]"; break;
            }

            // 선택된 경우 색상을 반전시키거나 강조
            if (isFirstSelected)
            {
                contentColor = Color::Yellow;
            }

            Vector2 tileDrawPos = drawPos;
            tileDrawPos.x += 3;

            // 그리기 제출
            Renderer::Get().Submit(tileText, drawPos, contentColor, 5);
        }
    }

    // 이펙트 그리기
    for (const auto& effect : m_effects)
    {
        Vector2 drawPos = GridToScreen(effect.pos.x, effect.pos.y);
        Renderer::Get().Submit(effect.icon, drawPos, effect.color, 3);
    }

    // 연결 경로 그리기
    if (!pathDisplayTimer.IsTimeOut() && m_currentPath.size() >= 2)
    {
        // i = 1 부터 size - 2 까지만 순회 (시작과 끝 제외)
        for (int i = 1; i < (int)m_currentPath.size() - 1; ++i)
        {
            Vector2 pos = m_currentPath[i];
            Vector2 drawPos = GridToScreen(pos.x, pos.y);

            Vector2 prev = m_currentPath[i - 1];
            Vector2 next = m_currentPath[i + 1];

            // 중간 경로만 GetPathChar로 가져와서 출력
            std::string icon = GetPathChar(prev, pos, next);
            Color pathColor = Color::Red;

            Renderer::Get().Submit(icon, drawPos, pathColor, 10);
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

        // 스크린 좌표 -> 그리드 인덱스 변환
        Vector2 gridIdx = ScreenToGrid(mousePos);
        // 유효범위 검사 (맵 안을 클릭했는지?)
        // 유효한 조건
        if (gridIdx.x >= 0 && gridIdx.x < m_mapSize.x && gridIdx.y >= 0 && gridIdx.y < m_mapSize.y)
        {
            // 빈칸 클릭 시, 처음에 호버된 인덱스 없게 만들기
            if (m_map[gridIdx.y][gridIdx.x] == NodeType::Empty)
            {
                firstSelected = InvalidPos;
                return;
            }

            // 이미 선택된 곳을 선택한다면 Hover 해제
            // 아무것도 선택하지 않은 상태를 Vector2(-1,-1)로
            if (firstSelected == gridIdx)
            {
                firstSelected = InvalidPos;
            }
            else
            {
                // 아무것도 선택되지 않은 상태라면 마우스 찍은 곳에 Hover
                if (firstSelected == InvalidPos)
                {
                    firstSelected = gridIdx;
                }
                // 두 번째로 찍은 곳에 대해 BFS, A* 알고리즘 실행
                else
                {
                    secondSelected = gridIdx;

                    if (m_map[firstSelected.y][firstSelected.x] == m_map[secondSelected.y][secondSelected.x])
                    {
                        // BFS 실행 -> 직선/꺾임 체크 함수 호출
                        if (CanConnect(firstSelected, secondSelected))
                        {
                            // 연결 성공
                            m_map[firstSelected.y][firstSelected.x] = NodeType::Empty;
                            m_map[secondSelected.y][secondSelected.x] = NodeType::Empty;

                            // 경로 잔상 이펙트 (순차적 느낌을 위해 i값에 따라 수명 조절)
                            for (int i = 0; i < m_currentPath.size(); i+=2)
                            {
                                m_effects.emplace_back(m_currentPath[i], Vector2(0, 0), ".", 0.2f, Color::Cyan);
                            }

                            // 폭발 이펙트 발생
                            CreateExplosion(firstSelected);
                            CreateExplosion(secondSelected);

                            // 타이머 리셋하고 경로 그리기
                            pathDisplayTimer.Reset();

                            firstSelected = InvalidPos;
                        }
                        else
                        {
                            // 만약 둘의 타입이 다르면 Hover 제거
                            firstSelected = InvalidPos;
                        }
                    }
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

void GameLevel::CreateExplosion(Vector2 gridPos)
{
    for (int i = 0; i < 8; ++i) // 입자 개수 증가
    {
        // -1.0 ~ 1.0 사이의 무작위 방향과 힘
        float vx = ((rand() % 200) - 100) / 40.0f;
        float vy = ((rand() % 200) - 150) / 40.0f;

        Vector2 velocity(vx, vy);
        std::string icon = (rand() % 2 == 0) ? "*" : "+";

        m_effects.emplace_back(gridPos, velocity, icon, 0.6f, Color::Yellow);
    }
}
