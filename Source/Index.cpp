/*============================================================================*/
/* Index.cpp - index portion of table partition                               */
/*============================================================================*/

#include "MetaData.h"


#include "Jobs.h"


Index*  gpReadApiIndex;

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
  Index* pIndex = (Index*)HeapMem::pAllocate(CN_INDEX);
  
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

  HeapMem::vFree(pIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Index::Index(Stz* szIndexNm)
: Object(CN_INDEX)
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
/* analyze the index                                                          */
/*----------------------------------------------------------------------------*/
void Index::vAnalyze(i8 iSlot, i8& nBuffers, i8& nEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  i8 nEntriesTotal = 0;
  i8 nBuffersTotal = 0;
  
  IndexTree* pIndexTree = m_pTable->pTablePtn(iSlot)->pIndexTree(m_iIndexNo);
  EntBuffer *pIndexBuffer = pIndexTree->pRootBuffer();
  
  nBuffers = 0;
  nEntries = 0;
  
  if (pIndexBuffer)
    pIndexBuffer->vAnalyze(nBuffers, nEntries);
  
  nEntriesTotal += nEntries;
  nBuffersTotal += nBuffers;
  
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
