#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

#include <iostream>
#include <Windows.h>

namespace Wanted
{
	Actor::Actor(const std::string& image, const Vector2& position, Color color)
		: image(image), position(position), color(color)
	{
		width = static_cast<int>(image.size());

		// 문자열 복사
		//width = static_cast<int>(strlen(image));
		//this->image = new char[width + 1];
		//strcpy_s(this->image, width + 1, image);
	}

	Actor::~Actor()
	{
		// 메모리 해제
		//SafeDeleteArray(image);
	}

	void Actor::BeginPlay()
	{
		// 이벤트를 받은 후에는 플래그 설정.
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::Draw()
	{
		// 렌더러에 데이터 제출
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::Destroy()
	{
		// 삭제 플래그 설정
		destroyRequested = true;
	
		// 삭제 이벤트 호출
		OnDestroy();
	}

	void Actor::OnDestroy()
	{

	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 변경하려는 위치가 현재 위치와 같으면 건너뜀
		if (position == newPosition)
		{
			return;
		}

		// 새로운 위치 설정
		position = newPosition;
	}
}