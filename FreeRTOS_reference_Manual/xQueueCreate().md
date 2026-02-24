162p - xQueueCreate() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “queue.h”`

#### 함수 원형

- `QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );`
    

#### summary

- 새로운 큐를 생성하고, 해당 큐를 참조할 수 있는 핸들을 반환한다.
    
- 큐를 생성하기 위해 필요한 RAM(큐의 상태 정보 및 아이템 저장 공간)은 FreeRTOS 힙(Heap)에서 자동으로 할당된다.
    
- 참고: 메모리를 직접 관리하고 싶다면 `xQueueCreateStatic()`을 사용해 정적 할당을 할 수도 있다.
    

#### Parameters

- `uxQueueLength`: 생성할 큐가 한 번에 보유할 수 있는 최대 항목 개수이다.
    
- `uxItemSize`: 큐에 저장될 각 데이터 항목의 크기(Byte 단위)이다.
    
    - 보통 `sizeof(구조체명)` 형식을 사용하여 실수 없이 크기를 지정한다.
        

#### Return Values

- `NULL`: FreeRTOS 힙 메모리가 부족하여 큐 데이터 구조나 저장 공간을 할당할 수 없는 경우이다.
    
- `그 외의 값`: 큐가 성공적으로 생성되었음을 의미하며, 반환된 값은 생성된 큐의 핸들이다.
    

#### 주의사항

- **용도**: 큐는 태스크와 태스크 간, 혹은 태스크와 인터럽트(ISR) 간에 데이터를 전달할 때 사용된다.
    
- **생성 시점**: 스케줄러가 시작되기 전(main 함수 등)이나 시작된 후 모두 호출 가능하다.
    
- **설정 확인**: 이 함수를 사용하려면 `FreeRTOSConfig.h` 파일에 `configSUPPORT_DYNAMIC_ALLOCATION`이 1로 설정되어 있거나 정의되어 있지 않아야 한다.
    

#### Example

```c
/* 큐에 담을 데이터 타입 정의 */
typedef struct A_Message
{
    char ucMessageID;
    char ucData[ 20 ];
} AMessage;

/* 큐 설정 값 정의 */
#define QUEUE_LENGTH 5
#define QUEUE_ITEM_SIZE sizeof( AMessage )

int main( void )
{
    QueueHandle_t xQueue;

    /* 큐 생성 및 핸들 저장 */
    xQueue = xQueueCreate( QUEUE_LENGTH, QUEUE_ITEM_SIZE );

    if( xQueue == NULL )
    {
        /* 힙 메모리 부족으로 큐 생성 실패 시 처리 */
        printf("Queue creation failed!\n");
    }
    else
    {
        /* 큐 생성 성공, 이후 코드 작성 */
    }
}
```