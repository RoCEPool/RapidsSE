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
//NamedBuffer::NamedBuffer(void)
//: RowBuffer(CN_NAMEDBUFFER)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_NAMEDBUFFER);
//
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_NAMEDBUFFER);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
NamedBuffer::NamedBuffer(StreamDef* pStreamDef, i8 iLogCtl)
: RowBuffer(CN_NAMEDBUFFER,pStreamDef,iLogCtl)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_NAMEDBUFFER);

  vBufferNm(pStreamDef->szName());
    
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_NAMEDBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
NamedBuffer::~NamedBuffer(void) noexcept(false)
{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_NAMEDBUFFER);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
