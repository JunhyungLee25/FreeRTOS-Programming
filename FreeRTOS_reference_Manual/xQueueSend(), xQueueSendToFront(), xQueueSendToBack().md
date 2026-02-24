199p - xQueueSend(), xQueueSendToFront(), xQueueSendToBack() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “queue.h”`

#### 함수 원형

- `BaseType_t xQueueSend( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );`
    
- `BaseType_t xQueueSendToFront( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );`
    
- `BaseType_t xQueueSendToBack( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );`
    

#### summary

- 큐의 맨 앞 또는 맨 뒤에 데이터 항목을 전송(쓰기)한다.
    
- **xQueueSend()**와 **xQueueSendToBack()**은 동일한 동작을 수행하며, 큐의 맨 뒤에 데이터를 추가한다. (일반적으로 `xQueueSendToBack()` 사용을 권장한다.)
    
- **xQueueSendToFront()**는 데이터를 큐의 맨 앞에 넣어, 다음에 읽을 때 가장 먼저 읽히도록 한다. (긴급 메시지 처리에 유용하다.)
    

#### Parameters

- `xQueue`: 데이터를 보낼 대상 큐의 핸들이다.
    
- `pvItemToQueue`: 큐에 복사될 데이터의 포인터이다.
    
    - 큐 생성 시 설정한 `uxItemSize`만큼의 바이트가 이 포인터로부터 큐 저장소로 복사된다.
        
- `xTicksToWait`: 큐가 이미 가득 찼을(Full) 경우, 빈 공간이 생길 때까지 태스크가 **Blocked(대기)** 상태에서 기다릴 최대 시간이다.
    
    - `0`: 큐가 꽉 찼으면 즉시 `errQUEUE_FULL`을 반환하며 종료한다.
        
    - `pdMS_TO_TICKS()`: 밀리초 단위를 틱 단위로 변환하여 설정한다.
        
    - `portMAX_DELAY`: 공간이 생길 때까지 무한정 기다린다. (단, `INCLUDE_vTaskSuspend` 설정 필요)
        

#### Return Values

- `pdPASS`: 데이터가 큐에 성공적으로 전송되었을 때 반환한다.
    
- `errQUEUE_FULL`: 큐가 가득 차서 데이터를 쓸 수 없을 때 반환한다. (대기 시간을 설정했다면, 그 시간 동안에도 공간이 생기지 않은 경우이다.)
    

#### 주의사항

- 이 함수들은 **인터럽트 서비스 루틴(ISR) 내에서 호출해서는 안 된다.** ISR에서는 `xQueueSendFromISR()` 등을 사용해야 한다.
    
- 데이터는 **값에 의한 복사(Copy by Value)** 방식으로 저장되므로, 함수 호출 후 원본 변수의 값이 변해도 큐에 이미 들어간 값에는 영향을 주지 않는다.
    

#### Example

```c
/* 큐에 담을 데이터 타입 정의 */
typedef struct A_Message {
    char ucMessageID;
    char ucData[ 20 ];
} AMessage;

void vATask( void *pvParameters )
{
    QueueHandle_t xQueue;
    AMessage xMessage;

    xQueue = ( QueueHandle_t ) pvParameters;

    for( ;; )
    {
        /* 보낼 데이터 준비 */
        xMessage.ucMessageID = SEND_EXAMPLE;

        /* 큐의 맨 뒤에 데이터를 전송. 큐가 찼다면 10 틱 동안 기다림 */
        if( xQueueSendToBack( xQueue, &xMessage, 10 ) != pdPASS )
        {
            /* 10 틱을 기다렸음에도 큐가 비워지지 않아 전송 실패 */
            printf("Could not send to queue!\n");
        }
        else
        {
            /* 전송 성공 */
        }
    }
}
```