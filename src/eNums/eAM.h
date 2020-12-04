/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,

  /*    */ AM_CRWBUFFER_HANDLER,
  /*    */ AM_CRWBUFFER_PREDICATE_HANDLER,
  /*    */ AM_CRWBUFFER_PROJECT_HANDLER,
  /*    */ AM_DATAPULSE_HANDLER,
  /*    */ AM_DEFAULT_CRWBUFFER_HANDLER,
  /*    */ AM_DEFAULT_DATAPULSE_HANDLER,
  /*    */ AM_DEFAULT_EOSMARKER_HANDLER,
  /*    */ AM_DEFAULT_STARTMARKER_HANDLER,
  /*    */ AM_DEFAULT_STREAMDEF_HANDLER,
  /*    */ AM_INIT,
  /*    */ AM_PREDICATE_BEGIN,
  /*    */ AM_PREDICATE_VARCHAR_COLUMN,
  /*    */ AM_PREDICATE_END,
  /*    */ AM_PROJECT_BEGIN,
  /*    */ AM_PROJECT_COLUMN,
  /*    */ AM_PROJECT_END,
  /*    */ AM_QRY_DONE,
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
    /*    */ "AM_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_CRWBUFFER_PREDICATE_HANDLER"_Stz,
    /*    */ "AM_CRWBUFFER_PROJECT_HANDLER"_Stz,
    /*    */ "AM_DATAPULSE_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_DATAPULSE_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_EOSMARKER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_STARTMARKER_HANDLER"_Stz,
    /*    */ "AM_DEFAULT_STREAMDEF_HANDLER"_Stz,
    /*    */ "AM_INIT"_Stz,
    /*    */ "AM_PREDICATE_BEGIN"_Stz,
    /*    */ "AM_PREDICATE_VARCHAR_COLUMN"_Stz,
    /*    */ "AM_PREDICATE_END"_Stz,
    /*    */ "AM_PROJECT_BEGIN"_Stz,
    /*    */ "AM_PROJECT_COLUMN"_Stz,
    /*    */ "AM_PROJECT_END"_Stz,
    /*    */ "AM_QRY_DONE"_Stz,
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
