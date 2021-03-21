/*============================================================================*/
/* TablePtn.cpp -- Table Partition                                            */
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
#include "Table.hpp"
#include "CrwTablePtn.hpp"
#include "RowTablePtn.hpp"
#include "RowBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,TablePtn,CN_TABLEPTN)
__aNewFunction(TablePtn,CN_TABLEPTN)
__vDeleteFunction(TablePtn,CN_TABLEPTN)
__aFooterFunction(TablePtn)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TablePtn::TablePtn(Table* pTable, tPN tPtnNo)
: Object(CN_TABLEPTN)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  m_pTable = pTable;
  m_tPtnNo = tPtnNo;
  
  eTT eTableKind = m_pTable->eKind();
  switch (eTableKind) {
    case TT_INFOSET:
      NEVER_REACH;
      break;
    case TT_INTERNAL:
      m_eKind = CN_ROWTABLEPTN;
      break;
    case TT_NORMAL:
      m_eKind = CN_CRWTABLEPTN;
      break;
    case TT_REPLICATED:
      NEVER_REACH;
      break;
    default: NEVER_REACH;
  }
  
  switch (m_eKind) {
    case CN_ROWTABLEPTN:
      m_pRowTablePtn = new RowTablePtn(m_pTable,m_tPtnNo,this);
      break;
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn = new CrwTablePtn(m_pTable,m_tPtnNo,this);
      break;
    default: NEVER_REACH;
  }

  _checkInitialised_(CN_TABLEPTN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn::~TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      delete m_pCrwTablePtn;
      break;
    case CN_ROWTABLEPTN:
      delete m_pRowTablePtn;
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop a read lock on the partition                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop a write lock on the partition                                         */
/*----------------------------------------------------------------------------*/
void TablePtn::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a read lock on the partition                           */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabRead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);

  return m_okLock.bGrabRead();
}

/*----------------------------------------------------------------------------*/
/* attempt at grabbing a write lock on the partition                          */
/*----------------------------------------------------------------------------*/
bool TablePtn::bGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is the table partition locked?                                             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
tPN TablePtn::tPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* TablePtn::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  return m_pTable;
}


/*----------------------------------------------------------------------------*/
/*                                    CRW                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index                                                                  */
/*----------------------------------------------------------------------------*/
void TablePtn::vAddIndex(IndexTree* pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vAddIndex(pIndexTree);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void TablePtn::vAppendBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
    
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vAppendBuffer(_pCrwBuffer(pDataBuffer));
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vAppendBuffer(_pRowBuffer(pDataBuffer));
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 TablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
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
IndexTree* TablePtn::pIndexTree(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTree* pIndexTree;
  
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      pIndexTree = m_pCrwTablePtn->pIndexTree(iIndexNo);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      break;
    default:
      NEVER_REACH;
  }

  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* get index data vector                                                      */
/*----------------------------------------------------------------------------*/
IndexTreeVec* TablePtn::pIndexTreeVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  IndexTreeVec* pIndexTreeVec;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      pIndexTreeVec = m_pCrwTablePtn->pIndexTreeVec();
      break;
    case CN_ROWTABLEPTN:
      pIndexTreeVec = m_pCrwTablePtn->pIndexTreeVec();
      break;
    default:
      NEVER_REACH;
  }

  return pIndexTreeVec;
}

/*----------------------------------------------------------------------------*/
/* insert entries into all indexes for all rows in given buffer               */
/*----------------------------------------------------------------------------*/
void TablePtn::vInsertIndexEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
    
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vInsertIndexEntries(pCrwBuffer);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool TablePtn::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  bool b;
    
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      b = m_pCrwTablePtn->bPopulateIndex(iIndexNo);
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
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* TablePtn::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  CrwBuffer* pRootBuffer;
  
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      pRootBuffer = m_pCrwTablePtn->pRootCrwBuffer();
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH
      break;
    default: NEVER_REACH;
  }

  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* remove index                                                               */
/*----------------------------------------------------------------------------*/
void TablePtn::vRemoveIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vRemoveIndex(iIndexNo);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void TablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
    
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
void TablePtn::vTruncateIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
    
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vTruncateIndex(pIndex);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool TablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  bool bVerify;

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEPTN,_FOOTER_);
  
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vVisualizeTable();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vTruncate();
      break;
    default: NEVER_REACH;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
