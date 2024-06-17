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
    case TT_METADATA:
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
void TablePtn::vAppendBuffer(DataBuffer* pDataBuffer, i8& nRowsInserted)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  // append buffer first, then
  // insert entries from the buffer into the indexes

  eCN eClass = pDataBuffer->eGetClassNo();
  switch (eClass) {
    case BASE::CN_CRWBUFFER: {
      CrwBuffer *pCrwBuffer = _pCrwBuffer(pDataBuffer);
      if (pCrwBuffer->nRows() == 0) {
        delete pCrwBuffer;
        return;
      }
      m_pCrwTablePtn->vAppendBuffer(pCrwBuffer);
      nRowsInserted = pCrwBuffer->nRows();
      // TODO: Arjun - Update below for paging and bufferids
      vInsertIndexEntries(_pCrwBuffer(pDataBuffer));
      break;
    }

    case CN_PDS: {
      Pds* pPds = _pPds(pDataBuffer);
      if (pPds->nItems() == 0) {
        delete pPds;
        return;
      }
      m_pCrwTablePtn->vAppendBuffer(pPds);
      nRowsInserted = pPds->nItems();
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
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
BufferId TablePtn::oExtractBufferId(void* pItemAddr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      return m_pCrwTablePtn->oExtractBufferId(pItemAddr);
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
bool TablePtn::bPopulateGblIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);

  bool b;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      b = m_pCrwTablePtn->bPopulateGblIndexTree(pIndexTree, m_tLglPtn);
      break;
    default:
      NEVER_REACH;
  }

  return b;
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

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vAnalyze(nBuffers, nRows, jSumRowBytes);
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
/* Root BufferID                                                              */
/*----------------------------------------------------------------------------*/
BufferId TablePtn::oRootBufferId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      return m_pCrwTablePtn->oRootCrwBufferId();
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* Root Pds                                                                   */
/*----------------------------------------------------------------------------*/
Pds* TablePtn::pRootPds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLEPTN, _FOOTER_);
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      return _pPds(AM::pHashMap()->pValueAt(m_pCrwTablePtn->oRootCrwBufferId().m_pvAsAddress, true));
    default: NEVER_REACH;
  }
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
    default:
      NEVER_REACH;
  }



  // Now, truncate all data
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vTruncate();
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
  UNUSED(bFoundIndex);
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
    default: NEVER_REACH;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
