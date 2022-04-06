/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include <array>
#include "BASE.h"

#include "BucketMap.hpp"
#include "RowCoder.hpp"
#include "HeadLine.hpp"


#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "PtnDef.hpp"

#include "Grid.hpp"
#include "MY.hpp"
#include "BucketInfo.hpp"

extern SizeHeap  gxaSizeHeap[];
Atomic gixTable;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Table,CN_TABLE)
__aNewFunction(Table,CN_TABLE)
__vDeleteFunction(Table,CN_TABLE)
__aFooterFunction(Table)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HACK(Table,"define table partitioning");

Table::Table(Stz* szTableNm, eTT eKind)
: Operand(CN_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Table.m_nCreates++;
#endif
  
  m_sTableNm = szTableNm;
  m_eKind    = eKind;
  
  m_oLocation.vSet();
  m_pStreamDef   = nullptr;
  m_pPtnDef    = new PtnDef;

  m_pBucketMap   = new BucketMap(m_eKind);

  m_pIdxVec      = new IdxVec;
  m_pTablePtnVec = new TablePtnVec;

  m_pParcel      = nullptr;

  m_pBucketMap->vSetTableNm(szTableNm); // for debug
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the table partitions
  // HACK : Don't do this for DEFINE TABLES (which occur before cluster
  //        has started). Experimentally allows new, but non-active nodes to be
  //        added after the cluster has started
  //        This must be fixed properly. Ray 14 Nov 2020
  //if (AM::pGrid()->bHasStarted()) {
  if (TT_NORMAL == m_eKind || TT_INTERNAL == m_eKind) {
    for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {

      LglPtn tLglPtn = Table::tTablePtnToPtnId(i_TablePtn);
      TablePtn* pTablePtn = new TablePtn(this,tLglPtn);

      m_pTablePtnVec->vAppend(pTablePtn);
    }
  }
  else {
    //vTrace("Did not create table partitions for %s\n"_Stz,szTableNm);
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  _checkInitialised_(CN_TABLE);
  
  ++ gixTable;
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Table::~Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  //vTrace(">>>>>>>>>>>>>>>>>>>>> ~Table gets called\n"_Stz);
  m_pIdxVec->vClear();
  
  delete m_pIdxVec;
  delete m_pBucketMap;

  delete m_pStreamDef;
  delete m_pPtnDef;
  
  // delete the TablePtn's
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    delete m_pTablePtnVec->pGet(i_TablePtn);
  }
  m_pTablePtnVec->vClear();

  // delete the partition vector
  delete m_pTablePtnVec;

  //if (m_pParcel) delete m_pParcel;
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Table::szCurrTable(DicStack* pDicStack)
{
  _static_;
  UNUSED(pDicStack);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump table                                                                 */
/*----------------------------------------------------------------------------*/
void Table::vDump(eCN eClass, Object* pObject, Stz* szTab)
{
  UNUSED(eClass);
  UNUSED(szTab);

  Table* pTable = _pTable(pObject);
  
  vTrace("name %s\n"_Stz,
         pTable->m_sTableNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* find table                                                                 */
/*----------------------------------------------------------------------------*/
Table* Table::pFindTable(DicStack* pDicStack, Stz* szTableNm)
{
  _static_
  
  Dic* pDic = pDicStack->pTop();
  if (! pDic)
    return nullptr;

  Word* pTableDicWord = pDic->pFindWord(szTableNm);
  if (! pTableDicWord )
    return nullptr;
  
  Dic* pTableDic = _pDic(pTableDicWord->pObject());
  
  Word* pTableWord = pTableDic->pFindWord(szTableNm);
  if (! pTableWord)
    return nullptr;
  
  Table* pTable = _pTable(pTableWord->pObject());
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* get indexDef of index of given table                                       */
/*----------------------------------------------------------------------------*/
IndexDef* Table::pIndexDef(DicStack* pDicStack, Stz* szTableNm, i8 iIndexNo)
{
  _static_
  
  Table* pTable = Table::pFindTable(pDicStack,szTableNm);
  if ( !pTable )
    return nullptr;
  
  TablePtn* pTablePtn = pTable->m_pTablePtnVec->pGet(0);
  
  IndexTreeVec* pIndexTreeVec = pTablePtn->pIndexTreeVec();
  if (pIndexTreeVec->nItems() < iIndexNo)
    return nullptr;
  
  IndexTree* pIndexTree = pTablePtn->pIndexTreeVec()->pGet(iIndexNo);
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  
  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* get streamDef of given table                                               */
/*----------------------------------------------------------------------------*/
StreamDef* Table::pStreamDef(DicStack* pDicStack, Stz* szTableNm)
{
  _static_
  
  Table* pTable = pFindTable(pDicStack,szTableNm);
  
  return pTable ? pTable->m_pStreamDef : nullptr;
}

/*----------------------------------------------------------------------------*/
/* convert grid-wide partition number to table partition on its node          */
/*----------------------------------------------------------------------------*/
i8 Table::iPtnNoToTablePtn(LglPtn tLglPtn)
{
  _static_

  i8 i_TablePtn = tLglPtn.iAsI8() / AM::GRID::nKnownNodes();

  return i_TablePtn;
}



/*----------------------------------------------------------------------------*/
/* convert tablePtn to partition ID (in grid)                                 */
/*----------------------------------------------------------------------------*/

LglPtn Table::tTablePtnToPtnId(i8 i_TablePtn)
{
  _static_

  i8 iPtnId = (i_TablePtn * AM::GRID::nKnownNodes()) + AM::iThisNodeId();

  LglPtn tLglPtn;

  tLglPtn = iPtnId;

  return tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Table::vPostRecv(void* aTable)
{
  _static_

  Table* pTable = _pTable(aTable);
  
  pTable->vResetPointers();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index to table                                                         */
/*----------------------------------------------------------------------------*/
void Table::vAddIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  //vTrace("Table::vAddIndex\n"_Stz);
  pIndex->pLocation()->vSet();
  
  // set the index no in the index
  
  i8 iIndexNo = m_pIdxVec->nItems();
  pIndex->vIndexNo(iIndexNo);
  
  // add index to table
  m_pIdxVec->vAppend(pIndex);
  
  // create an IndexTree for the index in each partition
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    TablePtn* pTablePtn = this->pTablePtn(i_TablePtn);
    pTablePtn->vAddIndex(new IndexTree(pIndex, pIndex->pIndexDef(), i_TablePtn));
  }
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
void Table::vAnalyze(i8 i_TablePtn, i8& nBuffers, i8& nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nRowsTotal    = 0;
  i8 nBuffersTotal = 0;
  
  TablePtn* pTablePtn = this->pTablePtn(i_TablePtn);

  pTablePtn->vAnalyze(nBuffers, nRows);
  
  nRowsTotal += nRows;
  nBuffersTotal += nBuffers;
}

/*----------------------------------------------------------------------------*/
/* append given buffer into replicated table                                  */
/*----------------------------------------------------------------------------*/
void Table::vAppendBuffer_replicated(DataBuffer *pDataBuffer)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLE, _FOOTER_);
  
  UNUSED(pDataBuffer);
  
  TablePtn* pTblPtn = pTablePtn(0);
  
  pTblPtn->vAppendBuffer_replicated(pDataBuffer);
}

/*----------------------------------------------------------------------------*/
/* get the bucket map for this table                                          */
/*----------------------------------------------------------------------------*/
BucketMap* Table::pBucketMap(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  return m_pBucketMap;
}

/*----------------------------------------------------------------------------*/
/* set the bucket map for this table                                          */
/*----------------------------------------------------------------------------*/
void Table::vBucketMap(BucketMap* pBucketMap)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  m_pBucketMap = pBucketMap;
}

/*----------------------------------------------------------------------------*/
/* get the partition No from Bucket No                                        */
/*----------------------------------------------------------------------------*/
LglPtn  Table::tBktToLglPtn(KeyBkt tKeyBkt)
{
  return m_pBucketMap->tBktToLglPtnId(tKeyBkt);
}


/*----------------------------------------------------------------------------*/
/* get the Next Bucket No from partition No                                   */
/*----------------------------------------------------------------------------*/
i8  Table::iPtnToBktNext(LglPtn tPtnNo, i8 iBktNo)
{
  return m_pBucketMap->iWhichBucketNext(tPtnNo, iBktNo);
}

/*----------------------------------------------------------------------------*/
/* get number of buffers in this table                                        */
/*----------------------------------------------------------------------------*/
i8 Table::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nBuffers = 0;
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    nBuffers += pTablePtn(i_TablePtn)->nBuffers();
  }
  
  return nBuffers;
}

/*----------------------------------------------------------------------------*/
/* create ptn stream def                                                      */
/*----------------------------------------------------------------------------*/
void Table::vCreatePtnDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Column* pColumn = nullptr;

  m_pPtnDef->m_nColumns = 0;
  
  // find the column from the table columnVector
  i8 iColumns = pStreamDef()->nColumns();
  
  for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
    pColumn = pStreamDef()->pGetColumn(i_Col);
    if (pColumn->bIsPtnKey()) {
      m_pPtnDef->m_xaColumn[m_pPtnDef->m_nColumns].vCopy(pColumn);
      m_pPtnDef->m_nColumns++;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void Table::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vDump(CN_TABLE, this, Stz::szTab(1));
}

/*----------------------------------------------------------------------------*/
/* find column by name                                                        */
/*----------------------------------------------------------------------------*/
Column* Table::pFindColumn(Stz* szColNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Column* pColumn = nullptr;
  bool bFound = false;
  i8 nColumns = m_pStreamDef->nColumns();

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    pColumn = m_pStreamDef->pGetColumn(i_Col);
    if (pColumn->szColNm()->bSame(szColNm)) {
      bFound = true;
      break;
    }
  }
  
  return bFound ? pColumn : nullptr;
}

/*----------------------------------------------------------------------------*/
/* find named index                                                           */
/*----------------------------------------------------------------------------*/
Index* Table::pFindIndex(Stz* szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  Index* pIndex = nullptr;
  
  IdxVec* pIdxVec = this->pIdxVec();
  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex_test = pIdxVec->pGet(i_Index);
    Stz* szIndexNm_test = pIndex_test->szIndexNm();
    
    if (szIndexNm_test->bSame(szIndexNm)) {
      pIndex = pIndex_test;
      break;
    }
  }

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index vector from the table                                        */
/*----------------------------------------------------------------------------*/
IdxVec* Table::pIdxVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pIdxVec;
}

/*----------------------------------------------------------------------------*/
/* get number of index buffers in this table                                  */
/*----------------------------------------------------------------------------*/
i8 Table::nIndexBuffers(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    IndexTree* pIndexTree = pTablePtn(i_TablePtn)->pIndexTree(iIndexNo);
    
    iBuffers += pIndexTree->nBufferCount();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* get if table is a definition only                                          */
/*----------------------------------------------------------------------------*/
bool Table::bIsDefinitionOnly(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return (TT_INFOSET == m_eKind || TT_INTERNAL == m_eKind);
}

/*----------------------------------------------------------------------------*/
/* get if table is a definition only                                          */
/*----------------------------------------------------------------------------*/
bool Table::bIsInternallyDefined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return (TT_INTERNAL == m_eKind);
}

/*----------------------------------------------------------------------------*/
/* get if table is replicated                                                 */
/*----------------------------------------------------------------------------*/
bool Table::bIsReplicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  bool bReplicated = m_pStreamDef->bReplicated();
  
  return bReplicated;
}

/*----------------------------------------------------------------------------*/
/* get if table is user created                                               */
/*----------------------------------------------------------------------------*/
bool Table::bIsUserCreated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return (TT_NORMAL == m_eKind);
}

/*----------------------------------------------------------------------------*/
/* get table kind                                                             */
/*----------------------------------------------------------------------------*/
eTT Table::eKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* set table kind                                                             */
/*----------------------------------------------------------------------------*/
void Table::vKind(eTT eKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_eKind = eKind;
}

/*----------------------------------------------------------------------------*/
/* list table contents                                                        */
/*----------------------------------------------------------------------------*/
void Table::vListContents(i8 i_Depth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vTrace("%sTable %s %s\n"_Stz,
         Stz::szTab(i_Depth),
         m_sTableNm.szStz(),
         aTT[m_eKind]
         );

  m_pStreamDef->vListContents(i_Depth+1);
}

/*----------------------------------------------------------------------------*/
/* get a partition                                                            */
/*----------------------------------------------------------------------------*/
TablePtn* Table::pTablePtn(i8 i_TablePtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  return m_pTablePtnVec->pGet(i_TablePtn);
}

/*----------------------------------------------------------------------------*/
/* populate the index                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 i_Partn = 0; i_Partn < AM::PTNMAP::nPtnsPerNode(); i_Partn++) {
    bool bPopulated = m_pTablePtnVec->pGet(i_Partn)->bPopulateIndex(iIndexNo);
    
    if (! bPopulated)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove index from table                                                    */
/*----------------------------------------------------------------------------*/
void Table::vRemoveIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // remove index from the table
  m_pIdxVec->vRemove(pIndex);
  
  // remove the index from each tablePtn
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    m_pTablePtnVec->pGet(i_TablePtn)->vRemoveIndex(pIndex->iIndexNo());
  }
}

/*----------------------------------------------------------------------------*/
/* reset members which are pointers (after being received)                    */
/*----------------------------------------------------------------------------*/
void Table::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_pStreamDef = nullptr;
  m_pPtnDef  = nullptr;
  m_pBucketMap = nullptr;
  m_pIdxVec    = nullptr;
  m_pTablePtnVec = nullptr;

  //Operand::ListItem_t::vResetPointers();

  m_pParcel    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get root buffer of given index                                             */
/*----------------------------------------------------------------------------*/
EntBuffer* Table::pRootBuffer(i8 i_TablePtn, i8 i_Index)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pTblPtn = pTablePtn(i_TablePtn);
  
  IndexTree* pIdxTree = pTblPtn->pIndexTree(i_Index);
  
  EntBuffer* pRootBuffer = pIdxTree->pRootBuffer();
  
  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the sharding definition                                                */
/*----------------------------------------------------------------------------*/
PtnDef* Table::pPtnDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pPtnDef;
}

/*----------------------------------------------------------------------------*/
/* get StreamDef                                                              */
/*----------------------------------------------------------------------------*/
StreamDef* Table::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set StreamDef                                                              */
/*----------------------------------------------------------------------------*/
void Table::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* get the table name                                                         */
/*----------------------------------------------------------------------------*/
Stz* Table::szTableNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_sTableNm;
}

/*----------------------------------------------------------------------------*/
/* verify all indexes                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pTablePtn = this->pTablePtn(0);             //# discuss with Alka
  IndexTreeVec* pIdxTreeVec = pTablePtn->pIndexTreeVec();
  i8 nIndexes = pIdxTreeVec->nItems();

  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    IndexTree* pIndexTree = pIdxTreeVec->pGet(i_Index);
    bool bValid = pIndexTree->bVerifyIndex();
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify index                                                               */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    
    bool bVerify = true;
    
    EntBuffer *pIndexBuffer =
      pTablePtn(i_TablePtn)->pIndexTree(pIndex->iIndexNo())->pRootBuffer();
    
    if (pIndexBuffer)
      bVerify = pIndexBuffer->bVerify();
    if ( ! bVerify )
      break;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify table                                                               */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // verify all the row buffers with the r ow data
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    
    bool bVerify = true;
    
    bVerify = pTablePtn(i_TablePtn)->bVerifyTable();
    
    if ( ! bVerify )
      return bVerify;
  }
  
  // now verify the indexes
  return bVerifyIndexes();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compact index                                                              */
/*----------------------------------------------------------------------------*/
void Table::vCompactIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {
    IndexTree *pIndexTree = pTablePtn(i_TablePtn)->pIndexTree(pIndex->iIndexNo());
    pIndexTree->vCompact();
  }
}

/*----------------------------------------------------------------------------*/
/* returns number of object parts includes the main                           */
/* put each object part in a parcel                                           */
/*----------------------------------------------------------------------------*/
i8 Table::iPrepareParcel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLE, _FOOTER_);

  if (!m_pParcel) {
    m_pParcel = new Parcel;

    // Set the state that this is created as a parcel
    m_pParcel->vOriginalParcel(true);
  }

  m_pParcel->vAddItem((Object *)this);

  if ( m_pStreamDef) {
    m_pParcel->vAddItem((Object *)m_pStreamDef);
  }
  m_pParcel->vAddItem((Object *)m_pPtnDef);

  m_pParcel->vAddItem((Object *)m_pBucketMap);

  if (m_pIdxVec->nItems() > 0)
    m_pParcel->vAddItem((Object *) m_pIdxVec);

  return m_pParcel->nItems();
}

/*----------------------------------------------------------------------------*/
/* put each meta data component of table into given parcel                    */
/*----------------------------------------------------------------------------*/
void Table::vGetMetaParts(Parcel* pParcel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TABLE, _FOOTER_);


  pParcel->vAddItem((Object *)this);

  if (m_pStreamDef)
    pParcel->vAddItem((Object *)m_pStreamDef);
  
  pParcel->vAddItem((Object *)m_pPtnDef);

  pParcel->vAddItem((Object *)m_pBucketMap);

  if (m_pIdxVec->nItems() > 0)
    pParcel->vAddItem((Object *) m_pIdxVec);
}

/*----------------------------------------------------------------------------*/
/* put bucketmap object part in a parcel                                      */
/*----------------------------------------------------------------------------*/
void Table::vGetBktMap(Parcel* pParcel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  pParcel->vAddItem((Object *)this);
  m_pBucketMap->vUpdateMap(); // update with new partition
  pParcel->vAddItem((Object *)m_pBucketMap);
}

/*----------------------------------------------------------------------------*/
/* returns pParcel                                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
Parcel* Table::pParcel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  return m_pParcel;
}

/*----------------------------------------------------------------------------*/
/* prints location info                                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Table::vDumpLocation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  m_oLocation.vPrint();
}

/*----------------------------------------------------------------------------*/
/* returns object parts includes the main by index                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
Object* Table::pGetParcelObject(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  if (!m_pParcel) return nullptr;

  return m_pParcel->pGetItem(i_Item);
}

/*----------------------------------------------------------------------------*/
/* returns number of object parts includes the main                           */
/* Insert each object part and put in an array                                */
/*----------------------------------------------------------------------------*/
i8 Table::iBuildParcelObjs(Object* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  if (!m_pParcel) return 0;

  m_pParcel->vAddItem(pItem);

  return m_pParcel->nItems();
}


/*----------------------------------------------------------------------------*/
/* reassemble object parts from the received parcel                           */
/*----------------------------------------------------------------------------*/
i8  Table::iReassembleFromParcel(Parcel* pParcel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  if (!pParcel) return 0;

  i8 nItems = pParcel->nItems();
  //vTrace(">>>>>>>>> iReassembleFromParcel with %lld parts\n"_Stz, nItems);

  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    Object* pObject =  pParcel->pGetItem(i_Item);
    eCN eClass = pObject ? pObject->eGetClassNo() : CN_NONE;

    switch (eClass) {
      case CN_TABLE:
        // do nothing for now
        break;
      case CN_STREAMDEF:
        m_pStreamDef = (StreamDef*) pObject;
        break;
      case CN_PTNDEF:
        m_pPtnDef = (PtnDef*) pObject;
        break;
      case CN_BUCKETMAP:
        m_pBucketMap = (BucketMap *) pObject;
        break;
      case CN_IDXVEC: {
        m_pIdxVec = (IdxVec *) pObject;
        break;
      }
      default:
        vTrace("iReassembleObj %ld Error\n"_Stz,eClass);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the table partitions

  // HACK : Don't do this for DEFINE TABLES (which occur before cluster
  //        has started). Experimentally allows new, but non-active nodes to be
  //        added after the cluster has started
  //        This must be fixed properly. Ray 14 Nov 2020

  m_pTablePtnVec = new TablePtnVec;



  if (TT_NORMAL == m_eKind || TT_INTERNAL == m_eKind) {
    for (i8 i_TablePtn = 0; i_TablePtn < AM::PTNMAP::nPtnsPerNode(); i_TablePtn++) {

      LglPtn tPtnNo = Table::tTablePtnToPtnId(i_TablePtn);
      TablePtn* pTablePtn = new TablePtn(this,tPtnNo);

      m_pTablePtnVec->vAppend(pTablePtn);
    }
  }
  else {
    //vTrace("Did not create table partitions for %s\n"_Stz,szTableNm);
  }

  // We are done with the received parcel, let's delete the received parcel
  delete pParcel;
  // Null the parcel pointer for this table
  m_pParcel = nullptr;

  return nItems;
}

/*----------------------------------------------------------------------------*/
/* put new bucketmap object part in the existing table                        */
/*----------------------------------------------------------------------------*/
bool Table::bUpdateBktMap(Parcel* pParcel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  if (!pParcel) return false;
  i8 nItems = pParcel->nItems();

  //vTrace(">>>>>>>>> iReassembleFromParcel with %lld parts\n"_Stz, nItems);

  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    Object* pObject =  pParcel->pGetItem(i_Item);
    eCN eClass = pObject ? pObject->eGetClassNo() : CN_NONE;

    switch (eClass) {
      case CN_TABLE:
        // do nothing for now
        break;
      case CN_BUCKETMAP:
        delete m_pBucketMap; // delete old bucket map
        m_pBucketMap = (BucketMap *) pObject;
        break;
      default:
      {
        vTrace("iReassembleObj %ld Error\n"_Stz, eClass);
        return false;
      }
    }
  }
  // We are done with the received parcel, let's delete the received parcel
  delete pParcel;
  // Null the parcel pointer for this table
  m_pParcel = nullptr;
  return true;
}


/*----------------------------------------------------------------------------*/
/* put new bucket object part in the existing table                           */
/*----------------------------------------------------------------------------*/
bool Table::bUpdateBucket(Parcel* pParcel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  if (!pParcel) return false;
  i8 nItems = pParcel->nItems();

  //vTrace(">>>>>>>>> UpdateBucket FromParcel with %lld parts\n"_Stz, nItems);

  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    Object* pObject =  pParcel->pGetItem(i_Item);
    eCN eClass = pObject ? pObject->eGetClassNo() : CN_NONE;

    switch (eClass) {
      case CN_BUCKETINFO:
        // do nothing for now
        break;
      case CN_CRWBUFFER:
      {
        CrwBuffer* pCrwBuffer = (CrwBuffer *) pObject;

        TablePtn* pTablePtn = this->pTablePtn(pCrwBuffer->tLglPtn().uPtnNo());
        pTablePtn->vAppendBuffer(pCrwBuffer);

        break;
      }
      default:
        {
          vTrace("iReassembleObj %ld Error\n"_Stz, eClass);
          return false;
        }
    }
  }
  // We are done with the received parcel, let's delete the received parcel
  delete pParcel;
  // Null the parcel pointer for this table
  m_pParcel = nullptr;
  return true;
}

/*----------------------------------------------------------------------------*/
/* returns number of object parts                                             */
/*----------------------------------------------------------------------------*/
i8  Table::nParcelObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pParcel ? m_pParcel->nItems() : 0;
}


/*----------------------------------------------------------------------------*/
/* dump all contents of the table partition                                   */
/*----------------------------------------------------------------------------*/
Stz* Table::szDumpTablePtn()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  s4K sDumpBuffer = ""_Stz;
  i8 nPtns = m_pTablePtnVec->nItems();

  for (i8 i_Ptn=0; i_Ptn< nPtns; i_Ptn++) {
    TablePtn *pTablePtn = m_pTablePtnVec->pGet(i_Ptn);
    sDumpBuffer.vConcat(pTablePtn->szDumpContents());
  }
  return sDumpBuffer;
}


/*----------------------------------------------------------------------------*/
/* dump all contents of the table bucket map                                  */
/*----------------------------------------------------------------------------*/
Stz* Table::szDumpBucketMap()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  return m_pBucketMap->szDumpMap();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
