175p - uxQueueMessagesWaiting() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “queue.h”`

#### 함수 원형

- `UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );`
    

#### summary

- 특정 큐에 현재 저장되어 있는 항목의 개수를 반환한다.
    

#### Parameters

- `xQueue`: 상태를 확인할 대상 큐의 핸들이다.
    

#### Return Values

- 함수가 호출되는 시점에 큐에 들어있는 항목의 개수를 반환한다. (0이면 비어있음을 의미)
    

#### 주의사항

- **동적 상태 변화**: 반환된 값은 함수가 호출된 그 '찰나'의 개수이다. 멀티태스킹 환경이므로, 값을 반환받은 직후에 다른 태스크가 데이터를 넣거나 빼갈 수 있다는 점을 유의해야 한다.
    
- **인터럽트 사용**: 인터럽트 서비스 루틴(ISR) 내에서는 이 함수 대신 `uxQueueMessagesWaitingFromISR()`을 사용해야 한다.
    
- **빈 공간 확인**: 큐의 전체 크기에서 이 함수의 반환 값을 빼면 현재 비어있는 공간(`uxQueueSpacesAvailable()`)을 알 수 있다.
    

#### Example

```c
void vAFunction( QueueHandle_t xQueue )
{
    UBaseType_t uxNumberOfItems;

    /* xQueue 핸들이 가리키는 큐에 현재 몇 개의 아이템이 있는가? */
    uxNumberOfItems = uxQueueMessagesWaiting( xQueue );

    if( uxNumberOfItems == 0 )
    {
        /* 큐가 비어있을 때의 처리 */
        printf("Queue is empty.\n");
    }
    else
    {
        printf("Current messages in queue: %u\n", (unsigned int)uxNumberOfItems);
    }
}
```