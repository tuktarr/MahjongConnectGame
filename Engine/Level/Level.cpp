#include "Level.h"
#include "Actor/Actor.h"

namespace Wanted
{
	Level::Level()
	{
	}

	Level::~Level()
	{
		// 메모리 정리.
		for (Actor*& actor : actors)
		{
			// 액터 객체 메모리 해제.
			if (actor)
			{
				delete actor;
				actor = nullptr;
			}
		}

		// 배열 초기화.
		actors.clear();
	}

	void Level::BeginPlay()
	{
		// 액터에 이벤트 흘리기.
		for (Actor* actor : actors)
		{
			// 이미 BeginPlay 호출된 액터는 건너뛰기.
			if (actor->HasBeganPlay())
			{
				continue;
			}

			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		// 액터에 이벤트 흘리기.
		for (Actor* actor : actors)
		{
			actor->Tick(deltaTime);
		}
	}

	void Level::Draw()
	{
		// 액터 순회하면서 Draw 함수 호출.
		for (Actor* const actor : actors)
		{
			if (!actor->IsActive())
			{
				continue;
			}

			actor->Draw();
		}
	}

	void Level::AddNewActor(Actor* newActor)
	{
		// 나중에 추가를 위해 임시 배열에 저장
		addRequestedActors.emplace_back(newActor);

		// 오너십 설정
		newActor->SetOwner(this);
	}

	void Level::ProcessAddAndDestroyActors()
	{
		// 제거 처리
		for (int ix = 0; ix < static_cast<int>(actors.size()); )
		{
			// 제거 요청된 액터가 있는지 확인
			if (actors[ix]->DestroyRequested())
			{
				// 삭제 처리
				delete actors[ix];

				// 포인터 연산(Iterator) 1번지 actors를 0번지로 바꿈
				actors.erase(actors.begin() + ix);

				continue;
			}

			++ix;
		}

		// 추가 처리
		if (addRequestedActors.size() == 0)
		{
			return;
		}

		for (Actor* const actor : addRequestedActors)
		{
			actors.emplace_back(actor);
		}

		// 처리가 끝났으면 배열 초기화
		addRequestedActors.clear();
	}
}