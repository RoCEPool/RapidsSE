/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,

  /*    */ AM_CRW_FILTER_END,
  /*    */ AM_CRW_FILTER_ROWS,

  /*    */ AM_CRW_PROJECT_COLUMNS,
  /*    */ AM_CRW_PROJECT_COLUMNS_AGG,
  /*    */ AM_CRW_PROJECT_END,
  /*    */ AM_CRW_PROJECT_NONE,
  /*    */ AM_CRW_PROJECT_STAR,

  /*    */ AM_CRW_PUSH_COLUMN,
  /*    */ AM_DATAPULSE_INDEXRANGE,

  /*    */ AM_DEFAULT_HANDLER_CRWBUFFER,
  /*    */ AM_DEFAULT_HANDLER_DATAPULSE,
  /*    */ AM_DEFAULT_HANDLER_EOSMARKER,
  /*    */ AM_DEFAULT_HANDLER_ROWBUFFER,
  /*    */ AM_DEFAULT_HANDLER_STARTMARKER,
  /*    */ AM_DEFAULT_HANDLER_STREAMDEF,

  /*    */ AM_HANDLER_EOSMARKER_GBY,

  /*    */ AM_HANDLER_CRWBUFFER,
  /*    */ AM_HANDLER_CRWBUFFER_AGG,
  /*    */ AM_HANDLER_CRWBUFFER_AGG_GBY,
  /*    */ AM_HANDLER_CRWBUFFER_LIMIT,
  /*    */ AM_HANDLER_DATAPULSE,
  /*    */ AM_HANDLER_ROWBUFFER,
  /*    */ AM_HANDLER_ROWBUFFER_AGG,
  /*    */ AM_HANDLER_ROWBUFFER_AGG_GBY,
  /*    */ AM_HANDLER_ROWBUFFER_LIMIT,

  /*    */ AM_HANDLER_STREAMDEF_GBY,
  /*    */ AM_HANDLER_STREAMDEF_GBY_END,

  /*    */ AM_INDEX_EQ,
  /*    */ AM_INDEX_LOWER_BOUND_GE,
  /*    */ AM_INDEX_LOWER_BOUND_GT,
  /*    */ AM_INDEX_RANGE_LOWER_GT,
  /*    */ AM_INDEX_RANGE_LOWER_GE,
  /*    */ AM_INDEX_TRAVERSAL,
  /*    */ AM_INDEX_UPPER_BOUND,

  /*    */ AM_PAYLOAD_ADD,
  /*    */ AM_PAYLOAD_END,
  /*    */ AM_PAYLOAD_GBY,
  /*    */ AM_PAYLOAD_KEY,
  /*    */ AM_PAYLOAD_STORE,
  /*    */ AM_PAYLOAD_TO_BUFFER,
  /*    */ AM_PAYLOAD_TO_BUFFER_END,

  /*    */ AM_QUERY_DONE,
  /*    */ AM_QUERY_INIT,

  /*    */ AM_ROW_FILTER_END,
  /*    */ AM_ROW_FILTER_ROWS,

  /*    */ AM_ROW_PROJECT_COLUMNS,
  /*    */ AM_ROW_PROJECT_COLUMNS_AGG,
  /*    */ AM_ROW_PROJECT_END,
  /*    */ AM_ROW_PROJECT_NONE,
  /*    */ AM_ROW_PROJECT_STAR,

  /*    */ AM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static Stz* aAM[]
{
    /*  0 */ "NONE"_Stz,

    /*    */ "CRW_FILTER_END"_Stz,
    /*    */ "CRW_FILTER_ROWS"_Stz,

    /*    */ "CRW_PROJECT_COLUMNS"_Stz,
    /*    */ "CRW_PROJECT_COLUMNS_AGG"_Stz,
    /*    */ "CRW_PROJECT_END"_Stz,
    /*    */ "CRW_PROJECT_NONE"_Stz,
    /*    */ "CRW_PROJECT_STAR"_Stz,

    /*    */ "CRW_PUSH_COLUMN"_Stz,
    /*    */ "DATAPULSE_INDEXRANGE"_Stz,

    /*    */ "DEFAULT_HANDLER_CRWBUFFER"_Stz,
    /*    */ "DEFAULT_HANDLER_DATAPULSE"_Stz,
    /*    */ "DEFAULT_HANDLER_EOSMARKER"_Stz,
    /*    */ "DEFAULT_HANDLER_ROWBUFFER"_Stz,
    /*    */ "DEFAULT_HANDLER_STARTMARKER"_Stz,
    /*    */ "DEFAULT_HANDLER_STREAMDEF"_Stz,

    /*    */ "HANDLER_EOSMARKER_GBY"_Stz,

    /*    */ "HANDLER_CRWBUFFER"_Stz,
    /*    */ "HANDLER_CRWBUFFER_AGG"_Stz,
    /*    */ "HANDLER_CRWBUFFER_AGG_GBY"_Stz,
    /*    */ "HANDLER_CRWBUFFER_LIMIT"_Stz,
    /*    */ "HANDLER_DATAPULSE"_Stz,
    /*    */ "HANDLER_ROWBUFFER"_Stz,
    /*    */ "HANDLER_ROWBUFFER_AGG"_Stz,
    /*    */ "HANDLER_ROWBUFFER_AGG_GBY"_Stz,
    /*    */ "HANDLER_ROWBUFFER_LIMIT"_Stz,

    /*    */ "HANDLER_STREAMDEF_GBY"_Stz,
    /*    */ "HANDLER_STREAMDEF_GBY_END"_Stz,

    /*    */ "INDEX_EQ"_Stz,
    /*    */ "INDEX_LOWER_BOUND_GE"_Stz,
    /*    */ "INDEX_LOWER_BOUND_GT"_Stz,
    /*    */ "INDEX_RANGE_LOWER_GT"_Stz,
    /*    */ "INDEX_RANGE_LOWER_GE"_Stz,
    /*    */ "INDEX_TRAVERSAL"_Stz,
    /*    */ "INDEX_UPPER_BOUND"_Stz,

    /*    */ "PAYLOAD_ADD"_Stz,
    /*    */ "PAYLOAD_END"_Stz,
    /*    */ "PAYLOAD_GBY"_Stz,
    /*    */ "PAYLOAD_KEY"_Stz,
    /*    */ "PAYLOAD_STORE"_Stz,
    /*    */ "PAYLOAD_TO_BUFFER"_Stz,
    /*    */ "PAYLOAD_TO_BUFFER_END"_Stz,

    /*    */ "QUERY_DONE"_Stz,
    /*    */ "QUERY_INIT"_Stz,

    /*    */ "ROW_FILTER_END"_Stz,
    /*    */ "ROW_FILTER_ROWS"_Stz,

    /*    */ "ROW_PROJECT_COLUMNS"_Stz,
    /*    */ "ROW_PROJECT_COLUMNS_AGG"_Stz,
    /*    */ "ROW_PROJECT_END"_Stz,
    /*    */ "ROW_PROJECT_NONE"_Stz,
    /*    */ "ROW_PROJECT_STAR"_Stz,

    /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
