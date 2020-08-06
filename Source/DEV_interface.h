/*============================================================================*/
/* DEV_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

i8 iDEV_MAP_DUMP      (void* pLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);


struct RluCommon* aRlu_INIT(void* pLU);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT

i8   iDev_ENT_COMPARE(void* pLU, void* pDevIXT);
i8   iDev_ENT_NEXT(void* pLU, void* pDevIXT);
void vDev_ENT_POSPRIOR(void* pLU, void* pDevIXT);
void* aDev_ENT_PUSH(void* pLU, void* pDevIXT);
i8   iDev_ENT_PUSHELMI0(void* pLU, void* pDevIXT);
Stz* szDev_ENT_PUSHELMS0(void* pLU, void* pDevIXT);
i8   iDev_ENT_PUSHOFS(void* pLU, void* pDevIXT);

void vDev_IXT_DEL_ENT(void* pLU, void* pDevIXT);
void vDev_IXT_DUMP(void* pLU, void* pDevIXT);
void vDev_IXT_INS_ENT(void* pLU, void* pDevIXT);
i8   iDev_IXT_TMP_ENTRIES(void* pLU, void* pDevIXT);
i8   iDev_IXT_TRV_BEGIN(void* pLU, void* pDevIXT);
i8   iDev_IXT_TRV_NEXT(void* pLU, void* pDevIXT);

i8   iDev_PAY_FETCH_COLI(void* pLU, void* pDevIXT, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* pLU, void* pDevIXT, i8 iArg);

void vDev_PAY_STORE_COLI(void* pLU, void* pDevIXT, i8 iArg, i8 iTop);
void vDev_PAY_STORE_COLS(void* pLU, void* pDevIXT, i8 iArg, Stz* szTop);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev RSB

void vDev_ENTWRK_APPENDELM(void* pLU, void* pDevRSB);
void vDev_ENTWRK_APPENDELMI(void* pLU, void* pDevRSB, i8 iTop);
void vDev_ENTWRK_APPENDELMS(void* pLU, void* pDevRSB, Stz* szTop);

void vDev_ENTWRK_POPOFS(void* pLU, void* pDevRSB, i8 iTop);
void vDev_ENTWRK_PRINT(void* pLU, void* pDevRSB);
void vDev_ENTWRK_RESET(void* pLU, void* pDevRSB);

void vDev_ENTWRK_CREATEPAY(void* pLU, void* pDevRSB, i8 iArg);


void vDev_RSB_PRINT(void* pLU, void* pDevRSB);
void vDev_RSB_BEGIN(void* pLU, void* pDevRSB);
i8   iDev_RSB_NEXT(void* pLU, void* pDevRSB);
i8   iDev_RSB_PUSHOFS(void* pLU, void* pDevRSB);
void vDev_RSB_SETBYOFS(void* pLU, void* pDevRSB, i8 iTop);
i8   iDev_RSB_PUSHCOLI(void* pLU, void* pDevRSB, i8 iArg);
Stz* szDev_RSB_PUSHCOLS(void* pLU, void* pDevRSB, i8 iArg);

void vDev_RSB_COL_PUSH(void* pLU, void* pDevRSB, i8 iArg);
Stz* szDev_RSB_COL_PUSHSTR(void* pLU, void* pDevRSB, i8 iArg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
