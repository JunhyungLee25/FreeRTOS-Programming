### 311p - xEventGroupSetBits() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “event_groups.h”`

#### 함수 원형

- `EventBits_t xEventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet );`
    

#### summary

- RTOS 이벤트 그룹 내의 특정 비트(플래그)들을 1로 설정한다.
    
- 비트가 설정되면, 해당 비트가 설정되기를 기다리며 **대기(Blocked) 중이던 모든 태스크가 자동으로 대기 상태에서 해제(Unblock)**된다.
    
- **주의**: 이 함수는 인터럽트(ISR) 내에서 호출할 수 없다. ISR용 버전인 `xEventGroupSetBitsFromISR()`이 별도로 존재한다.
    

#### Parameters

- `xEventGroup`: 비트를 설정할 대상 이벤트 그룹의 핸들이다. (미리 생성되어 있어야 함)
    
- `uxBitsToSet`: 설정할 비트를 나타내는 비트마스크 값이다.
    
    - 예: 3번 비트만 설정하려면 `0x08` (1 << 3) 전달.
        
    - 예: 0번과 3번 비트를 동시에 설정하려면 `0x09` (`BIT_0 | BIT_3`) 전달.
        

#### Return Values

- **함수가 반환될 시점의 이벤트 그룹 비트 값**: 호출이 완료된 순간의 최종 비트 상태를 반환한다.
    
- **주의**: 내가 방금 설정한 비트가 반환값에서 **0(clear)**으로 보일 수 있는 두 가지 이유:
    
    1. 비트가 설정되자마자 이를 기다리던 태스크가 깨어나면서 **자동으로 비트를 Clear** 하도록 설정된 경우 (`xClearBitsOnExit`).
        
    2. 깨어난 다른 태스크의 우선순위가 더 높아, `xEventGroupSetBits()`가 값을 반환하기 전에 실행되어 값을 변경한 경우.
        

#### 주의사항

- **빌드 설정**: 프로젝트 빌드 시 `FreeRTOS/source/event_groups.c` 파일이 반드시 포함되어 있어야 한다.
    

#### Example

```c
#define BIT_0 ( 1 << 0 )
#define BIT_4 ( 1 << 4 )

void aFunction( EventGroupHandle_t xEventGroup )
{
    EventBits_t uxBits;

    /* xEventGroup의 0번 비트와 4번 비트를 1로 설정한다. */
    uxBits = xEventGroupSetBits( xEventGroup, BIT_0 | BIT_4 );

    /* 반환된 값을 통해 최종 상태를 확인한다. */
    if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
    {
        /* 0번과 4번 비트가 여전히 1로 유지되고 있음. */
    }
    else
    {
        /* 일부 비트가 0으로 보인다면, 대기하던 태스크가 깨어나며 Clear 했을 가능성이 큼. */
    }
}
```