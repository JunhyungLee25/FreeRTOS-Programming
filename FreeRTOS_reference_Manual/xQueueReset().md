194p - xQueueReset() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “queue.h”`

#### 함수 원형

- `BaseType_t xQueueReset( QueueHandle_t xQueue );`
    

#### summary

- 큐를 원래의 비어 있는 상태로 재설정(Reset)한다.
    
- **주의**: 리셋 시점에 큐에 들어있던 모든 데이터는 즉시 폐기(Discard)된다.
    

#### Parameters

- `xQueue`: 리셋할 대상 큐의 핸들이다. (`xQueueCreate()` 등을 통해 반환된 핸들)
    

#### Return Values

- `pdPASS`: FreeRTOS V7.2.0 이후 버전부터는 항상 `pdPASS`를 반환한다. (이전 버전에서는 성공/실패 여부를 반환했음)
    

#### 주의사항

- **데이터 유실**: 이 함수를 호출하면 큐에 쌓여 대기 중이던 모든 메시지가 사라지므로, 정말로 데이터를 버려도 되는 상황인지 확인 후 호출해야 한다.
    
- **Blocked 태스크**: 큐가 가득 차서 데이터를 넣으려고 대기 중이던 태스크(`xQueueSend` 호출 후 Blocked 상태)나, 데이터가 없어서 기다리던 태스크(`xQueueReceive` 호출 후 Blocked 상태)들은 리셋 후에도 대기 상태를 유지할 수 있으므로 설계 시 주의가 필요하다.
    

#### Example

```c
void vResetFunction( QueueHandle_t xQueue )
{
    /* 시스템에 치명적인 오류가 발생하여 큐의 모든 데이터를 무효화해야 하는 경우 */
    if( CheckSystemError() == pdTRUE )
    {
        /* 큐를 비워 이전의 잘못된 데이터들이 처리되지 않도록 함 */
        xQueueReset( xQueue );
        
        printf("Queue has been reset due to system error.\n");
    }
}
```