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
  m_pCrvBufferTail = nullptr;
  
  _checkInitialised_(CN_READAPI);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ReadApi::~ReadApi(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);

  delete m_pIndexTree;
  
  for (CrvBuffer* pCrvBuffer = m_pCrvBufferTail; pCrvBuffer; /*none*/) {
    CrvBuffer* pCrvBufferTBD = pCrvBuffer;
    pCrvBuffer = _pCrvBuffer(pCrvBuffer->pNextBuffer());
    delete pCrvBufferTBD;
  }
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  Crw* aCrwXCheck = aGetCrwVersion(xwCurrentRef,tVersionPsp);
  
  i8 nCrwBytes = m_pStreamDef->m_nFixedSized;
  
  //MemLib::vHexDump(aCrw,nCrwBytes);
  //MemLib::vHexDump(aCrwXCheck,nCrwBytes);

  b = MemLib::bCompareBytes(aCrw,aCrwXCheck,nCrwBytes);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* get a consistent read version of row given a reference and perspective     */
/*----------------------------------------------------------------------------*/
Crw* ReadApi::aGetCrwConsistent(Ref xwCurrentRef, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  EntWrk xEntWrk;
  
  Crv::vFormFindKey(&xEntWrk,xwCurrentRef,tPerspective);
  
  Ref xwRef = m_pIndexTree->xwGetRef(xEntWrk.aKey());
  
  if (xwRef.bIsNil())
    return nullptr;
  
  i8 i_Buffer = xwRef.iBuffer();
  i8 i_Item   = xwRef.iItem();
  
  Crv* aCrv = CrvBuffer::aToCrv(i_Buffer,i_Item);
  
  Crw* aCrw = aCrv->aCrw();

  return aCrw;
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
  
  i8 i_Buffer = xwRef.iBuffer();
  i8 i_Item   = xwRef.iItem();
  
  Crv* aCrv = CrvBuffer::aToCrv(i_Buffer,i_Item);
  
  Crw* aCrw = aCrv->aCrw();

  return aCrw;
}

/*----------------------------------------------------------------------------*/
/* get a row version given a reference and perspective                        */
/*----------------------------------------------------------------------------*/
Crw* ReadApi::aGetCrwVersion(Ref xwCurrentRef, Crw* aCurrentCrw, t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  i8 tCrwPsp = aCurrentCrw->tPerspective();
  if (tCrwPsp <= tPerspective)
    return aCurrentCrw;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntWrk xEntWrk;
  
  Crv::vFormFindKey(&xEntWrk,xwCurrentRef,tPerspective);
  
  Ref xwRef = m_pIndexTree->xwGetRef(xEntWrk.aKey());
  
  if (xwRef.bIsNil())
    return nullptr;
  
  i8 i_Buffer = xwRef.iBuffer();
  i8 i_Item   = xwRef.iItem();
  
  Crv* aCrv = CrvBuffer::aToCrv(i_Buffer,i_Item);
  
  Crw* aCrw = aCrv->aCrw();

  return aCrw;
}

/*----------------------------------------------------------------------------*/
/* get a row version given a reference and perspective                        */
/*----------------------------------------------------------------------------*/
void ReadApi::vPurgePriorVersions(t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);

  //m_pIndexTree->vDump(false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Purge RC INDEX entry buffers
  vPurgeEntBuffers(tPerspective);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Purge RC INDEX Crv Buffers
  vPurgeCrvBuffers(tPerspective);
  
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
  
  if ( ! m_pCrvBufferTail)
    m_pCrvBufferTail = m_pCrvBufferHead;
  
  // attempt to add crw into current head buffer
  i8 i_Crv = m_pCrvBufferTail->iAddCrwVersion(xwCurrentRef,aCrw);
  if (C_UNKNOWN != i_Crv) {
    
    xwVersionRef.vBuffer(m_pCrvBufferTail->iBufferNo());
    xwVersionRef.vItem(i_Crv);
    
    return xwVersionRef;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // need a new head buffer
  
  //m_pIndexTree->vDump(false);
  
  CrvBuffer* pNewCrvBufferTail = new CrvBuffer(m_pStreamDef);
  
  pNewCrvBufferTail->vLinkAsPrev(m_pCrvBufferTail);
  m_pCrvBufferTail = pNewCrvBufferTail;
  
  i_Crv = m_pCrvBufferTail->iAddCrwVersion(xwCurrentRef,aCrw);
 
  xwVersionRef.vBuffer(m_pCrvBufferTail->iBufferNo());
  xwVersionRef.vItem(i_Crv);
  
  return xwVersionRef;
}

/*----------------------------------------------------------------------------*/
/* purge the RC index index buffers                                           */
/*----------------------------------------------------------------------------*/
void ReadApi::vPurgeEntBuffers(t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  Ent* aEntNext;
  EntBuffer* pRootBuffer;
  
  IdxColVec* pIdxColVec = m_pIndexTree->pIndex()->pIndexDef()->pIdxColVec();

  // first position at the first level 0 buffer
  EntBuffer* pEntBuffer = m_pIndexTree->pFirstH0Buffer();
  
  while (pEntBuffer) {
    
    Ent* aEnt = pEntBuffer->aLeadEntry();
    
    while (aEnt) {
      
      // get the key
      Key* aKey = aEnt->aKey();
      
      // get the psp of the Crv entry
      t8 tCrvPsp = aKey->iGetInt(pIdxColVec, 1);
      
      // delete if the CrvPsp is less than the tPerspective
      if (tCrvPsp < tPerspective) {
        
#ifdef WITH_TRACE_PURGING
        vTrace("PURGING...\n"_Stz);
        aKey->vShow(pIdxColVec);
        vTrace("\n"_Stz);
#endif
        
        // get the next entry before deleting
        aEntNext = pEntBuffer->aNextEntry(aEnt, 0);
        
        // delete the entry upwards in the skiplist
        pEntBuffer->vDeleteUpwards(aEnt, &pRootBuffer);
      }
      else {
        
#ifdef WITH_TRACE_PURGING
        vTrace("NOT PURGING\n"_Stz);
        aKey->vShow(pIdxColVec);
        vTrace("\n"_Stz);
#endif
        // get the next entry
        aEntNext = pEntBuffer->aNextEntry(aEnt, 0);
      }
      
      // set the next entry
      aEnt = aEntNext;
    }
    
    // move to the next buffer
    pEntBuffer = pEntBuffer->pNextBuffer();
  }
}
  
/*----------------------------------------------------------------------------*/
/* purge the RC Index CrvBuffers                                              */
/*----------------------------------------------------------------------------*/
void ReadApi::vPurgeCrvBuffers(t8 tPerspective)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_READAPI,_FOOTER_);
  
  CrvBuffer* pCrvBuffer = m_pCrvBufferHead;
  CrvBuffer* pCrvPrevBuffer = nullptr;
  
  while (pCrvBuffer) {
    
    // get the last crv entry from the buffer
    i8 iCrv  = pCrvBuffer->nCrv();
    Crv* aCrv = pCrvBuffer->aGetCrv(iCrv);
    
    // get the Crw entry corresoonding to the Crv entry
    Crw* aCrw = aCrv->aCrw();
    
    // get the Crw perspective
    i8 tCrvPsp = aCrw->tPerspective();
    
    // save the prevBuffer
    pCrvPrevBuffer = pCrvBuffer->pPrevBuffer();
    
    // check if psp of last entry is older than the given psp
    if (tCrvPsp < tPerspective) {

#ifdef WITH_TRACE_PURGING
      vTrace("DELETING CRV BUFFER %p...\n"_Stz, pCrvBuffer);
#endif
      
      // unlink the CrvBuffer
      pCrvBuffer->vUnLinkFromPrev();
      
      // should be the head
      ASSERT(pCrvBuffer == m_pCrvBufferHead);
      
      // set the new head
      m_pCrvBufferHead = pCrvPrevBuffer;
      
      // was it the tail ?
      if (pCrvBuffer == m_pCrvBufferTail) {
        ASSERT( ! pCrvPrevBuffer );
        m_pCrvBufferTail = nullptr;
      }
      
      // now delete the CrvBuffer
      delete pCrvBuffer;
    }
    
    // move to the prevBuffer
    pCrvBuffer = pCrvPrevBuffer;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
