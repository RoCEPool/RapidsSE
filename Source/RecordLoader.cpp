/*============================================================================*/
/* RecordLoader.cpp -- Record Loader                                          */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RecordLoader* Chunks::_pRecordLoader(void* aAddr)
{
  RecordLoader* pRecordLoader = reinterpret_cast<RecordLoader*>(aAddr);
  
  RASSERT_TYPE(pRecordLoader,CN_RECORDLOADER);
  
  return pRecordLoader;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RecordLoader::operator new(size_t)
{
  RecordLoader* pRecordLoader = (RecordLoader*)HeapMem::pAllocate(CN_RECORDLOADER);
  
  return pPoolCheck<void*>(pRecordLoader);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RecordLoader::operator delete(void* pRecordLoaderAddress)
{
  RecordLoader* pRecordLoader = static_cast<RecordLoader*>(pRecordLoaderAddress);
  
  HeapMem::vFree(pRecordLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordLoader::RecordLoader(Formatters* pFormatters)
: RecordExtractor(CN_RECORDLOADER,pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
 
  _checkInitialised_(CN_RECORDLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordLoader::~RecordLoader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void RecordLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  //RecordLoader* pRecordLoader = _pRecordLoader(pObject);
  
  //vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RecordLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
