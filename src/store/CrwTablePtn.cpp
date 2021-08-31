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
#include "Bucket.hpp"
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
CrwTablePtn::CrwTablePtn(Table* pTable, tPN tPtnNo, Bucket* pBucket)
    : Object(CN_CRWTABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

  m_pTable    = pTable;
  m_pBucket = pBucket;
  m_tPtnNo    = tPtnNo;

  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;

  m_pIndexTreeVec  = new IndexTreeVec;

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
  delete m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);
}

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
    m_pRootBuffer = pCrwBuffer;
  }
  else {
    pCrwBuffer->vNextLinkedBuffer(m_pCurrentBuffer->pNextLinkedBuffer());
    pCrwBuffer->vPrevLinkedBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextLinkedBuffer(pCrwBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the current buffer
  m_pCurrentBuffer = pCrwBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now insert entries from the buffer into the indexes
  vInsertIndexEntries(pCrwBuffer);
  
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
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
    
  while (pCrwBuffer) {
    pCrwBuffer->vShow();
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

}

/*----------------------------------------------------------------------------*/
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
IndexTree* CrwTablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = nullptr;
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
    pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    Index* pIndex = pIndexTree->pIndex();
    if (pIndex->iIndexNo() == iIndexNo)
      break;
  }
  
  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec* CrwTablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  return m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vInsertIndexEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  // get no of indexes of the table
  i8 nIndexes = m_pIndexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(i_Index);
    
    // insert entries from EntBuffer into the index
    pIndexTree->vInsertEntries(pCrwBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
tPN CrwTablePtn::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool CrwTablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  
  while (pCrwBuffer) {
    pIndexTree->vInsertEntries(pCrwBuffer);
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool CrwTablePtn::bPopulateIndexTree(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);

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
  
  return m_pRootBuffer;
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
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  
  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  // first truncate all indexes for this table ptn
  // This code gets moved to TablePtn
  //#ifdef WITH_MULTI_BUCKET
  for (i8 iIdxTre = 0; iIdxTre < m_pIndexTreeVec->nItems(); iIdxTre++) {
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
    pIndexTree->vTruncate();
  }
  //#endif
  
  if (m_pRootBuffer)
    m_pRootBuffer->vTruncate();
  
  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vTruncateIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  bool bFoundIndex = false;
  
  // find the index to be truncated for this table ptn
  for (i8 iIdxTre = 0; iIdxTre < m_pIndexTreeVec->nItems(); iIdxTre++) {
    IndexTree* pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
    
    if (pIndexTree->pIndex() == pIndex) {
      bFoundIndex = true;
      // truncate the index
      pIndexTree->vTruncate();
      break;
    }
  }
  
  ASSERT(bFoundIndex);
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool CrwTablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  bool bVerify = true;
  
  while (pCrwBuffer) {
    bVerify = pCrwBuffer->bVerify();
    if (! bVerify )
      break;
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
  }
  
  return bVerify;
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void CrwTablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWTABLEPTN,_FOOTER_);
  
  CrwBuffer* pCrwBuffer = m_pRootBuffer;
  
  while (pCrwBuffer) {
    // print the decile of this buffer
    vTrace("%lld"_Stz, pCrwBuffer->iDecile());
    pCrwBuffer = pCrwBuffer->pNextCrwBuffer();
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

