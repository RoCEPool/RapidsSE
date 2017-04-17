
/*============================================================================*/
/* Index.cpp - index portion of table partition                               */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern Grid*        gpGrid;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Index* DBobjects::_pIndex(void* pAddr)
{
  Index* pIndex = reinterpret_cast<Index*>(pAddr);
  
  RASSERT( !pIndex || CN_INDEX == pIndex->eClass() );
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Index::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEX);
  ASSERT(pObject->eClass() == CN_INDEX);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void Index::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_INDEX);
  ASSERT(pObject->eClass() == CN_INDEX);
  
  Index* pIndex = reinterpret_cast<Index*>(pObject);
  ASSERT(pIndex->bWordBody());
  
  pIndex->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Index::operator new(size_t iSize)
{
  RASSERT(iSize);
  Index* pIndex = nullptr;
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Index::operator new(size_t iSize, Index* pIndex)
{
  RASSERT(iSize);
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Index::operator delete(void* pIndexAddress)
{
  RASSERT(pIndexAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Index::Index(void)
: Object(CN_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  Word* pWord = Word::pToWord(this);
  
  m_iIndexNo = 0;
  m_sIndexName = pWord->sName();;
  m_eIndexType = IT_SKIP_LIST;
  m_pTable = nullptr;
  m_pIdxColVec = new IdxColVec;
  
  m_pIndexLevels = new IndexLevels();
  
  _checkInitialised_(CN_INDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Index::~Index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
Str* Index::pAnalyze(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s\n"_Str,
                       gpGrid->iThisNode(),
                       m_sIndexName.s(),
                       m_pTable->aTableName());
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Index::vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  vTrace("vCreateColumnVec: index %s column %s %s\n"_Str,
           m_sIndexName.s(),
           sColumnNm.aStr(),
           sColumnDefn.aStr());
  
  Column* pColumn = nullptr;
  
  // find the column from the table columnVector
  i8 iColumns = m_pTable->pColumnVec()->nItems();
  
  for (i8 iCol = 0; iCol < iColumns; iCol++) {
    pColumn = m_pTable->pColumnVec()->pGet(iCol);
    if (pColumn->sColumnName().bSame(sColumnNm))
      break;
  }
  ASSERT(pColumn);
  m_pIdxColVec->vInsert(pColumn);
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void Index::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

  IndexBuffer* pIndexBuffer = m_pIndexLevels->pFirstBuffer();
/*
  while (pIndexBuffer) {
    pIndexBuffer->vShow();
    
    EntrySeq* pEntry = pIndexBuffer->pFirstEntry();
    
    while (pEntry) {
      pEntry->vContentDump();
      
      pEntry = pIndexBuffer->pNextEntry(pEntry);
    }
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
 */
  pIndexBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for the row into the index                                 */
/*----------------------------------------------------------------------------*/
void Index::vInsertEntry(Row* pRow, i8 nRowNo, bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  EntWsp xEntWsp;
  RowId xRowId;
  ValWsp xValWsp;
  KeyWsp xKeyWsp;
  i8 iKey;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  xRowId.vBuffer(*nBufferNo);
  xRowId.vRow(nRowNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the rowId in the EntWsp
  xEntWsp.vRowId(xRowId);
  
  // get the first field (hardcoded)                            //ALKA
  StrVal* pStrVal = (StrVal*)pRow->pData();
  
  // extract the key from the first field
  iKey = pStrVal->iToI8();
  
  // add the key to the KeyWsp
  xValWsp = iKey;
  xKeyWsp.vAddElement(xValWsp);
  Key xKey(xKeyWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add key to entry
  xEntWsp.vAddKey(xKeyWsp);
  
  xEntWsp.vShow();
  
  // insert row entry in index
  m_pIndexLevels->vInsertEntry(&xKey, xRowId);
  
}

/*----------------------------------------------------------------------------*/
/* populate the index                                                         */
/*----------------------------------------------------------------------------*/
void Index::vPopulate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  for (i8 iSlot = 0; iSlot < gpGrid->m_nSlots; iSlot++) {
    
    //i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    RowStrand *pStrand = m_pTable->pPartition(iSlot)->pData()->pHeadStrand();
    
    i8 nBuffers = 0;
    i8 nStrands = 0;
    while (pStrand) {
      nStrands++;
      
        RowBuffer* pRowBuffer = pStrand->pFirstBuffer();
        i8 nRowsInBuffer = 0;
        i8 nRowBuffer = 0;
      
        while (pRowBuffer) {
          nRowBuffer++;
          nRowsInBuffer = pRowBuffer->nRows();
         
          Row* pRow = pRowBuffer->pFirstRow();
          bn nBufferNo = pRowBuffer->nBufferNo();
          i8 nRowNo = 0;
          
          while (pRow) {
            vInsertEntry(pRow, nRowNo, nBufferNo);
            pRow = pRowBuffer->pNextRow(pRow);
            nRowNo++;
          }
          
          // move to the next rowBuffer
          pRowBuffer = pRowBuffer->pNextBuffer();
        }
      
      nBuffers += pStrand->nRowBuffers();
      pStrand = pStrand->pNextStrand();
    }
    
  }

}

/*----------------------------------------------------------------------------*/
/* show column vector                                                         */
/*----------------------------------------------------------------------------*/
void Index::vShowColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  for (i8 iColumn = 0; iColumn < m_pIdxColVec->nItems(); iColumn++) {
    Column* pColumn = m_pIdxColVec->pGet(iColumn);
    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* get index sub dictionary                                                   */
/*----------------------------------------------------------------------------*/
Dic* Index::pSubDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return &m_xSubDic;
}

/*----------------------------------------------------------------------------*/
/* get the table                                                              */
/*----------------------------------------------------------------------------*/
Table* Index::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void Index::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void Index::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  // Since the column is picked from the ColVec of table
  // we need to just remove it from the IdxColVec before deleting it
  m_pIdxColVec->vClear();
  
  delete m_pIdxColVec;
  delete m_pIndexLevels;
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void Index::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pIndexLevels->vTruncate();
}

/*----------------------------------------------------------------------------*/
/* get index column vector                                                    */
/*----------------------------------------------------------------------------*/
IdxColVec* Index::pIdxColVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Index::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
