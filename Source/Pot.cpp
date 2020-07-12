/*============================================================================*/
/* Pot.cpp -- Working Pot of Buffers                                          */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,Pot,CN_POT)
__aNewFunction(Pot,CN_POT)
__vDeleteFunction(Pot,CN_POT)
__aFooterFunction(Pot)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pot::Pot(void)
: Object(CN_POT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  m_pStreamPix = nullptr;
  
  // m_oHashSet
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pot::~Pot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void Pot::vAddBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  m_oHashSet.vAddBuffer(0,pDataBuffer);
}

/*----------------------------------------------------------------------------*/
/* get current buffer                                                         */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_oHashSet.pBuffer(0);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pCompleted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_oHashSet.pCompletedBuffer(0);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pDangling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  DataBuffer* pDataBuffer = m_oHashSet.pDanglingBuffer(0);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* create new destination rsb (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Rsb* Pot::aNewDstRsb(RowSlab** ppRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  // get rsb buffer for given partition (new a first buffer if ncessary)
  RsbBuffer* pDstRsbBuffer = _pRsbBuffer(pBuffer());
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pStreamPix);
    vAddBuffer(pDstRsbBuffer);
  }
  
  // new a row in rsb buffer (new another rsb buffer if necessary)
  bool bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
  if (! bNewed) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pStreamPix);
    vAddBuffer(pDstRsbBuffer);
    bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
    ASSERT(bNewed);
  }
  
  pDstRsbBuffer->vPartitionNo(0);
  
  Rsb* aDstRsb = pDstRsbBuffer->pRowSlab()->aRsbAdr(0);
  
  * ppRowSlab = pDstRsbBuffer->pRowSlab();
  
  return aDstRsb;
}

/*----------------------------------------------------------------------------*/
/* set stream pix                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vStreamPix(StreamPix* pStreamPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  if (! m_pStreamPix) {
    m_pStreamPix = pStreamPix;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
