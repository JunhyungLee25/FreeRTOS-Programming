50p - vTaskDelayUntil() manual

#### 함수 라이브러리
`#include “FreeRTOS.h”`
`#include “task.h”`

#### 함수 원형
- `void vTaskDelayUntil( TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement );`
#### summary
- 호출한 태스크를 **절대 시간(Absolute time)**에 도달할 때까지 Blocked 상태로 만든다.  
- 일정한 실행 빈도를 유지해야 하는 **주기적 태스크(Periodic tasks)**를 구현할 때 사용한다.
#### vTaskDelay()와 vTaskDelayUntil()의 차이점
- **vTaskDelay()**: 호출된 시점부터 지정된 틱(Tick)만큼 대기한다. 즉, 대기 시간은 호출 시점에 비례하는 **상대적 시간**이다. (실행 시간 + 지연 시간 = 전체 주기 $\rightarrow$ 변동 발생) 
- **vTaskDelayUntil()**: 지정된 절대 시간에 도달할 때까지 대기한다. 태스크는 호출 시점과 상관없이 **정확히 정해진 시점**에 Blocked 상태에서 빠져나온다.
#### Parameters
- **pxPreviousWakeTime**: 태스크가 마지막으로 Blocked 상태에서 해제되었던(깨어났던) 시각을 담고 있는 변수의 포인터이다.
      - 이 시각은 다음 깨어날 시각을 계산하는 기준점(Reference point)이 된다. 
    - 함수 내부에서 자동으로 업데이트되므로, 사용자가 처음 초기화한 이후에는 직접 수정할 필요가 없다.
- **xTimeIncrement**: 태스크의 실행 주기를 틱(Tick) 단위로 설정한다.
    - 보통 `pdMS_TO_TICKS()` 매크로를 사용하여 밀리초를 틱으로 변환하여 입력한다.
#### Return Values
- 없음.
#### 주의사항
- 이 API를 사용하려면 `FreeRTOSConfig.h` 파일에서 `INCLUDE_vTaskDelayUntil` 상수가 반드시 **1**로 설정되어 있어야 한다.
#### Example
```c
/* 50밀리초마다 특정 동작을 수행하는 주기적 태스크 정의 */
void vCyclicTaskFunction( void * pvParameters )
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS( 50 );

    /* xLastWakeTime 변수를 현재 틱 카운트로 초기화한다. 
       이후에는 vTaskDelayUntil() 내부에서 자동으로 값이 업데이트된다. */
    xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        /* 이 태스크는 정확히 50ms마다 실행된다. 
           vTaskDelayUntil은 xLastWakeTime과 xPeriod를 이용해 
           정확한 다음 실행 시점을 계산하고, 그 시점까지 태스크를 Block 시킨다. */
        vTaskDelayUntil( &xLastWakeTime, xPeriod );

        /* 주기적으로 처리할 사용자 코드를 여기에 작성한다. */
    }
}
```