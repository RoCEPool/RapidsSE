/*============================================================================*/
/* SN_Bcd.cpp - Bcd scan helper                                               */
/*============================================================================*/

#include "BASE.h"

#include "SN_Bcd.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SN_Bcd::SN_Bcd(void)
: Scan(CN_SNBCD)
{
  _precondition_(_MAGIC_,CN_SNBCD,_FOOTER_);
    
  _checkInitialised_(CN_SNBCD);
  
  _postcondition_(_MAGIC_,CN_SNBCD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SN_Bcd::~SN_Bcd(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SNBCD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan                                                                       */
/*----------------------------------------------------------------------------*/
i8 SN_Bcd::i(char** aacCrs, i8 iLength, i8 iScale, Format* pFmt)
{
  _precondition_(_MAGIC_,CN_SNBCD);
  UNUSED(pFmt);
  
  if ( ! StrLib::bExtractDecimal(aacCrs,m_wDtm.m_wBcd.T.m_iAsI8,
                                 iLength,iScale) )
    THROW_USER("StrLib::bExtractDecimal"_Stz);
  
  return m_wDtm;
}

/*----------------------------------------------------------------------------*/
/* scan                                                                       */
/*----------------------------------------------------------------------------*/
i8 SN_Bcd::i(char** aacCrs, Format* pFmt)
{
  _precondition_(_MAGIC_,CN_SNBCD);
  UNUSED(pFmt);

  Stz* szStr = (Stz*)(*aacCrs);
  i8 nLen = szStr->nLength();
  ASSERT(szStr->nLength() > 0);

  i8 iSign = (szStr->azChar()[0] == '-' || szStr->azChar()[0] == '+') ? 1 : 0;

  i8 iPointPos = StrLib::iFind(szStr->azChar(), '.');
  ASSERT(iPointPos >= 0);

  i8 iScale = nLen - iPointPos - 1;
  i8 iPrecision = nLen - iSign - 1;

  i8 iMantissa;

  bool bResult = StrLib::bExtractDecimal(aacCrs, iMantissa, iPrecision + iScale, iScale);
  ASSERT(bResult);

  m_wDtm.m_wBcd.T.m_iMantissa  = iMantissa;
  m_wDtm.m_wBcd.T.m_iScale = iScale;
  m_wDtm.m_wBcd.T.m_iSpare = 0;

  return m_wDtm;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SN_Bcd::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SNBCD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
