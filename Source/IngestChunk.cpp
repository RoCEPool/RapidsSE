/*============================================================================*/
/* IngestChunk.cpp -- Ingest Chunk                                            */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IngestChunk* Chunks::_pIngestChunk(void* aAddr)
{
  IngestChunk* pIngestChunk = reinterpret_cast<IngestChunk*>(aAddr);
  
  return pIngestChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IngestChunk::operator new(size_t)
{
  IngestChunk* pIngestChunk = (IngestChunk*)HeapMem::pAllocate(CN_IOCHUNK);
  
  return pPoolCheck<void*>(pIngestChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IngestChunk::operator delete(void* pIngestChunkAddress)
{
  IngestChunk* pIngestChunk = static_cast<IngestChunk*>(pIngestChunkAddress);
  
  HeapMem::vFree(pIngestChunk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IngestChunk::IngestChunk(Formatters* pFormatters)
: RecordExtractor(CN_INGESTCHUNK,pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);

  m_iChunkNo = 0;
  
  //m_oFile
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IngestChunk::~IngestChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
i8 IngestChunk::nReadMore(File& oFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK);
  
  i8 nCarryOverBytes = m_nDataBytes;
  
  void* aReadDst = pAdd<void*>(aDataArea(),nCarryOverBytes);
  
  i8 nBytesToRead = C_DATA_CHUNK - nCarryOverBytes;
  
  i8 nBytesRead = oFile.nReadBytes(aReadDst,nBytesToRead);
  
  vSetBytes(nCarryOverBytes + nBytesRead);
  
  m_aNextRecord = (Stz*)aDataArea();
  
  if (! nBytesRead)
    return 0;

  return nBytesRead;
}

/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IngestChunk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
