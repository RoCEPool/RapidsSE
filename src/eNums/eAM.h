/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,
  /*    */ AM_CRW_FILTER_ROUTINE,
  /*    */ AM_CRW_PROJECT_ROUTINE,
  /*    */ AM_CRWBUFFER_EXISTS_HANDLER,
  /*    */ AM_CRWBUFFER_HANDLER,
  /*    */ AM_CRWBUFFER_FILTER_HANDLER,
  /*    */ AM_CRWBUFFER_PROJECT_HANDLER,
  /*    */ AM_DATAPULSE_HANDLER,
  /*    */ AM_DEFAULT_CRWBUFFER_HANDLER,
  /*    */ AM_DEFAULT_DATAPULSE_HANDLER,
  /*    */ AM_DEFAULT_EOSMARKER_HANDLER,
  /*    */ AM_DEFAULT_ROWBUFFER_HANDLER,
  /*    */ AM_DEFAULT_STARTMARKER_HANDLER,
  /*    */ AM_DEFAULT_STREAMDEF_HANDLER,
  /*    */ AM_FILTER_END,
  /*    */ AM_FILTER_VARCHAR_COLUMN,
  /*    */ AM_INIT,
  /*    */ AM_PROJECT_END,
  /*    */ AM_PUSH_SRC,
  /*    */ AM_QRY_DONE,
  /*    */ AM_SET_DST_COLUMN,
  /*    */ AM_SET_SRC_COLUMN,
  /*    */ AM_SRC_EXISTS_ALTINDEX,
  /*    */ AM_STARTMARKER_HANDLER,
  /*    */ AM_STREAMDEF_ADD_COLUMN,
  /*    */ AM_STREAMDEF_BEGIN,
  /*    */ AM_STREAMDEF_END,
  /*    */ AM_TABLE_SCAN,
  /*    */ AM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static Stz* aAM[]
{
    /*  0 */ "AM_NONE"_Stz,
    /*    */ "AM_CRW_FILTER_ROUTINE"_Stz,
    /*    */ "AM_CRW_PROJECT_ROUTINE"_Stz,
    /*    */ "AM_CRWBUFFER_EXISTS_HANDLER"_Stz,
    /*    */ "AM_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_CRWBUFFER_FILTER_HANDLER"_Stz,
    /*    */ "AM_CRWBUFFER_PROJECT_HANDLER"_Stz,
    /*    */ "AM_DATAPULSE_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_DATAPULSE_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_EOSMARKER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_ROWBUFFER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_STARTMARKER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_STREAMDEF_HANDLER"_Stz,
    /*    */ "AM_FILTER_END"_Stz,
    /*    */ "AM_FILTER_VARCHAR_COLUMN"_Stz,
    /*    */ "AM_INIT"_Stz,
    /*    */ "AM_PROJECT_END"_Stz,
    /*    */ "AM_PUSH_SRC"_Stz,
    /*    */ "AM_QRY_DONE"_Stz,
    /*    */ "AM_SET_DST_COLUMN"_Stz,
    /*    */ "AM_SET_SRC_COLUMN"_Stz,
    /*    */ "AM_SRC_EXISTS_ALTINDEX"_Stz,
    /*    */ "AM_STARTMARKER_HANDLER"_Stz,
    /*    */ "AM_STREAMDEF_ADD_COLUMN"_Stz,
    /*    */ "AM_STREAMDEF_BEGIN"_Stz,
    /*    */ "AM_STREAMDEF_END"_Stz,
    /*    */ "AM_TABLE_SCAN"_Stz,
    /*    */ "AM_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
