/*============================================================================*/
/* RowAgg.hpp -- Row Working Entry                                            */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowAgg* DBobjects::_pRowAgg(void* aAddr)
{
  RowAgg* pRowAgg = reinterpret_cast<RowAgg*>(aAddr);
  
  if (pRowAgg && CN_ROWAGG != pRowAgg->eClass())
    vTrace("_pRowAgg got class %s\n"_Stz,aCN[pRowAgg->eClass()]);
  
  RASSERT_TYPE(pRowAgg,CN_TABLE);
  
  return pRowAgg;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowAgg::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWAGG);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowAgg::operator delete(void* pAddress)
{
  RowAgg* pRowAgg = reinterpret_cast<RowAgg*>(pAddress);
  
  HeapMem::vFree(pRowAgg);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowAgg::RowAgg(void)
: Object(CN_ROWAGG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG);
  
  m_pTable     = nullptr;
  m_pKeyColumn = nullptr;
  m_pAggColVec = nullptr;
  m_pAggColVec = new ColumnVec;
  
  vReset();
  
  _checkInitialised_(CN_ROWAGG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowAgg::~RowAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  //clear all pointers back to the columns of the original table
  m_pAggColVec->vClear();
  
  delete m_pAggColVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* aggregate an entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void RowAgg::vAggregate(i8 i_Slot, EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s48K sRowString;
  
  Int xKey;
  Int xVal;
  byte* aSrc;
  Column* pAggColumn;
  i8 i_Col;
  Row* pRow;
  
  i8 nAggColumns = m_pAggColVec->nItems();
  
  vReset();
  
  m_oSince = pEntryBuffer->oWhenNewed();
  
  // aggregate the buffer
  for (Entry* pEntry = pEntryBuffer->pFirstEntry();
       pEntry != nullptr;
       pEntry = pEntryBuffer->pNextEntry(pEntry, 0)) {
    
    pRow = pEntry->aRow();
    
    //pRow->vAsBinaryStringVLQ(sRowString, m_pTable->pColumnVec());
    //vTrace("row: (%s)\n"_Stz, sRowString.aChar());
    
    aSrc = (byte*)pRow->aColumnDatum(m_pKeyColumn->iColumnNo());
    xKey = IntLib::iVlqToInt(aSrc);

    for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {
      
      pAggColumn = m_pAggColVec->pGet(i_AggCol);
      i_Col = pAggColumn->iColumnNo();
      
      aSrc = (byte*)pRow->aColumnDatum(i_Col);
      xVal = IntLib::iVlqToInt(aSrc);
    
      if (xVal.iAsI8() < m_iaMinVal[i_AggCol]) {
        m_iaMinVal[i_AggCol] = xVal.iAsI8();
        m_iaMinKey[i_AggCol] = xKey.iAsI8();
      }
      if (xVal.iAsI8() > m_iaMaxVal[i_AggCol]) {
        m_iaMaxVal[i_AggCol] = xVal.iAsI8();
        m_iaMaxKey[i_AggCol] = xKey.iAsI8();
      }
      m_iaSumVal[i_AggCol] += xVal.iAsI8();
    }
    
    m_nRows++;
    
  }
  
  // compute average
  for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++)
    m_iaAvgVal[i_AggCol] = m_iaSumVal[i_AggCol] / m_nRows;
  
  // temp trace
  
//  Time oDateTime;
//  oDateTime.vSetLocal(&m_oSince);
//  s32 sDateTime;
//  char* z = sDateTime.aChar();
//  oDateTime.vAppendLocal(&z);
  
//  vTrace("%s\n"_Stz,sDateTime.aChar());
//  for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {
//    pAggColumn = m_pAggColVec->pGet(i_AggCol);
//    vTrace("AggCol %s: min: %lld %lld; max: %lld %lld; avg: %lld\n"_Stz,
//           pAggColumn->sColumnName().aChar(),
//           m_iaMinKey[i_AggCol], m_iaMinVal[i_AggCol],
//           m_iaMaxKey[i_AggCol], m_iaMaxVal[i_AggCol],
//           m_iaAvgVal[i_AggCol]
//           );
//  }
  
  
  //vTrace("Slot %lld: published %lld rows\n"_Stz,
  //       i_Slot, pEntryBuffer->nItems());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
}

/*----------------------------------------------------------------------------*/
/* output the aggregation                                                     */
/*----------------------------------------------------------------------------*/
void RowAgg::vOutput(File& oOutputFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  bool bWritten = oOutputFile.bWriteBytes(this,sizeof(RowAgg));
  ASSERT(bWritten);
}

/*----------------------------------------------------------------------------*/
/* aggregate an entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void RowAgg::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  m_pTable = pTable;
  
  vDiscoverColumns();
}

/*----------------------------------------------------------------------------*/
/* discover aggregation columns                                               */
/*----------------------------------------------------------------------------*/
void RowAgg::vDiscoverColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  m_pKeyColumn = pColVec->pGet(0);
  
  i8 nTableColumns = pColVec->nItems();
  for (i8 i_Col = 1; i_Col < nTableColumns; i_Col++) {
    
    Column* pColumn = pColVec->pGet(i_Col);
    
    if (-1 == pColumn->iColumnNo())
      continue;
    
    if (CT_INTEGER != pColumn->eColumnType())
      continue;
    
    // keep a pointer to the oroginal table column
    m_pAggColVec->vAdd(pColumn);
  }
  
  i8 nAggColumns = m_pAggColVec->nItems();
  for (i8 i_Col = 1; i_Col < nAggColumns; i_Col++) {
    
    Column* pColumn = m_pAggColVec->pGet(i_Col);
   
    vTrace("Column: %lld %s %s\n"_Stz,
           pColumn->iColumnNo(),
           pColumn->sColumnName().aChar(),
           aCT[pColumn->eColumnType()]);
  }
}

/*----------------------------------------------------------------------------*/
/* reset the aggregation                                                      */
/*----------------------------------------------------------------------------*/
void RowAgg::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  m_oSince.vSet(0,0);
  
  m_nRows = 0;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLAGG; i_Col++) {
    
    m_iaMinKey[i_Col] = -1;
    m_iaMinVal[i_Col] = C_I8_MAX;
    
    m_iaSumVal[i_Col] = 0;
    m_iaAvgVal[i_Col] = 0;
    
    m_iaMaxKey[i_Col] = -1;
    m_iaMaxVal[i_Col] = C_I8_MIN;
  }
  
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowAgg::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
