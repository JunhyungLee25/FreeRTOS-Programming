87p - vTaskGetTaskInfo() manual

#### 함수 라이브러리
`#include “FreeRTOS.h”`
`#include “task.h”`
#### 함수 원형
`void vTaskGetTaskInfo( TaskHandle_t xTask, TaskStatus_t *pxTaskStatus, BaseType_t xGetFreeStackSpace, eTaskState eState );`
#### summary
- 단일 태스크에 대한 상세 정보를 TaskStatus_t 구조체에 담는다.
- 이 구조체에는 태스크의 핸들, 이름, 우선순위, 상태, 그리고 소비된 총 실행 시간 등이 포함된다.
#### Parameters
- xTask: 정보를 조회할 대상 태스크의 핸들이다.
- 태스크 핸들은 xTaskCreate() 시 pxCreatedTask 매개변수로 얻거나, xTaskGetHandle()을 사용해 이름으로 조회할 수 있다.
- pxTaskStatus: 조회된 정보가 채워질 TaskStatus_t 타입 변수의 포인터이다.
- xGetFreeStackSpace: 스택 고수위치(High Water Mark) 계산 여부를 결정한다.
- High Water Mark: 태스크가 생성된 이후 현재까지 스택이 가장 많이 차올랐을 때의 '남은 공간'이다. 0에 가까울수록 스택 오버플로우 위험이 크다는 뜻이다.
- 이 계산은 시간이 비교적 오래 걸려 시스템 응답성을 일시적으로 떨어뜨릴 수 있으므로, 필요 없을 경우 pdFALSE를 전달하여 생략할 수 있다.
- eState: 태스크의 현재 상태 정보를 포함할지 결정한다.
- 상태 정보를 조회하는 것은 단순히 값을 대입하는 것보다 느리므로 선택권을 제공한다.
- 상태 정보를 얻고 싶다면 eInvalid를 전달하고, 그렇지 않으면 임의의 값을 전달한다.
#### Return Values
- 없음.
#### 주의사항
- 디버깅 전용: 이 함수는 실행 중에 스케줄러를 장시간 정지시킬 가능성이 있으므로, 실제 제품용 코드보다는 디버깅 용도로만 사용하는 것이 권장된다.
- 전체 태스크 조회: 시스템 내 모든 태스크의 정보를 한꺼번에 얻고 싶다면 vTaskGetTaskInfo() 대신 uxTaskGetSystemState()를 사용해야 한다.
- 설정 필요: uxTaskGetSystemState()를 사용하려면 FreeRTOSConfig.h 파일에 configUSE_TRACE_FACILITY가 1로 정의되어 있어야 한다.
#### Example
```c
void vAFunction( void )
{
    TaskHandle_t xHandle;
    TaskStatus_t xTaskDetails;

    /* 태스크 이름을 사용하여 핸들을 획득한다. */
    xHandle = xTaskGetHandle( "Task_Name" );

    /* 핸들이 유효한지(NULL이 아닌지) 확인한다. */
    configASSERT( xHandle );

    /* 핸들을 사용하여 태스크의 상세 정보를 조회한다. */
    vTaskGetTaskInfo( 
        /* 조회할 대상 태스크 핸들 */
        xHandle,
        /* 정보를 담을 TaskStatus_t 구조체 주소 */
        &xTaskDetails,
        /* 스택 High Water Mark 정보를 포함한다 (pdTRUE) */
        pdTRUE,
        /* 실제 태스크 상태를 조회하여 포함한다 (eInvalid) */
        eInvalid 
    );
    
    /* 이제 xTaskDetails.usStackHighWaterMark 등을 통해 
       스택이 얼마나 남았는지 확인할 수 있다. */
}
```