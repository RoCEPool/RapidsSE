/*============================================================================*/
/* Values.h -- Values namespace                                               */
/*============================================================================*/

#pragma once

#include "Commands.h"

//-----------------------------------------------------------------------------

namespace Values
{
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
  
  /* 12 */ DM_ENUMS
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
  
  /* 12 */ "ENUMS"_Stz,
  };

  void vCheckAssumptions(void);
}

using namespace Values;

//-----------------------------------------------------------------------------

typedef String_t<C_MAX_VCH> sVCH;

#include "Dtm.hpp"
#include "Value.hpp"

#include "BcdVal.hpp"
#include "BolVal.hpp"
#include "DteVal.hpp"
#include "FipVal.hpp"
#include "FltVal.hpp"
#include "IntVal.hpp"
#include "IvlVal.hpp"
#include "PtrVal.hpp"
#include "StrVal.hpp"
#include "TmeVal.hpp"
#include "TspVal.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
