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
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PARTITIONS; i_Ptn++)
    m_paBufferChain[i_Ptn] = nullptr;
  
  _checkInitialised_(CN_HASHSET);
  
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
  
  return m_paBufferChain[i_Ptn];
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pCompletedBuffer = nullptr;
  
  DataBuffer* pNextBuffer =
    (DataBuffer*)
      (m_paBufferChain[i_Ptn]
        ? m_paBufferChain[i_Ptn]->pNextBuffer()
        : nullptr);
  
  while (pNextBuffer) {
    pCompletedBuffer = pNextBuffer;
    pNextBuffer = (DataBuffer*)pNextBuffer->pNextBuffer();
  }
  
  if (pCompletedBuffer) {
    DataBuffer* pPrevBuffer = (DataBuffer*)pCompletedBuffer->pPrevBuffer();
    pPrevBuffer->vNextBuffer(nullptr);
  }
  
  return pCompletedBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pDanglingBuffer = m_paBufferChain[i_Ptn];
  
  m_paBufferChain[i_Ptn] =
    pDanglingBuffer
      ? (DataBuffer*)pDanglingBuffer->pNextBuffer()
      : nullptr;
  
  if (pDanglingBuffer)
    pDanglingBuffer->vNextBuffer(nullptr);
  
  return pDanglingBuffer;
}

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Row* Pot::aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  RowBuffer* pDstRowBuffer;
  Row* aDstRow;
  
  *pbNewBuffer = false;
  
  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRsbBuffer = _pRowBuffer(m_paBufferChain[i_Ptn]);
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RowBuffer;
    pDstRowBuffer = pDstRsbBuffer;
    pDstRowBuffer->vCopyPix(m_pStreamPix);
    vAddBuffer(i_Ptn,pDstRsbBuffer);
  }
  else {
    pDstRowBuffer = pDstRsbBuffer;
  }
  
  // new a row in row buffer (new another row buffer if necessary)
  aDstRow = pDstRowBuffer->aAllotNewRow();
  if (! aDstRow) {
    *pbNewBuffer = true;
    pDstRsbBuffer = new RowBuffer;
    pDstRowBuffer = pDstRsbBuffer;
    pDstRowBuffer->vCopyPix(m_pStreamPix);
    vAddBuffer(i_Ptn,pDstRsbBuffer);
    aDstRow = pDstRsbBuffer->aAllotNewRow();
    ASSERT(aDstRow);
  }
  
  pDstRsbBuffer->vPtnNo(i_Ptn);
    
  * ppRowBuffer = pDstRowBuffer;
  
  return aDstRow;
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
void Pot::vAddBuffer(i8 i_Ptn, DataBuffer* pAddBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  //vTrace("Pot::vAddBuffer %lld\n"_Stz,i_Ptn);
  
  pAddBuffer->vPtnNo(i_Ptn);
  
  // link the old with this
  pAddBuffer->vNextBuffer(m_paBufferChain[i_Ptn]);
  
  
  if (m_paBufferChain[i_Ptn])
    m_paBufferChain[i_Ptn]->vPrevBuffer(pAddBuffer);
  
  m_paBufferChain[i_Ptn] = pAddBuffer;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
