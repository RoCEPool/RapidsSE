/*============================================================================*/
/* RowAgg.hpp -- Row Aggregation                                              */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowAgg* Jobs::_pRowAgg(void* aAddr)
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
/* add an aggregate to this aggrow                                            */
/*----------------------------------------------------------------------------*/
void RowAgg::vAdd(RowAgg* pAggArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  if (! m_pTable) {
    m_pTable     = pAggArg->m_pTable;
    m_pKeyColumn = pAggArg->m_pKeyColumn;
    m_pAggColVec = pAggArg->m_pAggColVec;
  }
  
  m_nRows += pAggArg->m_nRows;
  
  i8 nAggColumns = m_pAggColVec->nItems();
  
  for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {

    if (pAggArg->m_iaMinVal[i_AggCol] < m_iaMinVal[i_AggCol]) {
      m_iaMinVal[i_AggCol] = pAggArg->m_iaMinVal[i_AggCol];
      m_iaMinKey[i_AggCol] = pAggArg->m_iaMinKey[i_AggCol];
    }

    if (pAggArg->m_iaMaxVal[i_AggCol] > m_iaMaxVal[i_AggCol]) {
      m_iaMaxVal[i_AggCol] = pAggArg->m_iaMaxVal[i_AggCol];
      m_iaMaxKey[i_AggCol] = pAggArg->m_iaMaxKey[i_AggCol];
    }
    
    m_iaSumVal[i_AggCol] += pAggArg->m_iaSumVal[i_AggCol];
    m_iaAvgVal[i_AggCol] = m_iaSumVal[i_AggCol] / m_nRows;
  }
}

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
  RowData* aRowData;
  
  i8 nAggColumns = m_pAggColVec->nItems();
  
  vReset();
  
  m_oSince = pEntryBuffer->oWhenNewed();
  
  // aggregate the buffer
  for (Item* aItem = pEntryBuffer->aLeadEntry();
       aItem != nullptr;
       aItem = pEntryBuffer->aNextEntry(aItem, 0)) {
    
    aRowData = _aRowData(aItem->aDataArea());
    
    //pRow->vAsBinaryStringVLQ(sRowString, m_pTable->pColumnVec());
    //vTrace("row: (%s)\n"_Stz, sRowString.aChar());
    
    aSrc = (byte*)aRowData->aColumnDatum(m_pKeyColumn->iColumnNo());
    //xKey = IntLib::iVlqToInt(aSrc);
    xKey = *((i8*)aSrc);

    for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {
      
      pAggColumn = m_pAggColVec->pGet(i_AggCol);
      i_Col = pAggColumn->iColumnNo();
      
      aSrc = (byte*)aRowData->aColumnDatum(i_Col);
      //xVal = IntLib::iVlqToInt(aSrc);
      xVal = *((i8*)aSrc);
    
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
/* print this aggregation out                                                 */
/*----------------------------------------------------------------------------*/
void RowAgg::vPrintOut(File& oOutputFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  i8 nAggColumns = m_pAggColVec->nItems();

//  Time oDateTime;
//  oDateTime.vSetLocal(&m_oSince);
//  s32 sDateTime;
//  char* z = sDateTime.aChar();
//  oDateTime.vAppendLocal(&z);

  vTrace("  %lld rows\n"_Stz,m_nRows);
  for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {
    
    Column* pAggColumn = m_pAggColVec->pGet(i_AggCol);
    vTrace("  %18s: min: %4lld (key=%4lld); max: %4lld (key=%4lld); avg: %6lld\n"_Stz,
           pAggColumn->sColumnName().aChar(),
           m_iaMinVal[i_AggCol], m_iaMinKey[i_AggCol],
           m_iaMaxVal[i_AggCol], m_iaMaxKey[i_AggCol],
           m_iaAvgVal[i_AggCol]
           );
  }

}

/*----------------------------------------------------------------------------*/
/* wread into this aggregation                                                */
/*----------------------------------------------------------------------------*/
void RowAgg::vReadIn(File& oInFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  i8 nBytesRead = oInFile.nReadBytes(this,sizeof(RowAgg));
  ASSERT(nBytesRead == sizeof(RowAgg));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
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
/* set table to be aggregated                                                 */
/*----------------------------------------------------------------------------*/
void RowAgg::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  m_pTable = pTable;
  
  vDiscoverColumns();
}

/*----------------------------------------------------------------------------*/
/* write this aggregation out                                                 */
/*----------------------------------------------------------------------------*/
void RowAgg::vWriteOut(File& oOutputFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
  
  bool bWritten = oOutputFile.bWriteBytes(this,sizeof(RowAgg));
  ASSERT(bWritten);
  
  oOutputFile.vFlush();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWAGG,_FOOTER_);
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
  
//  i8 nAggColumns = m_pAggColVec->nItems();
//  for (i8 i_Col = 1; i_Col < nAggColumns; i_Col++) {
//    
//    Column* pColumn = m_pAggColVec->pGet(i_Col);
//   
//    vTrace("Column: %lld %s %s\n"_Stz,
//           pColumn->iColumnNo(),
//           pColumn->sColumnName().aChar(),
//           aCT[pColumn->eColumnType()]);
//  }
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
