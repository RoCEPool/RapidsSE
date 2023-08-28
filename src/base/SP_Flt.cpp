/*============================================================================*/
/* SP_Flt.cpp - Flt sprint helper                                             */
/*============================================================================*/

#include "BASE.h"

#include "SP_Flt.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Flt::SP_Flt(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  // set default Flt format
  vFormat(F_FLT);
  
  _checkInitialised_(CN_SPSMALL);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Flt::~SP_Flt(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint SP_Flt and return Stz* to result                                     */
/*----------------------------------------------------------------------------*/
Stz* SP_Flt::sz(d8 dFlt)
{
  _precondition_(_MAGIC_,_FOOTER_);

  if (StrLib::bEqualTo(m_caFormat,"%f")) {

//    s32 sFormat;
//
//         if (dFlt >  99999999999999 || dFlt < -99999999999999)
//      sFormat = "%e"_Stz;
//    else if (dFlt >   9999999999999 || dFlt <  -9999999999999)
//      sFormat = "%.1f"_Stz;
//    else if (dFlt >    999999999999 || dFlt <   -999999999999)
//      sFormat = "%.2f"_Stz;
//    else if (dFlt >     99999999999 || dFlt <    -99999999999)
//      sFormat = "%.3f"_Stz;
//    else if (dFlt >      9999999999 || dFlt <     -9999999999)
//      sFormat = "%.4f"_Stz;
//    else if (dFlt >       999999999 || dFlt <      -999999999)
//      sFormat = "%.5f"_Stz;
//    else sFormat = m_caFormat;
//
//    StrLib::vSprint(m_caResult,
//                    sFormat.szStz(),
//                    dFlt);

//
//    StrLib::vSprint(m_caResult,
//                    "%.9e"_Stz,
//                    dFlt);


    // Using %e will round the value at the last decimal place.
    // This becomes a problem when the value is very close to the largest/smallest
    // floating point value that we support. The rounding can push it out of bounds.
    // The fix here is to print the value with 10 decimal places to a temporary string
    // and then copy the first 9 of them to the output. That way we know that it has
    // been rounded down.
    // See SE-265.

    s256 sTmpStr;
    sTmpStr.vSprint("%.10e"_Stz, dFlt);

    // Find the position of the 'e' in the printed string
    i8 iIdx = StrLib::iFind(sTmpStr.azChar(), 'e');

    // Don't copy the last digit before the 'e'
    StrLib::vCopyLen(m_caResult, sizeof(m_caResult), sTmpStr.azChar(), iIdx - 1);

    // Now copy the 'e' and everything after it
    StrLib::vConcat(m_caResult, sizeof(m_caResult), sTmpStr.azChar() + iIdx);

  }
  else {
    StrLib::vSprint(m_caResult,
                    sizeof(m_caResult),
                    nullptr,
                    (Stz *) m_caFormat,
                    dFlt);
  }

  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Samples -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* max                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Flt::szMax(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(C_D8_MAX);
}

/*----------------------------------------------------------------------------*/
/* mid                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Flt::szMid(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(73.0);
}

/*----------------------------------------------------------------------------*/
/* min                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Flt::szMin(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(C_D8_MIN);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Flt::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
