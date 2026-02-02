## 1. co-Routine(코루틴)이란

일반적인 함수(Sub-routine)는 한 번 호출되면 끝날 때까지 쭉 실행되지만, 코루틴은 실행 중간에 멈췄다가 나중에 그 지점부터 다시 시작할 수 있는 특별한 능력이 있다.
- **협력적(Cooperative)**: 태스크가 스스로 실행 권한을 넘겨줘야 한다.(선점형과 대조적)
	
- **가벼움(Lightweight)**: 각 태스크마다 별도의 스택을 크게 할당할 필요가 없어서 메모리극도로 적은 MCU에서 매우 유리하다.

<br>

## 2. 사용 목적

RAM 제약이 심만 매우 작은 프로세서에서만 사용하기 위한 것으로 일반적으로 32비트 MCU에서는 사용되지 않는다.

- 메모리 절약: 일반적인 TASK는 각자 독립적인 스택 공간을 차지하지만, 코루틴은 모든 코루틴이 **하나의 스택**을 공유한다. RAM이 몇 KB 수준인 아주 작은 칩에서 여러 작업을 동시에 돌려야 할 때 사용한다.
    
- **단순한 구조:** 복잡한 스케줄러 알고리즘 없이도 순차적으로 작업을 처리할 수 있어 로직이 단순해진다.

<br>

## 3. 사용 방법

1. `FreeRTOSConfig.h` 설정
	`#define configUSE_CO_ROUTINES 1`로 설정
	
2. 코루틴 함수 구조
	**함수 원형**: `void vACoRoutine( CoRoutineHandle_t xHandle, UBaseType_t uxIndex )`
	
	**필수 매크로**: 모든 코루틴은 반드시 `crSTART(xHandle)`로 시작하고 `crEND(xHandle)`로 끝나야 한다.
	
3. **주의사항**
	1. **지역 변수의 휘발성**: 코루틴이 `Yield` 되어 멈췄다가 다시 시작될 때, 일반 지역 변수의 값은 유지되지 않는다. 따라서 상태를 유지해야하는 변수는 반드시 `static`으로 선언해야 한다.
		
	2. **전용 API 사용**: 코루틴 내에서는 `vTaskDelay()`를 사용할 수 없으며, 대신 `crDelay()`를 사용해야 한다.

<br>

## 4. 실제 모습

```c
crSTART(xHandle) -> switch( ( ( CRCB_t * )( xHandle ) )->uxState ) { case 0:

crEND() -> }
```

<br>

## 5.`vCoRoutineSchedule()`
`vCoRoutineSchedule()`를 실행시켜야 코루틴이 호출되어 코루틴이 동작한다.
-> Task는 스케줄러가 타이머 인터럽트를 이용해 강제로 CPU를 뺏어오지만, 코루틴은 그런 강제성이 없기 때문이다.(task로 불리지 않는 이유)

### `vCoRoutineSchedule()`이 하는 일
1. **우선순위 확인**: 실행 가능한 코루틴 중 우선순위가 가장 높은 녀석을 찾는다.
2. **상태 복구**: 해당 코루틴이 마지막으로 `Yield` 했던 지점(보통 `crDelay`으로 PC를 이동시킨다.
3. **실행**: 코루틴 코드를 실행하다가 다시 `Yield`를 만나면 제어권을 `vCoRoutineSchedule()`로 돌려준다.