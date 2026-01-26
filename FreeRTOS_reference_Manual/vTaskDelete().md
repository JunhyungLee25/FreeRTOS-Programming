53p - vTaskDelete() manual

#### 함수 라이브러리
`#include “FreeRTOS.h”`
`#include “task.h”`
#### 함수 원형
`void vTaskDelete( TaskHandle_t pxTask );`
#### summary
- xTaskCreate() 또는 xTaskCreateStatic()을 통해 이전에 생성된 태스크 인스턴스를 삭제한다.
- 삭제된 태스크는 더 이상 존재하지 않으므로 실행(Running) 상태로 진입할 수 없다.
- 삭제된 태스크를 참조하기 위해 태스크 핸들을 다시 사용하려고 시도해서는 안 된다.
#### Parameters
- pxTask: 삭제할 대상 태스크의 핸들이다.
- 태스크 핸들을 얻으려면 xTaskCreate() 호출 시 pxCreatedTask 매개변수를 사용하거나, xTaskCreateStatic()의 반환 값을 저장하거나, xTaskGetHandle() 함수를 사용해 태스크 이름을 조회하면 된다.
- 유효한 태스크 핸들 대신 NULL을 전달하면 호출한 자기 자신(현재 실행 중인 태스크)을 삭제한다.
#### Return Values
없음.
#### 주의사항
- 메모리 해제 책임: 태스크가 삭제될 때, 해당 태스크의 스택과 TCB(태스크 제어 블록)를 담고 있던 메모리를 해제하는 것은 **Idle Task(아이들 태스크)** 의 책임이다. 따라서 vTaskDelete()를 사용하는 애플리케이션은 아이들 태스크가 실행 시간을 박탈당하지 않도록(Starved) 보장해야 한다. 즉, 아이들 태스크가 실행 상태가 될 수 있는 충분한 시간이 할당되어야 커널 자원이 정상적으로 회수된다.

사용자 할당 자원: 커널이 태스크 생성 시 자동으로 할당한 메모리만 태스크 삭제 시 자동으로 해제된다. 애플리케이션(사용자)이 직접 할당한 메모리나 기타 리소스는 태스크가 삭제되기 전에 반드시 명시적으로 해제해야 한다.

#### Example
```c
void vAnotherFunction( void )
{
    TaskHandle_t xHandle;

    /* 태스크를 생성하고, 생성된 태스크의 핸들을 xHandle에 저장한다. */
    if( xTaskCreate( vTaskCode, 
                     "Demo task", 
                     STACK_SIZE, 
                     NULL, 
                     PRIORITY, 
                     &xHandle ) != pdPASS )
    {
        /* 태스크 데이터 구조와 스택을 할당할 FreeRTOS 힙 메모리가 부족하여 태스크를 생성할 수 없음. */
    }
    else
    {
        /* 방금 생성한 태스크를 삭제한다. xTaskCreate()에서 받은 핸들을 사용하여 대상 태스크를 참조한다. */
        vTaskDelete( xHandle );
    }

    /* NULL을 전달하여 이 함수를 호출한 자기 자신을 삭제한다. 
       자신의 핸들을 직접 전달해도 동일하게 삭제된다. */
    vTaskDelete( NULL );
}
```

