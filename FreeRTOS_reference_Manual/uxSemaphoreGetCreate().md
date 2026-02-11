#### 234p - uxSemaphoreGetCount() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “semphr.h”`

#### 함수 원형

- `UBaseType_t uxSemaphoreGetCount( SemaphoreHandle_t xSemaphore );`
    

#### summary

- 지정된 세마포어의 현재 카운트 값을 반환한다.
    
- **이진 세마포어**: 0(사용 불가) 또는 1(사용 가능)만 반환한다.
    
- **카운팅 세마포어**: 0부터 생성 시 설정한 `uxMaxCount` 사이의 값을 반환한다.
    

#### Parameters

- `xSemaphore`: 상태를 확인할 세마포어의 핸들이다.
    

#### Return Values

- 파라미터로 전달된 `xSemaphore`가 현재 보유하고 있는 카운트 값이다.
    

#### 주의사항

- **디버깅 및 모니터링**: 이 함수를 사용하여 로직을 결정하는 것은 주의해야 한다. 함수를 호출한 직후에 다른 태스크가 세마포어를 가져가 버릴 수 있기 때문이다. (Race Condition 주의)
    
- **ISR 사용**: ISR 내에서 카운트를 확인해야 한다면 이 함수의 ISR 버전인 `uxSemaphoreGetCountFromISR()`를 고려해야 할 수도 있다. (일반적으로는 태스크 레벨 모니터링에 많이 쓰인다.)
    

#### Example

```c
void vMonitorTask( void * pvParameters )
{
    UBaseType_t uxCurrentCount;

    for( ;; )
    {
        /* 현재 사용 가능한 자원의 개수를 확인 */
        uxCurrentCount = uxSemaphoreGetCount( xCountingSemaphore );

        printf( "현재 남은 자원 개수: %d\n", (int)uxCurrentCount );

        if( uxCurrentCount == 0 )
        {
            printf( "경고: 모든 자원이 사용 중입니다!\n" );
        }

        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}
```