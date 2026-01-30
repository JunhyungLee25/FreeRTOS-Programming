#### FreeRTOS 변수 이름 규칙
- 변수 prefix
	<br>c : char형 (문자 한 글자)
	<br>s : int16_t (short, 2바이트 정수)
	<br>l : int32_t (long, 4바이트 정수)
	<br>x : BaseType_t 또는 구조체, 태스크 핸들, 큐 핸들 같은 FreeRTOS 전용 타입.
	<br>u : unsigned (부호 없는 양수). 다른 타입 앞에 붙는다. (예: uc = unsigned char)
	<br>p : pointer (주소값). 다른 타입 앞에 붙는다. (예: px = 구조체나 핸들의 주소값)
- 예시:  `pucSomething` -> `p`(포인터) + `u`(unsigned) + `c`(char) 즉, **부호 없는 문자형 변수의 주소를 담고 있는 포인터 변수**라는 뜻임을 알 수 있다.

- 함수 prefix
	<br>v : void (돌려주는 값이 없음). 예: vTaskPrioritySet()
	<br>x : BaseType_t (성공/실패 여부나 정수값을 돌려줌). 예: xQueueReceive()
	<br>pv : pointer to void (어떤 데이터의 주소값을 돌려줌). 예: pvTimerGetTimerID()
	<br>prv : private (파일 내부 전용 함수). 외부에서는 부를 수 없다는 의미를 갖는다.

- 매크로 prefix
> 이름의 앞부분을 보면 그 상수가 어디에 위치해 있는지 알 수 있다.
- port (예: portMAX_DELAY)
	정의 위치: `portable.h` 또는 `portmacro.h`
	의미: 하드웨어(CPU 아키텍처)에 따라 달라지는 설정들이다. 
	
- task (예: taskENTER_CRITICAL())
	정의 위치: `task.h`
	의미: 태스크 관리와 관련된 매크로 함수들이다.
	
- pd (예: pdTRUE, pdPASS)
	정의 위치: `projdefs.h`
	의미: FreeRTOS 프로젝트 전반에서 공통으로 쓰이는 정의(Project Definitions)이다.
	
- config (예: configUSE_PREEMPTION)
	정의 위치: `FreeRTOSConfig.h`
	의미: 사용자가 OS의 기능을 켤지 끌지 결정하는 '설정값'들이다. 우리 시스템의 성격에 맞게 튜닝할 때 사용한다.
	
- err (예: errQUEUE_FULL)
	정의 위치: `projdefs.h`
	의미: 에러 상태를 나타내는 코드들이다.
	
- True/False
>가독성을 위해 1과 0 대신 전용 상수를 사용한다.
- `pdTRUE` 또는 `pdPASS`: 1
- `pdFALSE` 또는 `pdFAIL`: 0