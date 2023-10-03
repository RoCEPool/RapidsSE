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

  m_iSynthKey = 0;
  m_pPdsOfBuffers = new Pds_t<LglBufSetItem>(STORE::iLglBufSetKeyCompare);

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
  RASSERT(m_pPdsOfBuffers->iNumEntries(0) == 0);
  delete m_pPdsOfBuffers;
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

  // Append a buffer to this pds
  // Use a synthetic key to ensure the buffers are added in ascending order
  LglBufSetItem oItem;

  oItem.iLglPtn = tLglPtn.iAsI8() + 1;
  oItem.iSynthKey = m_iSynthKey++;
  oItem.oBufferId = pDataBuffer->oGetBufferId();

  RASSERT(oItem.oBufferId.m_iAsI8 != 0);
  RASSERT(oItem.oBufferId.m_bIdFlag == 1);

  SkipListBlock* pResultBlock = m_pPdsOfBuffers->pInsert((void*) &oItem);
  RASSERT(pResultBlock != nullptr);

  *(LglBufSetItem*)(pResultBlock->aItem()) = oItem;
}

/*----------------------------------------------------------------------------*/
/* return a DataBuffer for given partition from head of chain (non-completed buffer) (if any)*/
/*----------------------------------------------------------------------------*/
DataBuffer* LglBufSet::pBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

  LglBufSetItem oSearchKey;
  oSearchKey.iLglPtn = tLglPtn.iAsI8() + 1;
  oSearchKey.iSynthKey = C_INT_MAX;

  // Find the last block that is less than LglPtn,Max key
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    LglBufSetItem* pItem = (LglBufSetItem*) pHead->aItem();

    // If the LglPtn value matches, we found a buffer part of this LglPtn chain
    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    if(pItem != nullptr && pItem->iLglPtn == (tLglPtn.iAsI8() + 1)) {
      RASSERT(pItem->oBufferId.m_bIdFlag == 1);
      return _pDataBuffer(BufferId::aAddressFromId(pItem->oBufferId));
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return a full crwBuffer for given partition (if any)                       */
/*----------------------------------------------------------------------------*/
DataBuffer* LglBufSet::pCompletedBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

  // don't return a buffer if only one exists in the pds for this LglPtn
  // since it may not be full. Only the buffer at the end
  // of the pds chain may not be not full.
  // Cases:
  // a) [Root-Block]-->[Full-Buffer]-->[Full-Buffer]-->[Non-Full-Buffer]-->NULL
  // b) [Root-Block]-->[Non-Full-Buffer]-->NULL
  // c) [Root-Block]-->NULL

  // We increment the LglPtn number since index 0 is used for the Root Block in the Pds
  i8 iLglPtnKey = tLglPtn.iAsI8() + 1;

  LglBufSetItem oSearchKey;
  oSearchKey.iLglPtn = iLglPtnKey;
  oSearchKey.iSynthKey = C_INT_MIN;

  // Get the first block that is greater than LglPtn,Min
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindGreaterThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    LglBufSetItem* pItem = (LglBufSetItem*) pHead->aItem();

    // If this block doesn't match our LglPtn key, return nullptr
    if(pItem != nullptr && pItem->iLglPtn != iLglPtnKey) {
      return nullptr;
    }

    // Now figure out whether this is a completed buffer or not by checking the next buffer
    // in the chain
    SkipListBlock* pNextBlock = m_pPdsOfBuffers->pNextSLBlock(pHead, 0);
    if(pNextBlock != nullptr) {
      // Next block exists, check if the key is the same as this LglPtn
      LglBufSetItem* pNextItem = (LglBufSetItem*) pNextBlock->aItem();
      RASSERT(pNextItem != nullptr);

      if(pNextItem->iLglPtn == iLglPtnKey) {
        // Next block exists for this LglPtn chain
        // Which means our current buffer is a completed buffer

        // Extract the buffer id
        BufferId oBufferId = pItem->oBufferId;
        RASSERT(oBufferId.m_bIdFlag == 1);

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
/* return any final dangling buffer                                           */
/*----------------------------------------------------------------------------*/
DataBuffer* LglBufSet::pDanglingBuffer(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LGLBUFSET,_FOOTER_);

  LglBufSetItem oSearchKey;
  oSearchKey.iLglPtn = tLglPtn.iAsI8() + 1;
  oSearchKey.iSynthKey = C_INT_MIN;

  // Get the first block that is greater than LglPtn,Min
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindGreaterThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // Get the current head block of this LglPtn and extract the values
    LglBufSetItem* pItem = (LglBufSetItem*) pHead->aItem();

    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    // If the LglPtn matches.
    if(pItem != nullptr && pItem->iLglPtn == (tLglPtn.iAsI8() + 1)) {
      BufferId oBufferId = pItem->oBufferId;
      RASSERT(oBufferId.m_bIdFlag == 1);
      bool bResult = m_pPdsOfBuffers->bDelete((void*)pItem);
      RASSERT(bResult);
      return _pDataBuffer(BufferId::aAddressFromId(oBufferId));
    }
  }
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                          */
/*----------------------------------------------------------------------------*/
i8 STORE::iLglBufSetKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  LglBufSetItem* pKey_l = (LglBufSetItem*) aItem_l;
  LglBufSetItem* pKey_r = (LglBufSetItem*) aItem_r;

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
