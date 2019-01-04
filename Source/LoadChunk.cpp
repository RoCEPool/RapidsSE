/*============================================================================*/
/* LoadChunk.cpp -- LoadChunk                                                 */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoadChunk* Chunks::_pLoadChunk(void* aAddr)
{
  LoadChunk* pLoadChunk = reinterpret_cast<LoadChunk*>(aAddr);
  
  RASSERT_TYPE(pLoadChunk,CN_LOADCHUNK);
  
  return pLoadChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadChunk::operator new(size_t)
{
  LoadChunk* pLoadChunk = (LoadChunk*)HeapMem::pAllocate(CN_LOADCHUNK);
  
  return pPoolCheck<void*>(pLoadChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LoadChunk::operator delete(void* pLoadChunkAddress)
{
  LoadChunk* pLoadChunk = static_cast<LoadChunk*>(pLoadChunkAddress);
  
  HeapMem::vFree(pLoadChunk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadChunk::LoadChunk(Formatters* pFormatters)
: FileChunk(CN_LOADCHUNK,pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
 
  _checkInitialised_(CN_LOADCHUNK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadChunk::~LoadChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void LoadChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  //LoadChunk* pLoadChunk = _pLoadChunk(pObject);
  
  //vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LoadChunk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
