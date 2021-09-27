/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,

  /*    */ AM_CRW_FILTER_BEGIN,
  /*    */ AM_CRW_FILTER_END,

  /*    */ AM_CRW_HASHROWS_BEGIN,
  /*    */ AM_CRW_HASHROWS_END,

  /*    */ AM_CRW_INDEXJOIN_BEGIN,
  /*    */ AM_CRW_INDEXJOIN_NEXT,

  /*    */ AM_CRW_INDEXJOIN_NEXT_LO,

  /*    */ AM_CRW_PROJECT_COLUMNS,
  /*    */ AM_CRW_PROJECT_COLUMNS_AGG,
  /*    */ AM_CRW_PROJECT_END,
  /*    */ AM_CRW_PROJECT_STAR,

  /*    */ AM_DATAPULSE_INDEXRANGE,

  /*    */ AM_DEFAULT_HANDLER_CRWBUFFER,
  /*    */ AM_DEFAULT_HANDLER_DATAPULSE,
  /*    */ AM_DEFAULT_HANDLER_LEFTOUTER_CRWLFT,
  /*    */ AM_DEFAULT_HANDLER_LEFTOUTER_ROWLFT,
  /*    */ AM_DEFAULT_HANDLER_EOSMARKER,
  /*    */ AM_DEFAULT_HANDLER_ROWBUFFER,
  /*    */ AM_DEFAULT_HANDLER_STREAMDEF,

  /*    */ AM_HANDLER_CRWBUFFER,
  /*    */ AM_HANDLER_CRWBUFFER_AGG,
  /*    */ AM_HANDLER_CRWBUFFER_AGG_GBY,
  /*    */ AM_HANDLER_CRWBUFFER_LIMIT,
  /*    */ AM_HANDLER_CRWBUFFER_RHT,
  /*    */ AM_HANDLER_DATAPULSE,
  /*    */ AM_HANDLER_LEFTOUTER_CRWLFT,
  /*    */ AM_HANDLER_LEFTOUTER_ROWLFT,
  /*    */ AM_HANDLER_EOSMARKER_GBY,
  /*    */ AM_HANDLER_ROWBUFFER,
  /*    */ AM_HANDLER_ROWBUFFER_AGG,
  /*    */ AM_HANDLER_ROWBUFFER_AGG_GBY,
  /*    */ AM_HANDLER_ROWBUFFER_LIMIT,
  /*    */ AM_HANDLER_ROWBUFFER_RHT,

  /*    */ AM_HANDLER_STREAMDEF_GBY,
  /*    */ AM_HANDLER_STREAMDEF_GBY_END,

  /*    */ AM_INDEX_EQ,
  /*    */ AM_INDEX_LOWER_BOUND_GE,
  /*    */ AM_INDEX_LOWER_BOUND_GT,
  /*    */ AM_INDEX_POSITION_AT_START,
  /*    */ AM_INDEX_RANGE_LOWER_GT,
  /*    */ AM_INDEX_RANGE_LOWER_GE,
  /*    */ AM_INDEX_SCAN,
  /*    */ AM_INDEX_SCAN_END,
  /*    */ AM_INDEX_TRAVERSAL,
  /*    */ AM_INDEX_TRAVERSE_TILL_END,
  /*    */ AM_INDEX_TRAVERSE_TILL_UPPER,
  /*    */ AM_INDEX_UPPER_BOUND,

  /*    */ AM_JOINROWS_CRWLFT_CRWRHT,
  /*    */ AM_JOINROWS_CRWLFT_ENTRHT,
  /*    */ AM_JOINROWS_CRWLFT_NULENT,
  /*    */ AM_JOINROWS_CRWLFT_NULRHT,
  /*    */ AM_JOINROWS_CRWLFT_ROWRHT,
  /*    */ AM_JOINROWS_ROWLFT_CRWRHT,
  /*    */ AM_JOINROWS_ROWLFT_ENTRHT,
  /*    */ AM_JOINROWS_ROWLFT_NULENT,
  /*    */ AM_JOINROWS_ROWLFT_NULRHT,
  /*    */ AM_JOINROWS_ROWLFT_ROWRHT,

  /*    */ AM_JOINTEST_CRWLFT_CRWRHT,
  /*    */ AM_JOINTEST_CRWLFT_ROWRHT,
  /*    */ AM_JOINTEST_END,
  /*    */ AM_JOINTEST_ROWLFT_CRWRHT,
  /*    */ AM_JOINTEST_ROWLFT_ROWRHT,

  /*    */ AM_LOOPJOIN_CRWLFT_CRWRHT,
  /*    */ AM_LOOPJOIN_CRWLFT_ROWRHT,
  /*    */ AM_LOOPJOIN_ROWLFT_CRWRHT,
  /*    */ AM_LOOPJOIN_ROWLFT_ROWRHT,

  /*    */ AM_PAYLOAD_AGG,
  /*    */ AM_PAYLOAD_END,
  /*    */ AM_PAYLOAD_GBY,
  /*    */ AM_PAYLOAD_KEY,
  /*    */ AM_PAYLOAD_KEY_END,
  /*    */ AM_PAYLOAD_STORE,
  /*    */ AM_PAYLOAD_TO_BUFFER,
  /*    */ AM_PAYLOAD_TO_BUFFER_END,

  /*    */ AM_QUERY_DONE,
  /*    */ AM_QUERY_INIT,

  /*    */ AM_ROW_EXPR_AGG,
  /*    */ AM_ROW_EXPR_AGG_END,

  /*    */ AM_ROW_FILTER_BEGIN,
  /*    */ AM_ROW_FILTER_END,

  /*    */ AM_ROW_HASHROWS_BEGIN,
  /*    */ AM_ROW_HASHROWS_END,

  /*    */ AM_ROW_INDEXJOIN_BEGIN,
  /*    */ AM_ROW_INDEXJOIN_NEXT,

  /*    */ AM_ROW_INDEXJOIN_NEXT_LO,

  /*    */ AM_ROW_PROJECT_COLUMNS,
  /*    */ AM_ROW_PROJECT_COLUMNS_AGG,
  /*    */ AM_ROW_PROJECT_END,
  /*    */ AM_ROW_PROJECT_STAR,

  /*    */ AM_ROWBUFFER_AGGINIT,
  /*    */ AM_ROWBUFFER_AGGINIT_END,
  /*    */ AM_ROWBUFFER_TRAVERSAL,

  /*    */ AM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static Stz* aAM[]
{
    /*  0 */ "NONE"_Stz,

    /*    */ "CRW_FILTER_BEGIN"_Stz,
    /*    */ "CRW_FILTER_END"_Stz,

    /*    */ "CRW_HASHROWS_BEGIN"_Stz,
    /*    */ "CRW_HASHROWS_END"_Stz,

    /*    */ "CRW_INDEXJOIN_BEGIN"_Stz,
    /*    */ "CRW_INDEXJOIN_NEXT"_Stz,

    /*    */ "CRW_INDEXJOIN_NEXT_LO"_Stz,

    /*    */ "CRW_PROJECT_COLUMNS"_Stz,
    /*    */ "CRW_PROJECT_COLUMNS_AGG"_Stz,
    /*    */ "CRW_PROJECT_END"_Stz,
    /*    */ "CRW_PROJECT_STAR"_Stz,

    /*    */ "DATAPULSE_INDEXRANGE"_Stz,

    /*    */ "DEFAULT_HANDLER_CRWBUFFER"_Stz,
    /*    */ "DEFAULT_HANDLER_DATAPULSE"_Stz,
    /*    */ "DEFAULT_HANDLER_LEFTOUTER_CRWLFT"_Stz,
    /*    */ "DEFAULT_HANDLER_LEFTOUTER_ROWLFT"_Stz,
    /*    */ "DEFAULT_HANDLER_EOSMARKER"_Stz,
    /*    */ "DEFAULT_HANDLER_ROWBUFFER"_Stz,
    /*    */ "DEFAULT_HANDLER_STREAMDEF"_Stz,

    /*    */ "HANDLER_CRWBUFFER"_Stz,
    /*    */ "HANDLER_CRWBUFFER_AGG"_Stz,
    /*    */ "HANDLER_CRWBUFFER_AGG_GBY"_Stz,
    /*    */ "HANDLER_CRWBUFFER_LIMIT"_Stz,
    /*    */ "HANDLER_CRWBUFFER_RHT"_Stz,
    /*    */ "HANDLER_DATAPULSE"_Stz,
    /*    */ "HANDLER_LEFTOUTER_CRWLFT"_Stz,
    /*    */ "HANDLER_LEFTOUTER_ROWLFT"_Stz,
    /*    */ "HANDLER_EOSMARKER_GBY"_Stz,
    /*    */ "HANDLER_ROWBUFFER"_Stz,
    /*    */ "HANDLER_ROWBUFFER_AGG"_Stz,
    /*    */ "HANDLER_ROWBUFFER_AGG_GBY"_Stz,
    /*    */ "HANDLER_ROWBUFFER_LIMIT"_Stz,
    /*    */ "HANDLER_ROWBUFFER_RHT"_Stz,

    /*    */ "HANDLER_STREAMDEF_GBY"_Stz,
    /*    */ "HANDLER_STREAMDEF_GBY_END"_Stz,

    /*    */ "INDEX_EQ"_Stz,
    /*    */ "INDEX_LOWER_BOUND_GE"_Stz,
    /*    */ "INDEX_LOWER_BOUND_GT"_Stz,
    /*    */ "INDEX_POSITION_AT_START"_Stz,
    /*    */ "INDEX_RANGE_LOWER_GT"_Stz,
    /*    */ "INDEX_RANGE_LOWER_GE"_Stz,
    /*    */ "INDEX_SCAN"_Stz,
    /*    */ "INDEX_SCAN_END"_Stz,
    /*    */ "INDEX_TRAVERSAL"_Stz,
    /*    */ "INDEX_TRAVERSE_TILL_END"_Stz,
    /*    */ "INDEX_TRAVERSE_TILL_UPPER"_Stz,
    /*    */ "INDEX_UPPER_BOUND"_Stz,

    /*    */ "JOINROWS_CRWLFT_CRWRHT"_Stz,
    /*    */ "JOINROWS_CRWLFT_ENTRHT"_Stz,
    /*    */ "JOINROWS_CRWLFT_NULENT"_Stz,
    /*    */ "JOINROWS_CRWLFT_NULRHT"_Stz,
    /*    */ "JOINROWS_CRWLFT_ROWRHT"_Stz,
    /*    */ "JOINROWS_ROWLFT_CRWRHT"_Stz,
    /*    */ "JOINROWS_ROWLFT_ENTRHT"_Stz,
    /*    */ "JOINROWS_ROWLFT_NULENT"_Stz,
    /*    */ "JOINROWS_ROWLFT_NULRHT"_Stz,
    /*    */ "JOINROWS_ROWLFT_ROWRHT"_Stz,

    /*    */ "JOINTEST_CRWLFT_CRWRHT"_Stz,
    /*    */ "JOINTEST_CRWLFT_ROWRHT"_Stz,
    /*    */ "JOINTEST_END"_Stz,
    /*    */ "JOINTEST_ROWLFT_CRWRHT"_Stz,
    /*    */ "JOINTEST_ROWLFT_ROWRHT"_Stz,

    /*    */ "LOOPJOIN_CRWLFT_CRWRHT"_Stz,
    /*    */ "LOOPJOIN_CRWLFT_ROWRHT"_Stz,
    /*    */ "LOOPJOIN_ROWLFT_CRWRHT"_Stz,
    /*    */ "LOOPJOIN_ROWLFT_ROWRHT"_Stz,

    /*    */ "PAYLOAD_AGG"_Stz,
    /*    */ "PAYLOAD_END"_Stz,
    /*    */ "PAYLOAD_GBY"_Stz,
    /*    */ "PAYLOAD_KEY"_Stz,
    /*    */ "PAYLOAD_KEY_END"_Stz,
    /*    */ "PAYLOAD_STORE"_Stz,
    /*    */ "PAYLOAD_TO_BUFFER"_Stz,
    /*    */ "PAYLOAD_TO_BUFFER_END"_Stz,

    /*    */ "QUERY_DONE"_Stz,
    /*    */ "QUERY_INIT"_Stz,

    /*    */ "ROW_EXPR_AGG"_Stz,
    /*    */ "ROW_EXPR_AGG_END"_Stz,

    /*    */ "ROW_FILTER_BEGIN"_Stz,
    /*    */ "ROW_FILTER_END"_Stz,

    /*    */ "ROW_HASHROWS_BEGIN"_Stz,
    /*    */ "ROW_HASHROWS_END"_Stz,

    /*    */ "ROW_INDEXJOIN_BEGIN"_Stz,
    /*    */ "ROW_INDEXJOIN_NEXT"_Stz,

    /*    */ "ROW_INDEXJOIN_NEXT_LO"_Stz,

    /*    */ "ROW_PROJECT_COLUMNS"_Stz,
    /*    */ "ROW_PROJECT_COLUMNS_AGG"_Stz,
    /*    */ "ROW_PROJECT_END"_Stz,
    /*    */ "ROW_PROJECT_STAR"_Stz,

    /*    */ "ROWBUFFER_AGGINIT"_Stz,
    /*    */ "ROWBUFFER_AGGINIT_END"_Stz,
    /*    */ "ROWBUFFER_TRAVERSAL"_Stz,

    /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
