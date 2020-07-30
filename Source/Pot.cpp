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
/* get current buffer                                                         */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pDataBuffer = m_oHashSet.pBuffer(i_Ptn);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pDataBuffer = m_oHashSet.pCompletedBuffer(i_Ptn);
  
  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pDataBuffer = m_oHashSet.pDanglingBuffer(i_Ptn);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* create new destination rsb (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Rsb* Pot::aNewDstRsb(i8 i_Ptn, RowSlab** ppRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  RowSlab* pDstRowSlab;
  
  // get rsb buffer for given partition (new a first buffer if ncessary)
  RsbBuffer* pDstRsbBuffer = _pRsbBuffer(pBuffer(i_Ptn));
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRowSlab = pDstRsbBuffer->pRowSlab();
    pDstRowSlab->vCopyPix(m_pStreamPix);
    vAddBuffer(i_Ptn,pDstRsbBuffer);
  }
  else {
    pDstRowSlab = pDstRsbBuffer->pRowSlab();
  }
  
  // new a row in rsb buffer (new another rsb buffer if necessary)
  bool bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
  if (! bNewed) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRowSlab = pDstRsbBuffer->pRowSlab();
    pDstRowSlab->vCopyPix(m_pStreamPix);
    vAddBuffer(i_Ptn,pDstRsbBuffer);
    bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
    ASSERT(bNewed);
  }
  
  pDstRsbBuffer->vPartitionNo(i_Ptn);
  
  i8 iRow = pDstRowSlab->nRows() - 1;
  Rsb* aDstRsb = pDstRowSlab->aRsbAdr(iRow);
  
  * ppRowSlab = pDstRowSlab;
  
  return aDstRsb;
}

/*----------------------------------------------------------------------------*/
/* set stream pix                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vStreamPix(StreamPix* pStreamPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //if (! m_pStreamPix) {
    m_pStreamPix = pStreamPix;
  //}
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void Pot::vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  m_oHashSet.vAddBuffer(i_Ptn,pDataBuffer);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
