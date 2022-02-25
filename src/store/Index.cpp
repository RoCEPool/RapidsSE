/*============================================================================*/
/* Index.cpp - index portion of table partition                               */
/*============================================================================*/

#include "BASE.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"


Index*  gpReadApiIndex;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Index* STORE::_pIndex(void* pAddr)
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
  Index* pIndex = (Index*)Heaps::pAllocate(CN_INDEX);
  
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
void Index::operator delete(void* aAddrIndex)
{
  Index* pIndex = reinterpret_cast<Index*>(aAddrIndex);

  Heaps::vFree(pIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Index::Index(Stz* szIndexNm)
: Operand(CN_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.Index.m_nCreates++;
#endif
    
  m_pIndexDef = nullptr;
  
  m_iIndexNo  = -1;
  m_sIndexNm  = szIndexNm;
  
  m_pTable    = nullptr;
  
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
  UNUSED(eClass);
  UNUSED(pObject);
  UNUSED(aTab);
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Index::vPostRecv(void* aIndex)
{
  _static_
  
  Index* pIndex = _pIndex(aIndex);
  
  pIndex->vResetPointers();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze the index                                                          */
/*----------------------------------------------------------------------------*/
void Index::vAnalyze(i8& nBuffers, i8& nEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  i8 nPtnsPerNode = AM::SYSARGS::nPtnsPerNode();
  
  nBuffers = 0;
  nEntries = 0;

  for (i8 i_TablePtn = 0; i_TablePtn < nPtnsPerNode; i_TablePtn++)
    vAnalyze(i_TablePtn,nBuffers,nEntries);
}

/*----------------------------------------------------------------------------*/
/* analyze the index                                                          */
/*----------------------------------------------------------------------------*/
void Index::vAnalyze(i8 i_TablePtn, i8& nBuffers, i8& nEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
    
  IndexTree* pIndexTree = m_pTable->pTablePtn(i_TablePtn)->pIndexTree(m_iIndexNo);
  EntBuffer *pIndexBuffer = pIndexTree->pRootBuffer();
    
  if (pIndexBuffer)
    pIndexBuffer->vAnalyze(nBuffers, nEntries);
}

/*----------------------------------------------------------------------------*/
/* get index column                                                           */
/*----------------------------------------------------------------------------*/
Column* Index::pGetColumn(i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIndexDef->pGetColumn(iColumn);
}

/*----------------------------------------------------------------------------*/
/* get no of column vector                                                    */
/*----------------------------------------------------------------------------*/
i8 Index::nColumns()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIndexDef->nColumns();
}

/*----------------------------------------------------------------------------*/
/* get the index definition                                                   */
/*----------------------------------------------------------------------------*/
IndexDef* Index::pIndexDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return m_pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* set the index definition                                                   */
/*----------------------------------------------------------------------------*/
void Index::vIndexDef(IndexDef* pIndexDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pIndexDef = pIndexDef;
  
  m_pIndexDef->pLocation()->vSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pIndexDef->szIndexNm()->bSame(S_READAPI_I))
    AM::vReadApiIndex(this);
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
/* set index as a primary key index                                           */
/*----------------------------------------------------------------------------*/
void Index::vListContents(i8 i_Depth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  vTrace("%sIndex %lld %s\n"_Stz,
         Stz::szTab(i_Depth),
         m_iIndexNo,
         m_sIndexNm.szStz());
  
  m_pIndexDef->vListContents(i_Depth+1);
}

/*----------------------------------------------------------------------------*/
/* get location                                                               */
/*----------------------------------------------------------------------------*/
Location* Index::pLocation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  return & m_oLocation;
}

/*----------------------------------------------------------------------------*/
/* reset members which are pointers (after being received)                    */
/*----------------------------------------------------------------------------*/
void Index::vResetPointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pIndexDef = nullptr;
  m_pTable    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* compute the size of sort key datum                                         */
/*----------------------------------------------------------------------------*/
i8 Index::iSortKeyDatum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  i8 nKeyDatumSize = 0;
  i8 nIdxColumns = m_pIndexDef->nColumns();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxColumns; i_IdxCol++) {
    nKeyDatumSize += sizeof(i8);
  }
  
  return nKeyDatumSize;
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
  
  delete m_pIndexDef;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Index)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
