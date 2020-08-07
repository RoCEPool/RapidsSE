/*============================================================================*/
/* DEV_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

i8 iDEV_MAP_DUMP      (void* aLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT

i8   iDev_ENT_COMPARE(void* aLU, void* aDevIXT);
i8   iDev_ENT_NEXT(void* aLU, void* aDevIXT);
void vDev_ENT_POSPRIOR(void* aLU, void* aDevIXT);
void* aDev_ENT_PUSH(void* aLU, void* aDevIXT);
i8   iDev_ENT_PUSHELMI0(void* aLU, void* aDevIXT);
Stz* szDev_ENT_PUSHELMS0(void* aLU, void* aDevIXT);
i8   iDev_ENT_PUSHOFS(void* aLU, void* aDevIXT);

void vDev_IXT_DEL_ENT(void* aLU, void* aDevIXT);
void vDev_IXT_DUMP(void* aLU, void* aDevIXT);
void vDev_IXT_INS_ENT(void* aLU, void* aDevIXT);
i8   iDev_IXT_TMP_ENTRIES(void* aLU, void* aDevIXT);
i8   iDev_IXT_TRV_BEGIN(void* aLU, void* aDevIXT);
i8   iDev_IXT_TRV_NEXT(void* aLU, void* aDevIXT);

i8   iDev_PAY_FETCH_COLI(void* aLU, void* aDevIXT, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* aLU, void* aDevIXT, i8 iArg);
void vDev_PAY_STORE_COLI(void* aLU, void* aDevIXT, i8 iArg, i8 iTop);
void vDev_PAY_STORE_COLS(void* aLU, void* aDevIXT, i8 iArg, Stz* szTop);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev RSB

void vDev_ENTWRK_APPENDELM(void* aLU, void* aDevRSB, i8 iArg);
void vDev_ENTWRK_APPENDELMI(void* aLU, void* aDevRSB, i8 iTop);
void vDev_ENTWRK_APPENDELMS(void* aLU, void* aDevRSB, Stz* szTop);
void vDev_ENTWRK_CREATEPAY(void* aLU, void* aDevRSB, i8 iArg);
void vDev_ENTWRK_POPOFS(void* aLU, void* aDevRSB, i8 iTop);
void vDev_ENTWRK_PRINT(void* aLU, void* aDevRSB);
void vDev_ENTWRK_RESET(void* aLU, void* aDevRSB);

void vDev_RSB_BEGIN(void* aLU, void* aDevRSB);
i8   iDev_RSB_NEXT(void* aLU, void* aDevRSB);
i8   iDev_RSB_PUSHCOLI(void* aLU, void* aDevRSB, i8 iArg);
Stz* szDev_RSB_PUSHCOLS(void* aLU, void* aDevRSB, i8 iArg);
i8   iDev_RSB_PUSHOFS(void* aLU, void* aDevRSB);
void vDev_RSB_PRINT(void* aLU, void* aDevRSB);
void vDev_RSB_SETBYOFS(void* aLU, void* aDevRSB, i8 iTop);

void vDev_RSB_COL_PUSH(void* aLU, void* aDevRSB, i8 iArg);
Stz* szDev_RSB_COL_PUSHSTR(void* aLU, void* aDevRSB, i8 iArg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
