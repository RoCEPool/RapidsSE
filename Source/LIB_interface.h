/*============================================================================*/
/* LIB_interface.h -- Interface for RLU Libraries                             */
/*============================================================================*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for lib


i8   iLib_DTE_NOW(void* pLU);
Stz* szLib_DTE_PRINT(void* pLU, i8 iTopA);
i8   iLib_DTE_YEAR(void* pLU, i8 iTopA);

i8   iLib_HSH_ADD_HASHI(void* pLU, i8 iTopA);

i8   iLib_PTN_CALC(void* pLU);
void vLib_PTN_RUNNING(void* pLU, i8 iTopA);

i8   iLib_RANDOM(void* pLU, i8 iArg);

void vLib_SLEEP(void* pLU, i8 iArg);

i8   iLib_STR_COMPARE(void* pLU, Stz* szTopA, Stz* szTopB);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
