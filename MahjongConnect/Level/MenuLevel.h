#pragma once

#include "Level/Level.h"
#include <string>
#include "Math/Vector2.h"
using namespace Wanted;

class MenuLevel : public Level
{
    RTTI_DECLARATIONS(MenuLevel, Level)

public:
    MenuLevel();
    virtual ~MenuLevel();

    virtual void Tick(float deltaTime) override;
    virtual void Draw() override;
private:
    void HandleInput();

    // 버튼 영역 체크 함수 (점과 사각형의 충돌)
    bool IsInside(Vector2 mousePos, Vector2 btnPos, int width, int height);

private:
    // 버튼 위치 및 크기 설정
    Vector2 newGamePos;
    Vector2 exitGamePos;
    int btnWidth = 22;
    int btnHeight = 2;

};

