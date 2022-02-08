/*============================================================================*/
/* eFR.h -- Ribbon Format                                                     */
/*============================================================================*/
#pragma once

enum eFR // ribbon format
{
  /*    */ FR_HEAD,
  /*    */ FR_HEAD_UNIT,

  /*    */ FR_DOTS,
  /*    */ FR_LIST,
  /*    */ FR_NAME,
  /*    */ FR_TEXT,
  /*    */ FR_SIZE,
  /*    */ FR_SDEF_COLUMN,

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
  
  /*    */ "FR_DOTS"_Stz,
  /*    */ "FR_LIST"_Stz,
  /*    */ "FR_NAME"_Stz,
  /*    */ "FR_TEXT"_Stz,
  /*    */ "FR_SIZE"_Stz,
  /*    */ "FR_SDEF_COLUMN"_Stz,

  /*    */ "FR_ATOM"_Stz,
  /*    */ "FR_ATOM_ASTER"_Stz,
  /*    */ "FR_ATOM_BRACE"_Stz,
  /*    */ "FR_ATOM_ROUND"_Stz,
  /*    */ "FR_ATOM_SLASH"_Stz,
  /*    */ "FR_ATOM_SQUARE"_Stz,

  /*    */ "FR_ENUMS"_Stz,
};

static const i8 C_JUSTIFY_LEFT   = 0;
static const i8 C_JUSTIFY_CENTRE = 1;
static const i8 C_JUSTIFY_RIGHT  = 2;

static i8 iFR_justify[]
{
  /*    */ C_JUSTIFY_LEFT,  // FR_HEAD
  /*    */ C_JUSTIFY_LEFT,  // FR_HEAD_UNIT

  /*    */ C_JUSTIFY_LEFT,  // FR_DOTS
  /*    */ C_JUSTIFY_LEFT,  // FR_LIST
  /*    */ C_JUSTIFY_RIGHT, // FR_NAME
  /*    */ C_JUSTIFY_RIGHT, // FR_TEXT
  /*    */ C_JUSTIFY_RIGHT, // FR_SIZE
  /*    */ C_JUSTIFY_RIGHT, // FR_SDEF_COLUMN

  /*    */ C_JUSTIFY_RIGHT, // FR_ATOM
  /*    */ C_JUSTIFY_RIGHT, // FR_ATOM_ASTER
  /*    */ C_JUSTIFY_RIGHT, // FR_ATOM_BRACE
  /*    */ C_JUSTIFY_RIGHT, // FR_ATOM_SLASH
  /*    */ C_JUSTIFY_RIGHT, // FR_ATOM_ROUND
  /*    */ C_JUSTIFY_RIGHT, // FR_ATOM_SQUARE
  
  /*    */ C_JUSTIFY_LEFT,  // FR_ENUMS
};

static Stz* aFR_format[]
{
  /*    */ "--%02lld--------------=="_Stz,   // FR_HEAD
  /*    */ "--%02lld-------------%c%c-"_Stz, // FR_HEAD_UNIT
  
  /*    */ "%s%lld)%s"_Stz,// FR_DOTS
  /*    */ "%s %lld"_Stz,  // FR_LIST
  /*    */ "%s"_Stz,       // FR_NAME
  /*    */ "%s"_Stz,       // FR_TEXT
  /*    */ "%lld"_Stz,     // FR_SIZE
  /*    */ "%s %c"_Stz,    // FR_SDEF_COLUMN
  
  /*    */ "%s"_Stz,       // FR_ATOM
  /*    */ "*%s*"_Stz,     // FR_ATOM_ASTER
  /*    */ "{%s}"_Stz,     // FR_ATOM_BRACE
  /*    */ "/%s/"_Stz,     // FR_ATOM_SLASH
  /*    */ "(%s)"_Stz,     // FR_ATOM_ROUND
  /*    */ "[%s]"_Stz,     // FR_ATOM_SQUARE

  /*    */ "???"_Stz, // FR_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
