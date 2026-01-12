### 48p - vTaskDelay() manual

#### 함수 라이브러리
`#include “FreeRTOS.h”` 
`#include “task.h”`
#### 함수 원형
- `void vTaskDelay( TickType_t xTicksToDelay );`
#### summary
- `vTaskDelay()`를 호출한 태스크를 정해진 틱 인터럽트 횟수 동안 **Blocked** 상태로 전환한다.
- 지연 시간을 0으로 지정(`vTaskDelay(0)`)하면 호출한 태스크가 Blocked 상태로 전환되지는 않지만, 동일한 우선순위를 가진 다른 Ready 상태의 태스크에게 CPU를 양보(Yield)하게 된다. 이는 `taskYIELD()`를 호출하는 것과 동일하다.
#### Parameters
- `xTicksToDelay`: 호출한 태스크가 Ready 상태로 복귀하기 전까지 Blocked 상태를 유지할 틱 인터럽트 횟수이다.
    - 예를 들어, 현재 틱 카운트가 10,000일 때 태스크가 `vTaskDelay(100)`을 호출하면, 즉시 Blocked 상태가 되어 틱 카운트가 10,100이 될 때까지 해당 상태를 유지한다.
    - `vTaskDelay()`가 호출된 시점부터 다음 틱 인터럽트가 발생하기까지 남은 시간은 하나의 완전한 틱 주기로 간주된다. 따라서 지연 시간을 지정할 때 달성할 수 있는 시간 해상도(Resolution)는 최악의 경우 하나의 완전한 틱 인터럽트 주기와 같다.
    - 밀리초(ms) 단위를 틱(Tick) 단위로 변환하려면 `pdMS_TO_TICKS()` 매크로를 사용할 수 있다.
#### Return Values
- 없음.
#### 주의사항
- `vTaskDelay()` API 함수를 사용하려면 `FreeRTOSConfig.h` 파일에서 `INCLUDE_vTaskDelay` 설정값이 반드시 **1**로 지정되어 있어야 한다.
- 이 함수는 상대적인 지연(Relative delay)을 제공한다. 호출된 시점부터 지정된 시간만큼 대기하므로, 주기적인 작업을 수행할 때는 루프 내의 다른 코드 실행 시간에 영향을 받을 수 있다. (정확한 주기가 필요하다면 `vTaskDelayUntil()` 사용 권장)
#### Example

```c
void vAnotherTask( void * pvParameters )
{
    for( ;; )
    {
        /* 여기서 특정 프로세싱 작업을 수행한다. */
        // ...

        /* 20틱 인터럽트 동안 Blocked 상태로 진입한다. 
           Blocked 상태에 머무는 실제 시간은 틱 주파수(Tick Frequency)에 따라 달라진다. */
        vTaskDelay(20);

        /* 첫 번째 vTaskDelay() 호출이 실행된 시점부터 20틱이 경과한다. */

        /* 20밀리초(ms) 동안 Blocked 상태로 진입한다. 
           pdMS_TO_TICKS() 매크로를 사용하면 틱 주파수가 변경되더라도 
           Blocked 상태에서 보내는 실제 시간(20ms)은 변하지 않도록 관리할 수 있다. */
        vTaskDelay( pdMS_TO_TICKS(20));
    }
}
```