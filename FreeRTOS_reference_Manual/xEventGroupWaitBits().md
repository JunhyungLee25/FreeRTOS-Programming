### 320p - xEventGroupWaitBits() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “event_groups.h”`

#### 함수 원형

```c
EventBits_t xEventGroupWaitBits( 
    const EventGroupHandle_t xEventGroup,
    const EventBits_t uxBitsToWaitFor,
    const BaseType_t xClearOnExit,
    const BaseType_t xWaitForAllBits,
    TickType_t xTicksToWait 
);
```

#### summary

- RTOS 이벤트 그룹 내의 특정 비트들을 읽고, 조건이 충족될 때까지 **대기(Blocked)** 상태로 들어간다.
    
- 특정 비트가 설정될 때까지 타임아웃을 지정하여 기다릴 수 있으며, **AND(모두 만족)** 또는 **OR(하나라도 만족)** 조건을 설정할 수 있다.
    
- **주의**: 이 함수는 인터럽트 서비스 루틴(ISR) 내에서 호출할 수 없다.
    

#### Parameters

- `xEventGroup`: 비트를 확인할 대상 이벤트 그룹의 핸들이다.
    
- `uxBitsToWaitFor`: 대기할 비트들을 비트마스크 형태로 지정한다. (예: 0번과 2번 비트를 기다린다면 `0x05`, 절대 0으로 설정하면 안 된다.)
    
- `xClearOnExit`:
    
    - `pdTRUE`: 함수가 반환되기 직전, `uxBitsToWaitFor`로 지정한 비트들을 이벤트 그룹 내에서 다시 **0으로 초기화**한다. (타임아웃으로 반환될 경우는 제외)
        
    - `pdFALSE`: 비트 상태를 그대로 유지한다.
        
- `xWaitForAllBits`:
    
    - `pdTRUE` (**AND**): `uxBitsToWaitFor`에 지정된 **모든 비트**가 설정되어야 대기 상태가 해제된다.
        
    - `pdFALSE` (**OR**): `uxBitsToWaitFor`에 지정된 비트 중 **하나라도** 설정되면 대기 상태가 해제된다.
        
- `xTicksToWait`: 조건이 만족될 때까지 기다릴 최대 시간(Ticks)이다.
    

#### Return Values

- **이벤트 비트가 설정되었을 때 또는 타임아웃이 발생했을 때의 이벤트 그룹 값**:
    
    - 반환된 값을 테스트하여 어떤 비트들이 설정되었는지 확인할 수 있다.
        
    - 만약 `xClearOnExit`가 `pdTRUE`라면, 비트가 자동으로 클리어되기 전의 값을 반환한다.
        
    - 타임아웃으로 반환된 경우, 원래 기다리던 모든 비트가 설정되어 있지 않을 수 있다.
        

#### 주의사항

- **선점(Preemption)**: 반환된 값은 함수가 반환되는 시점의 실제 이벤트 그룹 값과 다를 수 있다. 이는 고우선순위 태스크나 인터럽트가 함수 종료 직전에 값을 바꿀 수 있기 때문이다.
    
- **빌드 설정**: 프로젝트 빌드 시 `FreeRTOS/source/event_groups.c`가 포함되어야 한다.
    

#### Example

```c
#define BIT_0 ( 1 << 0 )
#define BIT_4 ( 1 << 4 )

void aFunction( EventGroupHandle_t xEventGroup )
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );

    /* 0번 또는 4번 비트 중 하나라도 설정될 때까지 최대 100ms 대기.
       반환 시 해당 비트들은 자동으로 클리어함. */
    uxBits = xEventGroupWaitBits(
                xEventGroup,    /* 대상 그룹 */
                BIT_0 | BIT_4,  /* 기다릴 비트 */
                pdTRUE,         /* 반환 시 클리어 여부 */
                pdFALSE,        /* OR 조건 (둘 중 하나) */
                xTicksToWait ); /* 타임아웃 */

    if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
    {
        /* 둘 다 설정되어 깨어남 */
    }
    else if( ( uxBits & BIT_0 ) != 0 )
    {
        /* BIT_0 덕분에 깨어남 */
    }
    else if( ( uxBits & BIT_4 ) != 0 )
    {
        /* BIT_4 덕분에 깨어남 */
    }
    else
    {
        /* 100ms 동안 아무것도 안 와서 타임아웃 발생 */
    }
}
```