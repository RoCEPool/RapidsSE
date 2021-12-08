/*============================================================================*/
/* Totup.cpp - Totup                                                          */
/*============================================================================*/

#include "BASE.h"
#include "Totup.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Totup,CN_TOTUP)
__aNewFunction(Totup,CN_TOTUP)
__vDeleteFunction(Totup,CN_TOTUP)
__aFooterFunction(Totup)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Totup::Totup(void)
: Object(CN_TOTUP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  vReset();
  
  _checkInitialised_(CN_TOTUP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Totup::~Totup(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add qty                                                                    */
/*----------------------------------------------------------------------------*/
void Totup::vAdd(i8 i_Item, i8 iQty)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  m_xaItem[i_Item].m_iSum += iQty;
  m_xaItem[i_Item].m_nCnt ++;
  
  if (iQty < m_xaItem[i_Item].m_iMin)
    m_xaItem[i_Item].m_iMin = iQty;

  if (iQty > m_xaItem[i_Item].m_iMax)
    m_xaItem[i_Item].m_iMax = iQty;
}

/*----------------------------------------------------------------------------*/
/* get an nCnt                                                                */
/*----------------------------------------------------------------------------*/
i8 Totup::nCnt(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  return m_xaItem[i_Item].m_nCnt;
}

/*----------------------------------------------------------------------------*/
/* get an iMax                                                                */
/*----------------------------------------------------------------------------*/
i8 Totup::iMax(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  return m_xaItem[i_Item].m_iMax;
}

/*----------------------------------------------------------------------------*/
/* get an iMin                                                                */
/*----------------------------------------------------------------------------*/
i8 Totup::iMin(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  return m_xaItem[i_Item].m_iMin;
}

/*----------------------------------------------------------------------------*/
/* get an iSum                                                                */
/*----------------------------------------------------------------------------*/
i8 Totup::iSum(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  return m_xaItem[i_Item].m_iSum;
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
void Totup::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOTUP,_FOOTER_);
  
  for (i8 i_Item = 0; i_Item < C_MAX_TOT_ITEMS; i_Item++) {
    
    m_xaItem[i_Item].m_nCnt = 0;
    m_xaItem[i_Item].m_iSum = 0;
    m_xaItem[i_Item].m_iMin = C_I8_MAX;
    m_xaItem[i_Item].m_iMax = C_I8_MIN;

  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
