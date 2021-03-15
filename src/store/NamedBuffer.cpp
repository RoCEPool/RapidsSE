/*============================================================================*/
/* NamedBuffer.cpp -- Named Buffer                                            */
/*============================================================================*/

#include "BASE.h"

#include "NamedBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
NamedBuffer* STORE::_pNamedBuffer(void* aAddr)
{
  NamedBuffer* pNamedBuffer = reinterpret_cast<NamedBuffer*>(aAddr);
  
  return pNamedBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
NamedBuffer::NamedBuffer(eCN eClassNo)
: RowBuffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
    
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
NamedBuffer::NamedBuffer(eCN eClassNo, StreamDef* pStreamDef)
: RowBuffer(eClassNo,pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  vBufferNm(pStreamDef->szName());
    
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
NamedBuffer::~NamedBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/
i8 NamedBuffer::iToOffset(Stz* aItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i4 iOffset = static_cast<i4>(nDifference(aItem,this));
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(NamedBuffer);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
