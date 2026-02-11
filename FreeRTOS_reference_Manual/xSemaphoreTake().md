#### 244p - xSemaphoreTake() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “semphr.h”`

#### 함수 원형

- `BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait );`
    

#### summary

- 이미 생성된 이진 세마포어, 카운팅 세마포어, 혹은 뮤텍스를 '획득(Take)'한다.
    

#### Parameters

- `xSemaphore`: 획득하려는 세마포어의 핸들이다.
    
- `xTicksToWait`:
    
    - 세마포어가 즉시 사용 불가능할 경우, 태스크가 **Blocked(대기)** 상태로 머물 수 있는 최대 시간이다.
        
    - 단위는 'Tick'이며, `pdMS_TO_TICKS()` 매크로를 사용하여 밀리초(ms)를 틱으로 변환할 수 있다.
        
    - `0`으로 설정하면 기다리지 않고 즉시 반환한다.
        
    - `portMAX_DELAY`로 설정하면 세마포어를 얻을 때까지 무한정 대기한다. (이때 `INCLUDE_vTaskSuspend`가 1이어야 함)
        

#### Return Values

- `pdPASS`: 세마포어를 성공적으로 획득한 경우이다. (기다리는 도중에 자리가 나서 얻은 경우도 포함됨)
    
- `pdFAIL`: 지정된 대기 시간(`xTicksToWait`)이 지날 때까지 세마포어를 얻지 못한 경우이다.
    

#### 주의사항

- **태스크 전용**: 반드시 실행 중인 태스크 내에서 호출해야 하며, **ISR(인터럽트) 내에서는 절대 호출해서는 안 된다.** (ISR에서는 `xSemaphoreTakeFromISR` 사용)
    
- **스케줄러 확인**: 스케줄러가 시작되기 전(초기화 상태)이나, 스케줄러가 정지된 상태, 또는 임계 영역(Critical Section) 내에서 호출해서는 안 된다.
    

#### Example

```c
SemaphoreHandle_t xSemaphore = NULL;

void vAnotherTask( void * pvParameters )
{
    for( ;; )
    {
        if( xSemaphore != NULL )
        {
            /* 10틱 동안 세마포어 획득을 시도함 */
            if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdPASS )
            {
                /* 열쇠를 얻었으므로 안전하게 공유 자원에 접근 */
                // [Critical Section 로직 수행]

                /* 작업 완료 후 반드시 열쇠를 돌려줌 */
                xSemaphoreGive( xSemaphore );
            }
            else
            {
                /* 10틱을 기다렸지만 열쇠를 얻지 못함. 안전을 위해 자원에 접근하지 않음 */
            }
        }
    }
}
```