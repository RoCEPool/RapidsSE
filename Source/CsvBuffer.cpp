/*============================================================================*/
/* CsvBuffer.cpp -- Csv Buffer                                                */
/*============================================================================*/

#include "Buffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CsvBuffer* Buffers::_pCsvBuffer(void* aAddress)
{
  CsvBuffer* pCsvBuffer = reinterpret_cast<CsvBuffer*>(aAddress);
  
  RASSERT_TYPE(pCsvBuffer,CN_CSVBUFFER);
  
  return pCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new row buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* CsvBuffer::operator new(size_t)
{
  CsvBuffer* pCsvBuffer =
    static_cast<CsvBuffer*>(HeapMem::pAllocate(CN_CSVBUFFER));
  
  return pCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* free a row buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void CsvBuffer::operator delete(void* aAddress)
{
  CsvBuffer* pCsvBuffer = static_cast<CsvBuffer*>(aAddress);
  
  HeapMem::vFree(pCsvBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CsvBuffer::CsvBuffer(void)
: DataBuffer(CN_CSVBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  m_nRecords = 0;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_CSVBUFFER);
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_CSVBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CsvBuffer::~CsvBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the recoords pointer                                                   */
/*----------------------------------------------------------------------------*/
char* CsvBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return (char*)m_xWsp.ayArea();
}

/*----------------------------------------------------------------------------*/
/* allot space for a record of given size                                     */
/*----------------------------------------------------------------------------*/
char* CsvBuffer::aAllotRecord(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  char* aRecord = (char*)m_xWsp.ayAllot(nBytes);
  if (aRecord)
    m_nRecords++;
  
  return aRecord;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes                                                   */
/*----------------------------------------------------------------------------*/
i8 CsvBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
CsvBuffer* CsvBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return _pCsvBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* print the rows                                                             */
/*----------------------------------------------------------------------------*/
void CsvBuffer::vPrintRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  char* aData = aDataArea();
  
  vTrace("%s\n"_Stz, aData);
}

/*----------------------------------------------------------------------------*/
/* get number of records in buffer                                            */
/*----------------------------------------------------------------------------*/
i8 CsvBuffer::nRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return m_nRecords;
}

/*----------------------------------------------------------------------------*/
/* convert CswBuffer to a CrwBuffer                                           */
/*----------------------------------------------------------------------------*/
CrwBuffer* CsvBuffer::pToCrwBuffer(Table* pTable, RowCompactor* pRowCompactor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  //vPrintRows();
  
  CrwBuffer* pCrwBuffer = new CrwBuffer;
  pCrwBuffer->vTable(pTable);
  pCrwBuffer->vPartitionNo(0);  //!ALKA
  
  Row* pRow = new Row(pTable->pStreamDef(),C_ALL);

  char* aCursor = aDataArea();
  bool bBlankLine = false;
  Format* pFormat = AM::pFmt();
  pFormat->m_cRecordTerminator = '\n';
  
  pCrwBuffer->vAppendOpen(pRowCompactor, false);
  
  for(i8 i_Record = 0; i_Record < m_nRecords; i_Record++) {
    
    pRow->vResetValues();
    bool bResult = CsvCursor::bExtractNext(pRow, pTable->pStreamDef(), &aCursor, pFormat, bBlankLine);
    RASSERT(bResult);
    
    bResult = pCrwBuffer->bAppendRow(pRow);
    
    if ( ! bResult ) {
      CrwBuffer* pNewCsvBuffer = new CrwBuffer;
      pCrwBuffer->vTable(pTable);
      pCrwBuffer->vPartitionNo(0);  //!ALKA
      
      pCrwBuffer->vNextBuffer(pNewCsvBuffer);
      pCrwBuffer = pNewCsvBuffer;
      bResult = pCrwBuffer->bAppendRow(pRow);
      RASSERT(bResult);
    }
  }
  
  //pCrwBuffer->vDump();
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CsvBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
