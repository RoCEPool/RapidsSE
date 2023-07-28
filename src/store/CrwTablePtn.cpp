/*============================================================================*/
/* CrwTablePtn.cpp -- Crw Table Partition                                     */
/*============================================================================*/

#include "BASE.h"

#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "CrwTablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CrwTablePtn,CN_CRWTABLEPTN)
__aNewFunction(CrwTablePtn,CN_CRWTABLEPTN)
__vDeleteFunction(CrwTablePtn,CN_CRWTABLEPTN)
__aFooterFunction(CrwTablePtn)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CrwTablePtn::CrwTablePtn(Table* pTable, LglPtn tLglPtn)
    : Object(CN_CRWTABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  m_pTable    = pTable;
  m_tLglPtn    = tLglPtn;

  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;

  m_pPdsOfBufferIds = new Pds_t<xBufferId>();
  //m_pIndexTreeVec  = new IndexTreeVec;

  _checkInitialised_(CN_CRWTABLEPTN);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwTablePtn::~CrwTablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  // delete the index data vector
  //delete m_pIndexTreeVec;
  delete m_pPdsOfBufferIds;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* append a crw buffer into the CrwTablePtn                                      */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vAppendBuffer(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.IndexTree.m_nAppendBuffers++;
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if buffer has 0 entries (only head) then should be last buffer
  if (pCrwBuffer->nRows() == 0) {
    delete pCrwBuffer;
    return;
  }

  RASSERT(pCrwBuffer->pNextLinkedBuffer() == nullptr);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark crwBuffer as permanent
  pCrwBuffer->vIsPermanent(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the table in the buffer
  pCrwBuffer->vTable(m_pTable);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the root buffer
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    RASSERT(nBuffers() == 0);
    m_pRootBuffer = pCrwBuffer;
  }
  else {
#ifdef WITH_PAGING
    // TODO: Arjun - If we are doing a second insert, currentbuffer is null. Need to fix this.
    pCrwBuffer->vNextLinkedBufferId(m_pCurrentBuffer->oNextLinkedBufferId());
    pCrwBuffer->vPrevLinkedBufferId(m_pCurrentBuffer->oGetBufferId());
    m_pCurrentBuffer->vNextLinkedBufferId(pCrwBuffer->oGetBufferId());
#else
    pCrwBuffer->vNextLinkedBuffer(m_pCurrentBuffer->pNextLinkedBuffer());
    pCrwBuffer->vPrevLinkedBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextLinkedBuffer(pCrwBuffer);
#endif
  }

  // Append a new Skip List Block to the PDS
  SkipListBlock* pSLBlock = m_pPdsOfBufferIds->pAppendWithSynthKey();

  // Get the start address of it's body after the key in ITEM
  BufferId* pItem = (BufferId*) pSLBlock->aItemBody();

  // Update the contents of it's body with the BufferId
  *pItem = pCrwBuffer->oGetBufferId();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we are done with the current buffer, allow it to be paged out by dropping
  // the lock

  if(m_pCurrentBuffer)
    AM::pHashMap()->vDropKeyLock(m_pCurrentBuffer->oGetBufferId().m_pvAsAddress);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the current buffer
  m_pCurrentBuffer = pCrwBuffer;

  vVerifyPds();
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 CrwTablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  i8 nBufferCount = 0;

  if (m_pRootBuffer)
    nBufferCount = m_pRootBuffer->nLinkedBufferCount();

  RASSERT(m_pPdsOfBufferIds->iNumEntries() == nBufferCount);

  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* reset the current buffer ptr                                               */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vCleanup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

#ifdef WITH_PAGING
  m_pCurrentBuffer = nullptr;
#endif
}

/*----------------------------------------------------------------------------*/
/* drop all paging locks                                                      */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vDropAllPagingLocks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  if(m_pCurrentBuffer)
    AM::pHashMap()->vDropKeyLock(m_pCurrentBuffer->oGetBufferId().m_pvAsAddress);
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
#ifdef WITH_PAGING
  SkipListBlock* pCursor = m_pPdsBufferId->pFirstBlock();
  if(pCursor == nullptr)
    return;

  while(pCursor) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCursor);

    pCrwBuffer->vShow();

    pCursor = m_pPdsBufferId->pNextSLBlock(pCursor, 0);
  }
#else
  CrwBuffer* pCrwBuffer = m_pRootBuffer;

  while (pCrwBuffer) {
    pCrwBuffer->vShow();
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* extract the bufferid from pds and get the address of the buffer            */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwTablePtn::pGetBuffer(SkipListBlock* pSLBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  BufferId oBufferId = *(BufferId*)pSLBlock->aItemBody();
  CrwBuffer* pCrwBuffer = _pCrwBuffer(BufferId::aAddressFromId(oBufferId));

  RASSERT(pCrwBuffer != nullptr);
  RASSERT(pCrwBuffer->bIsClassNo(BASE::CN_CRWBUFFER));
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the address of the pds                                                 */
/*----------------------------------------------------------------------------*/
void* CrwTablePtn::pPds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  return (void*)m_pPdsOfBufferIds;
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool CrwTablePtn::bPopulateIndexTree(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  if (!m_pRootBuffer) return true;

  CrwBuffer* pCrwBuffer = m_pRootBuffer;

  while (pCrwBuffer) {
    pIndexTree->vInsertEntries(pCrwBuffer);
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

  return true;
}
/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwTablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  SkipListBlock* pSLBlock = m_pPdsOfBufferIds->pFirstBlock();
  if(pSLBlock == nullptr) {
    RASSERT(m_pRootBuffer == nullptr);
    return m_pRootBuffer;
  }

  CrwBuffer* pCrwBuffer = pGetBuffer(pSLBlock);
  RASSERT(m_pRootBuffer == pCrwBuffer);

  return m_pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer id                                                 */
/*----------------------------------------------------------------------------*/
BufferId CrwTablePtn::oRootCrwBufferId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  // Get the root block from the PDS
  SkipListBlock* pSLBlock = m_pPdsOfBufferIds->pFirstBlock();
  BufferId oBufferId;

  // Extract the buffer Id, set to 0 if it doesnt exist
  oBufferId.m_iAsI8 = 0;
  if(pSLBlock != nullptr) {
    oBufferId = *(BufferId*)pSLBlock->aItemBody();
  }

  // Extract the Root buffer Id from the LinkedList
  // Set to 0 if it doesnt exist
  BufferId oLLRootBufferId;
  oLLRootBufferId.m_iAsI8 = 0;
  if(m_pRootBuffer != nullptr) {
    oLLRootBufferId = m_pRootBuffer->oGetBufferId();
  }

  // Assert they are the same
  RASSERT(oLLRootBufferId.m_iAsI8 == oBufferId.m_iAsI8);

  return oLLRootBufferId;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* CrwTablePtn::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  return m_pTable;
}



/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  // first truncate all indexes for this table ptn
  // This code gets moved to TablePtn
  /*
  for (i8 iIdxTre = 0; iIdxTre < m_pIndexTreeVec->nItems(); iIdxTre++) {
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
    pIndexTree->vTruncate();
  }
  #*/
  vVerifyPds();

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();

  while(pCurrBlock) {
    SkipListBlock* pNextBlock = m_pPdsOfBufferIds->pNextSLBlock(pCurrBlock, 0);

    CrwBuffer* pCrwBuffer = pGetBuffer(pCurrBlock);
    // Uncomment when replacing the LinkedList with Pds
//    delete pCrwBuffer;
    RASSERT(pCrwBuffer->eGetClassNo() == BASE::CN_CRWBUFFER);
    m_pPdsOfBufferIds->bDelete(pCurrBlock->iKey());
    pCurrBlock = pNextBlock;
  }

  RASSERT(m_pPdsOfBufferIds->iNumEntries() == 0);

  if (m_pRootBuffer)
    m_pRootBuffer->vTruncate();

  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;

  vVerifyPds();
}

/*----------------------------------------------------------------------------*/
/* verify the Pds contains the same buffers as the linked list                */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vVerifyPds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  SkipListBlock* pPdsCursor = m_pPdsOfBufferIds->pFirstBlock();

  CrwBuffer* pLLCursor = m_pRootBuffer;

  pLLCursor = m_pRootBuffer;

  while(pPdsCursor) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pPdsCursor);
    RASSERT(pCrwBuffer == pLLCursor);

    pPdsCursor = m_pPdsOfBufferIds->pNextSLBlock(pPdsCursor, 0);
    pLLCursor = pLLCursor->pNextCrwBuffer();
  }

  RASSERT(pPdsCursor == nullptr);
  RASSERT(pLLCursor == nullptr);
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool CrwTablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  bool bVerify = true;

#ifdef WITH_PAGING
  SkipListBlock* pCurrBlock = m_pPdsBufferId->pFirstBlock();

  if(pCurrBlock == nullptr)
    return bVerify;

  while(pCurrBlock) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCurrBlock);
    bVerify = pCrwBuffer->bVerify();
    if (! bVerify )
      break;

    pCurrBlock = m_pPdsBufferId->pNextSLBlock(pCurrBlock, 0);
  }

  return bVerify;
#else
  CrwBuffer* pCrwBuffer = m_pRootBuffer;

  while (pCrwBuffer) {
    bVerify = pCrwBuffer->bVerify();
    if (! bVerify )
      break;
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

  return bVerify;
#endif
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

#ifdef WITH_PAGING
  SkipListBlock* pCurrBlock = m_pPdsBufferId->pFirstBlock();

  if(pCurrBlock == nullptr)
    return;

  while(pCurrBlock) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCurrBlock);
    vTrace("%lld"_Stz, pCrwBuffer->iDecile());

    pCurrBlock = m_pPdsBufferId->pNextSLBlock(pCurrBlock, 0);
  }
#else
  CrwBuffer* pCrwBuffer = m_pRootBuffer;

  if (! pCrwBuffer)
    return;

  while (pCrwBuffer) {
    // print the decile of this buffer
    vTrace("%lld"_Stz, pCrwBuffer->iDecile());
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }
  vTrace("\n"_Stz);
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* delete an existing row                                                     */
/*----------------------------------------------------------------------------*/
//void CrwTablePtn::vDeleteCrw(IndexTree* pIndexTree, EntBuffer* pEntBuffer, Ent* aEnt)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // delete the crw
//  bool bBufferEmpty = CrwBuffer::bDeleteRefdCrw(aEnt->rxRef());
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // is the crw buffer empty ?
//  if ( bBufferEmpty) {
//
//    // get the referenced buffer
//    CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->rxRef());
//
//    // unlink the buffer
//    pCrwBuffer->vUnLink();
//
//    // is it the currentBuffer ?
//    if (m_pCurrentBuffer == pCrwBuffer)
//      m_pCurrentBuffer = m_pCurrentBuffer->pPrevCrwBuffer();
//
//    // is it the root buffer ?
//    if (m_pRootBuffer == pCrwBuffer) {
//      m_pRootBuffer = m_pRootBuffer->pNextCrwBuffer();
//    }
//
//    // now delete the buffer
//    delete pCrwBuffer;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // now purge the entry
//  pIndexTree->vDelete(pEntBuffer, aEnt);
//
//}

