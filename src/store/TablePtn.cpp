/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
/*============================================================================*/

#include "BASE.h"

//#include "RowCoder.hpp"
//#include "HeadLine.hpp"

//#include "Crw.hpp"
//#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

//#include "CrvBuffer.hpp"

#include "TablePtn.hpp"
#include "Table.hpp"
#include "CrwTablePtn.hpp"
#include "RowTablePtn.hpp"
#include "RowBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, TablePtn, CN_TABLEPTN)

__aNewFunction(TablePtn, CN_TABLEPTN)

__vDeleteFunction(TablePtn, CN_TABLEPTN)

__aFooterFunction(TablePtn)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(Table *pTable, LglPtn tLglPtn, PhyPtn tPhyPtn)
    : Object(CN_TABLEPTN), m_okLock(this)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  m_pTable  = pTable;
  m_tLglPtn = tLglPtn;
  m_tPhyPtn = tPhyPtn;

  m_pIndexTreeVec  = new IndexTreeVec;

  eTT eTableKind = m_pTable->eKind();
  switch (eTableKind) {
    case TT_INFOSET:
      NEVER_REACH;
      break;
    case TT_INTERNAL:
      m_eKind = CN_CRWTABLEPTN;
      m_pCrwTablePtn = new CrwTablePtn(m_pTable,m_tLglPtn);
      break;
    case TT_NORMAL:
      m_eKind = CN_CRWTABLEPTN;
      m_pCrwTablePtn = new CrwTablePtn(m_pTable,m_tLglPtn);
      break;
    case TT_REPLICATED:
      NEVER_REACH;
      break;
    default:
      NEVER_REACH;
  }

  _checkInitialised_(CN_TABLEPTN);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
}
// TODO: Remove CN_ROWTABLEPTN from this file
/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      delete m_pCrwTablePtn;
      break;
    case CN_ROWTABLEPTN:
      delete m_pRowTablePtn;
      break;
    default:
      NEVER_REACH;
  }

  // delete the index data vector
  delete m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop all paging locks held and clear the current buffer ptr                */
/*----------------------------------------------------------------------------*/
void TablePtn::vCleanup(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vDropAllPagingLocks();
      m_pCrwTablePtn->vCleanup();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vDropAllPagingLocks();
      m_pRowTablePtn->vCleanup();
      break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* drop a read lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropRead(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                         */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_okLock.bGrabRead();

}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* return tablePtn kind                                                       */
/*----------------------------------------------------------------------------*/
eCN TablePtn::eKind(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
LglPtn TablePtn::tLglPtn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_tLglPtn;

}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table *TablePtn::pTable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  return m_pTable;
}


/*----------------------------------------------------------------------------*/
/*                                CRW/ROW                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree *pIndexTree)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  //vTrace("TablePtn::vAddIndex\n"_Stz);
  ASSERT(m_pIndexTreeVec->nItems() < C_MAX_INDEXES);
  m_pIndexTreeVec->vAdd(pIndexTree);


}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(DataBuffer *pDataBuffer)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // append buffer first, then
  // insert entries from the buffer into the indexes

  switch (m_eKind) {
    case CN_CRWTABLEPTN: {
      CrwBuffer *pCrwBuffer = _pCrwBuffer(pDataBuffer);
      if (pCrwBuffer->nRows() == 0) {
        delete pCrwBuffer;
        return;
      }
      m_pCrwTablePtn->vAppendBuffer(pCrwBuffer);
      // TODO: Arjun - Update below for paging and bufferids
      vInsertIndexEntries(_pCrwBuffer(pDataBuffer));
      break;
    }

    case CN_ROWTABLEPTN: {
      RowBuffer *pRowBuffer = _pRowBuffer(pDataBuffer);
      if (pRowBuffer->nRows() == 0) {
        delete pRowBuffer;
        return;
      }
      m_pRowTablePtn->vAppendBuffer(pRowBuffer);
      //m_pRowTablePtn->vAppendBuffer(_pRowBuffer(pDataBuffer));
      //vInsertIndexEntries(_pRowBuffer(pDataBuffer));
      break;
    }

    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  i8 nBufferCount;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      nBufferCount = m_pCrwTablePtn->nBuffers();
      break;
    case CN_ROWTABLEPTN:
      nBufferCount = m_pRowTablePtn->nBuffers();
      break;
    default:
      NEVER_REACH;
  }

  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void TablePtn::vDump(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vDump();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vDump();
      break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get IndexTree                                                              */
/*----------------------------------------------------------------------------*/
IndexTree *TablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

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
IndexTreeVec *TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsertIndexEntries(CrwBuffer *pCrwBuffer)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

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
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
void* TablePtn::pPdsCrwTablePtn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      return m_pCrwTablePtn->pPds();
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      return nullptr;
    default:
      NEVER_REACH;
      return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);
  bool b;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      b = m_pCrwTablePtn->bPopulateIndexTree(pIndexTree);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      break;
    default:
      NEVER_REACH;
  }

  return b;
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
void TablePtn::vAnalyze(i8& nBuffers, i8& nRows, i8& jSumRowBytes)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  CrwBuffer* pCrwRootBuffer = nullptr;
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      pCrwRootBuffer = m_pCrwTablePtn->pRootCrwBuffer();
      if(pCrwRootBuffer)
        pCrwRootBuffer->vAnalyze(nBuffers, nRows, jSumRowBytes);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  IndexTree* pIndexTree = this->pIndexTree(iIndexNo);

  // remove the index from the IdxDataVec
  m_pIndexTreeVec->vRemove(pIndexTree);
  delete pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* TablePtn::pRootBuffer(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      return m_pCrwTablePtn->pRootCrwBuffer();
    case CN_ROWTABLEPTN:
      return m_pRowTablePtn->pRootRowBuffer();
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* Root BufferID                                                              */
/*----------------------------------------------------------------------------*/
BufferId TablePtn::oRootBufferId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      return m_pCrwTablePtn->oRootCrwBufferId();
    case CN_ROWTABLEPTN:
      return m_pRowTablePtn->oRootRowBufferId();
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get root crw buffer                                                        */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_pCrwTablePtn->pRootCrwBuffer();
}

/*----------------------------------------------------------------------------*/
/* get root row buffer                                                        */
/*----------------------------------------------------------------------------*/
RowBuffer* TablePtn::pRootRowBuffer(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  return m_pRowTablePtn->pRootRowBuffer();
}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // truncate for all buckets

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      // first truncate all indexes for this table ptn
      {
        i8 nItems = m_pIndexTreeVec->nItems();
        for (i8 iIdxTre = 0; iIdxTre < nItems; iIdxTre++) {
          IndexTree *pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);
          if (pIndexTree) pIndexTree->vTruncate();
        }
      }
      break;
    case CN_ROWTABLEPTN:
      // Row buffer doesn't have index
      break;
    default:
      NEVER_REACH;
  }



  // Now, truncate all data
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vTruncate();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vTruncate();
      break;
    default: NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncateIndex(Index *pIndex)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // truncate all index, only for Crw buffer
  bool bFoundIndex = false;

  // find the index to be truncated for this table ptn
  i8 nItems = m_pIndexTreeVec->nItems();
  for (i8 iIdxTre = 0; iIdxTre < nItems; iIdxTre++) {
    IndexTree *pIndexTree = m_pIndexTreeVec->pGet(iIdxTre);

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
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  bool bVerify = false;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      bVerify = m_pCrwTablePtn->bVerifyTable();
      break;
    case CN_ROWTABLEPTN:
      bVerify = m_pRowTablePtn->bVerifyTable();
      break;
    default:
      NEVER_REACH;
  }

  return bVerify;
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void TablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vVisualizeTable();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vVisualizeTable();

      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* dump the table data of this partition                                      */
/*----------------------------------------------------------------------------*/
Stz* TablePtn::szDumpContents(void)
{
  i8 nBuffers = m_pCrwTablePtn->nBuffers();
  s4K sDumpBuffer = ""_Stz;
  if (nBuffers > 0) {
    sDumpBuffer.vCatSprint("  PtnId:%lld   nBuffers:%lld\n"_Stz, m_tLglPtn.iAsI8(), nBuffers);
    for (i8 i_PoolBuffer = 0; i_PoolBuffer < nBuffers; i_PoolBuffer++) {
      i8 nRows = m_pCrwTablePtn->pRootCrwBuffer()->nRows();
      sDumpBuffer.vCatSprint("     Buffer:%lld   PtnId:%lld   nRows:%lld\n"_Stz, i_PoolBuffer, m_tLglPtn.iAsI8(), nRows);
    }
  }
  return sDumpBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
