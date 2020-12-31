/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,

  /*    */ AM_CRW_PROJECT_COLUMNS,
  /*    */ AM_CRW_PROJECT_END,
  /*    */ AM_CRW_PROJECT_NONE,
  /*    */ AM_CRW_PROJECT_STAR,

  /*    */ AM_DATAPULSE_INDEXRANGE,

  /*    */ AM_DEFAULT_HANDLER_CRWBUFFER,
  /*    */ AM_DEFAULT_HANDLER_DATAPULSE,
  /*    */ AM_DEFAULT_HANDLER_EOSMARKER,
  /*    */ AM_DEFAULT_HANDLER_ROWBUFFER,
  /*    */ AM_DEFAULT_HANDLER_STARTMARKER,
  /*    */ AM_DEFAULT_HANDLER_STREAMDEF,

  /*    */ AM_HANDLER_CRWBUFFER,
  /*    */ AM_HANDLER_DATAPULSE,
  /*    */ AM_HANDLER_ROWBUFFER,

  /*    */ AM_INDEX_EQ,
  /*    */ AM_INDEX_RANGE_LOWER_GT,
  /*    */ AM_INDEX_RANGE_LOWER_GE,
  /*    */ AM_INDEX_LOWER_BOUND_GE,
  /*    */ AM_INDEX_LOWER_BOUND_GT,
  /*    */ AM_INDEX_UPPER_BOUND,

  /*    */ AM_QUERY_DONE,
  /*    */ AM_QUERY_INIT,

  /*    */ AM_ROW_PROJECT_COLUMNS,
  /*    */ AM_ROW_PROJECT_END,
  /*    */ AM_ROW_PROJECT_NONE,
  /*    */ AM_ROW_PROJECT_STAR,

  /*    */ AM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static Stz* aAM[]
{
    /*  0 */ "AM_NONE"_Stz,

    /*    */ "AM_CRW_PROJECT_COLUMNS"_Stz,
    /*    */ "AM_CRW_PROJECT_END"_Stz,
    /*    */ "AM_CRW_PROJECT_NONE"_Stz,
    /*    */ "AM_CRW_PROJECT_STAR"_Stz,

    /*    */ "AM_DATAPULSE_INDEXRANGE"_Stz,

    /*    */ "AM_DEFAULT_HANDLER_CRWBUFFER"_Stz,
    /*    */ "AM_DEFAULT_HANDLER_DATAPULSE"_Stz,
    /*    */ "AM_DEFAULT_HANDLER_EOSMARKER"_Stz,
    /*    */ "AM_DEFAULT_HANDLER_ROWBUFFER"_Stz,
    /*    */ "AM_DEFAULT_HANDLER_STARTMARKER"_Stz,
    /*    */ "AM_DEFAULT_HANDLER_STREAMDEF"_Stz,

    /*    */ "AM_HANDLER_CRWBUFFER"_Stz,
    /*    */ "AM_HANDLER_DATAPULSE"_Stz,
    /*    */ "AM_HANDLER_ROWBUFFER"_Stz,

    /*    */ "AM_INDEX_EQ"_Stz,
    /*    */ "AM_INDEX_RANGE_LOWER_GT"_Stz,
    /*    */ "AM_INDEX_RANGE_LOWER_GE"_Stz,
    /*    */ "AM_INDEX_LOWER_BOUND_GE"_Stz,
    /*    */ "AM_INDEX_LOWER_BOUND_GT"_Stz,
    /*    */ "AM_INDEX_UPPER_BOUND"_Stz,

    /*    */ "AM_QUERY_DONE"_Stz,
    /*    */ "AM_QUERY_INIT"_Stz,

    /*    */ "AM_ROW_PROJECT_COLUMNS"_Stz,
    /*    */ "AM_ROW_PROJECT_END"_Stz,
    /*    */ "AM_ROW_PROJECT_NONE"_Stz,
    /*    */ "AM_ROW_PROJECT_STAR"_Stz,

    /*    */ "AM_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
