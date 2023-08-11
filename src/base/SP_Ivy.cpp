/*============================================================================*/
/* SP_Ivy.cpp - Ivy sprint helper                                             */
/*============================================================================*/

#include "BASE.h"

#include "SP_Ivy.hpp"
#include "eIV.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Ivy::SP_Ivy(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  _checkInitialised_(CN_SPSMALL);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Ivy::~SP_Ivy(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Ivy and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivy::sz(i8 iIvy)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(iIvy,0);
}

/*----------------------------------------------------------------------------*/
/* sprint Ivy and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivy::sz(i8 iIvy, i8 iScale)
{
  _precondition_(_MAGIC_,_FOOTER_);
  UNUSED(iScale);
  
  Ivl wIvy{IV_YEAR};
  wIvy = iIvy;

  u8 uYears;
  u8 uMonths;

  switch(wIvy.T.m_xIvl.m_eType) {
    case IV_YEAR:
      uYears = wIvy.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12;

      if (wIvy.bIsNeg() && uYears)
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld' %s",
                        uYears,
                        aIV_field[wIvy.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld' %s",
                        uYears,
                        aIV_field[wIvy.T.m_xIvl.m_eType]);
      break;

    case IV_MONTH:
      uMonths = wIvy.T.m_xIvl.u.m_xIvlYM.m_uMonths;

      if (wIvy.bIsNeg() && uMonths)
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld' %s",
                        uMonths,
                        aIV_field[wIvy.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld' %s",
                        uMonths,
                        aIV_field[wIvy.T.m_xIvl.m_eType]);
      break;

    case IV_YEAR_TO_MONTH:
      uYears = wIvy.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12;
      uMonths = wIvy.T.m_xIvl.u.m_xIvlYM.m_uMonths % 12;

      if (wIvy.bIsNeg() && (uYears || uMonths))
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL -'%lld-%lld' YEAR TO MONTH",
                        uYears,
                        uMonths,
                        aIV_field[wIvy.T.m_xIvl.m_eType]);
      else
        StrLib::vSprint(m_caResult,
                        sizeof(m_caResult),
                        (Stz *) "INTERVAL '%lld-%lld' YEAR TO MONTH",
                        uYears,
                        uMonths,
                        aIV_field[wIvy.T.m_xIvl.m_eType]);
      break;

    default:
      NEVER_REACH;
  }

  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Samples -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* max                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivy::szMax(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(0x1UL << 53,1023);
}

/*----------------------------------------------------------------------------*/
/* mid                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivy::szMid(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(73,73);
}

/*----------------------------------------------------------------------------*/
/* min                                                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Ivy::szMin(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(0,0);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Ivy::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
