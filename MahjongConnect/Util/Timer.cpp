#include "Timer.h"

Timer::Timer(float targetTime)
	: targetTime(targetTime)
{
}

void Timer::Tick(float deltaTime)
{
	// 경과 시간 업데이트
	elapsedTime += deltaTime;
}


void Timer::Reset()
{
	elapsedTime = 0.0f;
}

bool Timer::IsTimeOut() const
{
	return elapsedTime >= targetTime;
}

void Timer::SetTargetTime(float newTargetTime)
{
	targetTime = newTargetTime;
}

float Timer::GetProgress() const
{
	// 0으로 나누기 방지
	if (targetTime <= 0.0f)
	{
		return 1.0f;
	}

	float ratio = elapsedTime / targetTime;

	// ratio 1을 넘지 않게하기
	return (ratio > 1.0f) ? 1.0f : ratio;
}

float Timer::GetRemainTime() const
{
	float remain = targetTime - elapsedTime;
	return (remain > 0.0f) ? remain : 0.0f; // 음수 시간 방지
}

void Timer::SetElapsedTime(float deltatime)
{
	elapsedTime = deltatime;
}

void Timer::Finish()
{
	elapsedTime = targetTime;
}
