/*============================================================================*/
/* ReadApi.cpp -- Read Consistency API                                        */
/*============================================================================*/

#include "ReadConsistency.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(ReadConsistency,ReadApi,CN_READAPI)
__aNewFunction(ReadApi,CN_READAPI)
__vDeleteFunction(ReadApi,CN_READAPI)
__aFooterFunction(ReadApi)

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
  delete m_pIndexTree;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a row version                                                          */
/*----------------------------------------------------------------------------*/
void ReadApi::vAddCrwVersion(Ref xwCurrentRef, Crw* aCrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
    
  EntWrk xEntVersion;
  
  // -- -- --
  Ref xwVersionRef = xwAddCrvItem(xwCurrentRef,aCrw);
  xEntVersion.vRef(xwVersionRef);
  
  // -- -- --
  xEntVersion.vAppendRef(xwCurrentRef);
  
  t8 tVersionPsp = aCrw->tPerspective();
  xEntVersion.vAppendInt(tVersionPsp);

  // -- -- --
  bool b = m_pIndexTree->bInsertEntry(xEntVersion.aEnt());
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* get a row version given a reference and perspective                        */
/*----------------------------------------------------------------------------*/
Crw* ReadApi::aGetCrwVersion(Ref xwCurrentRef, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  EntWrk xEntWrk;
  
  Crv::vFormFindKey(&xEntWrk,xwCurrentRef,tPerspective);
  
  Ref xwRef = m_pIndexTree->xwGetRef(xEntWrk.aKey());
  
  if (xwRef.bIsNil())
    return nullptr;
  
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
  
  // form the entry
  
  EntWrk xEntWrk;
  
  xEntWrk.vRef(xwVersionRef);

  xEntWrk.vAppendInt(xwCurrentRef.iAsI8());
  xEntWrk.vAppendInt(tVersionPsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the entry into the index tree

  m_pIndexTree->bInsertEntry(xEntWrk.aEnt());
}

/*----------------------------------------------------------------------------*/
/* add a row version                                                          */
/*----------------------------------------------------------------------------*/
Ref ReadApi::xwAddCrvItem(Ref xwCurrentRef, Crw* aCrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  Ref xwVersionRef;
  
  // test for current head buffer
  if (! m_pCrvBufferHead)
    m_pCrvBufferHead = new CrvBuffer(m_pStreamDef);
  
  // attempt to add crw into current head buffer
  i8 i_Crv = m_pCrvBufferHead->iAddCrwVersion(xwCurrentRef,aCrw);
  if (C_UNKNOWN != i_Crv) {
    
    xwVersionRef.vBuffer(*m_pCrvBufferHead->nPoolBufferNo());
    xwVersionRef.vItem(i_Crv);
    
    return xwVersionRef;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // need a new head buffer
  
  m_pIndexTree->vDump(false);
  
  CrvBuffer* pNewCrvBufferHead = new CrvBuffer(m_pStreamDef);
  
  pNewCrvBufferHead->vLinkAsNext(m_pCrvBufferHead);
  m_pCrvBufferHead = pNewCrvBufferHead;
  
  i_Crv = m_pCrvBufferHead->iAddCrwVersion(xwCurrentRef,aCrw);
 
  xwVersionRef.vBuffer(*m_pCrvBufferHead->nPoolBufferNo());
  xwVersionRef.vItem(i_Crv);
  
  return xwVersionRef;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
