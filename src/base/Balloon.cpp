/*============================================================================*/
/* Balloon.cpp -- Balloon                                                     */
/*============================================================================*/

#include "BASE.h"

#include "Balloon.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Balloon,CN_BALLOON)
__aNewFunction(Balloon,CN_BALLOON)
__vDeleteFunction(Balloon,CN_BALLOON)
__aFooterFunction(Balloon)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Balloon::Balloon(void)
: Object(CN_BALLOON)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  m_eAssocClass  = CN_NONE;
  
  m_iCurrentSize = 0;
  
  for (i8 i_Magn = 0; i_Magn < C_BALLOON_MAGN; i_Magn++) {
    m_iaWhenMagn_first[i_Magn] = 0;
    m_iaWhenMagn_last[i_Magn]  = 0;
  }
  
  _checkInitialised_(CN_BALLOON);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Balloon::~Balloon(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set class no of associated object                                          */
/*----------------------------------------------------------------------------*/
void Balloon::vAddSize(i8 iBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  i8 iSize = m_iCurrentSize + iBy;
 
  vCurrentSize(iSize);
}

/*----------------------------------------------------------------------------*/
/* get class no of associated object                                          */
/*----------------------------------------------------------------------------*/
eCN Balloon::eAssocClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  return m_eAssocClass;
}

/*----------------------------------------------------------------------------*/
/* set class no of associated object                                          */
/*----------------------------------------------------------------------------*/
void Balloon::vAssocClass(eCN eAssocClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  m_eAssocClass = eAssocClass;
}

/*----------------------------------------------------------------------------*/
/* get current size                                                           */
/*----------------------------------------------------------------------------*/
i8 Balloon::iCurrentSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  return m_iCurrentSize;
}

/*----------------------------------------------------------------------------*/
/* update current size                                                        */
/*----------------------------------------------------------------------------*/
void Balloon::vCurrentSize(i8 iSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
  
  i8 iTsc = Hardware::iReadTsc();
    
  m_iCurrentSize = iSize;
    
  i8 i_Magn = IntLib::iDecimalMagnitude(m_iCurrentSize);
  
  if (! m_iaWhenMagn_first[i_Magn])
    m_iaWhenMagn_first[i_Magn] = iTsc;
    
  if (! m_iaWhenMagn_last[i_Magn])
    m_iaWhenMagn_first[i_Magn] = iTsc;
}

/*----------------------------------------------------------------------------*/
/* reset for next use sequence                                                */
/*----------------------------------------------------------------------------*/
void Balloon::vNextSequence(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
  
  m_iCurrentSize = 0;
  
  for (i8 i_Magn = 0; i_Magn < C_BALLOON_MAGN; i_Magn++)
    m_iaWhenMagn_last[i_Magn]  = 0;
}

/*----------------------------------------------------------------------------*/
/* get when associated object first became given magnitude                    */
/*----------------------------------------------------------------------------*/
i8 Balloon::iWhenMagn_first(i8 i_Magn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  i8 iWhen = m_iaWhenMagn_first[i_Magn];
  
  return iWhen;
}

/*----------------------------------------------------------------------------*/
/* get when associated object last became given magnitude                     */
/*----------------------------------------------------------------------------*/
i8 Balloon::iWhenMagn_last(i8 i_Magn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BALLOON,_FOOTER_);
    
  i8 iWhen = m_iaWhenMagn_last[i_Magn];
  
  return iWhen;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
