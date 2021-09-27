/*============================================================================*/
/* DEV_interface.h -- Interface for RLU Devices                               */
/*============================================================================*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT CRWBUF functions
void vDev_IXT_CRWBUF_BEGIN  (i8** aaiDstk);
void vDev_IXT_CRWBUF_FINDCRW(i8** aaiDstk);
void vDev_IXT_CRWBUF_NEXT   (i8** aaiDstk);
void vDev_IXT_CRWBUF_NULL   (i8** aaiDstk);
void vDev_IXT_CRWBUF_ROWCNT (i8** aaiDstk);
void vDev_IXT_CRWBUF_ROWNO  (i8** aaiDstk);
void vDev_IXT_CRWBUF_TEST   (i8** aaiDstk);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT ENT functions
void vDev_IXT_ENT_COMPARE  (i8** aaiDstk);
void vDev_IXT_ENT_DEREF(i8** aaiDstk);
void vDev_IXT_ENT_FIND     (i8** aaiDstk);
void vDev_IXT_ENT_NEXT     (i8** aaiDstk);
void vDev_IXT_ENT_POSPRIOR (i8** aaiDstk);
void vDev_IXT_ENT_PRINTKEYS(i8** aaiDstk);
void vDev_IXT_ENT_PUSH     (i8** aaiDstk);
void vDev_IXT_ENT_PUSHELMI0(i8** aaiDstk);
void vDev_IXT_ENT_PUSHELMS0(i8** aaiDstk); // NOT COMPLETE!!
void vDev_IXT_ENT_PUSHUPPER(i8** aaiDstk);
void vDev_IXT_CMP_KEYS     (i8** aaiDstk);
void vDev_IXT_ENT_PRINT    (i8** aaiDstk);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev IXT IDX functions
void vDev_IXT_IDX_DEL      (i8** aaiDstk);
void vDev_IXT_IDX_DUMP     (i8** aaiDstk);
void vDev_IXT_IDX_ENTRIES  (i8** aaiDstk);
void vDev_IXT_IDX_INS      (i8** aaiDstk);
void vDev_IXT_INDEXNM_SET     (i8** aaiDstk);
void vDev_IXT_INDEXTREE_SET   (i8** aaiDstk);

void vDev_IXT_POS_BYKEY    (i8** aaiDstk);
void vDev_IXT_POS_LBOUND_GE(i8** aaiDstk);
void vDev_IXT_POS_LBOUND_GE_OLD(i8** aaiDstk);
void vDev_IXT_POS_LBOUND_GT(i8** aaiDstk);
void vDev_IXT_POS_UBOUND_LE(i8** aaiDstk);
void vDev_IXT_POS_UBOUND_LT(i8** aaiDstk);
void vDev_IXT_POS_UBOUND_LT_OLD(i8** aaiDstk);
void vDev_IXT_PUSH_ENT     (i8** aaiDstk);
void vDev_IXT_IDX_BEGIN    (i8** aaiDstk);
void vDev_IXT_IDX_NEXT     (i8** aaiDstk);

void vDev_PAY_ADDRESS(i8** aaiDstk);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// prototypes for dev ROW

void vDev_ROW_ENTWRK_APPENDELMI(i8** aaiDstk);
void vDev_ROW_ENTWRK_APPENDELMS(i8** aaiDstk);
void vDev_ROW_ENTWRK_CREATEPAY(i8** aaiDstk);

void vDev_ENTWRK_PRINT    (i8** aaiDstk);
void vDev_ENTWRK_PUSHELMI (i8** aaiDstk);
void vDev_ENTWRK_PUSHELMS (i8** aaiDstk);
void vDev_ENTWRK_RESET    (i8** aaiDstk);
void vDev_ENTWRK_SETENTKND(i8** aaiDstk);
void vDev_ENTWRK_SETINTELM(i8** aaiDstk);
void vDev_ENTWRK_SETINTELM_OLD(i8** aaiDstk);
void vDev_ENTWRK_SETSTZELM(i8** aaiDstk);

void vDev_ROW_COLS_TO_LOCALS     (i8** aaiDstk);
void vDev_ROW_COLS_TO_ROW        (i8** aaiDstk);
void vDev_ROW_COLS_TO_ROW_OFS    (i8** aaiDstk);
void vDev_ROW_COLS_PUSHSET_PST   (i8** aaiDstk);
void vDev_ROW_NULS_TO_ROW_OFS    (i8** aaiDstk);

void vDev_ROW_ROW_NEW      (i8** aaiDstk);
void vDev_ROW_ROW_PRINT    (i8** aaiDstk);
void vDev_ROW_ROW_SET        (i8** aaiDstk);
void vDev_ROW_ROW_TEST    (i8** aaiDstk);
void vDev_ROW_ROW_ZERO    (i8** aaiDstk);

void vDev_ROW_ROWBUF_BEGIN (i8** aaiDstk);
void vDev_ROW_ROWBUF_FINDROW(i8** aaiDstk);
void vDev_ROW_ROWBUF_NEXT  (i8** aaiDstk);
void vDev_ROW_ROWBUF_NULL  (i8** aaiDstk);
void vDev_ROW_ROWBUF_RESET (i8** aaiDstk);
void vDev_ROW_ROWBUF_ROWCNT (i8** aaiDstk);
void vDev_ROW_ROWBUF_ROWNO  (i8** aaiDstk);
void vDev_ROW_ROWBUF_TEST (i8** aaiDstk);

void vDev_ROW_POPCOLI    (i8** aaiDstk);
void vDev_ROW_POPCOLS    (i8** aaiDstk);
void vDev_ROW_PUSHCOLI   (i8** aaiDstk);
void vDev_ROW_PUSHCOLS   (i8** aaiDstk);
void vDev_ROW_TABLENM_SET(i8** aaiDstk);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void vDev_TABLEPTN_DROPREAD(void* aLU);
void vDev_TABLEPTN_GRABREAD(void* aLU);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void vDev_IXT_COLS_TO_LOCALS     (i8** aaiDstk);
void vDev_IXT_COLS_TO_ROW        (i8** aaiDstk);
void vDev_IXT_COLS_TO_ROW_OFS    (i8** aaiDstk);

void vDev_POT_USE_STREAMDEFOUT(void* aLU, i8 iAddPtnHash);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RLU functions
void vDev_RLU_INDEXDEF_ADDCOLUMN(i8** aaiDstk);
void vDev_RLU_INDEXDEF_DELETE   (i8** aaiDstk);
void vDev_RLU_INDEXDEF_NEW      (i8** aaiDstk);
void vDev_RLU_INDEXTREE_NEW     (i8** aaiDstk);
void vDev_RLU_ROWPIX_PRINT      (i8** aaiDstk);

void vDev_REG_PTNSTREAM_CLOSE(void* aLU);
void vDev_REG_PTNSTREAM_DELETE(void* aLU);
void vDev_REG_PTNSTREAM_NEW(void* aLU);
void vDev_REG_PTNSTREAM_OPEN(void* aLU);
void vDev_REG_PTNSTREAM_READ(void* aLU);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// -TO GO with old demo queries -

void vDev_IXT_COL_PUSHSET_ADR    (i8** aaiDstk);
void vDev_IXT_COL_PUSHSET_PST    (i8** aaiDstk);

Stz* szDev_ROW_PUSHCOLS(void* aLU, void* aROWDEVICE, i8 iArg);

void vDev_REG_EOSMARKER_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_NEW(void* aLU);
void vDev_REG_STREAMDEFOUT_ADDCOLUMN(void* aLU,
        Stz* sColumnNm, i8 iColumnType, i8 iColumnLength, i8 iColumnScale);
void vDev_REG_STREAMDEF_STREAMDEFOUT(void* aLU);
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
