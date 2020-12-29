/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,
  /*    */ AM_CRW_PROJECT_END,
  /*    */ AM_CRW_PROJECT_ALL_ROUTINE,
  /*    */ AM_CRW_PROJECT_NOSRC_ROUTINE,
  /*    */ AM_CRW_PROJECT_ROUTINE,
  /*    */ AM_CRWBUFFER_HANDLER,
  /*    */ AM_CRWBUFFER_PROJECT_HANDLER,
  /*    */ AM_DATAPULSE_HANDLER,
  /*    */ AM_DEFAULT_CRWBUFFER_HANDLER,
  /*    */ AM_DEFAULT_DATAPULSE_HANDLER,
  /*    */ AM_DEFAULT_EOSMARKER_HANDLER,
  /*    */ AM_DEFAULT_ROWBUFFER_HANDLER,
  /*    */ AM_DEFAULT_STARTMARKER_HANDLER,
  /*    */ AM_DEFAULT_STREAMDEF_HANDLER,
  /*    */ AM_INDEX_RANGE_ROUTINE,
  /*    */ AM_QUERY_DONE,
  /*    */ AM_QUERY_INIT,
  /*    */ AM_ROW_PROJECT_END,
  /*    */ AM_ROW_PROJECT_ROUTINE,
  /*    */ AM_ROWBUFFER_PROJECT_HANDLER,
  /*    */ AM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static Stz* aAM[]
{
    /*  0 */ "AM_NONE"_Stz,
    /*    */ "AM_CRW_PROJECT_END"_Stz,
    /*    */ "AM_CRW_PROJECT_ALL_ROUTINE"_Stz,
    /*    */ "AM_CRW_PROJECT_NOSRC_ROUTINE"_Stz,
    /*    */ "AM_CRW_PROJECT_ROUTINE"_Stz,
    /*    */ "AM_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_CRWBUFFER_PROJECT_HANDLER"_Stz,
    /*    */ "AM_DATAPULSE_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_DATAPULSE_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_EOSMARKER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_ROWBUFFER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_STARTMARKER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_STREAMDEF_HANDLER"_Stz,
    /*    */ "AM_INDEX_RANGE_ROUTINE"_Stz,
    /*    */ "AM_QUERY_DONE"_Stz,
    /*    */ "AM_QUERY_INIT"_Stz,
    /*    */ "AM_ROW_PROJECT_END"_Stz,
    /*    */ "AM_ROW_PROJECT_ROUTINE"_Stz,
    /*    */ "AM_ROWBUFFER_PROJECT_HANDLER"_Stz,
    /*    */ "AM_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
