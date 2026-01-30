`INCLUDE_` 접두사가 붙은 매크로들은 최적화를 위해 꼭 필요하다. 이를 이용하여 시스템에 필요한 기능한 골라 담아 OS의 무게를 줄일 수 있다. 
```c
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet             1
#define INCLUDE_uxTaskPriorityGet            1
#define INCLUDE_vTaskDelete                  1
#define INCLUDE_vTaskCleanUpResources        0
#define INCLUDE_vTaskSuspend                 1
#define INCLUDE_vTaskDelayUntil              0
#define INCLUDE_vTaskDelay                   1
#define INCLUDE_xTaskGetSchedulerState       1
```
- 각 상수들이 1로 설정되면 해당 API 함수가 컴파일에 포함되고, 0이면 코드에서 완전히 제외된다.
- 0으로 설정된 함수를 소스 코드에서 호출하면 `Undefined reference` 컴파일 에러가 발생한다.
- 이 방식을 사용하는 이유:
1. 코드 크기의 최소화
	임베디드 시스템의 Flash 메모리는 PC처럼 테라바이트 단위가 아니다. 따라서 사용하지 않는 함수를 0으로 설정하면, 컴파일러가 그 코드를 아예 바이너리에 포함하지 않는다. 이를 **조건부 컴파일**이라고 한다.
		조건부 컴파일: `#if (INCLUDE_vTaskDelete == 1)`과 같이 전처리기를 사용하여 코드 포함 여부를 결정한다.
2. 실행속도 및 효율성
	사용하지 않는 기능을 위한 체크 로직이나 배부 변수들이 제거되면서, 커널이 조금 더 가볍고 빠르게 동작할 수 있게 된다.
3. 보안 및 안정성
	사용하지 않는 함수(API)가 코드에 남아 있으면, 실수로 호출하여 버그를 만들거나 해킹의 통로가 될 수 있으므로 아예 없애버리는 것이 가장 안전하다.

- `#define INCLUDE_vTaskDelay` 상수값 1->0 변경 후 컴파일 결과 확인인
```c
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet             1
#define INCLUDE_uxTaskPriorityGet            1
#define INCLUDE_vTaskDelete                  1
#define INCLUDE_vTaskCleanUpResources        0
#define INCLUDE_vTaskSuspend                 1
#define INCLUDE_vTaskDelayUntil              0
#define INCLUDE_vTaskDelay                   0
#define INCLUDE_xTaskGetSchedulerState       1
```

- error log
```d
Z:/inflearn_freeRTOS/02_STACKOVERFLOW1/Debug/../Core/Src/task.c:99: undefined reference to `vTaskDelay'
Z:/inflearn_freeRTOS/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.12.3.rel1.win32_1.0.200.202406191623/tools/bin/../lib/gcc/arm-none-eabi/12.3.1/../../../../arm-none-eabi/bin/ld.exe: ./Core/Src/task.o: in function `Task2':
Z:/inflearn_freeRTOS/02_STACKOVERFLOW1/Debug/../Core/Src/task.c:128: undefined reference to `vTaskDelay'
collect2.exe: error: ld returned 1 exit status
make: *** [makefile:68: 02_STACKOVERFLOW1.elf] Error 1
"make -j8 all" terminated with exit code 2. Build might be incomplete.

16:02:43 Build Failed. 3 errors, 0 warnings. (took 11s.173ms)

```
error log 요약: 컴파일러는 헤더 파일(.h)의 선언을 보고 함수 호출을 승인했으나, 링크(LIink) 단계에서 전처리기(#if) 지시자에 의해 실제 구현부(.c)가 바이너리 생성에서 제외되어 해당 함수의 메모리 주소를 찾기 못해 발생한 에러이다.
