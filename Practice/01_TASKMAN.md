## main.c 

- 핵심함수
`USERTHREAD()`: 태스크 리소스를 만들기 위해 요청하는 함수.
`osKernelStart()`: 스케쥴러 동작시키는 함수.

- 하드웨어 초기화 함수
`HAL_Init()`: 하드웨어 추상화 계층(HAL) 초기화. 
`SystemClock_Config()`: CPU와 주변 장치의 동작 속도(Hz)를 설정. (HSE, PLL 등)
**`MX_GPIO_Init()`**: 입출력 핀(LED, 스위치 등)의 방향과 모드를 설정.
**`MX_USART3_UART_Init()`**: PC와 통신하기 위한 UART3 포트를 설정.
**`MX_USB_OTG_FS_PCD_Init()`**: USB 장치 통신을 위한 인터페이스 초기화.

****
## TODO #1
>FreeRTOS의 커널 서비스인 task 생성 함수를 이용한 실습 예제이다.'

- ‘FreeRTOS 커널 API 레퍼런스 가이드’를 참조, 태스크 생성 함수의 사용법을 확인 한 후 코드를 완성하면 된다. 
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
	xTaskCreate(	(TaskFunction_t)Task1,		/* 태스크를 구현하는 함수에 대한 포인터이다. */
					"Task1",	/* 태스크의 텍스트 이름이다. 오직 디버깅 편의를 위해 사용된다. */
					128,		/* 스택 깊이이다. 대부분의 작은 마이크로컨트롤러는 이보다 훨씬 적은 스택을 사용한다. */
					NULL,		/* 태스크 매개변수를 사용하지 않는다. */
					TASK_1_PRIO,	/* 이 태스크는 지정된 우선순위로 실행된다. */
					&xHandle1 );		/* 생성된 태스크의 핸들을 xHandle1 변수에 저장한다. */
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

[vTaskSuspend()](../FreeRTOS_reference_Manual/vTaskSuspend().md)함수에 task의 handle을 parameter로 넣어서 그 task를 일시정지 할 수 있다.
따라서, TODO2에서 TASK1을 일시정지 시키라고 하였으니 TASK1의 handle인 `xHandle1`을 인자로 한 `vTaskSuspend(xHandle1)`을 코드에 추가하면 된다.

```c
// TASK SUSPEND
#if 1
  vTaskSuspend(xHandle1);
#endif // TODO #2
```


- 실행결과
![](images/Pasted_image_20260111203226.png)

- 왜 TASK1이 출력되지 않았을까?
	TASK1이 실행되기 위해서는 `xTaskCreate()`로 task를 생성한 후 `osKernelStart()`로 스케쥴러를 동작시켜야 우선순위에 따라 task가 동작하게 되는데 TASKMAIN에서 `vTaskSuspend(xHandle1)`을 실행하여 TASK1을 강제로 일시정지 상태로 보냈기 때문에 TASK1이 실행되지 않았다.
	스케쥴러는 오직 READY 상태에 있는 task들 중에서만 실행상태를 고르기 때문이다.

****

## TODO #3
>FreeRTOS의 커널 서비스인 태스크 관리 함수를 이용한 실습 예제이다. 

"#if 0" 를 "#if 1"로 하여 컴파일 할 수 있다

```c
//TASK1 
    while(1) {
    /* TODO #3:
        코드를 실행 하여 보고
        vTaskDelay() 코드를 주석 처리한 후 그 결과를 설명한다 */
#if 1 // No comment
vTaskDelay (pdMS_TO_TICKS (1000));
printf("a"); fflush(stdout);	// 문자 'a' 출력
#endif // TODO #3


#endif // TODO #3 
//TASK2 
    while(1) {
    /* TODO #3:
        코드를 실행 하여 보고
        vTaskDelay() 코드를 주석 처리한 후 그 결과를 설명한다 */
#if 1
vTaskDelay (pdMS_TO_TICKS (1000));
printf("b"); fflush(stdout);	// 문자 'b' 출력
#endif // TODO #3
```

[vTaskDelay()](../FreeRTOS_reference_Manual/vTaskDelay().md)를 이용하여 TASK를 휴면 상태로 전이시킬 수 있다.

- `vTaskDelay` 주석 처리 전
    `vTaskDelay`에 의해 running 상태에서 1000ms동안 휴면 상태로 있다가 ready 상태가 된 후 다시 running 상태로 복귀한다 -> 1초에 'b'가 하나씩 찍히게 된다.
    이때, running state에 아무 task가 존재하지 않는 것처럼 보이지만 사실 'IDLE task'(우선순위가 가장 낮은 task)라고 하는 것이 아무 동작도 하지 않지만 존재한다.
```c
    while(1) {
    /* TODO #3:
        코드를 실행 하여 보고
        vTaskDelay() 코드를 주석 처리한 후 그 결과를 설명한다 */
#if 1 // No comment
vTaskDelay (pdMS_TO_TICKS (1000));
printf("b"); fflush(stdout);	// 문자 'b' 출력
#endif // TODO #3
```

- `vTaskDelay` 주석처리 후 
    delay 없이 'b'가 훨씬 빠르게 찍히게 된다.
```c
    /* TODO #3:
        코드를 실행 하여 보고
        vTaskDelay() 코드를 주석 처리한 후 그 결과를 설명한다 */
#if 1 // No comment
//vTaskDelay (pdMS_TO_TICKS (1000));
printf("b"); fflush(stdout);	// 문자 'b' 출력
```

- TASK1의 'a'가 출력되지 않은 이유:
    현재 TASK1은 `vTaskSuspend()`에 의해 일시정지 상태이다.

****
## TODO #4
>FreeRTOS의 커널 서비스인 태스크 우선 순위 변경 함수를 이용한 실습 예이다. ‘FreeRTOS 커널 API 레퍼런스 가이드’를 참조, 태스크 우선 순위 변경 함수의 사용법을 확인 한 후 코드를 완성하면 된다. 

"#if 0" 를 "#if 1"로 하여 컴파일 할 수 있다
```c
/* TODO #4:  
    Task1의 우선 순위를 'TASK_3_PRIO' 으로 변경
    use 'vTaskPrioritySet' and 'vTaskResume' */ 
#if 1 
#endif // TODO #4 
```
태스크 우선 순위 변경 함수를 구현한다
컴파일하고 실행하여 원하는 결과가 출력 되는지 확인한다
터미널창에 그림처럼 “ba ba ba . . .” 문자가 반복적으로 출력되어 보인다면 성공

우선 순위를 변경하기 위해 [vTaskPrioritySet()](../FreeRTOS_reference_Manual/vTaskPrioritySet().md)함수를 사용한다.
현재 suspend(blocked) 상태에 있는 TASK1을 깨우기 위해 [vTaskResume()](../FreeRTOS_reference_Manual/vTaskResume().md)함수를 사용한다.
```c
#if 1
    vTaskPrioritySet(xHandle1,TASK_3_PRIO); 
    /* xHandle1: 우선순위를 변경할 Task의 handle*/
    /* TASK_3_PRIO: 변경할 우선순위*/
    vTaskResume(xHandle1);
#endif // TODO #4
```

추가로, 두 task 모두 block 상태로 가지 않도록 `vTaskDelay(1000)` 함수를 주석 처리 해준다.
#### 실행결과

`#define TASK_2_PRIO		 9`
<br>`#define TASK_3_PRIO		 8`

- 여전히 TASK1의 우선순위가 TASK2보다 낮고 TASK2가 CPU를 점유하고 놔주지 않기 때문에 화면에는 'b'만 찍히게 된다.
![](images/Pasted_image_20260112223859.png)

- TASK1,2가 멀티태스킹을 하도록 `vTaskDelay(1000)` 함수의 주석을 해제 후 실행한다.
![](images/Pasted_image_20260112223914.png)
b->a->b->a를 반복해서 출력한다.

- 결과 분석: 
    TASK2의 우선순위(9)는 TASK1의 우선순위(8)보다 높다. FreeRTOS의 스케쥴러는 매 Tick 인터럽트가 발생할 때마다, 혹은 특정 이벤트가 발생할 때마다 현재 실행 가능한(Ready state) task 중 가장 우선순위가 높은 task를 실행한다. 
    따라서 TASK2가 계속 running 상태에서 존재하면 TASK1은 절대 실행되지 않는 기아 상태에 빠지게 된다.
    `vTaskDelay(1000)`을 실행함으로써 TASK2가 'b'를 화면에 출력하고 곧바로 1000ms 동안 block 상태가 된다. 이때, 스케쥴러는 실행가능한 task 중 가장 우선순위가 높은 TASK1을 실행하게 되고 결과적으로 'ba'가 반복해서 출력되는 것이다. 

#### 우선순위와 실행시간과의 관계

우선순위가 높다고 오래 실행되는것은 아니다. -> 우선순위는 단순히 누가 더 먼저 실행될지에 대한 문제이고, 실행시간은 얼마나 자주/길게 일할지에 대한 문제이기 때문에 휴면을 적게 할수록 더 많이 실행하게 된다.

*이런 관계 때문에 IDLE TASK가 가장 많이 CPU를 점유하게 된다.*

- `vTaskDelay(500)`: TASK1의 휴면시간을 500ms로 변경
![](images/Pasted_image_20260112224017.png)

- `vTaskDelay(500)`: TASK1의 휴면 시간을 100ms로 변경
![](images/Pasted_image_20260112224000.png)

- 결과분석:
    - 이전에는 동일하게 1000ms을 휴면 상태에 돌입했을 때는 우선순위에 밀려 'b'가 먼저 출력되다가 TASK1의 휴면시간을 줄임으로써 'a'가 먼저 출력되는 것을 알 수 있다. 
    - 휴면 시간 500ms라는건 1초에 2번 쉬는 것이고, 휴면시간 100ms라는 것은 1초에 10번 쉬는 것이기 때문에 첫번째 경우에는 'aab'가 한 세트로 나왔고 두번째 경우에서는 'aaaaaaaaaab'(a 10개)가 한 세트로 나오게 되고 이를 통해 휴면시간과 실행 시간과의 관계를 명확하게 알 수 있다.