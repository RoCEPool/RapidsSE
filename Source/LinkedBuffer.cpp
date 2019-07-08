/*============================================================================*/
/* LinkedBuffer.cpp -- Linked Buffer                                          */
/*============================================================================*/

#include "MetaData.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LinkedBuffer* MetaData::_pLinkedBuffer(void* aAddress)
{
  LinkedBuffer* pLinkedBuffer = reinterpret_cast<LinkedBuffer*>(aAddress);
  
  return pLinkedBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LinkedBuffer::LinkedBuffer(eCN eClassNo)
: AlignedObject(eClassNo,sizeof(LinkedBuffer))
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  
  m_pHigherBuffer = nullptr;
  m_pLowerBuffer  = nullptr;
  
  MemLib::vZero(m__Padding,sizeof(m__Padding),eClassNo);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LinkedBuffer::~LinkedBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get higher buffer                                                          */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vHigherBuffer(LinkedBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get lower buffer                                                           */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pLowerBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pLowerBuffer);
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLowerBuffer(LinkedBuffer* pLowerBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pLowerBuffer = pLowerBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vNextBuffer(LinkedBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vPrevBuffer(LinkedBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
