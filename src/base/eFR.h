/*============================================================================*/
/* eFR.h -- Ribbon Format                                                     */
/*============================================================================*/
#pragma once

enum eFR // ribbon format
{
  /*    */ FR_HEAD,
  /*    */ FR_HEAD_UNIT,

  /*    */ FR_LIST,
  /*    */ FR_NAME,
  /*    */ FR_DATA,

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
  /*    */ "FR_HEAD"_Stz,
  /*    */ "FR_HEAD_UNIT"_Stz,
  
  /*    */ "FR_LIST"_Stz,
  /*    */ "FR_NAME"_Stz,
  /*    */ "FR_DATA"_Stz,

  /*    */ "FR_ATOM"_Stz,
  /*    */ "FR_ATOM_ASTER"_Stz,
  /*    */ "FR_ATOM_BRACE"_Stz,
  /*    */ "FR_ATOM_ROUND"_Stz,
  /*    */ "FR_ATOM_SLASH"_Stz,
  /*    */ "FR_ATOM_SQUARE"_Stz,

  /*    */ "FR_ENUMS"_Stz,
};

static bool bFR_right[]
{
  /*    */ false, // FR_HEAD
  /*    */ false, // FR_HEAD_UNIT

  /*    */ false, // FR_LIST
  /*    */ true,  // FR_NAME
  /*    */ true,  // FR_DATA

  /*    */ false, // FR_ATOM
  /*    */ false, // FR_ATOM_ASTER
  /*    */ false, // FR_ATOM_BRACE
  /*    */ false, // FR_ATOM_SLASH
  /*    */ false, // FR_ATOM_ROUND
  /*    */ false, // FR_ATOM_SQUARE
  
  /*    */ false, // FR_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
