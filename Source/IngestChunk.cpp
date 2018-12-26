/*============================================================================*/
/* IngestChunk.cpp -- Ingest Chunk Buffer                                     */
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
  
  RASSERT_TYPE(pIngestChunk,CN_INGESTCHUNK);
  
  return pIngestChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IngestChunk::operator new(size_t)
{
  IngestChunk* pIngestChunk = (IngestChunk*)HeapMem::pAllocate(CN_INGESTCHUNK);
  
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
  
  _checkInitialised_(CN_INGESTCHUNK);
  
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
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void IngestChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

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
