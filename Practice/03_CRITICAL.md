FreeRTOS의 커널 서비스인 [taskENTER_CRITICAL()](../FreeRTOS_reference_Manual/taskENTER_CRITICAL().md), [taskEXIT_CRITICAL()](../FreeRTOS_reference_Manual/taskEXIT_CRITICAL().md) 함수를 사용하여 임계영역의 공유자원을 보호하는 방법을 구현해보고 의미를 알아보자.
이 실습은 가상의 비행기 티켓 무인 발급 시스템을 가정하고 만들어진 실습이다.
비행기 티켓수(tickets) 자료를 2개의 태스크가 공유하여 작동시 발생되는 문제점과 그 해결 방법을 고민해본다.
# TODO #1
> 이후의 TODO#2를 위한 사전 준비이다.

- 수정 전 코드
```c
#define TICKET_1MILLION 1000000
#define TICKET_10000 10000
#define TICKET_1000 1000

	/* TODO #1:
		초기 티켓수를 작은수(1,000 ~ 10,000정도) 입력 하여 테스트 한후
	    그 결과를 설명한다 */
#if 1 // No comment
	tickets= backupTickets = 1 * TICKET_10000; // MAX value( 10 million )
#endif // TODO #1
```
실습 내용: tickets 변수에 초기값으로 10,000~100,000 정도를 입력한 후 어떤 결과가 출력되는지 확인한다. "Good!. Matched" 메시지가 보이면 성공

- 실행 결과
<br>![](../images/Pasted_image_20260126195610.png)
<br>정상적으로 출력됨을 확인

- tickets의 초기 값을 매우 큰 수로 변경 
```c
tickets= backupTickets = 10 * TICKET_1MILLION;
```

- 실행결과 예측: 
	tickets는 공유 자원인데 비원자적 연산이 보호 없이 수행되고 있다.
	`tickets++`-> '++'는 c에서는 단 한줄이지만 어셈블리어로 보면 총 3단계의 연산 과정을 거치게 된다. 따라서, tickets의 값이 증가할수록 collison이 일어날 확률이 증가하여 "Not good..."이 출력될 것이다.

- 실행 결과
<br>![](../images/Pasted_image_20260126200447.png)
<br> 출력된 총 ticket 수가 최초 설정한 값과 다름을 통해 collison이 발생했음을 알 수 있다.

[임계 영역(Critical Section)](../Theory/CRITICAL_SECTION.md) - 동시성 문제

# TODO #2
