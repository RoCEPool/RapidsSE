/*============================================================================*/
/* Pot.cpp -- Working Pot of Buffers                                          */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"
#include "LglPtnMap.hpp"
#include "Pot.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Pot,CN_POT)
__aNewFunction(Pot,CN_POT)
__vDeleteFunction(Pot,CN_POT)
__aFooterFunction(Pot)

/*----------------------------------------------------------------------------*/
/*-- Externs -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
extern "C" void* aExtern_PotAllotRowPds(void* pPot, i8 iKeyHash, char** aazStrArea, i8* piErrorCode) {
  return Pot::aAllotRowPds((Pot*)pPot, iKeyHash, (char**)aazStrArea, piErrorCode);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_PotRowStrSize(void* pPot, i8 iKeyHash, i8 nRowStrSize) {
  Pot::vRowStrSize((Pot*)pPot, iKeyHash, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_PotRowPix(void* pPot, void* pRowPix) {
  Pot::vSetRowPix((Pot*)pPot, (RowPix*)pRowPix);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vPot_RowStrSize(void* pPot, i8 iKeyHash, i8 nRowStrSize) {
  Pot::vRowStrSize((Pot*)pPot, iKeyHash, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pot::Pot(void)
: Object(CN_POT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  m_pRowPix = nullptr;

  m_iSynthKey = 0;
  m_pPdsOfBuffers = new Pds_t<PotPdsItem>(STORE::iPotPdsItemKeyCompare);
  
  _checkInitialised_(CN_LGLBUFSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pot::~Pot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  delete m_pRowPix;
  delete m_pPdsOfBuffers;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
void* Pot::aAllotRowPds(Pot* pPot, i8 iKeyHash, char** aazStrArea, i8* piErrorCode)
{
  _static_

  try {
    Row* aDstRow;

    LglPtnMap* pPtnMap = AM::pLglPtnMap();
    //i8 iKeyHsh = HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));
    LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(iKeyHash);

    RowPix* pRowPix = pPot->m_pRowPix;

    // get pds row buffer for given partition (new a first buffer if ncessary)
    Pds* pPdsRowBuffer = _pPds(pPot->pBuffer(tLglPtn));
    if (! pPdsRowBuffer) {
      pPdsRowBuffer = new Pds(pRowPix);
      pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
    }

    // new a row in pds row buffer (new another pds row buffer if necessary)
    aDstRow = pPdsRowBuffer->pAllotRowFull(aazStrArea);
    if (! aDstRow) {
      pPdsRowBuffer = new Pds(pRowPix);
      pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
      aDstRow = pPdsRowBuffer->pAllotRowFull(aazStrArea);
      RASSERT(aDstRow);
    }

    pPdsRowBuffer->vLglPtn(tLglPtn);

    return aDstRow->aCellArray();;

  }
  catch (LowMemory xLowMemory) {
    *piErrorCode = EC_LOW_MEMORY;
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* set row string                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vRowStrSize(Pot* pPot, i8 iKeyHsh, i8 nRowStrSize)
{
  _static_

  RASSERT(CN_POT == pPot->eGetClassNo());

  LglPtnMap* pPtnMap = AM::pLglPtnMap();
  //i8 iKeyHsh = HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));
  LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(iKeyHsh);

  RowBuffer* pRowBuffer = _pRowBuffer(pPot->pBuffer(tLglPtn));

  pRowBuffer->vTakeHead(nRowStrSize);

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
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void Pot::vSetRowPix(Pot* pPot, RowPix* pRowPix)
{
  _static_

  RASSERT(CN_POT == pPot->eGetClassNo());

  pPot->vRowPix(pRowPix);

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

  PotPdsItem oSearchKey;
  oSearchKey.iLglPtn = i_Ptn;
  oSearchKey.iSynthKey = C_INT_MAX;

  // Find the last block that is less than LglPtn,Max key
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // If the LglPtn value matches, we found a buffer part of this LglPtn chain
    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    if(pItem != nullptr && pItem->iLglPtn == i_Ptn) {
      RASSERT(BufferId::bIsBufferId(pItem->oBufferId));
      return _pDataBuffer(BufferId::aAddressFromId(pItem->oBufferId));
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  PotPdsItem oSearchKey;
  // We increment the LglPtn number since index 0 is used for the Root Block in the Pds
  oSearchKey.iLglPtn = i_Ptn;
  oSearchKey.iSynthKey = C_INT_MIN;

  // Get the first block that is greater than LglPtn,Min
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindGreaterThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // If this block doesn't match our LglPtn key, return nullptr
    if(pItem != nullptr && pItem->iLglPtn != i_Ptn) {
      return nullptr;
    }

    // Now figure out whether this is a completed buffer or not by checking the next buffer
    // in the chain
    SkipListBlock* pNextBlock = m_pPdsOfBuffers->pNextSLBlock(pHead, 0);
    if(pNextBlock != nullptr) {
      // Next block exists, check if the key is the same as this LglPtn
      PotPdsItem* pNextItem = (PotPdsItem*) pNextBlock->aItem();
      RASSERT(pNextItem != nullptr);

      if(pNextItem->iLglPtn == i_Ptn) {
        // Next block exists for this LglPtn chain
        // Which means our current buffer is a completed buffer

        // Extract the buffer id
        BufferId oBufferId = pItem->oBufferId;
        RASSERT(BufferId::bIsBufferId(oBufferId));

        // Remove this block from the pds
        bool bResult = m_pPdsOfBuffers->bDelete((void*)pItem);
        RASSERT(bResult);

        // Get the address from the hashmap
        return _pDataBuffer(BufferId::aAddressFromId(oBufferId));
      }
    }
    // Next block does not exist or does not match the LglPtn, hence our current buffer is incomplete
  }
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  PotPdsItem oSearchKey;
  oSearchKey.iLglPtn = i_Ptn;
  oSearchKey.iSynthKey = C_INT_MAX;
  // Find the buffer part of this LglPtn at the front of the queue.
  // There may be more than one.
  // Pop the head and return it.
  // Note: This function is slightly different than the implementation in
  // LglBufSet as there can be more than one dangling buffer in the queue

  // Find last buffer less than the Key,MAX (i.e front of queue)
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // Get the current head block of this LglPtn and extract the values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    // if the LglPtn matches.
    if(pItem != nullptr && pItem->iLglPtn == i_Ptn) {
      BufferId oBufferId = pItem->oBufferId;
      RASSERT(BufferId::bIsBufferId(oBufferId));
      bool bResult = m_pPdsOfBuffers->bDelete((void*)pItem);
      RASSERT(bResult);
      return _pDataBuffer(BufferId::aAddressFromId(oBufferId));
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Row* Pot::aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  //ASSERT(0 == i_Ptn);

  LglPtn tLglPtn;

  Row* aDstRow;

  *pbNewBuffer = false;


  // get row buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRowBuffer = _pRowBuffer(pBuffer(i_Ptn));
  if (! pDstRowBuffer) {

    pDstRowBuffer = new RowBuffer;
    pDstRowBuffer->vCopyRowPix(m_pRowPix);
    vAddBuffer(i_Ptn,pDstRowBuffer);
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

  tLglPtn = i_Ptn;
  pDstRowBuffer->vLglPtn(tLglPtn);

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
/* get estimate of size                                                       */
/*----------------------------------------------------------------------------*/
i8 Pot::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  return m_pPdsOfBuffers->iNumEntries(0);
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

  LglPtn tLglPtn;
  tLglPtn = i_Ptn;
  pAddBuffer->vLglPtn(tLglPtn);

  // Append a buffer to this pds
  // Use a synthetic key to ensure the buffers are added in ascending order
  PotPdsItem oItem;
  oItem.iLglPtn = i_Ptn;
  oItem.iSynthKey = m_iSynthKey;
  oItem.oBufferId = pAddBuffer->oGetBufferId();
  m_iSynthKey++;
  RASSERT(BufferId::bIsBufferId(oItem.oBufferId));

  SkipListBlock* pResultBlock = m_pPdsOfBuffers->pInsert((void*) &oItem);
  RASSERT(pResultBlock != nullptr);

  *(PotPdsItem*)(pResultBlock->aItem()) = oItem;
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                                          */
/*----------------------------------------------------------------------------*/
i8 STORE::iPotPdsItemKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  PotPdsItem* pKey_l = (PotPdsItem*) aItem_l;
  PotPdsItem* pKey_r = (PotPdsItem*) aItem_r;

  // do the node names match?
  if (pKey_l->iLglPtn == pKey_r->iLglPtn) {
    if (pKey_l->iSynthKey == pKey_r->iSynthKey) {
      return 0;
    }
    else {
      if (pKey_l->iSynthKey >= pKey_r->iSynthKey)
        return 1;
      else
        return -1;
    }
  } else {
    if (pKey_l->iLglPtn >= pKey_r->iLglPtn)
      return 1;
    else
      return -1;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
