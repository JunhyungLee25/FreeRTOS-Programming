### 238p - xSemaphoreGiveFromISR() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”` `#include “semphr.h”`

#### 함수 원형

- `BaseType_t xSemaphoreGiveFromISR( SemaphoreHandle_t xSemaphore, BaseType_t *pxHigherPriorityTaskWoken );`
    

#### summary

- ISR 내에서 사용할 수 있는 `xSemaphoreGive()`의 인터럽트 버전이다.
    
- 일반 함수와 달리 대기 시간(Block time)을 지정할 수 없다.
    

#### Parameters

- `xSemaphore`: 해제할(Give) 세마포어의 핸들이다. 사용 전 반드시 생성되어 있어야 한다.
    
- `pxHigherPriorityTaskWoken`:
    
    - 세마포어를 해제함으로써 대기 중이던 높은 우선순위 태스크가 깨어났는지를 나타내는 지표이다.
        
    - 함수 내부에서 이 값을 체크하여, 깨어난 태스크가 현재 인터럽트된 태스크보다 우선순위가 높거나 같으면 `pdTRUE`로 설정한다.
        
    - 이 값이 `pdTRUE`가 되면 ISR 종료 전 반드시 문맥 전환을 수행해야 한다. (V7.3.0부터는 선택 사항이며 NULL 전달 가능)
        

#### Return Values

- `pdTRUE`: 세마포어 해제에 성공한 경우.
    
- `errQUEUE_FULL`: 세마포어가 이미 비어있지 않아(이미 Give 된 상태) 해제할 수 없는 경우.
    

#### 주의사항

- 인터럽트 서비스 루틴(ISR) 내에서 호출하면 세마포어를 기다리던 태스크가 Block 상태에서 해제될 수 있다.
    
- 일반 `xSemaphoreGive()`와 달리 함수 스스로 문맥 전환을 수행하지 않고, 문맥 전환이 필요한지 '지표(`pxHigherPriorityTaskWoken`)만 제공한다.
    
- **스케줄러가 시작되기 전에는 절대 호출해서는 안 된다.**
    

#### Example

```c
void vISR( void * pvParameters )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* 이벤트 발생! 세마포어를 주어 태스크를 깨움 */
    xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );

    /* 인터럽트 클리어 로직 필요 시 추가 */

    /* xHigherPriorityTaskWoken이 pdTRUE라면 더 높은 우선순위 태스크로 즉시 전환 */
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
```