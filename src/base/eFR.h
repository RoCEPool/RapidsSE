/*============================================================================*/
/* eFR.h -- Ribbon Format                                                     */
/*============================================================================*/
#pragma once

enum eFR // ribbon format
{
  /*    */ FR_HEADING,
  
  /*    */ FR_LIST,
  
  /*    */ FR_NAME,

  /*    */ FR_ATOM,
  /*    */ FR_ATOM_ASTER,
  /*    */ FR_ATOM_BRACE,
  /*    */ FR_ATOM_SLASH,
  /*    */ FR_ATOM_ROUND,
  /*    */ FR_ATOM_SQUARE,
  

  /*    */ FR_ENUMS,
};

static Stz* aFR[]
{
  /*    */ "FR_HEADING"_Stz,

  /*    */ "FR_LIST"_Stz,
  
  /*    */ "FR_NAME"_Stz,
  
  /*    */ "FR_ATOM"_Stz,
  /*    */ "FR_ATOM_ASTER"_Stz,
  /*    */ "FR_ATOM_BRACE"_Stz,
  /*    */ "FR_ATOM_ROUND"_Stz,
  /*    */ "FR_ATOM_SLASH"_Stz,
  /*    */ "FR_ATOM_SQUARE"_Stz,

  /*    */ "FR_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
