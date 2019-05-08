/*============================================================================*/
/* Datums.h -- Datums namespace (single units of data)                        */
/*============================================================================*/
#pragma once

#include "Correctness.h"

//-----------------------------------------------------------------------------

namespace Datums
{
  static const u8 C_BASE_YEAR  = 1900;
  static const u8 C_YEAR_RANGE = 255;
  static const i8 C_EXTENDED_AUX = 6;
  
  static const i8 C_MAX_VCH = C_4KB;

  enum eDM : u1 // Datum Type
  {
  /*  0 */ DM_NONE=0,
  
  /*  1 */ DM_BCD,
  /*  2 */ DM_BOL,
  /*  3 */ DM_CHR,
  /*  4 */ DM_DTE,
  /*  5 */ DM_FLT,
  /*  6 */ DM_INT,
  /*  7 */ DM_IVL,
  /*  8 */ DM_TME,
  /*  9 */ DM_TSP,
  /* 10 */ DM_VBN,
  /* 11 */ DM_VCH,
  
  // extended datums
  /* 12 */ DM_ALPHA,
  /* 13 */ DM_DAYS,
  /* 14 */ DM_DELTA_PLUS,
  /* 15 */ DM_DELTA_MINUS,
  /* 16 */ DM_DIGIT,
  /* 17 */ DM_TENTH,

  /* 18 */ DM_ENUMS
  };
  
  static Stz* aDM[]
  {
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "BCD"_Stz,
  /*  2 */ "BOL"_Stz,
  /*  3 */ "CHR"_Stz,
  /*  4 */ "DTE"_Stz,
  /*  5 */ "FLT"_Stz,
  /*  6 */ "INT"_Stz,
  /*  7 */ "IVL"_Stz,
  /*  8 */ "TME"_Stz,
  /*  9 */ "TSP"_Stz,
  /* 10 */ "VBN"_Stz,
  /* 11 */ "VCH"_Stz,

  // extended datums
  /* 12 */ "ALPHA"_Stz,
  /* 13 */ "DAYS"_Stz,
  /* 14 */ "DELTA_PLUS"_Stz,
  /* 15 */ "DELTA_MINUS"_Stz,
  /* 16 */ "DIGIT"_Stz,
  /* 17 */ "TENTH"_Stz,

  /* 18 */ "ENUMS"_Stz,
  };
  
  void vCheckAssumptions(void);
}

using namespace Datums;

//-----------------------------------------------------------------------------

#include "Formatters.hpp"
#include "HshLib.hpp"

#include "Bcd.hpp"
#include "Bol.hpp"
#include "Dte.hpp"
#include "Flt.hpp"
#include "Int.hpp"
#include "Ivl.hpp"
#include "Tme.hpp"
#include "Tsp.hpp"
#include "Vch.hpp"

#include "DtmLib.hpp"
#include "DatumUnion.hpp"

typedef String_t<C_MAX_VCH> sVCH;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
