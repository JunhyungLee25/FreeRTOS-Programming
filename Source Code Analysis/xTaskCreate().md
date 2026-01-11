# 1. API Prototype & Header

#include “FreeRTOS.h”
#include “task.h”

```c
BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, 
                        const char * const pcName, 
                        unsigned short usStackDepth, 
                        void *pvParameters, 
                        UBaseType_t uxPriority, 
                        TaskHandle_t *pxCreatedTask );
```

# 2. Parameters

xTaskCreate: task 생성 API

`pvTaskCode` (task 함수):  
    - 실제 수행할 코드가 담긴 함수의 이름이다.
    - 앞서 배운 것처럼 이 함수 안에는 보통 무한 루프가 들어있어야 한다.
        
`"pcName"` (task 이름):
    - 사람이 식별하기 위한 문자열 이름이다.
    - 주로 디버깅할 때 어떤 태스크가 돌고 있는지 확인하는 용도로 쓰인다.
        
`usStackDepth` (stack 크기):
    - Word 단위 -> Byte 단위 아님을 주의.    
    - task가 자기만의 데이터를 저장하기 위해 사용할 메모리 공간의 크기이다.   
    - 이 값을 너무 작게 주면 'Stack Overflow'로 시스템이 뻗고, 너무 크게 주면 RAM이 부족해지므로 신중하게 결정해야 한다.
        
`pvParameters` (매개변수):
    - task 함수에 전달하고 싶은 데이터가 있을 때 사용한다. 
      
`uxPriority` (priority):
    - 스케줄러가 누구를 먼저 실행할지를 결정하는 기준이다. 숫자가 높을수록 우선순위가 높다(OS에 따라 다를 수 있음).
        
`pxCreatedTask` (task handle):
    - 생성된 태스크의 '아이디 카드' 같은 것이다.
	- 나중에 다른 곳에서 이 task를 일시 정지하거나 삭제하고 싶을 때 이 핸들을 사용한다.

# 3. Return Value
task는 절대 return하면 안되기 때문에 항상 'void'로 return형을 사용한다. **임베디드 시스템에서 태스크는 '프로그램 그 자체'가 아니라 '시스템이 관리하는 무한 루프 서비스'이다.** tasl가 return한다는 것은 해당 서비스를 관리하는 스택 정보와 제어권이 갈 곳을 잃어버린다는 뜻이며, 이는 곧 시스템 크래시로 이어진다.