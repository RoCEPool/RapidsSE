/*============================================================================*/
/* spBCD.cpp - Bcd sprint helper                                              */
/*============================================================================*/

#include "Sprinting.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
spBCD::spBCD(void)
: Sprint(CN_SPBCD)
{
  _precondition_(_MAGIC_,CN_SPBCD,_FOOTER_);
  
  // set default Bcd format
  vFormat(F_BCD_frac);
  
  _checkInitialised_(CN_SPBCD);
  
  _postcondition_(_MAGIC_,CN_SPBCD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
spBCD::~spBCD(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPBCD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Bcd and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* spBCD::sz(i8 iBcd)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return sz(iBcd,0);
}

/*----------------------------------------------------------------------------*/
/* sprint Bcd and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* spBCD::sz(i8 iBcd, i8 iScale)
{
  //! refer: https://en.wikipedia.org/wiki/Fixed-point_arithmetic
  
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool bNegative = false;
  
  i8 iPower = 1;
  for (i8 i_Digit = 0; i_Digit < iScale; i_Digit++)
    iPower *= 10;
  
  // 12.34
  i8 iBcd_magn = iBcd / iPower; // 12
  i8 iBcd_frac = iBcd % iPower; // 34
  
  if (iBcd_frac < 0 || iBcd_magn < 0) {
    bNegative = true;
    iBcd_frac = - iBcd_frac;
    iBcd_magn = - iBcd_magn;
  }
  
  if (iScale)
    StrLib::vSprint(m_acResult,
                    (Stz*)F_BCD_frac,
                    bNegative ? "-" : "",
                    iBcd_magn,
                    iScale,
                    iBcd_frac);
  else
    StrLib::vSprint(m_acResult,
                    (Stz*)F_BCD,
                    bNegative ? "-" : "",
                    iBcd_magn);

  return (Stz*) &m_acResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* spBCD::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPBCD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
