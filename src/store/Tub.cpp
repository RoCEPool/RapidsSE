/*============================================================================*/
/* Tub.cpp -- Chain of Buffers with dangling support                          */
/*============================================================================*/

#include "BASE.h"

#include "Tub.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tub,CN_TUB)
__aNewFunction(Tub,CN_TUB)
__vDeleteFunction(Tub,CN_TUB)
__aFooterFunction(Tub)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tub::Tub(void)
: Object(CN_TUB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  m_pBufferChain = nullptr;
    
  _checkInitialised_(CN_TUB);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tub::~Tub(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  // assert we don't have any dangling buffers
  RASSERT(! m_pBufferChain);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Tub::vRepair(Object* pObject)
{
  _static_
  
  UNUSED(pObject);
  
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void Tub::vAddBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  // link the old with this
  pDataBuffer->vNextLinkedBuffer(m_pBufferChain);

  if (m_pBufferChain)
    m_pBufferChain->vPrevLinkedBuffer(pDataBuffer);

  m_pBufferChain = pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a non-first/full buffer (if any)                                    */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  DataBuffer* pCompletedBuffer = nullptr;
  
  DataBuffer* pDataBuffer = (DataBuffer*)
                              (m_pBufferChain
                                 ? m_pBufferChain->pNextLinkedBuffer()
                                 : nullptr);
  while (pDataBuffer) {
    pCompletedBuffer = pDataBuffer;
    pDataBuffer = (DataBuffer*)pDataBuffer->pNextLinkedBuffer();
  }
  
  if (pCompletedBuffer) {
    DataBuffer* pPrevBuffer = (DataBuffer*)pCompletedBuffer->pPrevLinkedBuffer();
    pPrevBuffer->vNextLinkedBuffer(nullptr);
  }
  
  return pCompletedBuffer;
}

/*----------------------------------------------------------------------------*/
/* return any first/final dangling buffer                                     */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_pBufferChain;
  
  m_pBufferChain = nullptr;
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool Tub::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  bool bEmpty = m_pBufferChain ? false : true;
    
  return bEmpty;
}

/*----------------------------------------------------------------------------*/
/* return first/working buffer                                                */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  DataBuffer* pBuffer = m_pBufferChain;

  return pBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
