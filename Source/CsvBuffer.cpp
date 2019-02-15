/*============================================================================*/
/* CsvBuffer.cpp -- Buffer of csv records                                     */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CsvBuffer* Jobs::_pCsvBuffer(void* aAddr)
{
  CsvBuffer* pCsvBuffer = reinterpret_cast<CsvBuffer*>(aAddr);
  
  return pCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* CsvBuffer::operator new(size_t)
{
  CsvBuffer* pCsvBuffer =
    static_cast<CsvBuffer*>(HeapMem::pAllocate(CN_CSVBUFFER));
  
  return pCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void CsvBuffer::operator delete(void* aAddr)
{
  CsvBuffer* pCsvBuffer = static_cast<CsvBuffer*>(aAddr);
  
  HeapMem::vFree(pCsvBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CsvBuffer::CsvBuffer(void)
: Object(CN_CSVBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  m_nRecords = 0;
  m_nExpectBuffers = C_UNKNOWN;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
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
/* allot space for a record of given size                                     */
/*----------------------------------------------------------------------------*/
char* CsvBuffer::aAllotRecord(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  char* aRecord = m_xWsp.aAllot(nBytes);
  if (aRecord)
    m_nRecords++;
  
  return aRecord;
}

/*----------------------------------------------------------------------------*/
/* get number of expected buffers (for partition)                             */
/*----------------------------------------------------------------------------*/
i8 CsvBuffer::nExpectBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return m_nExpectBuffers;
}

/*----------------------------------------------------------------------------*/
/* get number of expected buffers (for partition)                             */
/*----------------------------------------------------------------------------*/
void CsvBuffer::vExpectBuffers(i8 nExpectBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  m_nExpectBuffers = nExpectBuffers;
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
/* get number of records in buffer                                            */
/*----------------------------------------------------------------------------*/
i8 CsvBuffer::nRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CSVBUFFER,_FOOTER_);
  
  return m_nRecords;
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
