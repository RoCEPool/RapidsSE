/*============================================================================*/
/* Datums.h -- Datums namespace (single units of data)                        */
/*============================================================================*/
#pragma once

#include "ThirdParty.h"

//-----------------------------------------------------------------------------

namespace Datums
{
  enum eDM // Datum Type
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
  /* 12 */ DM_ENUMS
  };
  
  static Str* aDM[]
  {
  /*  0 */ "NONE"_Str,
  /*  1 */ "BCD"_Str,
  /*  2 */ "BOL"_Str,
  /*  3 */ "CHR"_Str,
  /*  4 */ "DTE"_Str,
  /*  5 */ "FLT"_Str,
  /*  6 */ "INT"_Str,
  /*  7 */ "IVL"_Str,
  /*  8 */ "TME"_Str,
  /*  9 */ "TSP"_Str,
  /* 10 */ "VBN"_Str,
  /* 11 */ "VCH"_Str,
  /* 12 */ "ENUMS"_Str,
  };
  
  void vCheckAssumptions(void);
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
#include "Vch.hpp"

#include "DtmLib.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
