/*============================================================================*/
/* DEV_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

void vDev_IXT_BEGIN(i8** aaiDstk);
void vDev_IXT_DEREF(i8** aaiDstk);
void vDev_IXT_NEXT (i8** aaiDstk);
void vDev_IXT_NULL (i8** aaiDstk);
void vDev_IXT_TEST (i8** aaiDstk);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT

void vDev_IXT_ENT_COMPARE  (i8** aaiDstk);
void vDev_IXT_ENT_FIND     (i8** aaiDstk);
void vDev_IXT_ENT_NEXT     (i8** aaiDstk);
void vDev_IXT_ENT_POSPRIOR (i8** aaiDstk);
void vDev_IXT_ENT_PRINTKEYS(i8** aaiDstk);
void vDev_IXT_ENT_PUSH     (i8** aaiDstk);
void vDev_IXT_ENT_PUSHELMI0(i8** aaiDstk);
void vDev_IXT_ENT_PUSHELMS0(i8** aaiDstk); // NOT COMPLETE!!
void vDev_IXT_ENT_PUSHUPPER(i8** aaiDstk);
void vDev_IXT_CMP_KEYS     (i8** aaiDstk);
void vDev_IXT_DEL_ENT      (i8** aaiDstk);
void vDev_IXT_DUMP         (i8** aaiDstk);
void vDev_IXT_ENT_PRINT    (i8** aaiDstk);

void vDev_IXT_INDEXNM_SET     (i8** aaiDstk);
void vDev_IXT_INDEXTREE_DELETE(i8** aaiDstk);
void vDev_IXT_INDEXTREE_SET   (i8** aaiDstk);
void vDev_IXT_INS_ENT         (i8** aaiDstk);
void vDev_IXT_SET_ENT         (void* aLU, void* aIXT, void* aEnt);

void vDev_IXT_TMP_ENTRIES  (i8** aaiDstk);
void vDev_IXT_POS_BYKEY    (i8** aaiDstk);
void vDev_IXT_POS_LBOUND   (i8** aaiDstk);
void vDev_IXT_POS_UBOUND   (i8** aaiDstk);
void vDev_IXT_POS_UBOUND_LT(i8** aaiDstk);
void vDev_IXT_PUSH_ENT     (i8** aaiDstk);
void vDev_IXT_IDX_BEGIN    (i8** aaiDstk);
void vDev_IXT_IDX_NEXT     (i8** aaiDstk);

i8*  aDev_PAY_ADDRESS(i8** aaiDstk);
i8   iDev_PAY_FETCH_COLI(void* aLU, void* aIXT, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* aLU, void* aIXT, i8 iArg); 
void vDev_PAY_STORE_COLI(void* aLU, void* aIXT, i8 iArg, i8 iTop);
Stz* szDev_PAY_STORE_COLS(void* aLU, void* aIXT, i8 iArg);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev ROW

void vDev_ROW_ENTWRK_APPENDELMI(i8** aaiDstk);
void vDev_ROW_ENTWRK_APPENDELMS(i8** aaiDstk);
void vDev_ROW_ENTWRK_CREATEPAY(i8** aaiDstk);

void vDev_ENTWRK_PRINT(i8** aaiDstk);
void vDev_ENTWRK_RESET(i8** aaiDstk);
void vDev_ENTWRK_SETINTELM(i8** aaiDstk);

void vDev_ROW_BEGIN(i8** aaiDstk);
void vDev_ROW_NEW(i8** aaiDstk);
void vDev_ROW_NEXT(i8** aaiDstk);
void vDev_ROW_NULL(i8** aaiDstk);
void vDev_ROW_POPCOLI(i8** aaiDstk);
void vDev_ROW_POPCOLS(i8** aaiDstk);
void vDev_ROW_PRINT(i8** aaiDstk);
void vDev_ROW_PUSHCOLI(i8** aaiDstk);
void vDev_ROW_PUSHCOLS(i8** aaiDstk);
void vDev_ROW_PUSHOFS(i8** aaiDstk);
void vDev_ROW_RESET(i8** aaiDstk);
void vDev_ROW_SET(i8** aaiDstk);
void vDev_ROW_TABLENM_SET(i8** aaiDstk);
void vDev_ROW_TESTBUFFER(i8** aaiDstk);
void vDev_ROW_TESTROW(i8** aaiDstk);
void vDev_ROW_SETBYOFS(i8** aaiDstk);
void vDev_ROW_ZERO(i8** aaiDstk);

Stz* szDev_ROW_PUSHCOLS(void* aLU, void* aROWDEVICE, i8 iArg);     // should go

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

void vDev_RLU_INDEXDEF_ADDCOLUMN(i8** aaiDstk);
void vDev_RLU_INDEXDEF_DELETE(i8** aaiDstk);
void vDev_RLU_INDEXDEF_NEW(i8** aaiDstk);

void vDev_RLU_INDEXTREE_NEW(i8** aaiDstk);

void vDev_REG_PTNSTREAM_CLOSE(void* aLU);
void vDev_REG_PTNSTREAM_DELETE(void* aLU);
void vDev_REG_PTNSTREAM_NEW(void* aLU);
void vDev_REG_PTNSTREAM_OPEN(void* aLU);
void vDev_REG_PTNSTREAM_READ(void* aLU);

void vDev_REG_STREAMDEFOUT_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_ADDCOLUMN(void* aLU,
        Stz* sColumnNm, i8 iColumnType, i8 iColumnLength, i8 iColumnScale);
void vDev_REG_STREAMDEF_STREAMDEFOUT(void* aLU);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
