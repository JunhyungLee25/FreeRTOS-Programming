### 247p - xSemaphoreTakeFromISR() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”` `#include “semphr.h”`

#### 함수 원형

- `BaseType_t xSemaphoreTakeFromISR( SemaphoreHandle_t xSemaphore, BaseType_t *pxHigherPriorityTaskWoken );`
    

#### summary

- ISR 내에서 사용할 수 있는 `xSemaphoreTake()`의 인터럽트 버전이다.
    
- **대기 시간(Block time)을 가질 수 없으며**, 호출 시 즉시 획득 가능 여부를 반환한다.
    

#### Parameters

- `xSemaphore`: 획득할(Take) 세마포어의 핸들이다.
    
- `pxHigherPriorityTaskWoken`:
    
    - 세마포어를 획득함으로써 대기 중이던 다른 태스크(주로 Give를 기다리던 태스크)가 깨어났는지 확인한다.
        
    - 만약 이 작업으로 인해 더 높은 우선순위 태스크가 Ready 상태가 되면 `pdTRUE`로 설정된다.
        
    - 이 값이 `pdTRUE`인 경우 ISR 종료 전 문맥 전환을 수행해야 한다. (V7.3.0부터 선택 사항, NULL 가능)
        

#### Return Values

- `pdPASS`: 세마포어를 성공적으로 획득한 경우.
    
- `pdFAIL`: 세마포어가 현재 사용 불가능하여 획득에 실패한 경우.
    

#### 주의사항

- 인터럽트는 지연되어서는 안 되므로, **세마포어가 없다고 해서 기다리는 기능이 아예 배제**되어 있다.
    
- ISR에서 세마포어를 Take 하는 경우는 드물지만, 공유 자원을 아주 짧게 확인해야 하는 경우에 사용된다.
    

#### Example

```c
void vAnInterruptHandler( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* ISR 내에서 세마포어 획득 시도 (기다리지 않음) */
    if( xSemaphoreTakeFromISR( xSemaphore, &xHigherPriorityTaskWoken ) == pdPASS )
    {
        /* 획득 성공! 짧은 작업 수행 */
        
        /* 작업 후 필요하다면 다시 Give 하거나 후속 조치 */
    }

    /* 문맥 전환 필요 여부 확인 및 실행 */
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
```