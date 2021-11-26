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
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
void* aPot_AllotRow(void* pPot, i8 iMixIn, char** aazStrArea) {
  return Pot::aAllotRow((Pot*)pPot, iMixIn, (char**)aazStrArea);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pot::Pot(void)
: Object(CN_POT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  m_pRowPix = nullptr;
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_LGLPTNS; i_Ptn++)
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
void* Pot::aAllotRow(Pot* pPot, i8 iMixIn, char** aazStrArea)
{
  _static_;

  PtnId tPtnId;

  Row* aDstRow;

  i8 iHash = HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));
  i8 iBktId = AM::BKTMAP::iHashToBktId(iHash);
  i8 i_GridPtn = AM::BKTMAP::tBktIdToPtnId(iBktId).iAsI8();

  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRowBuffer = _pRowBuffer(pPot->m_paBufferChain[i_GridPtn]);
  if (! pDstRowBuffer) {

    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(pPot->m_pRowPix);
    pPot->vAddBuffer(i_GridPtn,pDstRowBuffer);
  }

  // new a row in row buffer (new another row buffer if necessary)
  aDstRow = pDstRowBuffer->pAllotRow();
  if (! aDstRow) {
    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(pPot->m_pRowPix);
    pPot->vAddBuffer(i_GridPtn,pDstRowBuffer);
    //vTrace("Repld: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
    aDstRow = pDstRowBuffer->pAllotRow();
    ASSERT(aDstRow);
  }

  tPtnId = i_GridPtn;
  pDstRowBuffer->vPtnId(tPtnId);

  *aazStrArea = pDstRowBuffer->azDepositString_begin();

  return aDstRow->aCellArray();;
}

/*----------------------------------------------------------------------------*/
/* repair the pot                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vRepair(Object* pObject)
{
  _static_
  Pot* pPot = _pPot(pObject);

  delete pPot;
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
  pDstRowBuffer->vPtnId(tPtnId);

  * ppRowBuffer = pDstRowBuffer;

  //vTrace("Allot: %p %lld rows\n"_Stz,pDstRowBuffer,pDstRowBuffer->nRows());
  //if (pDstRowBuffer->nRows() < 2)
  //  vDebugPoint();

  return aDstRow;
}
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
  pAddBuffer->vPtnId(tPtnId);

  // link the old with this
  pAddBuffer->vNextLinkedBuffer(m_paBufferChain[i_Ptn]);
  
  
  if (m_paBufferChain[i_Ptn])
    m_paBufferChain[i_Ptn]->vPrevLinkedBuffer(pAddBuffer);
  
  m_paBufferChain[i_Ptn] = pAddBuffer;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
