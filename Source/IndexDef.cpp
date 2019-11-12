/*============================================================================*/
/* IndexDef.cpp -- Index Definition                                           */
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
IndexDef* MetaData::_pIndexDef(void* pAddr)
{
  IndexDef* pIndex = reinterpret_cast<IndexDef*>(pAddr);
  
  RASSERT_TYPE(pIndex,CN_INDEXDEF);
  
  return pIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexDef::operator new(size_t)
{
  Object* pIndexDef = HeapMem::pAllocate(CN_INDEXDEF);
  
  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexDef::operator delete(void* pAddress)
{
  IndexDef* pIndexDef = reinterpret_cast<IndexDef*>(pAddress);
  
  HeapMem::vFree(pIndexDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexDef::IndexDef(void)
: Object(CN_INDEXDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  m_sIndexNm      = C_NO_NAME;
  m_eIndexType    = IT_SKIP_LIST;
  
  m_bIsPrimaryKey = false;
  m_bIsUnique     = false;
  
  m_pIdxColVec    = new IdxColVec;
  
  m_sCmd          = ""_Stz;

  _checkInitialised_(CN_INDEXDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexDef::~IndexDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  delete m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
//void IndexDef::vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  //vTrace("vCreateColumnVec: index %s column %s %s\n"_Stz,
//  //        m_sIndexName.s(),
//  //         sColumnNm.szStz(),
//  //         sColumnDefn.szStz());
//
//  Column* pColumn = nullptr;
//
//  // find the column from the table columnVector
//  i8 iColumns = m_pTable->pColumnVec()->nItems();
//
//  bool bFound = false;
//  for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
//    pColumn = m_pTable->pColumnVec()->pGet(i_Col);
//    if (pColumn->sColumnName().bSame(sColumnNm)) {
//      bFound = true;
//      break;
//    }
//  }
//  RASSERT(bFound);
//
//  m_pIdxColVec->vInsert(pColumn);
//}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
//void IndexDef::vCreateColumnVec(Jobs::Job_CreateIndex* pJob)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  Column* pColumn = nullptr;
//  
//  // find the column from the table columnVector
//  i8 iColumns = m_pTable->pColumnVec()->nItems();
//  i8 iColumnsIndex = pJob->m_iColumns;
//  s32 sColumnNm;
//  
//  for (i8 i_ColumnIndex = 0; i_ColumnIndex < iColumnsIndex; i_ColumnIndex++) {
// 
//    sColumnNm = pJob->m_sColumnName[i_ColumnIndex];
//    
//    bool bFound = false;
//    for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
//      pColumn = m_pTable->pColumnVec()->pGet(i_Col);
//      if (pColumn->sColumnName().bSame(sColumnNm)) {
//        bFound = true;
//        break;
//      }
//    }
//    RASSERT(bFound);
//    
//    m_pIdxColVec->vAdd(pColumn);
//  }
//}

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
Stz* IndexDef::szIndexNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_sIndexNm;
}

/*----------------------------------------------------------------------------*/
/* set the indexNo                                                            */
/*----------------------------------------------------------------------------*/
//void IndexDef::vIndexNo(i8 iIndexNo)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  m_iIndexNo = iIndexNo;
//}

/*----------------------------------------------------------------------------*/
/* get the indexNo                                                            */
/*----------------------------------------------------------------------------*/
//i8 IndexDef::iIndexNo(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  return m_iIndexNo;
//}

/*----------------------------------------------------------------------------*/
/* show column vector                                                         */
/*----------------------------------------------------------------------------*/
void IndexDef::vShowColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  for (i8 i_Column = 0; i_Column < m_pIdxColVec->nItems(); i_Column++) {
    Column* pColumn = m_pIdxColVec->pGet(i_Column);
    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* get the table                                                              */
/*----------------------------------------------------------------------------*/
//Table* IndexDef::pTable(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  return m_pTable;
//}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
//void IndexDef::vTable(Table* pTable)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  m_pTable = pTable;
//}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
//void IndexDef::vTidyUp(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
//
//  // tidyup the index from the table partitions
//  m_pTable->vTidyUpIndex(m_iIndexNo);
//
//  // Since the column is picked from the ColVec of table
//  // we need to just remove it from the IdxColVec before deleting it
//  m_pIdxColVec->vClear();
//
//  delete m_pIdxColVec;
//}

/*----------------------------------------------------------------------------*/
/* is this a primary key  index                                               */
/*----------------------------------------------------------------------------*/
bool IndexDef::bIsPrimaryKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_bIsPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* set index as a primary key index                                           */
/*----------------------------------------------------------------------------*/
void IndexDef::vIsPrimaryKey(bool bPrimaryKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  m_bIsPrimaryKey = bPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* is this index unique?                                                      */
/*----------------------------------------------------------------------------*/
bool IndexDef::bIsUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* set index unique                                                           */
/*----------------------------------------------------------------------------*/
void IndexDef::vIsUnique(bool bUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  m_bIsUnique = bUnique;
}

/*----------------------------------------------------------------------------*/
/* get index column vector                                                    */
/*----------------------------------------------------------------------------*/
IdxColVec* IndexDef::pIdxColVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/* compute the size of sort key datum                                         */
/*----------------------------------------------------------------------------*/
i8 IndexDef::iSortKeyDatum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
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
__aFooterFunction(IndexDef)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
