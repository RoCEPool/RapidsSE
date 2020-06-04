/*============================================================================*/
/* CsvBuffer.cpp -- Csv Buffer                                                */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,CsvBuffer,CN_CSVBUFFER)
__aNewFunction(CsvBuffer,CN_CSVBUFFER)
__vDeleteFunction(CsvBuffer,CN_CSVBUFFER)
__aFooterFunction(CsvBuffer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CsvBuffer::CsvBuffer(void)
: DataBuffer(CN_CSVBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  m_sReportNm = C_NO_NAME;
  m_nRecords  = 0;
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot space for nBytes record , returning nullptr if insufficient space    */
/*----------------------------------------------------------------------------*/
Stz* CsvBuffer::szAllotRecord(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  Stz* szRecord = (Stz*)m_xWsp.ayAllot(nBytes);
  if (! szRecord)
    return nullptr;
  
  m_nRecords++;
  
  // Linuxerror: error:
  //   ambiguous overload for 'operator=' (operand types are 'Strings::Stz' and 'char')
  // *szRecord = '\0';
  char* cRecord = (char*)szRecord;
  *cRecord = '\0';
  
  return szRecord;
}

/*----------------------------------------------------------------------------*/
/* append record, returning nullptr if insufficient space                     */
/*----------------------------------------------------------------------------*/
Stz* CsvBuffer::szAppendRecord(Stz* szRecord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  i8 nSize = szRecord->nSize();

  if (nFreeBytes() < nSize)
    return nullptr;
  
  Stz* szAllotted = szAllotRecord(nSize - 1);
  
  StrLib::vCopy(szAllotted->azChar(),szRecord->azChar());

  return szAllotted;
}

/*----------------------------------------------------------------------------*/
/* get the records pointer                                                    */
/*----------------------------------------------------------------------------*/
byte* CsvBuffer::ayDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return m_xWsp.ayArea();
}

/*----------------------------------------------------------------------------*/
/* get the recoords pointer                                                   */
/*----------------------------------------------------------------------------*/
void CsvBuffer::vTake(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  m_xWsp.vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get number of data bytes                                                   */
/*----------------------------------------------------------------------------*/
i8 CsvBuffer::nDataBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  i8 nDataBytes = m_xWsp.nUsed();
  
  return nDataBytes;
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
/* print the records                                                          */
/*----------------------------------------------------------------------------*/
void CsvBuffer::vPrintRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  char* aData = (char*)ayDataArea();
  
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
/* get the records pointer                                                    */
/*----------------------------------------------------------------------------*/
Stz* CsvBuffer::szRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return (Stz*) m_xWsp.ayArea();
}

/*----------------------------------------------------------------------------*/
/* get optional report name                                                   */
/*----------------------------------------------------------------------------*/
Stz* CsvBuffer::szReportNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
    
  return m_sReportNm;
}

/*----------------------------------------------------------------------------*/
/* get optional report name                                                   */
/*----------------------------------------------------------------------------*/
void CsvBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);

  m_nRecords = 0;
  
  m_xWsp.vReset();
}

/*----------------------------------------------------------------------------*/
/* set optional report name                                                   */
/*----------------------------------------------------------------------------*/
void CsvBuffer::vReportNm(Stz* szReportNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
    
  m_sReportNm = szReportNm;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
