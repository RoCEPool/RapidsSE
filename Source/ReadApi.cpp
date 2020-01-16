/*============================================================================*/
/* ReadApi.cpp -- Read Consistency API                                        */
/*============================================================================*/

#include "ReadConsistency.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ReadApi* ReadConsistency::_pReadApi(void* aAddr)
{
  ReadApi* pReadApi = reinterpret_cast<ReadApi*>(aAddr);
  
  return pReadApi;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ReadApi::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_READAPI);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ReadApi::operator delete(void* aAddr)
{
  ReadApi* pReadApi = reinterpret_cast<ReadApi*>(aAddr);
  
  HeapMem::vFree(pReadApi);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ReadApi::ReadApi(void)
: Object(CN_READAPI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  m_pStreamDef = nullptr;
  
  m_pCrvBufferHead = nullptr;
  
  _checkInitialised_(CN_READAPI);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ReadApi::~ReadApi(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a row version                                                          */
/*----------------------------------------------------------------------------*/
void ReadApi::vAddCrwVersion(Ref xwRef, Crw* pCrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  ASSERT(m_pStreamDef);
  
  i8 nFixedCrwSize = m_pStreamDef->m_nFixedSized;
  
  if (! m_pCrvBufferHead)
    m_pCrvBufferHead = new CrvBuffer;
  
  if (! m_pCrvBufferHead->bSpaceFor(nFixedCrwSize)) {
    CrvBuffer* pNewCrvBufferHead = new CrvBuffer;
    
    pNewCrvBufferHead->vLinkAsNext(m_pCrvBufferHead);
    m_pCrvBufferHead = pNewCrvBufferHead;
  }
  
  m_pCrvBufferHead->vAddCrwVersion(xwRef,pCrw,m_pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* get a row version given a reference and perspective                        */
/*----------------------------------------------------------------------------*/
Crw* ReadApi::aGetCrwVersion(Ref xwRef, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a row version given a reference and perspective                        */
/*----------------------------------------------------------------------------*/
void ReadApi::vPurgePriorVersions(t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* set steam definition                                                       */
/*----------------------------------------------------------------------------*/
void ReadApi::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ReadApi);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
