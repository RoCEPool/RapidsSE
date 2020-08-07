/*============================================================================*/
/* RLU_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for rlu

struct RluCommon* aRlu_INIT(void* pLU);

void vRlu_TMR_AGG_ADD(void* pLU);
i8   iRlu_TMR_AGG_NSECS(void* pLU);
void vRlu_TMR_AGG_RESET(void* pLU);

i8   iRlu_TMR_NSECS(void* pLU);
void vRlu_TMR_START(void* pLU);
void vRlu_TMR_UPDATE(void* pLU);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
