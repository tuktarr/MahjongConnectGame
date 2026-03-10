#include "MenuLevel.h"
#include "GameLevel.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"
#include "Util/RankManager.h"


MenuLevel::MenuLevel()
{
    SetConsoleOutputCP(949);
}

MenuLevel::~MenuLevel()
{
}

void MenuLevel::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    HandleInput();
}

void MenuLevel::Draw()
{
    super::Draw();

    // 화면 정중앙 좌표 가져오기 (타이틀용)
    Vector2 screenSize = Renderer::Get().GetScreenSize();
    Vector2 titlePos = Vector2(screenSize.x / 2 + 12  , screenSize.y / 2 - 14);
    Vector2 rankBoardPos = Vector2(titlePos.x, exitGamePos.y + 2);

    // 버튼 위치
    newGamePos = Vector2(titlePos.x, titlePos.y + 5);
    exitGamePos = Vector2(titlePos.x, titlePos.y + 8);

    // 마우스 위치 가져오기
    Vector2 mousePos = Input::Get().GetMousePosition();

    // 호버 여부에 따른 색상 결정
    Color newGameColor = IsInside(mousePos, newGamePos, btnWidth, btnHeight) ? Color::Yellow : Color::Green;
    Color exitGameColor = IsInside(mousePos, exitGamePos, btnWidth, btnHeight) ? Color::Red : Color::Green;

    // 게임 타이틀 그리기
    Renderer::Get().Submit("==============================", titlePos, Color::Yellow, 100);
    Renderer::Get().Submit("     MAHJONG Connect Game     ", Vector2(titlePos.x, titlePos.y + 1), Color::Yellow, 100);
    Renderer::Get().Submit("==============================", Vector2(titlePos.x, titlePos.y + 2), Color::Yellow, 100);

    // 버튼 그리기 (헤더에 설정한 좌표 사용)
    Renderer::Get().Submit("▶ NEW GAME", newGamePos, newGameColor, 120);
    Renderer::Get().Submit("▶ EXIT", exitGamePos, exitGameColor, 120);

    // 랭킹 확인 버튼 추가
    Renderer::Get().Submit("====== Top 5 RANKING ======", rankBoardPos, Color::Green, 100);

    // 랭킹 데이터 가져오기
    const std::vector<RankData>& rankings = RankManager::Get().GetRankings();

    // 랭킹 데이터 없을 때
    if (rankings.empty())
    {
        Renderer::Get().Submit("     No Records Yet.     ", Vector2(rankBoardPos.x, rankBoardPos.y + 2), Color::Gray, 100);
    }
    // 랭킹 데이터 출력
    else
    {
        for (size_t i = 0; i < rankings.size(); ++i)
        {
            Color rankColor = Color::White;
            std::string rankTitle = "";

            // 1 2 3등 꾸미기 분기 처리
            switch (i)
            {
            case 0: // 1등 (Gold)
                rankColor = Color::Yellow;
                rankTitle = "[1st] ";
                break;
            case 1: // 2등 (SILVER)
                rankColor = Color::White;
                rankTitle = "[2nd] ";
                break;
            case 2: // 3등 (BRONZE)
                rankColor = Color::Brown;
                rankTitle = "[3rd] ";
                break;
            default: // 그 외
                rankColor = Color::Gray;
                rankTitle = "[" + std::to_string(i + 1) + "th] ";
                break;
            }
            std::string rankText = rankTitle + rankings[i].name + " : " + std::to_string(rankings[i].score) + " sec";

            // 1등 이름 꾸미기
            if (i == 0)
            {
                rankText = "★ " + rankText + " ★";
            }

            Renderer::Get().Submit(rankText, Vector2(rankBoardPos.x, rankBoardPos.y + 2 + i * 2), Color::White, 100);
        }
    }
}

void MenuLevel::HandleInput()
{
    if (Input::Get().GetButtonDown(VK_LBUTTON))
    {
        Vector2 mousePos = Input::Get().GetMousePosition();

        if(IsInside(mousePos, newGamePos, btnWidth, btnHeight))
        {
            Engine::Get().SetNewLevel(new GameLevel);
        }
        else if (IsInside(mousePos, exitGamePos, btnWidth, btnHeight))
        {
            Engine::Get().QuitEngine();
        }
    }
}

bool MenuLevel::IsInside(Vector2 mousePos, Vector2 btnPos, int width, int height)
{
    return (mousePos.x >= btnPos.x && mousePos.x <= btnPos.x + width &&
        mousePos.y >= btnPos.y && mousePos.y <= btnPos.y + height);
}
