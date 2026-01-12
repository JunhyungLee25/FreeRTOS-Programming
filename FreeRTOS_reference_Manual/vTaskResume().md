133p - vTaskResume() manual
#### 함수 라이브러리
`#include “FreeRTOS.h”` 
`#include “task.h”`
#### 함수 원형
- void vTaskResume( TaskHandle_t pxTaskToResume );
#### summary
- 태스크를 일시 정지(Suspended) 상태에서 준비(Ready) 상태로 전환한다. 이 함수를 통해 재개하려는 태스크는 이전에 반드시 vTaskSuspend() 호출을 통해 일시 정지 상태가 되어 있어야 한다.
#### Parameters
- `pxTaskToResume`: 재개(일시 정지 상태에서 전환)시킬 대상 태스크의 핸들이다.
	- 태스크 핸들을 얻으려면 xTaskCreate() 호출 시 pxCreatedTask 매개변수를 사용하거나, xTaskCreateStatic()의 반환 값을 저장하거나, xTaskGetHandle() 함수를 사용해 태스크 이름을 조회하면 된다.
#### Return Values
- 없음.
#### 주의사항
- Queue/Semaphore 대기 중 정지된 경우: 태스크가 타임아웃을 지정하여 큐 이벤트를 기다리며 Blocked 상태에 있을 때 vTaskSuspend()로 정지되었다면, vTaskResume() 호출 시 다시 Ready 상태가 된다. 이후 다시 Running 상태에 진입하면 그동안 타임아웃 시간이 만료되었는지 확인하며, 만료되지 않았다면 원래 지정된 타임아웃의 남은 시간 동안 다시 Blocked 상태로 들어가 대기한다.
- vTaskDelay/Until 대기 중 정지된 경우: 태스크가 vTaskDelay() 등에 의해 시간 지연(Blocked) 중일 때 정지되었다가 재개되면, 실제 지연 시간이 다 지나지 않았더라도 지연 시간이 만료된 것처럼 즉시 지연 함수를 빠져나와 실행된다.
- vTaskResume()은 반드시 실행 중인 태스크(Executing task) 내에서 호출되어야 하며, 스케줄러가 초기화 상태(스케줄러 시작 전)일 때는 호출해서는 안 된다.
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
                     &xHandle 
                   ) != pdPASS )
    {
        /* 태스크가 성공적으로 생성되지 않음. */
    }
    else
    {           
        /* 핸들을 사용하여 생성된 태스크를 일시 정지시킨다. */
        vTaskSuspend( xHandle );

        /* 다른 태스크에서 vTaskResume(xHandle)을 호출하기 전까지
           일시 정지된 태스크는 실행되지 않는다. */

        /* 일시 정지된 태스크를 다시 재개시킨다. */
        vTaskResume( xHandle );

        /* 이제 생성되었던 태스크는 다시 스케줄러에 의해 선택될 수 있으며,
           Running 상태로 진입할 수 있다. */
    }
}
```
