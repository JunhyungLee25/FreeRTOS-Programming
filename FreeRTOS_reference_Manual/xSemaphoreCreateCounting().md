#### 218p - xSemaphoreCreateCounting() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”`

`#include “semphr.h”`

#### 함수 원형

- `SemaphoreHandle_t xSemaphoreCreateCounting( UBaseType_t uxMaxCount, UBaseType_t uxInitialCount );`
    

#### summary

- 카운팅 세마포어를 생성하고, 이를 참조할 수 있는 핸들을 반환한다.
    
- 세마포어 상태를 유지하기 위해 FreeRTOS 힙(Heap)에서 적은 양의 RAM을 자동으로 할당한다.
    

#### Parameters

- `uxMaxCount`: 세마포어가 도달할 수 있는 최대 카운트 값이다. 이 값에 도달하면 더 이상 `Give`를 할 수 없다.
    
- `uxInitialCount`: 생성 시 세마포어에 할당되는 초기 카운트 값이다.
    

#### Return Values

- `NULL`: 힙 메모리가 부족하여 세마포어를 생성하지 못한 경우이다.
    
- **그 외의 값**: 생성된 세마포어의 핸들이다.
    

#### 주의사항

1. **이벤트 카운팅 용도**: 이벤트가 발생할 때마다 `Give`를 하고 처리할 때마다 `Take`를 한다. 이때 초기값은 `0`으로 설정하는 것이 일반적이다. (아직 발생한 이벤트가 없기 때문)
    
2. **자원 관리 용도**: 자원을 얻기 위해 `Take`를 하고 반납할 때 `Give`를 한다. 이때 초기값은 `uxMaxCount`와 동일하게 설정하여, 처음부터 모든 자원을 사용할 수 있게 한다.
    
3. **대안 기술**: 최신 FreeRTOS에서는 **'Direct to task notifications'** 이 카운팅 세마포어보다 더 가볍고 빠르므로 성능이 중요하다면 고려해 볼 만하다.
    
4. `FreeRTOSConfig.h`에서 `configSUPPORT_DYNAMIC_ALLOCATION`이 1로 설정되어 있어야 사용 가능하다.
    

#### Example

```c
void vATask( void * pvParameters )
{
    SemaphoreHandle_t xSemaphore;

    /* 최대 카운트 10, 초기 카운트 0인 카운팅 세마포어 생성.
       주로 이벤트 발생 횟수를 세는 용도로 사용됨.
    */
    xSemaphore = xSemaphoreCreateCounting( 10, 0 );

    if( xSemaphore != NULL )
    {
        /* 세마포어 생성 성공! 이제 핸들을 사용하여 Take/Give 가능 */
    }
    else
    {
        /* 메모리 할당 실패 처리 */
    }
}
```