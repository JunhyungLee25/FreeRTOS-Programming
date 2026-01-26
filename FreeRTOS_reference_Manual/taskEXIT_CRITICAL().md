63p - taskEXIT_CRITICAL() manual

#### 함수 라이브러리
`#include “FreeRTOS.h”`
`#include “task.h”`
#### 함수 원형
- `void taskEXIT_CRITICAL( void );`
#### summary
- `taskENTER_CRITICAL()`을 통해 진입했던 임계 영역(Critical Section)을 빠져나온다.
- 호출 시 중첩 카운트(Nesting Count)를 감소시키며, 카운트가 0이 되면 인터럽트를 다시 활성화한다.
#### Parameters
- 없음.
#### Return Values
- 없음.
#### 주의사항
- **ISR 사용 금지**: 이 매크로는 인터럽트 서비스 루틴(ISR) 내에서 호출해서는 안 된다. ISR 환경에서는 `taskEXIT_CRITICAL_FROM_ISR()`를 사용해야 한다.
    
- **인터럽트 재활성화**: 호출 즉시 인터럽트가 켜지는 것이 아니라, 이전의 모든 `taskENTER_CRITICAL()` 호출에 대응하는 `taskEXIT_CRITICAL()`이 모두 실행되어 **중첩 깊이가 0이 될 때만** 인터럽트가 다시 활성화된다.
    
- **스케줄링 재개**: 인터럽트가 다시 활성화되면, 임계 영역 동안 지연되었던 컨텍스트 스위칭(태스크 전환)이 즉시 발생할 수 있다.
    
- **쌍을 이루는 호출**: 모든 `taskENTER_CRITICAL()`은 반드시 대응하는 `taskEXIT_CRITICAL()`과 쌍을 이루어야 한다. 하나라도 누락되면 시스템의 인터럽트가 영원히 차단될 수 있다.
    
- **임계 영역 내 API 제한**: 임계 영역 내부(진입 후 탈출 전까지)에서는 FreeRTOS API 함수를 호출해서는 안 된다.
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