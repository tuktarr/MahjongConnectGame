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
            int score = 0;
            // 읽어온 줄에서 정수 데이터만 추출
            if (sscanf_s(buffer, "%d", &score) == 1)
            {
                rankings.push_back(score);
            }
        }

        fclose(file);
    }

    // 새 점수 추가 및 정렬
    void RankManager::AddScore(int newScore)
    {
        // 새 점수 넣기
        rankings.push_back(newScore);

        // 내림차순 정렬 (높은 점수가 1등으로 오도록 std::greater 사용)
        std::sort(rankings.begin(), rankings.end(), std::greater<int>());

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

        for (int score : rankings)
        {
            fprintf_s(file, "%d\n", score);
        }

        fclose(file);
    }
}