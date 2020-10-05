/*============================================================================*/
/* DEV_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

void vDev_CRW_BEGIN(void* aLU, void* aIXT);
void vDev_CRW_COL_PUSHONE(void* aLU, i8 iArg, i8** aaiDstk);
void vDev_CRW_DEREF(void* aLU, void* aIXT);
i8   iDev_CRW_NEXT(void* aLU, void* aIXT);
void vDev_CRW_NULL(void* aLU, void* aIXT);
i8   iDev_CRW_TEST(void* aLU, void* aIXT);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT

i8   iDev_ENT_COMPARE(void* aLU, void* aIXT);
struct Ent_int* aDev_ENT_NEXT(void* aLU, void* aIXT);
struct Ent_int* aDev_ENT_POSPRIOR(void* aLU, void* aIXT,
                                  struct Ent_int** aaSkipRoots);
void vDev_ENT_PRINTKEYS(void* aLU, void* aIXT);
void* aDev_ENT_PUSH(void* aLU, void* aIXT);
void* aDev_ENT_PUSHUPPER(void* aLU, void* aIXT);
i8   iDev_ENT_PUSHELMI0(void* aLU, void* aIXT);
Stz* szDev_ENT_PUSHELMS0(void* aLU, void* aIXT); // NOT COMPLETE!!
i8   iDev_ENT_PUSHOFS(void* aLU, void* aIXT);

i8   iDev_IXT_CMP_SRCALT(void* aLU);
i8   iDev_IXT_CMP_WRKALT(void* aLU);
void vDev_IXT_DEL_ENT(void* aLU, void* aIXT);
void vDev_IXT_DUMP(void* aLU, void* aIXT);
void vDev_IXT_ENT_PRINT(void* aLU, void* aIXT);
void vDev_IXT_INDEXNM_SET(void* aLU, void* aIXT, Stz* szIndexNm);
void vDev_IXT_INDEXTREE_DELETE(void* aLU, void* aIXT);
void vDev_IXT_INDEXTREE_SET(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_INS_ENT(void* aLU, void* aIXT);
i8   iDev_IXT_TMP_ENTRIES(void* aLU, void* aIXT);

struct Ent_int* aDev_IXT_POS_ALTBYSRC(void* aLU);
struct Ent_int* aDev_IXT_POS_ALTBYWRK(void* aLU);
struct Ent_int* aDev_IXT_POS_LBOUND(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_POS_UBOUND(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_POS_UBOUND_LT(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_TRV_BEGIN(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_TRV_NEXT(void* aLU, void* aIXT);

i8*  aDev_PAY_ADDRESS(void* aLU, void* aIXT);
i8   iDev_PAY_FETCH_COLI(void* aLU, void* aIXT, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* aLU, void* aIXT, i8 iArg); 
void vDev_PAY_STORE_COLI(void* aLU, void* aIXT, i8 iArg, i8 iTop);
Stz* szDev_PAY_STORE_COLS(void* aLU, void* aIXT, i8 iArg);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev RSB

void vDev_ENTWRK_APPENDELM(void* aLU, void* aRSB, i8 iArg);
void vDev_ENTWRK_APPENDELMI(void* aLU, void* aRSB, i8 iTop);
void vDev_ENTWRK_APPENDELMS(void* aLU, void* aRSB, Stz* szTop);
void vDev_ENTWRK_CREATEPAY(void* aLU, void* aRSB, i8 iArg);
void vDev_ENTWRK_POPOFS(void* aLU, void* aRSB, i8 iTop);
void vDev_ENTWRK_PRINT(void* aLU, void* aRSB);
void vDev_ENTWRK_RESET(void* aLU, void* aRSB);
void vDev_ENTWRK_SETINTELM(void* aLU, void* aRSB, i8 iTop);

void vDev_RSB_BEGIN(void* aLU, void* aRSB);
i8   iDev_RSB_NEW(void* aLU, void* aRSB);
i8   iDev_RSB_NEXT(void* aLU, void* aRSB);
void vDev_RSB_NULL(void* aLU, void* aRSB);
void vDev_RSB_POPCOLI(void* aLU, void* aRSB, i8 iArg, i8 iTop);
void vDev_RSB_POPCOLS(void* aLU, void* aRSB, i8 iArg, Stz* szTop);
void vDev_RSB_PRINT(void* aLU, void* aRSB);
i8   iDev_RSB_PUSHCOLI(void* aLU, void* aRSB, i8 iArg);
Stz* szDev_RSB_PUSHCOLS(void* aLU, void* aRSB, i8 iArg);
i8   iDev_RSB_PUSHOFS(void* aLU, void* aRSB);
void vDev_RSB_RESET(void* aLU, void* aRSB);
void vDev_RSB_TABLENM_SET(void* aLU, void* aRSB, Stz* szTableNm);
i8   iDev_RSB_TEST(void* aLU, void* aRSB);
i8   iDev_RSB_TESTROW(void* aLU, void* aRSB);
void vDev_RSB_SETBYOFS(void* aLU, void* aRSB, i8 iTop);
void vDev_RSB_ZERO(void* aLU, void* aRSB);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void vDev_ALT_COL_PUSHSET(void* aLU, i8** aamDstk, i8 ieStrInf, i8* iaColSet);
void vDev_RSB_COL_PUSHSET(void* aLU, i8** aamDstk, i8 ieStrInf, i8* iaColSet);
void vDev_SRC_COL_PUSHSET(void* aLU, i8** aamDstk, i8 ieStrInf, i8* iaColSet);

void vDev_INDEXTREE_DEV_SET(void* aLU, i8 iDev);

void vDev_PIX_STM_PRINT(void* aLU);

void vDev_POT_USE_STREAMDEFOUT(void* aLU, i8 iAddPtnHash);

void vDev_REG_EOSMARKER_NEW(void* aLU);
void vDev_REG_INDEXDEF_ADDCOLUMN(void* aLU, Stz* sColumnNm, i8 iColumnType);
void vDev_REG_INDEXDEF_DELETE(void* aLU);
void vDev_REG_INDEXDEF_NEW(void* aLU, Stz* sName);
void vDev_REG_INDEXTREE_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_ADDCOLUMN(void* aLU,
        Stz* sColumnNm, i8 iColumnType, i8 iColumnLength, i8 iColumnScale);
void vDev_REG_STREAMDEF_STREAMDEFOUT(void* aLU);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
