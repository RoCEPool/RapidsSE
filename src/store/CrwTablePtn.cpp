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
  m_iSynthKey  = 0;

  m_pPdsOfBufferIds = new Pds_t<PdsItem_BufferId>(nullptr);
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
/* analyze this buffer chain                                                  */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vAnalyze(i8& nBuffers, i8& nRows, i8& jSumRowBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  SkipListBlock* pCursor = m_pPdsOfBufferIds->pFirstBlock();
  if(pCursor == nullptr)
    return;

  while(pCursor) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCursor);
    nBuffers++;

    //pCrwBuffer->vShow();
    nRows += (pCrwBuffer->nRows());
    jSumRowBytes += pCrwBuffer->nAllotedBytes();

    pCursor = m_pPdsOfBufferIds->pNextSLBlock(pCursor, 0);
  }

  return;
}

/*----------------------------------------------------------------------------*/
/* append a crw buffer into the CrwTablePtn                                   */
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark crwBuffer as permanent
  pCrwBuffer->vIsPermanent(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the table in the buffer
  pCrwBuffer->vTable(m_pTable);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append a new Skip List Block to the PDS
  SkipListBlock* pSLBlock = m_pPdsOfBufferIds->pInsert((void*)&(m_iSynthKey));
  RASSERT(pSLBlock != nullptr);

  PdsItem_BufferId oBufferIdStruct;
  oBufferIdStruct.iKey = m_iSynthKey;
  oBufferIdStruct.oBufferId = pCrwBuffer->oGetBufferId();

  m_iSynthKey++;

  // Update the contents of SkipListBlock Item with the key and bufferId
  *(PdsItem_BufferId*)(pSLBlock->aItem()) = oBufferIdStruct;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we are done with the current buffer, allow it to be paged out by dropping
  // the lock

  AM::pHashMap()->vDropKeyLock(pCrwBuffer->oGetBufferId().m_pvAsAddress);
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 CrwTablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  return m_pPdsOfBufferIds->iNumEntries(0);
}

/*----------------------------------------------------------------------------*/
/* reset the current buffer ptr                                               */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vCleanup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* drop all paging locks                                                      */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vDropAllPagingLocks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

//  if(m_pCurrentBuffer)
//    AM::pHashMap()->vDropKeyLock(m_pCurrentBuffer->oGetBufferId().m_pvAsAddress);
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  SkipListBlock* pCursor = m_pPdsOfBufferIds->pFirstBlock();
  if(pCursor == nullptr)
    return;

  while(pCursor) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCursor);

    pCrwBuffer->vShow();

    pCursor = m_pPdsOfBufferIds->pNextSLBlock(pCursor, 0);
  }
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
BufferId CrwTablePtn::oExtractBufferId(void *pItemAddr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pItemAddr;

  return oBufferIdStruct.oBufferId;

}

/*----------------------------------------------------------------------------*/
/* extract the bufferid from pds and get the address of the buffer            */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwTablePtn::pGetBuffer(SkipListBlock* pSLBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  PdsItem_BufferId* pBufferIdStruct = (PdsItem_BufferId*) pSLBlock->aItem();
  CrwBuffer* pCrwBuffer = _pCrwBuffer(BufferId::aAddressFromId(pBufferIdStruct->oBufferId));

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
  SkipListBlock* pCursor = m_pPdsOfBufferIds->pFirstBlock();
  if(!pCursor) return true;

  while(pCursor) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCursor);

    pIndexTree->vInsertEntries(pCrwBuffer);

    pCursor = m_pPdsOfBufferIds->pNextSLBlock(pCursor, 0);
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
  if(!pSLBlock) {
    return nullptr;
  }

  CrwBuffer* pCrwBuffer = pGetBuffer(pSLBlock);

  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer id                                                 */
/*----------------------------------------------------------------------------*/
BufferId CrwTablePtn::oRootCrwBufferId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  // Get the root block from the PDS
  SkipListBlock* pSLBlock = m_pPdsOfBufferIds->pFirstBlock();
  BufferId oRootBufferId;

  // Extract the buffer Id, set to 0 if it doesn't exist
  oRootBufferId.m_iAsI8 = 0;
  if(pSLBlock != nullptr) {
    PdsItem_BufferId* pPdsItem = (PdsItem_BufferId*)pSLBlock->aItem();
    oRootBufferId = pPdsItem->oBufferId;
  }

  return oRootBufferId;
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

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();

  while(pCurrBlock) {
    SkipListBlock* pNextBlock = m_pPdsOfBufferIds->pNextSLBlock(pCurrBlock, 0);

    PdsItem_BufferId oBufferIdStruct = *(PdsItem_BufferId*) pCurrBlock->aItem();

    CrwBuffer* pCrwBuffer = _pCrwBuffer(BufferId::aAddressFromId(oBufferIdStruct.oBufferId));
    m_pPdsOfBufferIds->bDelete((void*)&oBufferIdStruct.iKey);
    delete pCrwBuffer;

    pCurrBlock = pNextBlock;
  }

  RASSERT(m_pPdsOfBufferIds->iNumEntries(0) == 0);
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool CrwTablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  bool bVerify = true;
#ifdef WITH_DEBUG_VERSION

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();

  if(!pCurrBlock) return bVerify;

  // Verify the integrity of the Pds
  if(! m_pPdsOfBufferIds->bVerify())
    return false;

  // Verify the contents of the crwbuffers
  while(pCurrBlock) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCurrBlock);
    bVerify = pCrwBuffer->bVerifyRows();
    if (! bVerify )
      break;

    pCurrBlock = m_pPdsOfBufferIds->pNextSLBlock(pCurrBlock, 0);
  }
#endif
  return bVerify;
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  SkipListBlock* pCurrBlock = m_pPdsOfBufferIds->pFirstBlock();

  if(!pCurrBlock) return;

  while(pCurrBlock) {
    CrwBuffer* pCrwBuffer = pGetBuffer(pCurrBlock);

    vTrace("%lld"_Stz, pCrwBuffer->iDecile());

    pCurrBlock = m_pPdsOfBufferIds->pNextSLBlock(pCurrBlock, 0);
  }

  vTrace("\n"_Stz);
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

