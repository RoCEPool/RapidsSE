/*============================================================================*/
/* PdsBufferIdQueue.hpp -- Pds of BufferIds used as a queue                   */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,PdsBufferIdQueue,CN_PDSBUFFERIDQUEUE)
__aNewFunction(PdsBufferIdQueue,CN_PDSBUFFERIDQUEUE)
__vDeleteFunction(PdsBufferIdQueue,CN_PDSBUFFERIDQUEUE)
__aFooterFunction(PdsBufferIdQueue)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PdsBufferIdQueue::PdsBufferIdQueue(void)
    : Object(CN_PDSBUFFERIDQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  m_iSynthKey  = 0;

  m_pPdsOfBufferIds = new Pds_t<PdsItem_BufferId>(nullptr);

  _checkInitialised_(CN_PDSBUFFERIDQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PdsBufferIdQueue::~PdsBufferIdQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  delete m_pPdsOfBufferIds;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* repair the file                                                            */
/*----------------------------------------------------------------------------*/
void PdsBufferIdQueue::vRepair(Object* pObject)
{
  _static_

  PdsBufferIdQueue* pPdsBufferIdQueue = _pPdsBufferIdQueue(pObject);
  Pds* pPds = pPdsBufferIdQueue->m_pPdsOfBufferIds;

  SkipListBlock* pCurrBlock = pPds->pFirstBlock();

  while(pCurrBlock) {
    SkipListBlock* pNextBlock = pPds->pNextSLBlock(pCurrBlock, 0);

    PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

    DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));
    pPds->bDelete((void*)&oBufferIdStruct.iKey);
    AM::vDeleteOperand(pDataBuffer,nullptr);

    pCurrBlock = pNextBlock;
  }

//  RASSERT(pPds->iNumEntries(0) == 0);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* push buffer to head of queue                                               */
/*----------------------------------------------------------------------------*/
void PdsBufferIdQueue::vAddBuffer(DataBuffer* pNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  PdsItem_BufferId oItem;
  oItem.iKey = ++m_iSynthKey;
  oItem.oBufferId = pNewBuffer->oGetBufferId();

  SkipListBlock* pBlock = m_pPdsOfBufferIds->pInsertItem((void *) &oItem);
  RASSERT(pBlock != nullptr);
  *(PdsItem_BufferId*)(pBlock->aItem()) = oItem;

  return;
}

/*----------------------------------------------------------------------------*/
/* return a completed buffer (if any)                                         */
/*----------------------------------------------------------------------------*/
DataBuffer* PdsBufferIdQueue::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  // don't return a buffer if only one exists in the pds
  // since it may not be full. Only the buffer at the end
  // of the pds chain may not be not full.
  // a) [Root-Block]-->[Full-Buffer]-->[Full-Buffer]-->[Non-Full-Buffer]-->NULL
  // b) [Root-Block]-->[Non-Full-Buffer]-->NULL

  if(m_pPdsOfBufferIds->iNumEntries(0) <= 1)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();
  RASSERT(pCurrBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

  DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));

  m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling buffer (if any)                             */
/*----------------------------------------------------------------------------*/
DataBuffer* PdsBufferIdQueue::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) <= 1);

  if(m_pPdsOfBufferIds->iNumEntries(0) == 0)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();
  RASSERT(pCurrBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

  DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));

  m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) == 0);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return tail of chain (most recently added buffer)                          */
/*----------------------------------------------------------------------------*/
DataBuffer* PdsBufferIdQueue::pDataBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  if(m_pPdsOfBufferIds->iNumEntries(0) == 0)
    return nullptr;

  PdsItem_BufferId oKey;
  oKey.iKey = C_INT_MAX;

  // Find the last block less than MAX key
  SkipListBlock* pSearchBlock = m_pPdsOfBufferIds->pFindLessThan((void *) &oKey);

  if(pSearchBlock != nullptr) {
    PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pSearchBlock->aItem();

    DataBuffer* pDataBuffer = _pDataBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));
    return pDataBuffer;
  }

  RASSERT(0);
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool PdsBufferIdQueue::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERIDQUEUE,_FOOTER_);

  return m_pPdsOfBufferIds->iNumEntries(0) == 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
