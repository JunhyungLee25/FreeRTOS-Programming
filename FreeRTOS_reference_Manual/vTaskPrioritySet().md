131p - vTaskPrioritySet() manual
#### 함수 라이브러리
`#include “FreeRTOS.h”` 
`#include “task.h”`
#### 함수 원형
- `void vTaskPrioritySet( TaskHandle_t pxTask, UBaseType_t uxNewPriority );`
#### summary
- Task의 priority를 변경한다.
#### Parameters
- `pxTask`: 수정할 대상 태스크의 핸들이다.
    - 태스크 핸들을 얻으려면 `xTaskCreate()` 호출 시 `pxCreatedTask` 매개변수를 사용하거나, `xTaskCreateStatic()`의 반환 값을 저장하거나, `xTaskGetHandle()` 함수를 사용해 태스크 이름을 조회하면 된다.
    - 유효한 태스크 핸들 대신 `NULL`을 전달하면 호출한 자기 자신의 우선순위를 변경한다.
- `uxNewPriority`: 대상 태스크에 설정할 새로운 우선순위이다.
    - 우선순위는 0(최저 우선순위)부터 (`configMAX_PRIORITIES – 1`, 최고 우선순위)까지 할당할 수 있다.
    - `configMAX_PRIORITIES`는 `FreeRTOSConfig.h`에 정의되어 있다. 만약 (`configMAX_PRIORITIES – 1`)보다 큰 값을 전달하면, 해당 태스크에 할당되는 우선순위는 허용되는 최대치로 제한된다.
#### Return Values
- 없음.
#### 주의사항
- `vTaskPrioritySet()`은 반드시 실행 중인 태스크(Executing task) 내에서 호출되어야 한다. 따라서 스케줄러가 초기화 상태일 때(스케줄러가 시작되기 전) 호출해서는 안 된다.
- 동일한 큐나 세마포어 이벤트를 기다리며 Blocked 상태에 있는 태스크 세트가 있을 수 있다. 이 태스크들은 우선순위에 따라 정렬된다. 예를 들어 첫 번째 이벤트는 기다리고 있던 태스크 중 가장 높은 우선순위 태스크의 Block 상태를 해제한다. 이때 `vTaskPrioritySet()`을 사용하여 이러한 Blocked 태스크의 우선순위를 변경하더라도, 이미 정렬된 대기 순서(Blocked tasks are assessed)가 재평가되어 순서가 바뀌지는 않는다.
#### Example
```c
void vAFunction( void )
{
    TaskHandle_t xHandle;

    /* 태스크를 생성하고, 생성된 태스크의 핸들을 xHandle에 저장한다. */
    if( xTaskCreate( vTaskCode, 
                     "Demo task", 
                     STACK_SIZE, 
                     NULL, 
                     PRIORITY, 
                     &xHandle ) != pdPASS )
    {
        /* 태스크가 성공적으로 생성되지 않음. */
    }
    else
    {
        /* 핸들을 사용하여 생성된 태스크의 우선순위를 높인다. */
        vTaskPrioritySet( xHandle, PRIORITY + 1 );

        /* 유효한 태스크 핸들 대신 NULL을 사용하여 호출한 태스크의 우선순위를 1로 설정한다. */
        vTaskPrioritySet( NULL, 1 );
    }
}
```