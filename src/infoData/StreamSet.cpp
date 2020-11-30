/*============================================================================*/
/* StreamSet.cpp -- Stream Information Set                                    */
/*============================================================================*/

#include "InfoData.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamSet* InfoData::_pStreamSet(void* aAddr)
{
  StreamSet* pStreamSet = reinterpret_cast<StreamSet*>(aAddr);
  
  return pStreamSet;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* StreamSet::operator new(size_t)
{
  StreamSet* pStreamSet =
    static_cast<StreamSet*>(HeapMem::pAllocate(CN_STREAMSET));
  
  return pStreamSet;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void StreamSet::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamSet::StreamSet(void)
: InfoSetBase(CN_STREAMSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMSET);
  
  m_sInfoSetNm = C_NO_NAME;
  m_nRowSize   = C_UNKNOWN;
  m_nInfoRows  = 0;
  
  _checkInitialised_(CN_STREAMSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMSET);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamSet::StreamSet(StreamDef* pStreamDef)
: InfoSetBase(CN_STREAMSET, pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMSET);
  
  m_sInfoSetNm = pStreamDef->szName();
  m_nRowSize  = Row::nSizeBytes(pStreamDef);
  m_nInfoRows = 0;
  
  _checkInitialised_(CN_STREAMSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMSET);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamSet::~StreamSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMSET);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
