### 233p - vSemaphoreDelete() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “semphr.h”`

#### 함수 원형

- `void vSemaphoreDelete( SemaphoreHandle_t xSemaphore );`
    

#### summary

- `vSemaphoreCreateBinary()`, `xSemaphoreCreateCounting()`, `xSemaphoreCreateMutex()`, `xSemaphoreCreateRecursiveMutex()` 등을 통해 생성된 세마포어를 삭제한다.
    
- 삭제 시 세마포어에 할당되었던 메모리가 해제된다.
    

#### Parameters

- `xSemaphore`: 삭제할 대상 세마포어의 핸들이다.
    

#### Return Values

- 없음.
    

#### 주의사항

- **태스크 대기 상태 확인**: 만약 특정 세마포어를 획득하기 위해 **대기(Blocked) 중인 태스크가 있는 상태에서 해당 세마포어를 삭제하면 안 된다.** 이는 대기 중인 태스크가 깨어날 통로가 영구적으로 사라짐을 의미하며, 시스템 크래시나 데드락의 원인이 된다.
    
- **핸들 관리**: 삭제된 세마포어 핸들을 참조하려고 하면 오류가 발생하므로, 삭제 직후 핸들 변수에 `NULL`을 대입하는 습관을 갖는 것이 안전하다.
    

#### Example

```c
void vTaskFunction( void * pvParameters )
{
    SemaphoreHandle_t xSemaphore;

    /* 1. 이진 세마포어 생성 */
    xSemaphore = xSemaphoreCreateBinary();

    if( xSemaphore != NULL )
    {
        /* 2. 세마포어를 사용한 작업 수행 */
        // [작업 로직]

        /* 3. 더 이상 세마포어가 필요 없는 경우 삭제 */
        /* 주의: 이 세마포어를 기다리는 다른 태스크가 없는지 확인 완료된 상태여야 함 */
        vSemaphoreDelete( xSemaphore );
        
        /* 4. 실수 방지를 위한 핸들 초기화 */
        xSemaphore = NULL;
    }
}
```