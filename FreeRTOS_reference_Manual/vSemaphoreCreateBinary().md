#### 209p - vSemaphoreCreateBinary() manual

#### 함수 라이브러리

`#include “FreeRTOS.h”` `#include “semphr.h”`

#### 함수 원형

- `void vSemaphoreCreateBinary( SemaphoreHandle_t xSemaphore );`
    

#### summary

- 이진 세마포어를 생성하는 매크로이다.
    
- **주의**: 이전 버전과의 호환성을 위해 유지되는 기능이므로, 신규 프로젝트에서는 `xSemaphoreCreateBinary()` 함수를 사용해야 한다.
    

#### Parameters

- `xSemaphore`: 생성된 세마포어의 핸들을 저장할 `SemaphoreHandle_t` 타입의 변수이다.
    

#### Return Values

- 없음.
    
- (단, 호출 후 `xSemaphore` 변수가 `NULL`인지 확인해야 한다. `NULL`이라면 힙 메모리 부족으로 생성이 실패한 것이다.)
    

#### 주의사항

- **초기 상태의 차이**: 이 매크로로 생성된 세마포어는 **'가득 찬(Available)'** 상태로 시작한다. 즉, 생성 직후 첫 번째 `xSemaphoreTake()` 호출이 성공한다. (반면, 최신 함수인 `xSemaphoreCreateBinary()`는 '비어 있는' 상태로 생성되어 `Give`를 먼저 해야 `Take`가 가능하다.)
    
- **우선순위 상속 없음**: 이진 세마포어는 뮤텍스와 달리 우선순위 상속 메커니즘이 없다. 따라서 작업 간 동기화(태스크-태스크, 태스크-ISR)에 더 적합하다.
    
- **상호 배제 시 주의**: 뮤텍스 대용으로 쓸 경우, 세마포어를 얻은 태스크의 우선순위가 낮으면 높은 우선순위 태스크가 무한정 기다리는 '우선순위 역전' 현상이 발생할 수 있다.
    

#### Example

```c
SemaphoreHandle_t xSemaphore;

void vATask( void * pvParameters )
{
    /* 세마포어 생성을 시도한다. 
       참고: 신규 설계 시 vSemaphoreCreateBinary 대신 xSemaphoreCreateBinary를 사용한다. */
    vSemaphoreCreateBinary( xSemaphore );

    if( xSemaphore == NULL )
    {
        /* 메모리가 부족하여 세마포어 생성에 실패한 경우의 처리 */
    }
    else
    {
        /* 세마포어 생성 성공! 이제 xSemaphore 핸들을 사용하여 
           Take(획득)나 Give(해제)를 할 수 있다. 
           이 매크로로 생성했으므로 바로 Take가 가능하다. */
    }
}
```