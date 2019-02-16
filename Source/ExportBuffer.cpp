/*============================================================================*/
/* ExportBuffer.cpp -- Buffer of csv records to be exported                   */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ExportBuffer* Jobs::_pExportBuffer(void* aAddr)
{
  ExportBuffer* pExportBuffer = reinterpret_cast<ExportBuffer*>(aAddr);
  
  return pExportBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ExportBuffer::operator new(size_t)
{
  ExportBuffer* pExportBuffer =
    static_cast<ExportBuffer*>(HeapMem::pAllocate(CN_EXPORTBUFFER));
  
  return pExportBuffer;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void ExportBuffer::operator delete(void* aAddr)
{
  ExportBuffer* pExportBuffer = static_cast<ExportBuffer*>(aAddr);
  
  HeapMem::vFree(pExportBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExportBuffer::ExportBuffer(void)
: Object(CN_EXPORTBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  m_nRecords = 0;
  m_nExpectBuffers = C_UNKNOWN;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExportBuffer::~ExportBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the recoords pointer                                                   */
/*----------------------------------------------------------------------------*/
char* ExportBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* allot space for a record of given size                                     */
/*----------------------------------------------------------------------------*/
char* ExportBuffer::aAllotRecord(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  char* aRecord = m_xWsp.aAllot(nBytes);
  if (aRecord)
    m_nRecords++;
  
  return aRecord;
}

/*----------------------------------------------------------------------------*/
/* get number of expected buffers (for partition)                             */
/*----------------------------------------------------------------------------*/
i8 ExportBuffer::nExpectBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  return m_nExpectBuffers;
}

/*----------------------------------------------------------------------------*/
/* get number of expected buffers (for partition)                             */
/*----------------------------------------------------------------------------*/
void ExportBuffer::vExpectBuffers(i8 nExpectBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  m_nExpectBuffers = nExpectBuffers;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes                                                   */
/*----------------------------------------------------------------------------*/
i8 ExportBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get number of records in buffer                                            */
/*----------------------------------------------------------------------------*/
i8 ExportBuffer::nRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
  
  return m_nRecords;
}

/*----------------------------------------------------------------------------*/
/* correct this csv buffer after being received                               */
/*----------------------------------------------------------------------------*/
void ExportBuffer::vRecvPostCorrection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ExportBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPORTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
