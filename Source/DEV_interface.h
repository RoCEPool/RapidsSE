/*============================================================================*/
/* DEV_itf.h -- Interface for RLU Devices                                     */
/*============================================================================*/

i8 iDEV_MAP(void* aRLU, void* aDev, i8 eDO, void** aaCrs, void* aArg, i8 iArg);

i8 iDEV_MAP_DEL_ENT   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_DUMP      (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_ENTRIES   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_INS_ENT   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_BEGIN (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_NEXT  (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT

i8   iDev_ENT_COMPARE(void* pRLU, void* pDevIXT);
i8   iDev_ENT_NEXT(void* pRLU, void* pDevIXT);
void vDev_ENT_POSPRIOR(void* pRLU, void* pDevIXT);
void* aDev_ENT_PUSH(void* pRLU, void* pDevIXT);
i8   iDev_ENT_PUSHELMI0(void* pRLU, void* pDevIXT);
Stz* szDev_ENT_PUSHELMS0(void* pRLU, void* pDevIXT);
i8   iDev_ENT_PUSHOFS(void* pRLU, void* pDevIXT);

void vDev_IXT_DEL_ENT(void* pRLU, void* pDevIXT);
void vDev_IXT_DUMP(void* pRLU, void* pDevIXT);
void vDev_IXT_INS_ENT(void* pRLU, void* pDevIXT);
i8   iDev_IXT_TMP_ENTRIES(void* pRLU, void* pDevIXT);
i8   iDev_IXT_TRV_BEGIN(void* pRLU, void* pDevIXT);
i8   iDev_IXT_TRV_NEXT(void* pRLU, void* pDevIXT);

i8   iDev_PAY_FETCH_COLI(void* pRLU, void* pDevIXT, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* pRLU, void* pDevIXT, i8 iArg);

void vDev_PAY_STORE_COLI(void* pRLU, void* pDevIXT, i8 iArg, i8 iTop);
void vDev_PAY_STORE_COLS(void* pRLU, void* pDevIXT, i8 iArg, Stz* szTop);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev RSB

void vDev_COL_SRC_PUSH(void* pRLU, void* pDevRSB, i8 iArg);
Stz* szDev_COL_SRC_PUSHSTR(void* pRLU, void* pDevRSB, i8 iArg);

void vDev_ENTWRK_APPENDELM(void* pRLU, void* pDevRSB);
void vDev_ENTWRK_APPENDELMI(void* pRLU, void* pDevRSB, i8 iTop);
void vDev_ENTWRK_APPENDELMS(void* pRLU, void* pDevRSB, Stz* szTop);

void vDev_RSB_SRC_PRINT(void* pRLU, void* pDevRSB);
void vDev_RSB_SRC_BEGIN(void* pRLU, void* pDevRSB);
i8   iDev_RSB_SRC_NEXT(void* pRLU, void* pDevRSB);
i8   iDev_RSB_SRC_PUSHOFS(void* pRLU, void* pDevRSB);
void vDev_RSB_SRC_SETBYOFS(void* pRLU, void* pDevRSB, i8 iTop);
i8   iDev_RSB_SRC_PUSHCOLI(void* pRLU, void* pDevRSB, i8 iArg);
Stz* szDev_RSB_SRC_PUSHCOLS(void* pRLU, void* pDevRSB, i8 iArg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
