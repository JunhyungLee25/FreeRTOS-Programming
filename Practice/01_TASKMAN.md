## main.c 

- 핵심함수
`USERTHREAD()`: 태스크 리소스를 만들기 위해 요청하는 함수.
`osKerrnelStart()`: 스케쥴러 동작시키는 함수.

- 하드웨어 초기화 함수
`HAL_Init()`: 하드웨어 추상화 계층(HAL) 초기화. 
`SystemClock_Config()`: CPU와 주변 장치의 동작 속도(Hz)를 설정. (HSE, PLL 등)
**`MX_GPIO_Init()`**: 입출력 핀(LED, 스위치 등)의 방향과 모드를 설정.
**`MX_USART3_UART_Init()`**: PC와 통신하기 위한 UART3 포트를 설정.
**`MX_USB_OTG_FS_PCD_Init()`**: USB 장치 통신을 위한 인터페이스 초기화.

****
## TODO #1
>FreeRTOS의 커널 서비스인 task 생성 함수를 이용한 실습 예제이다.'

- ‘FreeRTOS 커널 API 레퍼런스 가이드’를 참조, 태스크 실행 중단 함수의 사용법을 확인 한 후 코드를 완성하면 된다. 
- "#if 0" 를 "#if 1"로 하여 컴파일 할 수 있다.

```c
// TASK CREATE
	/*TODO #1
		TASK1을 생성
		use 'xTaskCreate' */
#if 0

#endif // TODO #1
```

태스크 생성 함수를 구현한다.
컴파일하고 실행하여 원하는 결과가 출력되는지 확인한다.
터미널 창에 "Task1 is running" 메시지가 보인다면 성공.


```c
// TASK CREATE
#if 1
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					128,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */
#endif // TODO #1
```

- 실행결과
![](images/Pasted_image_20260111200639.png)
TASK2(9)는 TASK1(10)에 우선순위가 밀려 출력되지 않음

****
## TODO #2
>FreeRTOS의 커널 서비스인 태스크 실행 중단 함수를 이용한 실습 예제이다. 

- ‘FreeRTOS 커널 API 레퍼런스 가이드’를 참조, 태스크 실행 중단 함수의 사용법을 확인 한 후 코드를 완성하면 된다. 
- "#if 0" 를 "#if 1"로 하여 컴파일 할 수 있다.

```c
	/* TODO #2:
		Task1을 중지
		use 'vTaskSuspend' */
#if 0

#endif // TODO #2
```

태스크 동작 중지 함수를 구현한다
컴파일하고 실행하여 원하는 결과가 출력 되는지 확인한다. 
터미널 창에  “Task2 is running” 메시지가 보인다면 성공.

### 2.33 vTaskSuspend() (151p) 내용 정리

#### Header
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
    if (xTaskCreate(vTaskCode,
                    "Demo task",
                    STACK_SIZE,
                    NULL,
                    PRIORITY,
                    &xHandle) != pdPASS)
    {
        /* The task was not created successfully. */
    }
    else
    {
        vTaskSuspend(xHandle);
        vTaskSuspend(NULL);
    }
}
```
example을 통해 task의 handle을 parameter로 넣으므로써 그 task를 일시정지 할 수 있음을 알 수 있다.
따라서, TODO2에서 TASK1을 일시정지 시키라고 하였으니 TASK1의 handle인 `xHandle1`을 인자로 한 `vTaskSuspend(xHandle1)`을 추가하면 된다.

```c
// TASK SUSPEND
#if 1

  vTaskSuspend(xHandle1);

#endif // TODO #2
```

- 실행결과
![](images/Pasted_image_20260111203226.png)

- TASK1이 출력되지 않은 이유:
TASK1이 실행되기 위해서는 `xTaskCreate()`로 task를 생성한 후 `osKernelStart()`로 스케쥴러를 동작시켜야 우선순위에 따라 task가 동작하게 되는데 TASK2에서 `vTaskSuspend(xHandle1)`을 실행하여 TASK1을 강제로 일시정지 상태로 보냈기 때문에 TASK1이 실행되지 않았다.
스케쥴러는 오직 READY 상태에 있는 task들 중에서만 실행상태를 고르기 때문이다.

****