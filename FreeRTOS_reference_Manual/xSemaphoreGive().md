#### 236p - xSemaphoreGive() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “semphr.h”`

#### 함수 원형

- `BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore );`
    

#### summary

- 이전 단계에서 `vSemaphoreCreateBinary()`, `xSemaphoreCreateCounting()`, 또는 `xSemaphoreCreateMutex()`를 통해 생성되고, `xSemaphoreTake()`로 성공적으로 획득했던 세마포어를 해제(반납)한다.
    

#### Parameters

- `xSemaphore`: 해제할(반납할) 세마포어의 핸들이다. 사용 전 반드시 생성되어 있어야 한다.
    

#### Return Values

- `pdPASS`: 세마포어 해제 작업이 성공적으로 완료된 경우이다.
    
- `pdFAIL`: 세마포어 해제에 실패한 경우이다. 주로 호출한 태스크가 해당 세마포어의 소유자(Holder)가 아니거나, 이미 해제된 세마포어를 다시 해제하려고 할 때 발생한다. (특히 뮤텍스의 경우 획득한 태스크만 반납할 수 있다.)
    

#### 주의사항

- **반드시 짝을 이뤄야 함**: 뮤텍스(Mutex)나 상호 배제 용도의 세마포어는 `Take`를 성공했다면 반드시 `Give`를 호출해야 한다. 그렇지 않으면 다른 태스크들이 해당 자원을 영원히 사용하지 못하게 된다.
    
- **ISR 사용 금지**: 이 함수는 일반 태스크용이다. 인터럽트 서비스 루틴 내에서는 반드시 인터럽트 전용인 `xSemaphoreGiveFromISR()`를 사용해야 한다.
    

#### Example

```c
SemaphoreHandle_t xSemaphore = NULL;

void vATask( void * pvParameters )
{
    /* 상호 배제를 위해 우선순위 상속 기능이 있는 뮤텍스를 생성함 */
    xSemaphore = xSemaphoreCreateMutex();

    if( xSemaphore != NULL )
    {
        for( ;; )
        {
            /* 세마포어를 획득하지 않은 상태에서 Give를 호출하면 실패함(pdFAIL) */
            if( xSemaphoreGive( xSemaphore ) != pdTRUE ) {
                // 반납 실패 처리
            }

            /* 세마포어 획득 시도 (기다리지 않음) */
            if( xSemaphoreTake( xSemaphore, 0 ) == pdPASS )
            {
                /* 자원 보호 구역 진입 */
                // [공유 자원 접근 로직]

                /* 작업 완료 후 반드시 세마포어를 반납함 */
                if( xSemaphoreGive( xSemaphore ) != pdPASS ) {
                    // 이 시점에서는 성공해야 함
                }
            }
        }
    }
}
```