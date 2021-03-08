/*============================================================================*/
/* WriteBehind.cpp -- Working WriteBehind Buffer                              */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "WriteBehind.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,WriteBehind,CN_WRITEBEHIND)
__aNewFunction(WriteBehind,CN_WRITEBEHIND)
__vDeleteFunction(WriteBehind,CN_WRITEBEHIND)
__aFooterFunction(WriteBehind)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WriteBehind::WriteBehind(void)
: Object(CN_WRITEBEHIND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pRowPix = nullptr;
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PARTITIONS; i_Ptn++)
    m_paBufferChain[i_Ptn] = nullptr;
  
  _checkInitialised_(CN_HASHSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WriteBehind::~WriteBehind(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current buffer                                                         */
/*----------------------------------------------------------------------------*/
DataBuffer* WriteBehind::pBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  return m_paBufferChain[i_Ptn];
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
DataBuffer* WriteBehind::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pCompletedBuffer = nullptr;
  
  DataBuffer* pNextBuffer =
    (DataBuffer*)
      (m_paBufferChain[i_Ptn]
        ? m_paBufferChain[i_Ptn]->pNextLinkedBuffer()
        : nullptr);
  
  while (pNextBuffer) {
    pCompletedBuffer = pNextBuffer;
    pNextBuffer = (DataBuffer*)pNextBuffer->pNextLinkedBuffer();
  }
  
  if (pCompletedBuffer) {
    DataBuffer* pPrevBuffer = (DataBuffer*)pCompletedBuffer->pPrevLinkedBuffer();
    pPrevBuffer->vNextLinkedBuffer(BufferNo::tUNKNOWN());
  }
  
  return pCompletedBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* WriteBehind::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  DataBuffer* pDanglingBuffer = m_paBufferChain[i_Ptn];
  
  m_paBufferChain[i_Ptn] =
    pDanglingBuffer
      ? (DataBuffer*)pDanglingBuffer->pNextLinkedBuffer()
      : nullptr;
  
  if (pDanglingBuffer)
    pDanglingBuffer->vNextLinkedBuffer(BufferNo::tUNKNOWN());
  
  return pDanglingBuffer;
}

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Row* WriteBehind::aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  Row* aDstRow;
  
  *pbNewBuffer = false;
  
  bool bWasNewed = false;
  
  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRowBuffer = _pRowBuffer(m_paBufferChain[i_Ptn]);
  if (! pDstRowBuffer) {
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    pDstRowBuffer->vAssertAux0();
    vAddBuffer(i_Ptn,pDstRowBuffer);
    bWasNewed = true;
    //vTrace("Newed: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  }
  else {
    //vTrace("Exist: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
    pDstRowBuffer->vAssertAux0();
  }
  
  // new a row in row buffer (new another row buffer if necessary)
  aDstRow = pDstRowBuffer->pAllotNewRow();
  if (! aDstRow) {
    *pbNewBuffer = true;
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    pDstRowBuffer->vAssertAux0();
    vAddBuffer(i_Ptn,pDstRowBuffer);
    //vTrace("Repld: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
    aDstRow = pDstRowBuffer->pAllotNewRow();
    ASSERT(aDstRow);
  }
  
  pDstRowBuffer->vPtnNo(i_Ptn);
    
  * ppRowBuffer = pDstRowBuffer;
  
  pDstRowBuffer->vAssertAux0();
  
  //vTrace("Allot: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  //if (pDstRowBuffer->nRows() < 2)
  //  vDebugPoint();

  return aDstRow;
}

/*----------------------------------------------------------------------------*/
/* set stream pix                                                             */
/*----------------------------------------------------------------------------*/
void WriteBehind::vRowPix(RowPix* pRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  //if (! m_pRowPix) {
    m_pRowPix = pRowPix;
  //}
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void WriteBehind::vAddBuffer(i8 i_Ptn, DataBuffer* pAddBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);

  //vTrace("WriteBehind::vAddBuffer %lld\n"_Stz,i_Ptn);
  
  pAddBuffer->vPtnNo(i_Ptn);
  
  // link the old with this
  pAddBuffer->vNextLinkedBuffer(m_paBufferChain[i_Ptn]);
  
  
  if (m_paBufferChain[i_Ptn])
    m_paBufferChain[i_Ptn]->vPrevLinkedBuffer(pAddBuffer);
  
  m_paBufferChain[i_Ptn] = pAddBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
