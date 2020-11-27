/*============================================================================*/
/* eAM.h -- Asm Macros                                                        */
/*============================================================================*/
#pragma once

enum eAM // ASM Macros
{
  /*  0 */ AM_NONE=0,

  /*    */ AM_INIT,
  /*    */ AM_DEFAULT_HANDLER,
  /*    */ AM_STREAMDEF_HANDLER_BEGIN,
  /*    */ AM_STREAMDEF_HANDLER_END,
  /*    */ AM_STREAMDEF_ADD_COLUMN,
  /*    */ AM_CRWBUFFER_HANDLER,
  /*    */ AM_PROJECT_COLUMNS_BEGIN,
  /*    */ AM_PROJECT_COLUMNS,
  /*    */ AM_PROJECT_COLUMNS_END,
  /*    */ AM_QRY_DONE,

  /*    */ AM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static Stz* aAM[]
{
    /*  0 */ "AM_NONE"_Stz,
    /*    */ "AM_INIT"_Stz,
    /*    */ "AM_DEFAULT_HANDLER"_Stz,
    /*    */ "AM_STREAMDEF_HANDLER_BEGIN"_Stz,
    /*    */ "AM_STREAMDEF_HANDLER_END"_Stz,
    /*    */ "AM_STREAMDEF_ADD_COLUMN"_Stz,
    /*    */ "AM_CRWBUFFER_HANDLER"_Stz,
    /*    */ "AM_PROJECT_COLUMNS_BEGIN"_Stz,
    /*    */ "AM_PROJECT_COLUMNS"_Stz,
    /*    */ "AM_PROJECT_COLUMNS_END"_Stz,
    /*    */ "AM_QRY_DONE"_Stz,

    /*    */ "AM_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
