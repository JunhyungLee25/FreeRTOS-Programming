186p - xQueueReceive() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “queue.h”`

#### 함수 원형

- `BaseType_t xQueueReceive( QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait );`
    

#### summary

- 큐로부터 항목 하나를 수신(읽기)한다.
    
- 수신된 항목은 큐에서 제거되며, 데이터는 지정된 버퍼로 복사된다.
    

#### Parameters

- `xQueue`: 데이터를 읽어올 대상 큐의 핸들이다. (`xQueueCreate()`로 생성된 핸들)
    
- `pvBuffer`: 수신한 데이터를 복사해 넣을 메모리 버퍼의 포인터이다.
    
    - **주의**: 이 버퍼의 크기는 큐를 생성할 때 설정한 `uxItemSize`와 같거나 커야 한다.
        
- `xTicksToWait`: 큐가 비어있을 경우, 데이터가 들어올 때까지 태스크가 **Blocked(대기)** 상태에서 기다릴 최대 시간이다.
    
    - `0`: 큐가 비어있으면 즉시 반환한다.
        
    - `pdMS_TO_TICKS()`: 밀리초 단위를 틱(Tick) 단위로 변환하여 설정한다.
        
    - `portMAX_DELAY`: 데이터가 들어올 때까지 무한정 기다린다. (단, `INCLUDE_vTaskSuspend`가 1로 설정되어 있어야 함)
        

#### Return Values

- `pdPASS`: 큐에서 데이터를 성공적으로 읽었을 때 반환한다.
    
- `errQUEUE_EMPTY`: 큐가 비어있어 읽기에 실패했을 때 반환한다. (대기 시간을 설정했다면, 그 시간 동안에도 데이터가 들어오지 않은 경우이다.)
    

#### 주의사항

- **데이터 복사 방식 (Copy by Value)**: 큐는 데이터를 참조(주소)가 아닌 **값 자체를 복사**하여 전달한다. 따라서 수신 측 버퍼에서 데이터를 수정해도 송신 측의 원본 데이터에는 영향을 주지 않는다.
    
- 인터럽트(ISR) 내에서는 이 함수 대신 `xQueueReceiveFromISR()`을 사용해야 한다.
    

#### Example

```c
/* 큐에 담을 데이터 타입 정의 */
typedef struct A_Message
{
    char ucMessageID;
    char ucData[ 20 ];
} AMessage;

void vAnotherTask( void *pvParameters )
{
    QueueHandle_t xQueue;
    AMessage xMessage;

    /* 넘겨받은 파라미터를 큐 핸들로 캐스팅 */
    xQueue = ( QueueHandle_t ) pvParameters;

    for( ;; )
    {
        /* 큐에 데이터가 들어올 때까지 무한정 대기(Blocked) */
        if( xQueueReceive( xQueue, &xMessage, portMAX_DELAY ) == pdPASS )
        {
            /* xMessage에 데이터가 성공적으로 수신됨 */
            // 데이터 처리 로직 작성...
        }
        else
        {
            /* 무한 대기이므로 실제로는 여기까지 내려오지 않지만, 
               일반적인 타임아웃 설정 시 실패 처리를 여기서 함 */
        }
    }
}
```