/*============================================================================*/
/* Index.cpp - index portion of table partition                               */
/*============================================================================*/

#include "MetaData.h"
#include "AM.hpp"

#include "Jobs.h"


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Index* MetaData::_pIndex(void* pAddr)
{
  Index* pIndex = reinterpret_cast<Index*>(pAddr);
  
  RASSERT_TYPE(pIndex,CN_INDEX);
  
  return pIndex;
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

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Index.m_nCreates++;
#endif
  
  Word* pWord  = Word::pToWord(this);
  
  m_pIndexDef = new IndexDef;
  
  m_iIndexNo  = -1;
  m_sIndexNm  = pWord->szWordNm();
  
  m_pTable    = nullptr;
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

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Index.m_nDrops++;
#endif
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Index::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();
  
  sPortrait.vSprint("Index: creates %lld drops %lld\n"_Stz,
                    pStats->Index.m_nCreates,
                    pStats->Index.m_nDrops
                    );
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
  //         sColumnNm.szStz(),
  //         sColumnDefn.szStz());
  
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
  
  m_pIndexDef->m_pIdxColVec->vInsert(pColumn);
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Index::vCreateColumnVec(Jobs::Job_CreateIndex* pJob)
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
    
    m_pIndexDef->m_pIdxColVec->vAdd(pColumn);
  }
}

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
Stz* Index::szIndexNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_sIndexNm;
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
/* show column vector                                                         */
/*----------------------------------------------------------------------------*/
void Index::vShowColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  for (i8 i_Column = 0; i_Column < m_pIndexDef->m_pIdxColVec->nItems(); i_Column++) {
    Column* pColumn = m_pIndexDef->m_pIdxColVec->pGet(i_Column);
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
  m_pIndexDef->m_pIdxColVec->vClear();
  
  delete m_pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* is this a primary key  index                                               */
/*----------------------------------------------------------------------------*/
bool Index::bIsPrimaryKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIndexDef->m_bIsPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* set index as a primary key index                                           */
/*----------------------------------------------------------------------------*/
void Index::vIsPrimaryKey(bool bPrimaryKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pIndexDef->m_bIsPrimaryKey = bPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* is this index unique?                                                      */
/*----------------------------------------------------------------------------*/
bool Index::bIsUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIndexDef->m_bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* set index unique                                                           */
/*----------------------------------------------------------------------------*/
void Index::vIsUnique(bool bUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pIndexDef->m_bIsUnique = bUnique;
}

/*----------------------------------------------------------------------------*/
/* get index column vector                                                    */
/*----------------------------------------------------------------------------*/
IdxColVec* Index::pIdxColVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIndexDef->m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/* compute the size of sort key datum                                         */
/*----------------------------------------------------------------------------*/
i8 Index::iSortKeyDatum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  i8 nKeyDatumSize = 0;
  for (i8 i_IdxCol = 0; i_IdxCol < m_pIndexDef->m_pIdxColVec->nItems(); i_IdxCol++) {
    Column* pColumn = m_pIndexDef->m_pIdxColVec->pGet(i_IdxCol);
    nKeyDatumSize += sizeof(i8);
  }
  
  return nKeyDatumSize;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Index)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
