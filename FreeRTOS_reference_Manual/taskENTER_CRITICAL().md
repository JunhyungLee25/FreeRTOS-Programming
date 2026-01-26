58p - taskENTER_CRITICAL() manual

#### 함수 라이브러리
`#include “FreeRTOS.h”`
`#include “task.h”`
#### 함수 원형
- `void taskENTER_CRITICAL( void );`
  - `void taskEXIT_CRITICAL( void );`
#### summary
- 임계 영역(Critical Section)에 진입하고 빠져나온다.
- `taskENTER_CRITICAL()`을 호출하여 진입하고, `taskEXIT_CRITICAL()`을 호출하여 빠져나온다.
- 인터럽트를 비활성화하여 현재 태스크가 선점(Preemption)되지 않도록 보장한다.
#### Parameters
- 없음.
#### Return Values
- 없음.
#### 주의사항
- **ISR 사용 금지**: 이 매크로는 인터럽트 서비스 루틴(ISR)에서 호출해서는 안 된다. ISR 전용인 `taskENTER_CRITICAL_FROM_ISR()`를 사용해야 한다.
- **인터럽트 제어**: `configMAX_SYSCALL_INTERRUPT_PRIORITY` 설정에 따라 해당 우선순위 이하의 인터럽트만 비활성화하거나, 설정이 없으면 모든 인터럽트를 비활성화한다.
- **중첩(Nesting) 지원**: 호출 횟수를 카운트하므로, `taskENTER_CRITICAL()`을 두 번 호출했다면 `taskEXIT_CRITICAL()`도 반드시 두 번 호출해야 인터럽트가 다시 활성화된다.
- **실행 시간 최소화**: 임계 영역 내의 코드는 매우 짧게 유지해야 한다. 너무 길어지면 인터럽트 응답 속도가 떨어져 시스템 전체 성능에 악영향을 준다.
- **API 호출 제한**: 임계 영역 내부에서는 FreeRTOS API 함수를 호출해서는 안 된다.
- **태스크 상태 유지**: 인터럽트가 비활성화된 동안에는 스케줄링이 일어나지 않으므로, 명시적으로 블록(Block)되거나 양보(Yield)하지 않는 한 호출한 태스크의 Running 상태가 보장된다.
#### Example

```c
/* 임계 영역을 사용하는 예제 함수 */
void vDemoFunction( void )
{
    /* 임계 영역 진입. 
       이 함수가 이미 임계 영역인 곳에서 호출된다면, 중첩 깊이(Nesting Depth)는 2가 된다. */
    taskENTER_CRITICAL();

    /* 보호가 필요한 공유 자원 접근 등의 로직을 여기서 수행한다. */

    /* 임계 영역 탈출. 
       중첩된 상태에서 호출되면 카운트만 감소하고 인터럽트는 아직 활성화되지 않는다. */
    taskEXIT_CRITICAL();
}

/* vDemoFunction을 임계 영역 내부에서 호출하는 태스크 */
void vTask1( void * pvParameters )
{
    for( ;; )
    {
        /* 일반적인 기능 수행 */

        /* 임계 영역 생성을 위해 taskENTER_CRITICAL() 호출 */       
        taskENTER_CRITICAL();

        /* 임계 영역이 필요한 코드 실행 */

        /* taskENTER_CRITICAL()은 중첩을 지원하므로, 
           내부에서 자체적인 임계 영역을 가진 함수를 호출해도 안전하다. */
        vDemoFunction();

        /* 보호 작업이 완료되었으므로 임계 영역을 빠져나간다. 
           이 호출 후 중첩 깊이가 0이 되면 인터럽트가 다시 활성화된다. */
        taskEXIT_CRITICAL();
    }
}
```