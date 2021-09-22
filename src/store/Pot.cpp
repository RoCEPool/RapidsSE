/*============================================================================*/
/* Pot.cpp -- Working Pot of Buffers                                          */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "Pot.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Pot,CN_POT)
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
  
  m_pRowPix = nullptr;
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PTNSINGRID; i_Ptn++)
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
DataBuffer* Pot::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
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
#ifdef WITH_PTNID
Row* Pot::aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  //ASSERT(0 == i_Ptn);

  PtnId tPtnId;

  Row* aDstRow;

  *pbNewBuffer = false;

  bool bWasNewed = false;

  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRowBuffer = _pRowBuffer(m_paBufferChain[i_Ptn]);
  if (! pDstRowBuffer) {

    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    vAddBuffer(i_Ptn,pDstRowBuffer);
    bWasNewed = true;
    //vTrace("Newed: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  }
  else {
    //vTrace("Exist: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  }

  // new a row in row buffer (new another row buffer if necessary)
  aDstRow = pDstRowBuffer->pAllotRow();
  if (! aDstRow) {
    *pbNewBuffer = true;
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    vAddBuffer(i_Ptn,pDstRowBuffer);
    //vTrace("Repld: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
    aDstRow = pDstRowBuffer->pAllotRow();
    ASSERT(aDstRow);
  }
  tPtnId = i_Ptn;
  pDstRowBuffer->vPtnNo(tPtnId);

  * ppRowBuffer = pDstRowBuffer;


  //vTrace("Allot: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  //if (pDstRowBuffer->nRows() < 2)
  //  vDebugPoint();

  return aDstRow;
}
#else
Row* Pot::aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //ASSERT(0 == i_Ptn);
  
  Row* aDstRow;
  
  *pbNewBuffer = false;
  
  bool bWasNewed = false;
  
  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRowBuffer = _pRowBuffer(m_paBufferChain[i_Ptn]);
  if (! pDstRowBuffer) {
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    vAddBuffer(i_Ptn,pDstRowBuffer);
    bWasNewed = true;
    //vTrace("Newed: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  }
  else {
    //vTrace("Exist: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  }
  
  // new a row in row buffer (new another row buffer if necessary)
  aDstRow = pDstRowBuffer->pAllotRow();
  if (! aDstRow) {
    *pbNewBuffer = true;
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    vAddBuffer(i_Ptn,pDstRowBuffer);
    //vTrace("Repld: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
    aDstRow = pDstRowBuffer->pAllotRow();
    ASSERT(aDstRow);
  }
  
  pDstRowBuffer->vPtnNo(i_Ptn);
    
  * ppRowBuffer = pDstRowBuffer;
  
  
  //vTrace("Allot: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  //if (pDstRowBuffer->nRows() < 2)
  //  vDebugPoint();

  return aDstRow;
}
#endif
/*----------------------------------------------------------------------------*/
/* set stream pix                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vRowPix(RowPix* pRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
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
void Pot::vAddBuffer(i8 i_Ptn, DataBuffer* pAddBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  //vTrace("Pot::vAddBuffer %lld\n"_Stz,i_Ptn);
  PtnId tPtnId;
  tPtnId = i_Ptn;
  pAddBuffer->vPtnNo(tPtnId);
  
  // link the old with this
  pAddBuffer->vNextLinkedBuffer(m_paBufferChain[i_Ptn]);
  
  
  if (m_paBufferChain[i_Ptn])
    m_paBufferChain[i_Ptn]->vPrevLinkedBuffer(pAddBuffer);
  
  m_paBufferChain[i_Ptn] = pAddBuffer;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
