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
  
  Index* pReadApiIndex = AM::pReadApiIndex();
  
  // -- -- --
  
  m_pStreamDef = nullptr;
  
  m_pIndexTree = new IndexTree(pReadApiIndex,0);
  
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
    
  // test for current head buffer
  if (! m_pCrvBufferHead)
    m_pCrvBufferHead = new CrvBuffer(m_pStreamDef);
  
  // attempt to add crw into current head buffer
  if (m_pCrvBufferHead->bAddCrwVersion(xwRef,pCrw))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // need a new head buffer
  
  CrvBuffer* pNewCrvBufferHead = new CrvBuffer(m_pStreamDef);
  
  pNewCrvBufferHead->vLinkAsNext(m_pCrvBufferHead);
  m_pCrvBufferHead = pNewCrvBufferHead;
  
  bool b = m_pCrvBufferHead->bAddCrwVersion(xwRef,pCrw);
  ASSERT(b);
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an index entry for version                                             */
/*----------------------------------------------------------------------------*/
void ReadApi::vAddVersionEntry(Ref xwCurrentRef,
                               Ref xwVersionRef, t8 tVersionPsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  EntWrk xEntWrk;
  
  xEntWrk.vRef(xwVersionRef);

  xEntWrk.vAppendInt(xwCurrentRef.iAsI8());
  xEntWrk.vAppendInt(tVersionPsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ReadApi);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
