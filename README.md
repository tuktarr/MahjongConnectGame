## 사천성 게임개발 프로젝트

#### 프로젝트 개요
- 장르 : Console Game (사천성게임)
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
  => ScreenBuffer 클래스를 통한 이중 버퍼링 구현으로 콘솔 깜빡임(Flickering) 원천 차단
  => Command Queue 방식의 렌더링: 모든 액터가 Submit을 통해 출력 정보를 예약하고, 렌더러가 일괄 처리하는 현대적 렌더링 파이프라인 모사
  => 모듈화 및 DLL 연동: Engine 프로젝트를 별도로 분리하고, 필요한 API(랭킹 시스템 등)를 DLL로 노출(WANTED_API)하여 게임 프로젝트와 엔진의 종속성을 깔끔하게 분리

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
  => 콘솔 렌더링 환경 최적화 (인코딩 대응): 최신 Windows 터미널에서 기존 CP949 인코딩의 2바이트 특수문자가 1칸으로 압축되어 화면이 수축되는 렌더링 이슈 파악. 이를 해결하기 위해 터미널 출력 인코딩을 확장 ASCII(CP437)로 변경하고, 타일 출력 시 X축 오프셋(+3)을 적용해 시각적인 정중앙 정렬과 격자 비율을 완벽히 맞춤.
  => 논블로킹(Non-blocking) 입력 처리 구조 개편: 기존 std::cin 사용 시 커서가 튀고 화면 렌더링이 멈추는 현상을 해결하기 위해, _kbhit()과 _getch()를 이용한 직접 입력 루프 구현. while문을 통해 한 프레임에 쌓인 모든 키를 일괄 처리하도록 변경하여 조작 응답성을 극대화.
  => 동적 이펙트 및 피드백: 타일 제거 시 단순 삭제가 아닌 파티클이 솟아나는 폭발 이펙트(CreateExplosion)를 적용하고 Z-order를 조정하여 시인성을 높임. 경로 연결 성공 시 Timer 헬퍼를 활용해 짧은 틱(Tick) 동안 역추적된 선 문자를 화면에 그려주는 시각적 피드백 제공. 
