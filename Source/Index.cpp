
/*============================================================================*/
/* Index.cpp - index portion of table partition                               */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Index* DBobjects::_pIndex(void* pAddr)
{
  Index* pIndex = reinterpret_cast<Index*>(pAddr);
  
  RASSERT_TYPE(pIndex,CN_INDEX);
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Index::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
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
void* Index::operator new(size_t)
{
  Index* pIndex = nullptr;
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Index::operator new(size_t, Index* pIndex)
{
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Index::operator delete(void*)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Index::Index(void)
: Object(CN_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  Word* pWord = Word::pToWord(this);
  
  m_iIndexNo   = -1;
  m_sIndexName = pWord->sName();;
  m_eIndexType = IT_SKIP_LIST;
  m_bIsPrimaryKey = false;
  m_bIsUnique     = false;
  m_pTable        = nullptr;
  m_pIdxColVec    = new IdxColVec;
  
  m_bSingleColumn = false;
  m_fGreaterEqual = nullptr;
  m_fGreaterThan  = nullptr;
  
  // m_xSubDic
  
  _checkInitialised_(CN_INDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Index::~Index(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Index::vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  //vTrace("vCreateColumnVec: index %s column %s %s\n"_Stz,
  //        m_sIndexName.s(),
  //         sColumnNm.aStz(),
  //         sColumnDefn.aStz());
  
  Column* pColumn = nullptr;
  
  // find the column from the table columnVector
  i8 iColumns = m_pTable->pColumnVec()->nItems();
  
  bool bFound = false;
  for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
    pColumn = m_pTable->pColumnVec()->pGet(i_Col);
    if (pColumn->sColumnName().bSame(sColumnNm)) {
      bFound = true;
      break;
    }
  }
  RASSERT(bFound);
  
  m_pIdxColVec->vInsert(pColumn);
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Index::vCreateColumnVec(JobModel::Job_CreateIndex* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

  Column* pColumn = nullptr;
  
  // find the column from the table columnVector
  i8 iColumns = m_pTable->pColumnVec()->nItems();
  i8 iColumnsIndex = pJob->m_iColumns;
  s32 sColumnNm;
  
  for (i8 i_ColumnIndex = 0; i_ColumnIndex < iColumnsIndex; i_ColumnIndex++) {
 
    sColumnNm = pJob->m_sColumnName[i_ColumnIndex];
    
    bool bFound = false;
    for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
      pColumn = m_pTable->pColumnVec()->pGet(i_Col);
      if (pColumn->sColumnName().bSame(sColumnNm)) {
        bFound = true;
        break;
      }
    }
    RASSERT(bFound);
    
    m_pIdxColVec->vAdd(pColumn);
  }
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void Index::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
/*
  IndexBuffer* pIndexBuffer = m_pIndexLevels->pFirstL0Buffer();

  while (pIndexBuffer) {
    pIndexBuffer->vShow();
    
    EntrySeq* pEntry = pIndexBuffer->pFirstEntry();
    
    while (pEntry) {
      pEntry->vContentDump();
      
      pEntry = pIndexBuffer->pNextEntry(pEntry);
    }
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
 
  pIndexBuffer->vShow();
 */
}

/*----------------------------------------------------------------------------*/
/* is single value keyA greater than or equal to keyB?                        */
/*----------------------------------------------------------------------------*/
bool Index::bGreaterEqual(char* aDatumA, char* aDatumB)
{
  bool bIsGreaterEqual = (*m_fGreaterEqual)(aDatumA,aDatumB);
  
  return bIsGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is single value keyA greater than keyB?                                    */
/*----------------------------------------------------------------------------*/
bool Index::bGreaterThan(char* aDatumA, char* aDatumB)
{
  bool bIsGreaterThan = (*m_fGreaterThan)(aDatumA,aDatumB);
  
  return bIsGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* compute the index form                                                     */
/*----------------------------------------------------------------------------*/
eIF Index::eIndexForm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

  Column* pColumn0;
  Column* pColumn1;
  Column* pColumn2;
  
  if (1 == m_pIdxColVec->nItems()) {
    pColumn0 = m_pIdxColVec->pGet(0);
    if (DM_INT == pColumn0->eDatumType())
      return IF_I8;
    if (DM_VCH == pColumn0->eDatumType())
      return IF_STZ;
    return IF_COMPLEX;
  }
  
  if (2 == m_pIdxColVec->nItems()) {
    pColumn0 = m_pIdxColVec->pGet(0);
    pColumn1 = m_pIdxColVec->pGet(1);
    if (DM_INT == pColumn0->eDatumType() && DM_INT == pColumn1->eDatumType() )
      return IF_I8_I8;
    return IF_COMPLEX;
  }
  
  if (3 == m_pIdxColVec->nItems()) {
    pColumn0 = m_pIdxColVec->pGet(0);
    pColumn1 = m_pIdxColVec->pGet(1);
    pColumn2 = m_pIdxColVec->pGet(2);
    if (DM_INT == pColumn0->eDatumType() &&
        DM_INT == pColumn1->eDatumType() &&
        DM_INT == pColumn2->eDatumType())
      return IF_I8_I8_I8;
    return IF_COMPLEX;
  }

  return IF_COMPLEX;
}

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
Stz* Index::aIndexName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_sIndexName;
}

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
s256 Index::sIndexName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_sIndexName;
}

/*----------------------------------------------------------------------------*/
/* set the indexNo                                                            */
/*----------------------------------------------------------------------------*/
void Index::vIndexNo(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_iIndexNo = iIndexNo;
}

/*----------------------------------------------------------------------------*/
/* get the indexNo                                                            */
/*----------------------------------------------------------------------------*/
i8 Index::iIndexNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_iIndexNo;
}

/*----------------------------------------------------------------------------*/
/* get if the index is a single column index                                  */
/*----------------------------------------------------------------------------*/
bool Index::bSingleColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

  return m_bSingleColumn;
}

/*----------------------------------------------------------------------------*/
/* set if the index is a single column index                                  */
/*----------------------------------------------------------------------------*/
void Index::vSetSingleColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

  if (! m_pIdxColVec || 1 != m_pIdxColVec->nItems()) {
    m_bSingleColumn = false;
    return;
  }
  
  Column* pColumn = m_pIdxColVec->pGet(0);
  
  eDM eDatum = pColumn->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: m_fGreaterEqual = Bcd::bGreaterEqual;
                 m_fGreaterThan  = Bcd::bGreaterThan;
                 break;
    case DM_BOL: m_fGreaterEqual = Bol::bGreaterEqual;
                 m_fGreaterThan  = Bol::bGreaterThan;
                 break;
    case DM_CHR: m_fGreaterEqual = Vch::bGreaterEqual;
                 m_fGreaterThan  = Vch::bGreaterThan;
                 break;
    case DM_DTE: m_fGreaterEqual = Dte::bGreaterEqual;
                 m_fGreaterThan  = Dte::bGreaterThan;
                 break;
    case DM_FLT: m_fGreaterEqual = Flt::bGreaterEqual;
                 m_fGreaterThan  = Flt::bGreaterThan;
                 break;
    case DM_INT: m_fGreaterEqual = Int::bGreaterEqual;
                 m_fGreaterThan  = Int::bGreaterThan;
                 break;
    case DM_IVL: m_fGreaterEqual = Ivl::bGreaterEqual;
                 m_fGreaterThan  = Ivl::bGreaterThan;
                 break;
    case DM_TME: m_fGreaterEqual = Tme::bGreaterEqual;
                 m_fGreaterThan  = Tme::bGreaterThan;
                 break;
    case DM_TSP: m_fGreaterEqual = Tsp::bGreaterEqual;
                 m_fGreaterThan  = Tsp::bGreaterThan;
                 break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: m_fGreaterEqual = Vch::bGreaterEqual;
                 m_fGreaterThan  = Vch::bGreaterThan;
                 break;
      
    default:
      THROW("Index::vSetSingleColumn: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  m_bSingleColumn = true;
}

/*----------------------------------------------------------------------------*/
/* show column vector                                                         */
/*----------------------------------------------------------------------------*/
void Index::vShowColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  for (i8 i_Column = 0; i_Column < m_pIdxColVec->nItems(); i_Column++) {
    Column* pColumn = m_pIdxColVec->pGet(i_Column);
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
  
  // tidyup the index from the table partitions
  m_pTable->vTidyUpIndex(m_iIndexNo);
  
  // Since the column is picked from the ColVec of table
  // we need to just remove it from the IdxColVec before deleting it
  m_pIdxColVec->vClear();
  
  delete m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/* is this a hidden index                                                     */
/*----------------------------------------------------------------------------*/
bool Index::bIsHidden(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_bIsHidden;
}

/*----------------------------------------------------------------------------*/
/* set index as a hidden index                                                */
/*----------------------------------------------------------------------------*/
void Index::vIsHidden(bool bHidden)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_bIsHidden = bHidden;
}

/*----------------------------------------------------------------------------*/
/* is this a primary key  index                                               */
/*----------------------------------------------------------------------------*/
bool Index::bIsPrimaryKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_bIsPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* set index as a primary key index                                           */
/*----------------------------------------------------------------------------*/
void Index::vIsPrimaryKey(bool bPrimaryKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_bIsPrimaryKey = bPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* is this index unique?                                                      */
/*----------------------------------------------------------------------------*/
bool Index::bIsUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* set index unique                                                           */
/*----------------------------------------------------------------------------*/
void Index::vIsUnique(bool bUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_bIsUnique = bUnique;
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
/* compute the size of sort key datum                                         */
/*----------------------------------------------------------------------------*/
i8 Index::iSortKeyDatum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  i8 nKeyDatumSize = 0;
  for (i8 i_IdxCol = 0; i_IdxCol < m_pIdxColVec->nItems(); i_IdxCol++) {
    Column* pColumn = m_pIdxColVec->pGet(i_IdxCol);
    nKeyDatumSize += sizeof(i8);
  }
  
  return nKeyDatumSize;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Index::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
