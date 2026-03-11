## 사천성 게임개발 프로젝트

#### 프로젝트 개요
- 장르 : Console Game ( 방치형 광산 경영 시뮬레이션 게임)
- 개발 기간 : 5일 (3/4 ~ 6, 3/9 ~ 10)
- 핵심 컨셉 : 콘솔에서 마우스 클릭으로 플레이하는 서천성 퍼즐 게임
- 참여 인원 : 1인
- 결과물 링크 : <>
- 회고록 : <>

#### 기술스택
- Language : C++
- Environment : Visual Studio 2022, Windows Console API
- Version Control : Git / GitHub
- AI : Gemini

#### 핵심 시스템 설계
1. 프로그래밍 언어 및 설계 (C++ & OOP)
- 커스텀 RTTI(Run-Time Type Information) 구현: * 표준 dynamic_cast 대신 클래스의 주소값과 매크로를 이용한 자체 타입 식별 시스템을 구축하여 객체 지향의 다형성을 안전하고 빠르게 활용함 (Actor::As<T>()).
- 디자인 패턴 활용:
  => Singleton Pattern: Engine, Renderer, Input 등 전역 시스템의 단일 객체 보장 및 접근성 확보.

2. 게임 엔진 아키텍처 (Engine Architecture)
- 더블 버퍼링 및 렌더링 파이프라인:
  => ScreenBuffer 클래스를 통한 이중 버퍼링 구현으로 콘솔 깜빡임(Flickering) 원천 차단.
  => Command Queue 방식의 렌더링: 모든 액터가 Submit을 통해 출력 정보를 예약하고, 렌더러가 일괄 처리하는 현대적 렌더링 파이프라인 모사.

3. 게임 시스템 구현 (Game Systems)
- A* 알고리즘과 BFS 사용(꺾임 제한 2회):
  => CanConnect함수에서 BFS로 경로를 탐색하고 A*에 priority_queue에서 Back-Tracking을 사용해서 경로선 표시
  => 꺾임 2회 이상의 경로는 priority_queue에 push못하도록 막음
  => 3차원 배열로 좌표와 방향을 받아서 경로마다 덮어쓰기 안되도록 만들기
  
- 랭킹 시스템:
  => IsRankIn으로 랭킹 5위안에 못들면 이름 입력 로직 무시
  => 입력은 영어로, 한글 무시
- 스테이지 시스템:
  => StageManager를 통해서 FSM으로 상태마다 다른 함수 실행
  => 스테이지를 통과 할 때 마다, 한 단계 높은 스테이지로 이동
  => Open 버튼을 통한 Timer와 맵 생성 지연처리
  => 교착 상태인 경우 GameOver로 상태 처리

4. 최적화 및 연출 (Optimization & UX)
   => 
