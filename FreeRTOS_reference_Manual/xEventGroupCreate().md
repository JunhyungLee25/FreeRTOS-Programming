### 304p - xEventGroupCreate() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “event_groups.h”`

#### 함수 원형

- `EventGroupHandle_t xEventGroupCreate( void );`
    

#### summary

- 새로운 이벤트 그룹을 생성하고, 이를 참조할 수 있는 핸들(Handle)을 반환한다.
    
- 각 이벤트 그룹은 상태를 저장하기 위해 매우 적은 양의 RAM이 필요하다. `xEventGroupCreate()`를 사용하면 FreeRTOS 힙(Heap)에서 자동으로 메모리가 할당된다. (정적 할당을 원할 경우 `xEventGroupCreateStatic()` 사용 가능)
    
- 이벤트 그룹 내에서 구현되는 **비트(플래그)의 개수**는 `FreeRTOSConfig.h`의 설정에 따라 결정된다.
    
    - `configUSE_16_BIT_TICKS`가 1인 경우: **8비트** 사용 가능
        
    - `configUSE_16_BIT_TICKS`가 0인 경우: **24비트** 사용 가능
        
- **이 함수는 인터럽트 서비스 루틴(ISR) 내에서 호출할 수 없다.**
    

#### Parameters

- **없음**
    

#### Return Values

- `NULL`: FreeRTOS 힙 메모리가 부족하여 이벤트 그룹을 생성할 수 없는 경우 반환된다.
    
- `기타 값`: 이벤트 그룹이 성공적으로 생성되었음을 의미하며, 반환된 값은 생성된 이벤트 그룹의 핸들이다.
    

#### 주의사항

- **빌드 설정**: 이 함수를 사용하려면 `FreeRTOSConfig.h`에서 `configSUPPORT_DYNAMIC_ALLOCATION`이 1로 설정되어 있어야 하며(기본값 1), 프로젝트 빌드 시 `FreeRTOS/source/event_groups.c` 파일이 포함되어 있어야 한다.
    
- **인터럽트 제약**: ISR에서는 이 함수 대신 인터럽트 안전 버전이 없으므로, 반드시 태스크 레벨에서 생성해야 한다.
    

#### Example

```c
/* 생성된 이벤트 그룹을 담을 변수를 선언한다. */
EventGroupHandle_t xCreatedEventGroup;

/* 이벤트 그룹 생성을 시도한다. */
xCreatedEventGroup = xEventGroupCreate();

/* 이벤트 그룹이 성공적으로 생성되었는지 확인한다. */
if( xCreatedEventGroup == NULL )
{
    /* 힙 메모리가 부족하여 이벤트 그룹이 생성되지 않음. */
}
else
{
    /* 이벤트 그룹 생성 성공. 이제 이 핸들을 사용하여 비트를 설정하거나 기다릴 수 있다. */
}
```