/*============================================================================*/
/* DEV_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

void vDev_CRW_BEGIN(void* aLU, void* aIXT);
void vDev_CRW_COL_PUSHONE(void* aLU, i8 iArg, i8** aaiDstk);
void vDev_CRW_DEREF(void* aLU, void* aIXT);
i8   iDev_CRW_NEXT(void* aLU, void* aIXT);
void vDev_CRW_NULL(void* aLU, void* aIXT);
i8   iDev_CRW_TEST(void* aLU, void* aIXT);

void vDev_IXT_BEGIN(i8** aaiDstk);
void vDev_IXT_DEREF(i8** aaiDstk);
void vDev_IXT_NEXT(i8** aaiDstk);
void vDev_IXT_NULL(i8** aaiDstk);
void vDev_IXT_TEST(i8** aaiDstk);

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
void vDev_IXT_DEL_ENT(i8** aaiDstk);
void vDev_IXT_DUMP(i8** aaiDstk);
void vDev_IXT_ENT_PRINT(void* aLU, void* aIXT);

void vDev_IXT_INDEXNM_SET(i8** aaiDstk);
void vDev_IXT_INDEXTREE_DELETE(i8** aaiDstk);
void vDev_IXT_INDEXTREE_SET(i8** aaiDstk);

struct Ent_int* aDev_IXT_INS_ENT(void* aLU, void* aIXT);
void vDev_IXT_SET_ENT(void* aLU, void* aIXT, void* aEnt);
void vDev_IXT_TMP_ENTRIES(i8** aaiDstk);

struct Ent_int* aDev_IXT_POS_ALTBYSRC(void* aLU);
struct Ent_int* aDev_IXT_POS_ALTBYWRK(void* aLU);
struct Ent_int* aDev_IXT_POS_LBOUND(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_POS_UBOUND(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_POS_UBOUND_LT(void* aLU, void* aIXT);
struct Ent_int* aDev_IXT_TRV_BEGIN_OLD(void* aLU, void* aIXT);
void vDev_IXT_TRV_BEGIN(i8** aaiDstk);
void vDev_IXT_TRV_NEXT(i8** aaiDstk);
struct Ent_int* aDev_IXT_TRV_NEXT_OLD(void* aLU, void* aIXT);

i8*  aDev_PAY_ADDRESS(void* aLU, void* aIXT);
i8   iDev_PAY_FETCH_COLI(void* aLU, void* aIXT, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* aLU, void* aIXT, i8 iArg); 
void vDev_PAY_STORE_COLI(void* aLU, void* aIXT, i8 iArg, i8 iTop);
Stz* szDev_PAY_STORE_COLS(void* aLU, void* aIXT, i8 iArg);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev ROW

void vDev_ENTWRK_APPENDELM(void* aLU, void* aROWDEVICE, i8 iArg);
void vDev_ENTWRK_APPENDELMI(void* aLU, void* aROWDEVICE, i8 iTop);
void vDev_ENTWRK_APPENDELMS(void* aLU, void* aROWDEVICE, Stz* szTop);
void vDev_ENTWRK_CREATEPAY(void* aLU, void* aROWDEVICE, i8 iArg);
void vDev_ENTWRK_POPOFS(void* aLU, void* aROWDEVICE, i8 iTop);
void vDev_ENTWRK_PRINT(void* aLU, void* aROWDEVICE);
void vDev_ENTWRK_RESET(void* aLU, void* aROWDEVICE);
void vDev_ENTWRK_SETINTELM(void* aLU, void* aROWDEVICE, i8 iTop);

void vDev_ROW_BEGIN(i8** aaiDstk);
void vDev_ROW_NEW(i8** aaiDstk);
void vDev_ROW_NEXT(i8** aaiDstk);
void vDev_ROW_NULL(i8** aaiDstk);
void vDev_ROW_POPCOLI(i8** aaiDstk);
void vDev_ROW_POPCOLS(i8** aaiDstk);
void vDev_ROW_PRINT(i8** aaiDstk);
void vDev_ROW_PUSHCOLI(i8** aaiDstk);
Stz* szDev_ROW_PUSHCOLS(void* aLU, void* aROWDEVICE, i8 iArg);
void vDev_ROW_PUSHOFS(i8** aaiDstk);
void vDev_ROW_RESET(i8** aaiDstk);
void vDev_ROW_SET(i8** aaiDstk);
void vDev_ROW_TABLENM_SET(i8** aaiDstk);
void vDev_ROW_TESTBUFFER(i8** aaiDstk);
void vDev_ROW_TESTROW(i8** aaiDstk);
void vDev_ROW_SETBYOFS(i8** aaiDstk);
void vDev_ROW_ZERO(i8** aaiDstk);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void vDev_TABLEPTN_DROPREAD(void* aLU);
void vDev_TABLEPTN_GRABREAD(void* aLU);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void vDev_ALT_COL_PUSHSET(void* aLU, i8** aamDstk, i8 ieStrInf, i8* iaColSet);
void vDev_ROW_COL_PUSHSET(void* aLU, i8** aamDstk, void* aROWDEVICE, i8* iaColSet);
void vDev_SRC_COL_PUSHDST(void* aLU, void* aROWDEVICE);
void vDev_SRC_COL_PUSHSET(void* aLU, i8** aamDstk, i8 ieStrInf, i8* iaColSet);

void vDev_IXT_COL_PUSHONE_PST(void* aLU, i8** aaiDstk);

void vDev_INDEXTREE_DEV_SET(void* aLU, i8 iDev);

void vDev_PIX_STM_PRINT(void* aLU);

void vDev_POT_USE_STREAMDEFOUT(void* aLU, i8 iAddPtnHash);

void vDev_REG_EOSMARKER_NEW(void* aLU);
void vDev_REG_INDEXDEF_ADDCOLUMN(void* aLU, Stz* sColumnNm, i8 iColumnType);
void vDev_REG_INDEXDEF_DELETE(void* aLU);
void vDev_REG_INDEXDEF_NEW(void* aLU, Stz* sName);
void vDev_REG_INDEXTREE_NEW(void* aLU);

void vREG_INDEXTREE_NULL(i8** aaiDstk);
void vREG_INDEXTREE_PUSH(i8** aaiDstk);

void vDev_REG_PTNSTREAM_CLOSE(void* aLU);
void vDev_REG_PTNSTREAM_DELETE(void* aLU);
void vDev_REG_PTNSTREAM_NEW(void* aLU);
void vDev_REG_PTNSTREAM_OPEN(void* aLU);
void vDev_REG_PTNSTREAM_READ(void* aLU);

void vDev_REG_SRC_STREAMDEFOUT_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_ADDCOLUMN(void* aLU,
        Stz* sColumnNm, i8 iColumnType, i8 iColumnLength, i8 iColumnScale);
void vDev_REG_STREAMDEF_STREAMDEFOUT(void* aLU);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
