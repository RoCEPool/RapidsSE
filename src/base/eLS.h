/*============================================================================*/
/* eLS.h -- Stencil Slot                                                      */
/*============================================================================*/
#pragma once

enum eLS // stencil slot
{
  /*  0 */ LS_NONE=0,
  
  /*    */ LS_INFLATE_T,
  /*    */ LS_FILTER_T,
  /*    */ LS_PROJECT_T,
  /*    */ LS_JOIN_KEY,
  /*    */ LS_JOIN,
  /*    */ LS_INFLATE_J,
  /*    */ LS_PROJECT_J,
  /*    */ LS_AGG_KEY,
  /*    */ LS_AGG,
  /*    */ LS_fetch,
  /*    */ LS_PROJECT_H,
  /*    */ LS_FILTER_H,
  /*    */ LS_OUT_H,

  /*    */ LS_ENUMS
};

static Stz* aLS[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "LS_INFLATE_T"_Stz,
  /*    */ "LS_FILTER_T"_Stz,
  /*    */ "LS_PROJECT_T"_Stz,
  /*    */ "LS_JOIN_KEY"_Stz,
  /*    */ "LS_JOIN"_Stz,
  /*    */ "LS_INFLATE_J"_Stz,
  /*    */ "LS_PROJECT_J"_Stz,
  /*    */ "LS_AGG_KEY"_Stz,
  /*    */ "LS_AGG"_Stz,
  /*    */ "LS_fetch"_Stz,
  /*    */ "LS_PROJECT_H"_Stz,
  /*    */ "LS_FILTER_H"_Stz,
  /*    */ "LS_OUT_H"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
