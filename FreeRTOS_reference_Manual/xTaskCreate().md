34p - xTaskCreate() manual
#### 함수 라이브러리
`#include “FreeRTOS.h”` 
`#include “task.h”`
#### 함수 원형
- `BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, const char * const pcName, unsigned short usStackDepth, void *pvParameters, UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask );`
#### summary
- 새로운 태스크 인스턴스를 생성한다.
- 각 태스크는 상태 정보(TCB, Task Control Block)를 저장할 RAM과 스택(Stack)으로 사용할 RAM이 필요하다. `xTaskCreate()`를 사용하면 필요한 RAM이 FreeRTOS 힙(Heap)에서 자동으로 할당된다.
- 새롭게 생성된 태스크는 처음에 Ready(준비) 상태로 배치되지만, 실행 가능한 더 높은 우선순위의 태스크가 없다면 즉시 Running(실행) 상태가 된다.
- 스케줄러가 시작되기 전과 후 모두에서 태스크를 생성할 수 있다.
#### Parameters
- `pvTaskCode`: 태스크를 구현하는 함수에 대한 포인터이다. 태스크는 종료되지 않는 무한 루프 형태의 C 함수로 작성되어야 한다.
- `pcName`: 태스크를 설명하는 이름이다. 주로 디버깅을 돕기 위해 사용되며, `xTaskGetHandle()`을 통해 핸들을 얻을 때도 사용될 수 있다.
    - `configMAX_TASK_NAME_LEN`에 정의된 최대 길이를 초과하면 자동으로 잘린다.
- `usStackDepth`: 커널이 태스크를 위해 할당할 스택의 크기이다.
    - **주의**: 바이트(Byte) 단위가 아니라 **워드(Word) 단위**이다. 예를 들어 스택 폭이 4바이트인 아키텍처에서 100을 전달하면 400바이트가 할당된다.
    - Idle 태스크의 스택 크기는 `configMINIMAL_STACK_SIZE`로 정의되며, 일반적인 태스크는 이보다 큰 값을 할당해야 하는 경우가 많다.
- `pvParameters`: 태스크 함수로 전달될 매개변수 포인터(`void *`)이다.
    - 어떤 타입의 데이터도 전달할 수 있도록 `void *` 타입을 가지며, 태스크 함수 내부에서 적절한 타입으로 형변환하여 사용한다.
- `uxPriority`: 태스크가 실행될 우선순위이다.
    - 0(최저 우선순위)부터 (`configMAX_PRIORITIES – 1`, 최고 우선순위)까지 할당할 수 있다.
    - 범위를 초과하는 값을 전달하면 자동으로 최대 허용치로 제한된다.
- `pxCreatedTask`: 생성된 태스크의 핸들을 받아올 포인터이다.
    - 이 핸들은 이후 우선순위 변경이나 태스크 삭제 등 API 호출 시 해당 태스크를 참조하는 데 사용된다. 핸들이 필요 없다면 `NULL`을 전달하면 된다.
#### Return Values
- `pdPASS`: 태스크가 성공적으로 생성되었음을 나타낸다.
- `errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`: FreeRTOS 힙 메모리가 부족하여 태스크의 데이터 구조(TCB)나 스택을 할당할 수 없는 경우 반환된다.
#### 주의사항
- 이 함수를 사용하기 위해서는 `FreeRTOSConfig.h` 파일 내에 `configSUPPORT_DYNAMIC_ALLOCATION`이 1로 설정되어 있거나 정의되지 않은 상태여야 한다.
- 메모리 할당 실패 시 `vApplicationMallocFailedHook()` 콜백 함수를 통해 예외 처리를 할 수 있으며, `xPortGetFreeHeapSize()`로 남은 힙 크기를 확인할 수 있다.
- 스택 크기는 해당 아키텍처의 변수 타입 `size_t`가 가질 수 있는 최대치를 넘지 않도록 설정해야 한다. 
#### Example
```c
/* 태스크로 전달할 구조체 정의 */
typedef struct A_STRUCT
{
    char cStructMember1;
    char cStructMember2;
} xStruct;

xStruct xParameter = {1, 2};

/* 구현될 태스크 함수 */
void vTaskCode( void * pvParameters )
{
    xStruct *pxParameters;

    /* void * 매개변수를 원래 타입으로 형변환 */
    pxParameters = ( xStruct * ) pvParameters;

    if( pxParameters->cStructMember1 != 1 )
    {
        /* 예외 처리 등 */
    }

    /* 태스크 처리를 위한 무한 루프 */
    for( ;; )
    {
        /* 태스크 코드 실행 */
    }
}

/* 태스크를 생성하는 함수 */
void vAnotherFunction( void )
{
    TaskHandle_t xHandle;

    /* 태스크 생성 시도 */
    if( xTaskCreate( 
            vTaskCode,         /* 태스크 함수 포인터 */
            "Demo task",       /* 태스크 이름 */
            STACK_SIZE,        /* 스택 크기 (워드 단위) */
            (void*) &xParameter, /* 전달할 매개변수 */
            TASK_PRIORITY,     /* 우선순위 */
            &xHandle           /* 핸들 저장 위치 */
        ) != pdPASS )
    {
        /* 힙 메모리 부족으로 생성 실패 시 처리 */
    }
    else
    {
        /* 생성 성공. 핸들을 사용하여 우선순위 변경 예시 */
        vTaskPrioritySet( xHandle, 2 );
    }
}
```