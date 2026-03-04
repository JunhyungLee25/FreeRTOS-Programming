259p - xTimerCreate() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “timers.h”`

#### 함수 원형

- `TimerHandle_t xTimerCreate( const char *pcTimerName, const TickType_t xTimerPeriod, const UBaseType_t uxAutoReload, void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction );`
    

#### summary

- 새로운 소프트웨어 타이머를 생성하고, 이를 참조할 수 있는 핸들을 반환한다.
    
- 타이머의 상태를 유지하기 위해 소량의 RAM이 필요하며, `xTimerCreate()`를 사용하면 FreeRTOS 힙(Heap)에서 자동으로 할당된다. (정적 할당을 원할 경우 `xTimerCreateStatic()`을 사용한다.)
    
- **주의:** 타이머를 생성한다고 해서 자동으로 시작되지 않는다. `xTimerStart()`, `xTimerReset()` 등의 API를 호출해야 비로소 타이머가 동작한다.
    

#### Parameters

- `pcTimerName`: 디버깅을 돕기 위해 할당하는 단순 텍스트 이름이다. 커널은 이 이름을 사용하지 않는다.
    
- `xTimerPeriod`: 타이머의 주기이다. 틱(Tick) 단위로 지정하며, `pdMS_TO_TICKS()` 매크로를 사용하여 밀리초(ms) 단위를 틱으로 변환하는 것이 권장된다. (예: 500ms는 `pdMS_TO_TICKS(500)`)
    
- `uxAutoReload`: 타이머 모드를 설정한다.
    
    - `pdTRUE`: 자동 재실행(Autoreload) 타이머. 만료될 때마다 주기적으로 콜백을 실행한다.
        
    - `pdFALSE`: 원샷(One-shot) 타이머. 한 번 만료되면 정지하며, 필요 시 수동으로 다시 시작해야 한다.
        
- `pvTimerID`: 생성되는 타이머에 할당된 식별자이다.
    
    - 동일한 콜백 함수를 여러 타이머가 공유할 때, 콜백 내부에서 어떤 타이머가 만료되었는지 구분하는 용도로 사용한다.
        
    - 또한 콜백 함수 호출 사이에 특정 값을 저장하는 용도로도 활용할 수 있다.
        
- `pxCallbackFunction`: 타이머가 만료되었을 때 호출될 함수이다.
    
    - `void vCallbackFunction( TimerHandle_t xTimer );` 형태의 원형을 가져야 한다.
        

#### Return Values

- `NULL`: FreeRTOS 힙 메모리가 부족하여 타이머 구조체를 생성할 수 없는 경우이다.
    
- `이외의 값`: 성공적으로 생성되었으며, 반환된 값은 해당 타이머의 핸들이다.
    

#### 주의사항

- `FreeRTOSConfig.h` 파일에서 `configUSE_TIMERS`와 `configSUPPORT_DYNAMIC_ALLOCATION`이 반드시 1로 설정되어 있어야 이 함수를 사용할 수 있다.
    
- 타이머 콜백 함수 내에서 블록(Block) 상태가 될 수 있는 API를 호출하면 데드락(Deadlock)이 발생할 수 있으므로 주의해야 한다.
    

#### Example

```c
/* 여러 타이머 인스턴스에서 공유할 콜백 함수 */
void vTimerCallback( TimerHandle_t xTimer )
{
    const uint32_t ulMaxExpiryCountBeforeStopping = 10;
    uint32_t ulCount;

    /* 타이머 ID에 저장된 실행 횟수를 가져온다. */
    ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
    ulCount++;

    /* 10번 실행되었다면 타이머를 중지시킨다. */
    if( ulCount >= ulMaxExpiryCountBeforeStopping )
    {
        /* 콜백 내에서 타이머 API 호출 시 대기 시간(Block time)을 0으로 설정한다. */
        xTimerStop( xTimer, 0 );
    }
    else
    {
        /* 증가된 카운트 값을 다시 타이머 ID에 저장한다. */
        vTimerSetTimerID( xTimer, ( void * ) ulCount );
    }
}

#define NUM_TIMERS 5
TimerHandle_t xTimers[ NUM_TIMERS ];

void main( void )
{
    long x;
    for( x = 0; x < NUM_TIMERS; x++ )
    {
        /* 자동 재실행 모드로 5개의 타이머를 생성한다. */
        xTimers[ x ] = xTimerCreate( "Timer", 
                                     ( 100 * x ) + 100, 
                                     pdTRUE, 
                                     ( void * ) 0, 
                                     vTimerCallback );

        if( xTimers[ x ] != NULL )
        {
            /* 스케줄러가 시작되기 전이라도 타이머를 시작 상태로 설정할 수 있다. */
            xTimerStart( xTimers[ x ], 0 );
        }
    }

    /* 스케줄러가 시작되면 타이머들도 즉시 동작을 시작한다. */
    vTaskStartScheduler();
    for( ;; );
}
```