#pragma once

#include <vector>
#include <string>
#include "Common/Common.h"

namespace Wanted
{

    class WANTED_API RankManager
    {
    public:
        // 싱글톤 패턴
        static RankManager& Get();

        void Init(); // 초기화 (파일 불러오기)
        void AddScore(int newScore); // 새 점수 추가 및 정렬
        void Save(); // 파일로 저장하기

        // 랭킹 데이터 가져오기 (메뉴에서 그릴 때 사용)
        const std::vector<int>& GetRankings() const { return rankings; }

    private:
        RankManager() = default;
        ~RankManager() = default;

        std::vector<int> rankings; // 점수들을 담아둘 배낭
        std::string filePath = "../Config/Ranking.txt"; // 저장할 파일 경로
    };
}



