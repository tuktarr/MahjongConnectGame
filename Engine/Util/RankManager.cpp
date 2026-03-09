#include "RankManager.h"
#include <algorithm>
#include <iostream>
#include <cstdio>

namespace Wanted
{
    RankManager& RankManager::Get()
    {
        static RankManager instance;
        return instance;
    }

    void RankManager::Init()
    {
        rankings.clear();

        FILE* file = nullptr;
        errno_t err = fopen_s(&file, filePath.c_str(), "rt");

        if (err != 0 || file == nullptr)
        {
            return;
        }

        char buffer[256] = {};

        while (fgets(buffer, sizeof(buffer), file) != nullptr)
        {
            char nameBuf[128] = {};
            int score = 0;
            // 읽어온 줄에서 정수 데이터만 추출
            if (sscanf_s(buffer, "%s %d", nameBuf, (unsigned int)sizeof(nameBuf), &score) == 2)
            {
                rankings.push_back({ std::string(nameBuf),score });
            }
        }

        fclose(file);
    }
    
    // 새 점수 추가 및 정렬
    void RankManager::AddScore(const std::string& name, int newScore)
    {
        // 새 점수 넣기
        rankings.push_back({name, newScore});

        // 내림차순 정렬 (높은 점수가 1등으로 오도록 std::greater 사용)
        std::sort(rankings.begin(), rankings.end(), [](const RankData& a, const RankData& b) { return a.score > b.score; });

        // 기록이 5개를 초과하면 6등부터는 잘라내기
        if (rankings.size() > 5)
        {
            rankings.resize(5);
        }

        // 순위가 변동되었으니 파일에 즉시 덮어쓰기 저장
        Save();
    }

    // 파일에 랭킹 데이터 저장하기
    void RankManager::Save()
    {
        FILE* file = nullptr;
        errno_t err = fopen_s(&file, filePath.c_str(), "wt");

        if (err != 0 || file == nullptr)
        {
            std::cout << "Failed to save ranking file.\n";
            return;
        }

        for (const auto& r : rankings)
        {
            
            fprintf_s(file, "%s %d\n",r.name.c_str(), r.score);
        }

        fclose(file);
    }
    bool RankManager::IsRankIn(int score)
    {
        // 아직 랭킹이 5개 미만이면 무조건 진입 가능
        if (rankings.size() < 5)
        {
            return true;
        }
        // 랭킹이 꽉 찼다면, 가장 낮은 점수(5등)보다 내 점수가 높은지 확인
        return score > rankings.back().score;
    }
}