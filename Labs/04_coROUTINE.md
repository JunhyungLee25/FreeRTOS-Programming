[co-Routine](../Theory/co-Routine.md)이란?

# TODO #1

```c
void vApplicationIdleHook (void)
{
  /* TODO #1:
    코루틴 스케줄러 실행
    vCoRoutineSchedule */
#if 1
   vCoRoutineSchedule ();
#endif // TODO #1
}
```

- 실습 내용: IdleHook 함수 내에서 코루틴 스케줄러 실행해보기
- 실습 결과: LED 점멸하면 성공.

<br>

# TODO #2

```c
void USER_THREADS( void )
{

vParTestSetLED( 0, 0 ); // LD2 'OFF'

	printf("vFlashCoRoutine\n");
	//vStartLEDFlashTasks( mainLED_TASK_PRIORITY );
	xCoRoutineCreate (vFlashCoRoutine, PRIORITY_0, 0);

  /* TODO #2:
    두번째 코루틴 함수 생성 */
#if 1
	xCoRoutineCreate (vFlashCoRoutine, PRIORITY_0, 1); << 추가한 부분. uxIndex에 1을 집어넣어 `vFlashCoRoutine`내에서 printf문을 실행하도록 한다.
#endif // TODO #2

vParTestSetLED( 0, 1 ); // LD2 'ON'

	/* Should never reach here! */
	return;
}

...

void vFlashCoRoutine (CoRoutineHandle_t xHandle,
                 UBaseType_t uxIndex)
{
   // 공동 루틴은 crSTART () 호출로 시작해야합니다.
   crSTART (xHandle);

   for (;;)
   {
      // 고정 된 기간 동안 지연.
      crDELAY (xHandle, 100);
      if (uxIndex)
    	  printf("uxIndex is %d\n", (int)uxIndex);
      else
      // LED를 플래시합니다.
    	  vParTestToggleLED (0);
   }

   // 공동 루틴은 crEND ()에 대한 호출로 끝나야합니다.
   crEND ();
}

```

- 실습 내용: `USER_TREADS` 함수 내에 두번째 코루틴 함수 생성
- 실습 결과: LED가 점멸하면서 터미널에 1 출력.  
<br>![](../images/Pasted_image_20260202120153.png)
- 결과 분석: `xCoRoutineCreate`의 세번째 인자인 `uxIndex`에 다른 값을 주어 같은 코드 내에서 서로 다른 동작을 하도록 구현하는 법을 배웠다..