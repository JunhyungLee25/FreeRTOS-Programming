### 308p - vEventGroupDelete() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “event_groups.h”`

#### 함수 원형

- `void vEventGroupDelete( EventGroupHandle_t xEventGroup );`
    

#### summary

- `xEventGroupCreate()` 호출을 통해 이전에 생성되었던 이벤트 그룹을 삭제한다.
    
- **자원 해제**: 해당 이벤트 그룹에 할당되었던 RAM 메모리가 다시 시스템으로 반환된다.
    

#### Parameters

- `xEventGroup`: 삭제할 대상 이벤트 그룹의 핸들이다.
    

#### Return Values

- 없음.
    

#### 주의사항

- **대기 중인 태스크 처리**: 삭제되는 이벤트 그룹 때문에 **대기(Blocked) 상태에 있던 모든 태스크는 즉시 대기 상태에서 해제(Unblocked)** 된다. 이때 해당 태스크들은 이벤트 그룹의 값을 **0**으로 보고받게 된다.
    
- **인터럽트 제약**: 이 함수는 인터럽트 서비스 루틴(ISR) 내에서 호출해서는 안 된다.
    
- **빌드 설정**: 이 함수를 사용하려면 프로젝트 빌드 시 `FreeRTOS/source/event_groups.c` 파일이 반드시 포함되어 있어야 한다.