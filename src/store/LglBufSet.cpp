/*============================================================================*/
/* LglBufSet.cpp -- Set of Partition Hashed Buffers with dangling support     */
/*============================================================================*/

#include "BASE.h"

#include "LglBufSet.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,LglBufSet,CN_LGLBUFSET)
__aNewFunction(LglBufSet,CN_LGLBUFSET)
__vDeleteFunction(LglBufSet,CN_LGLBUFSET)
__aFooterFunction(LglBufSet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LglBufSet::LglBufSet(void)
: Object(CN_LGLBUFSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);
  

  for (LglPtn tLglPtn = 0; tLglPtn < C_MAX_LGLPTNS; tLglPtn++)
    m_paBufferChain[tLglPtn] = nullptr;
    
  _checkInitialised_(CN_LGLBUFSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LglBufSet::~LglBufSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

  // Assert we don't have any dangling buffers
  for (LglPtn tLglPtn = 0; tLglPtn < AM::PTNMAP::nPtnsInGrid(); tLglPtn++)
    RASSERT(! m_paBufferChain[tLglPtn]);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void LglBufSet::vAddBuffer(LglPtn tLglPtn, DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

   // link the old with this
   pDataBuffer->vNextLinkedBuffer(m_paBufferChain[tLglPtn]);
   
   if (m_paBufferChain[tLglPtn])
     m_paBufferChain[tLglPtn]->vPrevLinkedBuffer(pDataBuffer);
   
   m_paBufferChain[tLglPtn] = pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full crwBuffer for given partition (if any)                       */
/*----------------------------------------------------------------------------*/
DataBuffer* LglBufSet::pBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

  DataBuffer* pBuffer = m_paBufferChain[tLglPtn];

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a full crwBuffer for given partition (if any)                       */
/*----------------------------------------------------------------------------*/
DataBuffer* LglBufSet::pCompletedBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

  DataBuffer* pCompletedBuffer = nullptr;
  
  DataBuffer* pDataBuffer = (DataBuffer*)
                              (m_paBufferChain[tLglPtn]
                                 ? m_paBufferChain[tLglPtn]->pNextLinkedBuffer()
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
/* return any final dangling buffer                                           */
/*----------------------------------------------------------------------------*/
DataBuffer* LglBufSet::pDanglingBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_paBufferChain[tLglPtn];
  
  m_paBufferChain[tLglPtn] = nullptr;
  
  return pDataBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
