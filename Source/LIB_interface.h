/*============================================================================*/
/* LIB_interface.h -- Interface for RLU Libraries                             */
/*============================================================================*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for lib

void vLib_ASSERT(void* pLU, void* aArg);
void vLib_BREAKPOINT(void* pLU);

i8   iLib_DTE_NOW(void* pLU);
Stz* szLib_DTE_PRINT(void* pLU, i8 iTopA);
i8   iLib_DTE_YEAR(void* pLU, i8 iTopA);

void vLib_ERROR(void* pLU, Stz* szErr);

void vLib_HSH_ADD_HASHI(void* pLU, i8 iTopA);

void vLib_PTN_CALC(void* pLU);
void vLib_PTN_RUNNING(void* pLU, i8 iTopA);

i8   iLib_RANDOM(void* pLU, i8 iArg);

void vLib_SLEEP(void* pLU, i8 iArg);

i8   iLib_STR_COMPARE(void* pLU, Stz* szTopA, Stz* szTopB);


void vRlu_TMR_AGG_ADD(void* pLU);
i8   iRlu_TMR_AGG_NSECS(void* pLU);
void vRlu_TMR_AGG_RESET(void* pLU);

i8   iRlu_TMR_NSECS(void* pLU);
void vRlu_TMR_START(void* pLU);
void vRlu_TMR_UPDATE(void* pLU);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
