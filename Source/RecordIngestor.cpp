/*============================================================================*/
/* RecordIngestor.cpp -- Record Ingestor                                       */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RecordIngestor* Chunks::_pRecordIngestor(void* aAddr)
{
  RecordIngestor* pRecordIngestor = reinterpret_cast<RecordIngestor*>(aAddr);
  
  RASSERT_TYPE(pRecordIngestor,CN_RECORDINGESTOR);
  
  return pRecordIngestor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RecordIngestor::operator new(size_t)
{
  RecordIngestor* pRecordIngestor = (RecordIngestor*)HeapMem::pAllocate(CN_RECORDINGESTOR);
  
  return pPoolCheck<void*>(pRecordIngestor);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RecordIngestor::operator delete(void* pRecordIngestorAddress)
{
  RecordIngestor* pRecordIngestor = static_cast<RecordIngestor*>(pRecordIngestorAddress);
  
  HeapMem::vFree(pRecordIngestor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordIngestor::RecordIngestor(Formatters* pFormatters)
: RecordExtractor(CN_RECORDINGESTOR,pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDINGESTOR,_FOOTER_);
  
  _checkInitialised_(CN_RECORDINGESTOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORDINGESTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordIngestor::~RecordIngestor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDINGESTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void RecordIngestor::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RecordIngestor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDINGESTOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
