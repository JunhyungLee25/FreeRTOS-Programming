151p - vTaskSuspend() manual 
#### 함수 라이브러리
`#include “FreeRTOS.h”`
`#include “task.h”`
#### 함수 원형
- `void vTaskSuspend( TaskHandle_t pxTaskToSuspend );`
#### summary
- task를 Suspended(일시정지) state로 전환한다. Suspended state에 있는 task는 결코 Running state로 들어가도록 선택될 수 없다. task를 Suspended 상태에서 해제하는 유일한 방법은 `VTaskResume()`을 호출하는 것이다. 
#### Parameters
- `pxTaskToSuspend`: suspended 시킬 task의 handle이다.
- task handle을 얻으려면 `xTaskCreate()`호출 시 `pxCreatedTask` 매개변수를 사용하거나, `xTaskCreateStatic()`의 반환 값을 저장하거나, `xTaskGetHandle()` 함수를 사용해 task 이름을 조회하면 된다.
- 자기 자신을 일시정지 시키려면 유효한 핸들 대신 `NULL`을 전달하면 된다.
#### Return Values
- 없음.
#### 주의사항
- FreeRTOS 버전 6.1.0 이상을 사용하는 경우, 스케줄러가 시작되기 전(`vTaskStartScheduler()` 호출 전)에도 `vTaskSuspend()`를 호출하여 task를 일시 정지 상태로 둘 수 있다. 이 경우 해당 task는 사실상 일시 정지 상태에서 시작하게 된다.
#### Example
```c
void vAFunction(void)
{
    TaskHandle_t xHandle;
    /* 태스크를 생성하고, 생성된 태스크의 핸들을 xHandle 변수에 저장합니다. */
    if (xTaskCreate(vTaskCode,
                    "Demo task",
                    STACK_SIZE,
                    NULL,
                    PRIORITY,
                    &xHandle) != pdPASS)
    {
        /* 태스크가 성공적으로 생성되지 않았을 경우의 예외 처리입니다. */
    }
    else
    {
        /* 생성된 태스크의 핸들을 사용하여 해당 태스크를 일시 정지 상태로 전환합니다. 
        FreeRTOS 버전 6.1.0부터는 스케줄러가 시작되기 전에도 이 작업이 가능합니다. */
        vTaskSuspend(xHandle);

        /* 다른 태스크에서 vTaskResume(xHandle)을 호출하여 깨워주기 전까지, 
           방금 생성된 태스크는 이 기간 동안 절대 실행되지 않습니다. */

        /* NULL 매개변수를 전달하여 이 함수를 호출하고 있는 현재 task(자기 자신)를 일시 정지시킵니다. */
        vTaskSuspend(NULL);

        /* 이 태스크는 다른 태스크가 vTaskResume()을 사용하여 일시 정지를 해제(Un-suspended)해 
        주어야만 vTaskSuspend(NULL) 호출 이후의 코드를 실행할 수 있습니다. */
    }
}
```