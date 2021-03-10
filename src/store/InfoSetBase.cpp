/*============================================================================*/
/* InfoSetBase.cpp -- Information Set Base                                    */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "InfoSetBase.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSetBase* STORE::_pInfoSetBase(void* aAddr)
{
  InfoSetBase* pInfoSetBase = reinterpret_cast<InfoSetBase*>(aAddr);
  
  return pInfoSetBase;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSetBase::InfoSetBase(eCN eClassNo)
: RowBuffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_sInfoSetNm = C_NO_NAME;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSetBase::InfoSetBase(eCN eClassNo, StreamDef* pStreamDef)
: RowBuffer(eClassNo,pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_sInfoSetNm = pStreamDef->szName();
    
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSetBase::~InfoSetBase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get name of info set                                                       */
/*----------------------------------------------------------------------------*/
Stz* InfoSetBase::szInfoSetNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_sInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* set name of info set                                                       */
/*----------------------------------------------------------------------------*/
void InfoSetBase::vInfoSetNm(Stz* szInfoSetNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_sInfoSetNm = szInfoSetNm;
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/
i8 InfoSetBase::iToOffset(Stz* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i4 iOffset = static_cast<i4>(nDifference(aItem,this));
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(InfoSetBase);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
