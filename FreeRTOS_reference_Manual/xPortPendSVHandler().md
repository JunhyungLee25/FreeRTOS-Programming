03_CRITICAL1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c

```c
void xPortPendSVHandler( void )
{
	/* This is a naked function. */

	__asm volatile
	(
	"   mrs r0, psp                         \n" /* 1. 현재 태스크의 스택 포인터(PSP)를 R0로 읽어옴 */
	"   isb                                 \n" /* 2. 명령어 동기화 장벽: 이전 명령이 완전히 끝날 때까지 대기 */
	"                                       \n"
	"   ldr r3, pxCurrentTCBConst           \n" /* 3. pxCurrentTCB(현재 실행 중인 TCB 포인터)의 주소를 R3에 로드 */
	"   ldr r2, [r3]                        \n" /* 4. R3가 가리키는 곳에서 실제 TCB의 시작 주소를 R2에 로드 */
	"                                       \n"
	"   tst r14, #0x10                      \n" /* 5. R14(EXC_RETURN)의 4번 비트를 테스트하여 FPU(부동소수점 유닛) 사용 여부 확인 */
	"   it eq                               \n" /* 6. 만약 FPU를 사용 중이라면 (Equal) */
	"   vstmdbeq r0!, {s16-s31}             \n" /* 7. FPU 레지스터 S16~S31을 현재 태스크 스택에 저장 */
	"                                       \n"
	"   stmdb r0!, {r4-r11, r14}            \n" /* 8. 나머지 핵심 레지스터 R4~R11과 R14(복귀 주소 정보)를 스택에 저장 */
	"   str r0, [r2]                        \n" /* 9. 레지스터들이 저장된 최종 스택 위치(R0)를 TCB의 첫 번째 멤버(pxTopOfStack)에 기록 */
	"										\n"
	"   stmdb sp!, {r0, r3}                 \n" /* 10. 현재 R0, R3 값을 잠시 메인 스택(MSP)에 보관 (C 함수 호출 준비) */
	"   mov r0, %0                          \n" /* 11. configMAX_SYSCALL_INTERRUPT_PRIORITY 값을 R0에 로드 */
	"   cpsid i                             \n" /* 12. 우선순위 변경 중 사고 방지를 위해 인터럽트를 잠시 비활성화 */
	"   msr basepri, r0                     \n" /* 13. 커널이 관리하는 최대 인터럽트 우선순위 설정 (임계 영역 진입) */
	"   dsb                                 \n" /* 14. 데이터 동기화 장벽 */
	"   isb                                 \n" /* 15. 명령어 동기화 장벽 */
	"   cpsie i                             \n" /* 16. 인터럽트 다시 활성화 */
	"   bl vTaskSwitchContext               \n" /* 17. 드디어 C 함수 호출! 스케줄러가 pxCurrentTCB를 다음 태스크로 변경함 */
	"   mov r0, #0                          \n" /* 18. R0를 0으로 설정 */
	"   msr basepri, r0                     \n" /* 19. BASEPRI를 0으로 풀어 인터럽트 제한을 해제 */
	"   ldmia sp!, {r0, r3}                 \n" /* 20. 보관했던 R0, R3 값을 다시 복원 */
	"										\n"
	"   ldr r1, [r3]                        \n" /* 21. 새로 바뀐 pxCurrentTCB 주소를 R1에 로드 */
    "   ldr r0, [r1]                        \n" /* 22. 새 태스크의 TCB 첫 번째 멤버(저장된 스택 주소)를 R0로 읽어옴 */
    "                                       \n"
    "   ldmia r0!, {r4-r11, r14}            \n" /* 23. 새 태스크 스택에서 R4~R11, R14 값을 꺼내 실제 CPU 레지스터에 복원 */
    "                                       \n"
    "   tst r14, #0x10                      \n" /* 24. 이 새 태스크가 이전에 FPU를 사용했었는지 확인 */
    "   it eq                               \n" /* 25. 사용했었다면 */
    "   vldmiaeq r0!, {s16-s31}             \n" /* 26. 스택에 보관된 FPU 레지스터 S16~S31 값을 CPU로 복원 */
    "                                       \n"
    "   msr psp, r0                         \n" /* 27. 복원 후 남은 최종 스택 주소를 CPU의 PSP 레지스터에 다시 설정 */
    "   isb                                 \n" /* 28. 명령어 동기화 장벽 */
    "                                       \n"
	#ifdef WORKAROUND_PMU_CM001 /* XMC4000 specific errata workaround. */
		#if WORKAROUND_PMU_CM001 == 1
	"			push { r14 }				\n"
	"			pop { pc }					\n"
		#endif
	#endif
	"										\n"
	"   bx r14                              \n" /* 29. R14 값에 따라 예외 복귀. 새 태스크의 나머지 레지스터가 하드웨어에 의해 자동 복원됨*/
	"										\n"
	"	.align 4							\n"
	"pxCurrentTCBConst: .word pxCurrentTCB	\n"
	::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
	);
}
```

- 현재 태스크 문맥 저장 과정(1~9)
<br>목적: 지금 하던 일을 나중에 이어서 하기 위해 모든 상태를 저장함.
- 스케줄러 호출 및 새 태스크 선택(10~20)
<br>목적: 잠시 인터럽트를 통제하고, 다음에 누구를 실행할지 결정함
- 새로운 태스크 문맥 복원(21~29)
<br>목적: 새로 선택된 태스크의 과거 기록을 꺼내 CPU에 세팅함.