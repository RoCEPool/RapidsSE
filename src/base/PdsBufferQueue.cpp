/*============================================================================*/
/* PdsBufferQueue.hpp -- Pds of BufferIds used as a queue                   */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,PdsBufferQueue,CN_PDSBUFFERQUEUE)
__aNewFunction(PdsBufferQueue,CN_PDSBUFFERQUEUE)
__vDeleteFunction(PdsBufferQueue,CN_PDSBUFFERQUEUE)
__aFooterFunction(PdsBufferQueue)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PdsBufferQueue::PdsBufferQueue(void)
    : Object(CN_PDSBUFFERQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  m_iSynthKey  = 0;
//  m_pWorkingBuffer = nullptr;
  m_pPdsOfBufferAddrs = new Pds_t<PdsItem_Buffer>(nullptr);

  _checkInitialised_(CN_PDSBUFFERQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PdsBufferQueue::~PdsBufferQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  delete m_pPdsOfBufferAddrs;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* repair the file                                                            */
/*----------------------------------------------------------------------------*/
void PdsBufferQueue::vRepair(Object* pObject)
{
  _static_

  PdsBufferQueue* pPdsBufferQueue = _pPdsBufferQueue(pObject);
  Pds* pPds = pPdsBufferQueue->m_pPdsOfBufferAddrs;

  SkipListBlock* pCurrBlock = pPds->pFirstBlock();

  while(pCurrBlock) {
    SkipListBlock* pNextBlock = pPds->pNextSLBlock(pCurrBlock, 0);

    PdsItem_Buffer oPdsStruct = *(PdsItem_Buffer*) pCurrBlock->aItem();

    DataBuffer* pDataBuffer = oPdsStruct.pDataBuffer;
    pPds->bDelete((void*)&oPdsStruct.iKey);
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
void PdsBufferQueue::vAddBuffer(DataBuffer* pNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  PdsItem_Buffer oItem;
  oItem.iKey = m_iSynthKey;
  oItem.pDataBuffer = pNewBuffer;

  SkipListBlock* pBlock = m_pPdsOfBufferAddrs->pAppend((void*) &oItem);
  RASSERT(pBlock != nullptr);
  *(PdsItem_Buffer*)(pBlock->aItem()) = oItem;

  m_iSynthKey++;

//  m_pWorkingBuffer = pNewBuffer;

  return;
}

/*----------------------------------------------------------------------------*/
/* return a completed buffer (if any)                                         */
/*----------------------------------------------------------------------------*/
DataBuffer* PdsBufferQueue::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  // don't return a buffer if only one exists in the pds
  // since it may not be full. Only the buffer at the end
  // of the pds chain may not be not full.
  // a) [Root-Block]-->[Full-Buffer]-->[Full-Buffer]-->[Non-Full-Buffer]-->NULL
  // b) [Root-Block]-->[Non-Full-Buffer]-->NULL

  if(m_pPdsOfBufferAddrs->iNumEntries(0) <= 1)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferAddrs->pFirstBlock();
  ASSERT(pCurrBlock != nullptr);

  PdsItem_Buffer oPdsStruct = *(PdsItem_Buffer*) pCurrBlock->aItem();

  DataBuffer* pDataBuffer = oPdsStruct.pDataBuffer;

//  if(pDataBuffer == m_pWorkingBuffer)
//    m_pWorkingBuffer = nullptr;

  m_pPdsOfBufferAddrs->bDelete((void*)&oPdsStruct.iKey);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return last remaining dangling buffer (if any)                             */
/*----------------------------------------------------------------------------*/
DataBuffer* PdsBufferQueue::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  ASSERT(m_pPdsOfBufferAddrs->iNumEntries(0) <= 1);

  if(m_pPdsOfBufferAddrs->iNumEntries(0) == 0)
    return nullptr;

  SkipListBlock* pCurrBlock = m_pPdsOfBufferAddrs->pFirstBlock();
  ASSERT(pCurrBlock != nullptr);

  PdsItem_Buffer oPdsStruct = *(PdsItem_Buffer*) pCurrBlock->aItem();

  DataBuffer* pDataBuffer = oPdsStruct.pDataBuffer;

  m_pPdsOfBufferAddrs->bDelete((void*)&oPdsStruct.iKey);

//  ASSERT(m_pWorkingBuffer == pDataBuffer);
//  m_pWorkingBuffer = nullptr;

  ASSERT(m_pPdsOfBufferAddrs->iNumEntries(0) == 0);

  return pDataBuffer;
}

/*----------------------------------------------------------------------------*/
/* return tail of chain (most recently added buffer)                          */
/*----------------------------------------------------------------------------*/
DataBuffer* PdsBufferQueue::pDataBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  if(m_pPdsOfBufferAddrs->iNumEntries(0) == 0) {
//    ASSERT(m_pWorkingBuffer == nullptr);
    return nullptr;
  }

  PdsItem_Buffer oKey;
  oKey.iKey = C_INT_MAX;
  // Find the last block less than MAX key
  SkipListBlock* pSearchBlock = m_pPdsOfBufferAddrs->pFindLessThan((void *) &oKey);
  if(pSearchBlock != nullptr) {
    PdsItem_Buffer oPdsStruct = *(PdsItem_Buffer*) pSearchBlock->aItem();
    DataBuffer* pDataBuffer = oPdsStruct.pDataBuffer;
    return pDataBuffer;
  }
  RASSERT(0);
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Print Pds chain                                                            */
/*----------------------------------------------------------------------------*/
void PdsBufferQueue::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  vTrace("Format: SynthKey(BufferId)(DataBufferAddr)-->\n"_Stz);

  vTrace("In Use List\n"_Stz);
  vTrace("=================================================="
         "===================================================\n"_Stz);
  vTrace("| %5s | %6s | %s\n"_Stz, "Level", "# Ents", "Items");

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);


  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    SkipListBlock* pCursor = (SkipListBlock*) m_pPdsOfBufferAddrs->pGetAddrFromOffset(m_pPdsOfBufferAddrs->m_uInUseRootOffset);

    vTrace("| %5lld | %6lld | "_Stz, i_Level, m_pPdsOfBufferAddrs->m_iNumEntries[i_Level]);

    for(i8 iNumEntries = 0; iNumEntries < m_pPdsOfBufferAddrs->m_iNumEntries[i_Level] + 1; iNumEntries++) {
      PdsItem_Buffer oItem;
      oItem = *(PdsItem_Buffer*)(pCursor->aItem());
      vTrace("%lld(%p)(%p)"_Stz, oItem.iKey, oItem.pDataBuffer);

      pCursor = m_pPdsOfBufferAddrs->pNextSLBlock(pCursor, i_Level);
      if(pCursor == nullptr) {
        break;
      }
    }
    vTrace("END\n"_Stz);
    vTrace("--------------------------------------------------"
           "--------------------------------------------------\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool PdsBufferQueue::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSBUFFERQUEUE,_FOOTER_);

  return m_pPdsOfBufferAddrs->iNumEntries(0) == 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
