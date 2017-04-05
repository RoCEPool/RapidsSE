/*============================================================================*/
/* Datums.h -- Datums namespace (single units of data)                        */
/*============================================================================*/
#pragma once

#include "Debugging.h"

//-----------------------------------------------------------------------------

namespace Datums
{
  enum eDM // Datum Type
  {
  /* 0 */ DM_NONE=0,
  /* 1 */ DM_BCD,
  /* 2 */ DM_BOL,
  /* 3 */ DM_DTE,
  /* 4 */ DM_FLT,
  /* 5 */ DM_INT,
  /* 6 */ DM_IVL,
  /* 7 */ DM_TME,
  /* 8 */ DM_TSP,
  /* 9 */ DM_ENUMS
  };
  
  static Str* aDM[]
  {
  /* 0 */ "NONE"_Str,
  /* 1 */ "BCD"_Str,
  /* 2 */ "BOL"_Str,
  /* 3 */ "DTE"_Str,
  /* 4 */ "FLT"_Str,
  /* 5 */ "INT"_Str,
  /* 6 */ "IVL"_Str,
  /* 7 */ "TME"_Str,
  /* 8 */ "TSP"_Str,
  /* 9 */ "ENUMS"_Str,
  };
  
  void vCheckAssumptions(void);
  bool bExtractDatum(Str* aDst, Str* aSrc, eDM eDatum, i8& nBytesTaken);
  
  
}

using namespace Datums;

//-----------------------------------------------------------------------------

#include "Bcd.hpp"
#include "Bol.hpp"
#include "Dte.hpp"
#include "Flt.hpp"
#include "Int.hpp"
#include "Ivl.hpp"
#include "Tme.hpp"
#include "Tsp.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
