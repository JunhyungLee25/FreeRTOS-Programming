#### 관련 라이브러리

`#include "FreeRTOS.h"`
`#include "task.h"`
#### 정의 및 타입

- `BaseType_t xHigherPriorityTaskWoken;` (또는 `portBASE_TYPE`)
    
- 초기값은 반드시 `pdFALSE` (0)로 설정해야 한다.
    
#### summary

- 인터럽트 발생 시, 깨어난 태스크(Unblocked Task)가 현재 실행 중인 태스크보다 우선순위가 높은지를 나타내는 논리 변수이다.
    
- ISR 내에서 사용되는 `...FromISR()` 계열의 API(세마포어 Give, 큐 Send, 태스크 알림 등)에 매개변수로 전달되어 값이 업데이트된다.
    
#### 주요 동작 메커니즘

1. **초기화**: ISR 시작 부분에서 `pdFALSE`로 초기화한다.
    
2. **API 호출**: `vTaskNotifyGiveFromISR(handle, &xHigherPriorityTaskWoken)`과 같이 주소값(`&`)을 전달한다.
    
3. **커널의 판단**: 만약 신호를 받은 태스크가 현재 CPU를 점유한 태스크보다 우선순위가 높다면, 커널은 이 변수 값을 내부적으로 `pdTRUE`로 변경한다.
    
4. **문맥 전환 실행**: ISR 끝단에서 `portYIELD_FROM_ISR(xHigherPriorityTaskWoken)`을 호출하여, 값이 `pdTRUE`일 경우 즉시 스케줄링을 수행한다.
    
#### Return Values

- 변수 자체는 반환값이 없으나, API 호출 결과에 따라 `pdTRUE`(1) 또는 `pdFALSE`(0) 값을 갖게 된다.
    
#### 주의사항

- **반드시 static 또는 지역 변수로 선언**: ISR 내에서만 유효하며, 여러 API가 이 변수 하나를 공유하여 최종적인 Yield 여부를 결정할 수 있다.
    
- **포인터 전달**: API에 전달할 때는 반드시 주소 연산자(`&`)를 붙여서 참조 전달(Pass by reference)을 해야 커널이 값을 수정할 수 있다.
    
#### Example (실습 코드 기반)

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 1. 변수 선언 및 초기화 (static 권장)
    static BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    // 2. 알림을 주면서 고우선순위 태스크가 깨어났는지 확인
    // 만약 깨어난 태스크가 더 중요하다면 xHigherPriorityTaskWoken은 pdTRUE가 됨
    vTaskNotifyGiveFromISR( xHandle1, &xHigherPriorityTaskWoken );

    // 3. 변수 값이 pdTRUE라면 즉시 문맥 전환 수행
    // 만약 pdFALSE라면 원래 실행되던 태스크로 돌아감
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
```