/*============================================================================*/
/* EntBuffer.cpp -- Index Entry Buffer                                        */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "RowSingle.hpp"
#include "EntWrk.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

bool mbWatch = false;


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntBuffer* STORE::_pEntBuffer(void* aAddress)
{
  EntBuffer* pEntBuffer = reinterpret_cast<EntBuffer*>(aAddress);
  
  RASSERT_TYPE(pEntBuffer,CN_ENTBUFFER);
  
  return pEntBuffer;
}

void* aEntBuffer_nextEntry(void** ppEntBuffer, void* aPrevEntry) {

  return EntBuffer::aGetNextEntry((EntBuffer**) ppEntBuffer, (Ent*) aPrevEntry);
/*
  EntBuffer* pEntBuffer = _pEntBuffer(*ppEntBuffer);
  Ent* aCurrent = (Ent*)aPrevEntry;

  tET tNextEntry = aCurrent->m_taNextEntry[0];

  if (C_ENT_END_TICKET == tNextEntry)
    return nullptr;

  i8 iNextEntryOffset = (i8)tNextEntry * C_ENT_QUANTA;

  Ent* aNextEntry = (Ent*) ((char*)pEntBuffer + 624 + iNextEntryOffset);

  //Ent *aNextEntry = pEntBuffer->aNextEntry_flat((Ent*)(aPrevEntry), 0);

  if (! aNextEntry ) {
    pEntBuffer = pEntBuffer->pNextEntBuffer();
    *ppEntBuffer = pEntBuffer;
    if (pEntBuffer) {
      tET tNextEntry = aCurrent->m_taNextEntry[0];
      i8 iNextEntryOffset = (i8)(&pEntBuffer->m_xSkipRoots) * C_ENT_QUANTA;
      aNextEntry = (Ent*) ((char*)pEntBuffer + 624 + iNextEntryOffset);
    }

  }

  return aNextEntry;
  */
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* EntBuffer::operator new(size_t)
{
  EntBuffer* pEntBuffer =
    static_cast<EntBuffer*>(Heaps::pAllocate(CN_ENTBUFFER));
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* EntBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void EntBuffer::operator delete(void* aAddress)
{
  EntBuffer* pEntBuffer = static_cast<EntBuffer*>(aAddress);
  
  Heaps::vFree(pEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntBuffer::EntBuffer(IndexTree* pIndexTree)
: DataBuffer(CN_ENTBUFFER, true)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(CacheLine::bOnCacheLineBoundary(this));
  
  m_pIndexTree  = pIndexTree;
  m_eEntKind    = pIndexTree->pIndexDef()->eKeyKind();

  // zero the m_cSpaceForTx
  MemLib::vZero(m_cSpaceForTx, sizeof(m_cSpaceForTx), CN_ENTBUFFER);

  //m_xSkipRoots
  m_aLazyChain  = nullptr;
  
  
  m_nInsert             = 0;
  m_nSplit              = 0;
  m_nMove               = 0;
  m_nDelete             = 0;
  m_nInsertEntUpwards   = 0;
  m_nNewInsertIntoSplit = 0;
  m_nNewInsertIntoRight = 0;


  if (AM::nIndexEntries() != 0)
    vMaxItems(AM::nIndexEntries());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_pIndexTree->vEntBuffersInc();
  
  _checkInitialised_(CN_ENTBUFFER);
  
  AM::METRICS::vDerefMetrics((i8)this,CN_ENTBUFFER,DR_NEW);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntBuffer::~EntBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //vTrace("::~EntBuffer %p %lld\n"_Stz,this,iBufferNo());
  
  m_pIndexTree->vEntBuffersDec();
  
  AM::METRICS::vDerefMetrics((i8)this,CN_ENTBUFFER,DR_DELETE);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy all keys of right buffer to left buffer; create new left when needed  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vCopyLeftH0(EntBuffer** ppLeftH0, EntBuffer* pRightH0)
{
  _static_
  
  TRACE_COMPACT( vTrace("IndexTree::vCopyLeft (right %lld)\n"_Stz,
                        pRightL0->iBufferNo()); )
  
  EntWrk xLeftEntWrk;
  
  EntBuffer* pLeftH0 = *ppLeftH0;
  
  Index* pIndex = pRightH0->pIndex();
  ASSERT(pLeftH0->pIndex() == pIndex);
  UNUSED(pIndex);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert the parameters are sensible
  
  ASSERT(0 == pLeftH0->iBufferHeight());
  ASSERT(0 == pRightH0->iBufferHeight());
  EntBuffer* pH1 = pLeftH0->pHigherEntBuffer();
  ASSERT(1 == pH1->iBufferHeight());
  ASSERT(pRightH0->pHigherEntBuffer() == pLeftH0->pHigherEntBuffer());
  
  //vTrace("IndexTree::vCopyLeft (left %lld right %lld)\n"_Stz,
  //       pLeftL0->iBufferNo(),pRightL0->iBufferNo());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set up for using pInsertKeyAbsL0
  
  Ent* aPriorEntry = pLeftH0->aLastEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru each key in right L0
  
  TRACE_COMPACT( pRightL0->vShow(); )
  
  //pLeftL0->vShow();
  
  for (Ent* aRightH0_Entry = pRightH0->aLeadEntry();
       aRightH0_Entry;
       aRightH0_Entry = pRightH0->aNextEntry(aRightH0_Entry,0)) {
    
    TRACE_COMPACT( aRightH0_Entry->vShow(0, pIndex, true); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the elements of the right key
    
    Rrf xRef = aRightH0_Entry->rxRef();
    eKE eEntKind = xRef.eKind();
    Dtm* aElmSeq = aRightH0_Entry->aElmSeq_dtm();
    
    i8 nElmSeqSize = aRightH0_Entry->nElmSeqSize();
    i8 nOffsets = aRightH0_Entry->nSkipLevels();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form the working version of the left key
    
    xLeftEntWrk.vRef(xRef);
    
    if (KE_GENERAL == eEntKind) {
      xLeftEntWrk.vElmSeq_dtm(nElmSeqSize,aElmSeq);
    }
    else {
      i8 iVal = aRightH0_Entry->iIntElm0();
      *(i8*)xLeftEntWrk.aKey() = iVal;
    }

    xLeftEntWrk.vOffsets(nOffsets);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // compute the size of the left key
    
    i8 nEntrySizeBytes = xLeftEntWrk.aEnt()->nSizeBytes();
    RASSERT(nEntrySizeBytes <= C_MAX_ENTRY);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a new left buffer if not enough space in current left buffer
    
    if (! pLeftH0->bHasSpace(nEntrySizeBytes)) {
      
      //pLeftH0->bVerifyItems();
      
     // build the skip list for left L0 thats now full
      pLeftH0->vBuildSkipListSorted();
      
      //pLeftL0->vShow();
      
      // new left buffer
      EntBuffer* pNewLeftH0 = new EntBuffer(pLeftH0->m_pIndexTree);
      pNewLeftH0->vHigherEntBuffer(pH1);
      
      // splice the new left buffer into the L0 list
      pLeftH0->vSpliceAfter(pNewLeftH0);
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      // adjust variables
      *ppLeftH0 = pNewLeftH0;
      pLeftH0 = pNewLeftH0;
      
      aPriorEntry = pLeftH0->aLastEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a copy of the right key into left buffer
    
    TRACE_COMPACT( pLeftL0->vShow(); )
    Ent* aInsertedEntry = pLeftH0->aInsertEntAbsL0(xLeftEntWrk.aEnt(),&aPriorEntry);
    UNUSED(aInsertedEntry);
    
    TRACE_COMPACT( pLeftKeyEntryL0->vShow(0, pIndex, true); )
    TRACE_COMPACT( pLeftL0->vShow(); )
    
    //pLeftL0->vShow();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert a first L0 key into H1
    
    if (1 == pLeftH0->nItems()) {
      
      i8 iLeftL0BufferNo = pLeftH0->iBufferNo();
      xRef.vBuffer(iLeftL0BufferNo);
      xLeftEntWrk.vRef(xRef);
      
      TRACE_COMPACT( pH1->vShow(); )
      //ASSERT(pH1->bVerifySkipListEntry());
      Ent* aLeftEntryH1 = pH1->aInsertEntAbs(xLeftEntWrk.aEnt());
      UNUSED(aLeftEntryH1);
      //ASSERT(pH1->bVerifySkipListEntry());
      TRACE_COMPACT( pLeftEntryH1->vShow(0, pIndex, true); )
      TRACE_COMPACT( pH1->vShow(); )
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
      
      //pH1->vShowShort();
      
      //pH1->vShow();
      
      //pH1->vShowTree();
      
      //pH1->bVerifyKeyBufferOrder();
      
      // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    }
    
    //pH1->bVerifyKeyBufferOrder();
    
  } // for
  
  //pLeftH0->bVerifyItems();
}

/*----------------------------------------------------------------------------*/
/* reinitialise the H1 buffer with just the first key of the first H0 buffer  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vCopyReInitH1(EntBuffer* pH1, EntBuffer* pFirstH0)
{
  _static_
  
  ASSERT(0 == pFirstH0->iBufferHeight());
  ASSERT(1 == pH1->iBufferHeight());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset the H1 buffer
  
  pH1->vReset();
  //pH1->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reinsert the first key of the first L0 buffer back into the H1 buffer
  
  Ent* aFirstL0_ent = pFirstH0->aLeadEntry();
  
  Rrf xRef = aFirstL0_ent->rxRef();
  eKE eEntKind = xRef.eKind();
  Dtm* aElmSeq = aFirstL0_ent->aElmSeq_dtm();
  i8 nElmSeqSize = aFirstL0_ent->nElmSeqSize();
  i8 nLevels = aFirstL0_ent->nSkipLevels();
  
  i8 iFirstL0_BufferNo = pFirstH0->iBufferNo();
  xRef.vBuffer(iFirstL0_BufferNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntWrk xFirstH1_ent;
  
  xFirstH1_ent.vRef(xRef);
  
  if (KE_GENERAL == eEntKind) {
    xFirstH1_ent.vElmSeq_dtm(nElmSeqSize,aElmSeq);
  }
  else {
    i8 iVal = aFirstL0_ent->iIntElm0();
    *(i8*)xFirstH1_ent.aKey() = iVal;
  }
  
  xFirstH1_ent.vOffsets(nLevels);
  
  Ent* aInsertedEntry_H1 = pH1->aInsertEntAbs(xFirstH1_ent.aEnt());
  UNUSED(aInsertedEntry_H1);
  
  //pH1->vShow();
  
  //pH1->bVerifyItems();
}

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_
  UNUSED(eClass);
  UNUSED(aTab);

  EntBuffer* pEntBuffer = _pEntBuffer(pObject);
  
  pEntBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* aGetNextEntry -- get the next entry                                        */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aGetNextEntry(EntBuffer** ppEntBuffer, Ent* aPrevEntry)
{
  _static_
  
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  Ent *aNextEntry = pEntBuffer->aNextEntry(aPrevEntry, 0);
  if (! aNextEntry ) {
    pEntBuffer = pEntBuffer->pNextEntBuffer();
    *ppEntBuffer = pEntBuffer;
    if (pEntBuffer)
      aNextEntry = pEntBuffer->aLeadEntry();
  }
  
  return aNextEntry;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pRefdEntBuffer(Ent* pItem)
{
  _static_
  
  Rrf xRrf = pItem->rxRef();
  
  EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(xRrf);
    
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pRefdEntBuffer(Rrf& xRrf)
{
  _static_
  
  i8 i_Buffer = xRrf.iBuffer();
  
  HeapBuffer* pHeapBuffer =
    _pHeapBuffer(gpBufferPool->pToPoolBuffer(i_Buffer));
  
  EntBuffer* pEntBuffer =
    _pEntBuffer(pHeapBuffer->aFirstChunk());
  
  ASSERT(pEntBuffer);
  ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  
  AM::METRICS::vDerefMetrics((i8)pEntBuffer,pEntBuffer->eGetClassNo(),DR_DEREF);
    
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* convert entry ticket to entry offset                                       */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::iToEntryOffset(tET tEntryTicket)
{
  _static_
  
  i8 iEntryOffset = (i8)tEntryTicket * C_ENT_QUANTA;
  
  return iEntryOffset;
}

/*----------------------------------------------------------------------------*/
/* convert entry offset to entry ticket                                       */
/*----------------------------------------------------------------------------*/
tET EntBuffer::tToEntryTicket(i8 iEntryOffset)
{
  _static_
  
  tET tEntryTicket = (tET)(iEntryOffset / C_ENT_QUANTA);
  
  return tEntryTicket;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aAllotEntry(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt = aAllotItem(CN_ENT,nSize);
  if (! aEnt)
    return nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vAnalyze(i8& nBuffers, i8& nEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // recursively dump the lower level buffers
  if (m_iBufferHeight != 0) {
    
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      ASSERT(pEntBuffer);
      ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
      
      pEntBuffer->vAnalyze(nBuffers, nEntries);
      
    }
  }
  else {
    nBuffers++;
    nEntries += m_nItems;
  }
}

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
/*
void EntBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  iBuffers++;
  iEntries += m_nItems;
  
  if (bDetailed)
    sAnalysis.vCatSprint(" row: (%s, %lld, %5lld,' L%lld', %lld)\n"_Stz,
                         AM::szNodeNm(),
                         m_tPtnNo,
                         iBuffers,
                         m_iBufferHeight,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_iBufferHeight != 0) {
    
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      ASSERT(pEntBuffer);
      ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
      
      pEntBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}
*/

/*----------------------------------------------------------------------------*/
/* assert valid entry buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vAssertEntBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* convert entry to entry ticket                                              */
/*----------------------------------------------------------------------------*/
tET EntBuffer::tToTicket(Ent* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iEntryOffset = iToOffset(pEntry);
  
  tET tEntryTicket = tToEntryTicket(iEntryOffset);
  
  return tEntryTicket;
}






/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/
void EntBuffer::vBufferCount(i8& nBuffers)
{
  EntBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  
  // count this buffer
  nBuffers++;
  
  // recursively count the lower level buffers
  if (! m_iBufferHeight)
    return;
  
  for (Ent* aEnt = aLeadEntry();
       aEnt != nullptr;
       aEnt = aNextEntry(aEnt, 0)) {
    
    // get the index buffer from the entry
    pNextBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    
    ASSERT(pNextBuffer);
    ASSERT(pNextBuffer->eGetClassNo() == CN_ENTBUFFER);
    
    pNextBuffer->vBufferCount(nBuffers);
    
  }
}

/*----------------------------------------------------------------------------*/
/* cut entry from skip list                                                   */
/*----------------------------------------------------------------------------*/
void EntBuffer::vCutEntry(Ent* aCutEnt, Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 nLevels = aCutEnt->nSkipLevels();
  
  for (i8 i_Level = 0; i_Level < nLevels; i_Level++) {
    tET tNextEntry = aCutEnt->tNextEntry(i_Level);
    aPriorVec[i_Level]->vNextEntry(i_Level,tNextEntry);
  }
}

/*----------------------------------------------------------------------------*/
/* delete the given entry                                                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDeleteEntry(Ent** aaEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //bVerifyItems();
  
  Ent* aEnt = *aaEnt;
  
  Ent* xPriorVec[C_MAX_LVL_LEVELS];
  
  vPosVecPriorToEntry(aEnt, aEnt->aKey(), xPriorVec);
  
  vCutEntry(aEnt,xPriorVec);
  
  i8 nEntSize = aEnt->nSizeBytes();
  
#ifdef __ENT_HEADER
  vFreeItem(aEnt,nEntSize);
  //vGiveSpace(nEntSize);
  
#else
  //if (aEnt->bPayload()) {
    aEnt->rxRef().vFree(true);
    aEnt->rxRef().vFreeSize(nEntSize);
    
    if (! m_aFreeSpare) {
      m_aFreeSpare = aEnt;
      m_nFreeSize  = nEntSize;
    }
    vDecItems();
  //}
  //else {
  //  vFreeItem(aEnt,nEntSize);
  //}

#endif
  
  m_nDelete++;
  
#ifdef EB_TRACE_INSERT
  SP_Ref spRef;
  SP_Key spKey;
  vTraceWhere();
  vTrace("-- delete d%lld %s %s\n"_Stz,
         m_nDelete,spRef.sz(aEnt->wRef()),spKey.sz(aEnt->aKey(),m_pIndexDef));
#endif

  // entry no longer exists, set the pointer to be null
  *aaEnt = nullptr;
  
  //bVerifyItems();

}

/*----------------------------------------------------------------------------*/
/* print the dipstick for this buffer                                         */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDipStick(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  if (pHigherEntBuffer())
    _pEntBuffer(pHigherEntBuffer())->vDipStick();
    
  EntBuffer* pL  = _pEntBuffer(this->pPrevEntBuffer());
  EntBuffer* pLL = pL ? _pEntBuffer(pL->pPrevEntBuffer()) : nullptr;
  
  EntBuffer* pR  = _pEntBuffer(this->pNextEntBuffer());
  EntBuffer* pRR = pR ? _pEntBuffer(pR->pNextEntBuffer()) : nullptr;

  vTrace("%lld "_Stz,m_iBufferHeight);
  
  if (pLL)
    vTrace("[%04lld %c]"_Stz,pLL->m_iBufferNo,pLL->bHasSpace(32)?'y':'n');
  else
    vTrace("[---- -]"_Stz);

  if (pL)
    vTrace("[%04lld %c]"_Stz,pL->m_iBufferNo,pL->bHasSpace(32)?'y':'n');
  else
    vTrace("[---- -]"_Stz);

  vTrace(" [%04lld %c] "_Stz,m_iBufferNo,bHasSpace(32)?'y':'n');

  if (pR)
    vTrace("[%04lld %c]"_Stz,pR->m_iBufferNo,pR->bHasSpace(32)?'y':'n');
  else
    vTrace("[---- -]"_Stz);

  if (pRR)
    vTrace("[%04lld %c]"_Stz,pRR->m_iBufferNo,pRR->bHasSpace(32)?'y':'n');
  else
    vTrace("[---- -]"_Stz);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDump(bool bEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  // show this buffer
  vShow(bEntries);
  
  if (! bEntries)
    return;
  
  // recursively dump the lower level buffers
  if (m_iBufferHeight != 0) {
    
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      ASSERT(pEntBuffer);
      ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
      
      pEntBuffer->vDump();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/
void EntBuffer::vDumpPriorVec(Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vTrace("Prior Vector\n"_Stz);
  
//  vTrace("  0:%p 1:%p 2:%p\n"_Stz,
//         aPriorVec[0],aPriorVec[1],aPriorVec[2]);
  
  i8 nLvlLevels = AM::nLvlLevels();

  for (i8 i_Lvl = 0; i_Lvl < nLvlLevels; i_Lvl++) {
    Ent* aEnt = aPriorVec[i_Lvl];
    if (aEnt == aSkipRoots())
      vTrace("SKIP ROOT\n"_Stz);
    else
      vShowEntry(aEnt);
  }
}

/*----------------------------------------------------------------------------*/
/* get the entry kind                                                         */
/*----------------------------------------------------------------------------*/
eKE EntBuffer::eEntKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return m_eEntKind;
}

/*----------------------------------------------------------------------------*/
/* set the entry kind                                                         */
/*----------------------------------------------------------------------------*/
void EntBuffer::vEntKind(eKE eEntKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_eEntKind = eEntKind;
}

/*----------------------------------------------------------------------------*/
/* get first child buffer                                                     */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFirstChildBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
  Ent* aFirstEntry = this->aLeadEntry();
  EntBuffer* pChildEntBuffer = EntBuffer::pRefdEntBuffer(aFirstEntry);
  
  return pChildEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* get an entry by offset                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aGetEntry(tET tEntryTicket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iEntryOffset = iToEntryOffset(tEntryTicket);
  Ent* aEnt = aGetItem(iEntryOffset);

  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* get split entry by split number                                            */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aGetEntrySplit(i8 iSplitEntryNo, Ent** aaPriorEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(aSkipRoots());
    
  RASSERT(iSplitEntryNo > 10);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  *aaPriorEntry = nullptr;
  
  i8 iEntryNo = 0;
  for (Ent* aSplitEntry = aNextEntry(aSkipRoots(),0);
       aSplitEntry;
       aSplitEntry = aNextEntry(aSplitEntry,0)) {
    
    // found split point?
    if (iEntryNo++ == iSplitEntryNo)
      return aSplitEntry;
    
    *aaPriorEntry = aSplitEntry;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pHigherEntBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return _pEntBuffer(LinkedBuffer::pHigherLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set the higher level index                                                 */
/*----------------------------------------------------------------------------*/
void EntBuffer::vHigherEntBuffer(EntBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  LinkedBuffer::vHigherLinkedBuffer(pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return m_pIndexTree->pIndex();
}

/*----------------------------------------------------------------------------*/
/* get the indexDef of the entry buffer                                       */
/*----------------------------------------------------------------------------*/
IndexDef* EntBuffer::pIndexDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  IndexDef* pIdxDef = m_pIndexTree->pIndexDef();
  
  return pIdxDef;
}

/*----------------------------------------------------------------------------*/
/* get last entry                                                             */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aLastEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
  Ent* aLast = aSkipRoots();
  
  for (;;) {
    Ent* aNext = aNextEntry(aLast,0);
    if (! aNext)
      break;
    aLast = aNext;
  }
  
  return aLast;
}

/*----------------------------------------------------------------------------*/
/* get entry array                                                           */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aEntArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  Ent* aEntAry = aItemArray();
  
  return aEntAry;
}


/*----------------------------------------------------------------------------*/
/* get lead entry (the entry after head entry                                 */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aLeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  
  Ent* aFirst = aNextEntry(aSkipRoots(),0);
  
  ASSERT(! aFirst || aFirst->bValidEnt());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get length of list of buffers                                               */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::iLengthList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  for (EntBuffer* pLn = this; pLn; pLn = pLn->pNextEntBuffer())
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get length of sublist of buffers                                           */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::iLengthSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  
  EntBuffer* pThisH0 = this;
  EntBuffer* pThisH1 = pThisH0->pHigherEntBuffer();
  
  EntBuffer* pH0;
  for (pH0 = pThisH0;
       pH0 && pH0->pHigherEntBuffer() == pThisH1;
       pH0 = pH0->pNextEntBuffer()) {
    
    iLength++;
  }
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeItem* EntBuffer::aMakeFreeEntry(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  i8 nEntSize = aEnt->nSizeBytes();
  
  FreeItem* aFreeItem = new(aEnt) FreeItem(nEntSize);
  
  return aFreeItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterThanEntry(Ent* aEnt, Key* aGivenKey, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  if (KE_INT == pEntBuffer->m_eEntKind)
    aEnt = pMoveToGreaterThanEntry_int(aEnt, aGivenKey, ppEntBuffer);
  else
    aEnt = pMoveToGreaterThanEntry_gen(aEnt, aGivenKey, ppEntBuffer);
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterThanEntry_gen(Ent* aEnt, Key* aGivenKey, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  while (aEnt) {
    eKC eCompare = Key::eCompare(aEnt->aKey(),aGivenKey);
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if ( ! aEnt) {
      
      // move to the next buffer
      *ppEntBuffer = pEntBuffer->pNextEntBuffer();
      
      if (*ppEntBuffer) {
        return (*ppEntBuffer)->pMoveToGreaterThanEntry_gen((*ppEntBuffer)->aSkipRoots(), aGivenKey, ppEntBuffer);
      }
      else
        break;
    }
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterThanEntry_int(Ent* aEnt, Key* aGivenKey, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  while (aEnt) {
    i8 iKey = *(i8*)aEnt->aKey();
    i8 iGivenKey = *(i8*)aGivenKey;
    
    // have we moved beyond ?
    if (iKey > iGivenKey)
      break;;
    
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if ( ! aEnt) {
      
      // move to the next buffer
      *ppEntBuffer = pEntBuffer->pNextEntBuffer();
      
      if (*ppEntBuffer) {
        return (*ppEntBuffer)->pMoveToGreaterThanEntry_int((*ppEntBuffer)->aSkipRoots(), aGivenKey, ppEntBuffer);
      }
      else
        break;
    }
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterEqualEntry(Ent* aEnt, Key* aGivenKey, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  if (KE_INT == pEntBuffer->m_eEntKind)
    aEnt = pMoveToGreaterEqualEntry_int(aEnt, aGivenKey, ppEntBuffer);
  else
    aEnt = pMoveToGreaterEqualEntry_gen(aEnt, aGivenKey, ppEntBuffer);
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterEqualEntry_gen(Ent* aEnt, Key* aGivenKey, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  while (aEnt) {
    eKC eCompare = Key::eCompare(aEnt->aKey(),aGivenKey);
    
    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if (!aEnt) {
      
      // move to the next buffer
      *ppEntBuffer = pEntBuffer->pNextEntBuffer();
      
      if (*ppEntBuffer) {
        return pMoveToGreaterEqualEntry((*ppEntBuffer)->aSkipRoots(), aGivenKey, ppEntBuffer);
      }
    }
    else
      break;
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToGreaterEqualEntry_int(Ent* aEnt, Key* aGivenKey, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  if (aEnt == pEntBuffer->aSkipRoots())
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  while (aEnt) {
    i8 iKey = *(i8*)aEnt->aKey();
    i8 iGivenKey = *(i8*)aGivenKey;
    
    // have we reached the key?
    if (iKey == iGivenKey)
      break;
    
    // have we moved beyond ?
    if (iKey > iGivenKey)
      break;
    
    aEnt = pEntBuffer->aNextEntry(aEnt,0);
    
    if (!aEnt) {
      
      // move to the next buffer
      *ppEntBuffer = pEntBuffer->pNextEntBuffer();
      
      if (*ppEntBuffer) {
        return pMoveToGreaterEqualEntry((*ppEntBuffer)->aSkipRoots(), aGivenKey, ppEntBuffer);
      }
    }
    else
      break;
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pMoveToNextEntry(Ent* aEnt, EntBuffer** ppEntBuffer)
{
  EntBuffer* pEntBuffer = *ppEntBuffer;
  
  aEnt = pEntBuffer->aNextEntry(aEnt,0);
  
  if (aEnt)
    return aEnt;
  
  while( ! aEnt) {
    
    // move to the next buffer
    *ppEntBuffer = pEntBuffer->pNextEntBuffer();
    
    if (*ppEntBuffer) {
      return pMoveToNextEntry((*ppEntBuffer)->aSkipRoots(), ppEntBuffer);
    }
    else
      break;
  }
  
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aNextEntry(Ent* aPrevEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  UNUSED(aPrevEntry);

  Ent* aNext = this->aNextEntry(aSkipRoots(),0);
  
  ASSERT(! aNext || aNext->bValidEnt());
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pNextEntBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return _pEntBuffer(LinkedBuffer::pNextLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntBuffer::vNextEntBuffer(EntBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  LinkedBuffer::vNextLinkedBuffer(pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
#if 0

Ent* EntBuffer::aNextEntry(Ent* aCurrent, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(aCurrent);
  ASSERT(iLevel <= 5);
  
  ASSERT(aCurrent->bValidEnt());
  
  tET tNextEntry = aCurrent->tNextEntry(iLevel);
  
  if (C_ENT_END_TICKET == tNextEntry)
    return nullptr;
  
  //RASSERT(tNextEntry < m_nItems);
  
  //vTrace("DIFF: %lldB\n"_Stz,nDifference(this,aGetItem(0)));
  
  i8 iNextEntryOffset = iToEntryOffset(tNextEntry);
  Ent* aNextEnt = aGetItem(iNextEntryOffset);

  ASSERT(! aNextEnt || aNextEnt->bValidEnt());
  
  return aNextEnt;
}

/*----------------------------------------------------------------------------*/

#else

Ent* EntBuffer::aNextEntry(Ent* aCurrent, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
  return aNextEntry_flat(aCurrent,iLevel);
}

#endif

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aNextEntry_flat(Ent* aCurrent, i8 i_Level)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
  tET tNextEntry = aCurrent->m_taNextEntry[i_Level];
  
  if (C_ENT_END_TICKET == tNextEntry)
    return nullptr;
    
  i8 iNextEntryOffset = (i8)tNextEntry * C_ENT_QUANTA;
  
  Ent* aNextEnt = (Ent*) ((char*)this + 624 + iNextEntryOffset);
  
  return aNextEnt;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return nullptr;
  
  Ent* aPoppedLazyEntry = m_aLazyChain;
  
  tET tNextLazyOffset = aPoppedLazyEntry->tNextEntry(0);
  
  if (C_ENT_END_TICKET == tNextLazyOffset)
    m_aLazyChain = nullptr;
  else
    m_aLazyChain = aGetEntry(tNextLazyOffset);
  
  return aPoppedLazyEntry;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pPrevEntBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return _pEntBuffer(LinkedBuffer::pPrevLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPrevEntBuffer(EntBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  LinkedBuffer::vPrevLinkedBuffer(pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* print rows of the buffer                                                   */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPrintRefdRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  StreamDef* pStreamDef = m_pIndexTree->pIndex()->pTable()->pStreamDef();
  
  HV_RowSingle hvRowSingle(pStreamDef);
  
  
  for (Ent* aEnt = aLeadEntry(); aEnt; aEnt = aNextEntry(aEnt, 0)) {
    
    // get the crw buffer of the row
    CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->rxRef());
    
    // get the compact row
    Crw* aCrw = pCrwBuffer->aCrw(aEnt->rxRef().iItem());
    
    // inflate compact row into row
    RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle,aCrw);
    
    hvRowSingle->vPrintRow();
  }

}

/*----------------------------------------------------------------------------*/
/* reset/empty the buffer                                                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(1 == m_iBufferHeight);
  
  vResetUsed();
  
  m_aFreeSpare = nullptr;
  m_nFreeSize  = 0;

  m_xSkipRoots.vNextEntryReset();
  
  m_aLazyChain  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void EntBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
//  ASSERT( ! m_pHigherBuffer );
//  ASSERT( ! m_pNextBuffer );
//  ASSERT( ! m_pPrevBuffer );
  
  // set the index to nullptr (this pointer is not valid any more)
  m_pIndexTree = nullptr;
  
  // get this buffer address
  PoolBuffer* pThisBuffer =
  pBitAnd<PoolBuffer*>(this,C_OBJECT_TO_BUFFER_MASK);

  // calculate offset for m_aLazyChain and set it
  if (m_aLazyChain) {
    PoolBuffer* pOriginalBuffer =
    pBitAnd<PoolBuffer*>(m_aLazyChain,C_OBJECT_TO_BUFFER_MASK);
    u8 uOffset = nDifference(m_aLazyChain, pOriginalBuffer);
    m_aLazyChain = pBitOr<Ent*>(pThisBuffer,uOffset);
  }
  
  // get the buffer no
  i8 i_Buffer = iBufferNo();
    
  // now restore the row id's to point to this buffer no
  for (Ent* aEnt = aLeadEntry();
       aEnt != nullptr;
       aEnt = aNextEntry(aEnt, 0)) {
    
    Rrf xRef(i_Buffer, aEnt->rxRef().iItem(), aEnt->nSkipLevels());
    aEnt->vRef(xRef);
  }
  
}

/*----------------------------------------------------------------------------*/
/* restore the index of the index buffer                                      */
/*----------------------------------------------------------------------------*/
void EntBuffer::vRestoreIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  UNUSED(pIndex);

  //ASSERT( ! m_pIndex );
  //m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get skip roots                                                             */
/*----------------------------------------------------------------------------*/
SkipRoots* EntBuffer::aSkipRoots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return & m_xSkipRoots;
}

/*----------------------------------------------------------------------------*/
/* show entry                                                                 */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowEntry(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Key* aKey  = aEnt->aKey();
  i8 iOffset = iToOffset(aEnt);
  tET tEntry = tToTicket(aEnt);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("tET %04llx %07lld %p %3lluB %s "_Stz,
         (i8)tEntry,
         iOffset,
         aEnt,
         aEnt->nSizeBytes(),
         aKE[aEnt->eKind()]
         );
  
  if (aEnt->bPayload())
    vTrace("Pay "_Stz);
  else
    vTrace("NoPay "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTrace("Rrf %lld.%05lld"_Stz,
         aEnt->rxRef().iBuffer(),
         aEnt->rxRef().iItem()
         );
  
  //vTrace("Tx: %lld.%03lld "_Stz,
  //       Tx::iTxNode(m_iTxId),
  //       Tx::iTxNo(m_iTxId)
  //       );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (KE_INT == aEnt->eKind()) {
    Ent_int* aEnt_int = (Ent_int*)aEnt;
    vTrace(" IntElm0 %lld"_Stz,aEnt_int->m_iIntElm0);
  }
  else
    aKey->vShow(pIndexDef());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (aEnt->bPayload()) {
    i8 nPaySize = aEnt->nPaySize();
    vTrace(" PaySize %2lld"_Stz,nPaySize);
    vTrace(" Payload "_Stz);
    i8 iCells = nPaySize / 8;
    for (i8 i_Cell=0; i_Cell < iCells; i_Cell++)
      vTrace("%2lld "_Stz,*(aEnt->aiPayCell(i_Cell)));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace(" EntNo %05lld levels %lld"_Stz,aEnt->iEntryNo(),aEnt->nSkipLevels());
  for (i8 i_Level = 0; i_Level < aEnt->nSkipLevels(); i_Level++)
    vTrace(" %04llx"_Stz,(i8)aEnt->tNextEntry(i_Level));
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShow(bool bEntries, Ent* aMarkEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  s256 sHigherBuffer;
  if (pHigherEntBuffer())
    StrLib::wToString((i8)((EntBuffer*)pHigherEntBuffer())->m_iBufferHeight,
                      sHigherBuffer.azChar());
  else
    sHigherBuffer = "..."_Stz;
  
  i8 iL0Length;
  i8 iL1Length;
  i8 iL2Length;
  i8 iL3Length;
  i8 iL4Length;
  i8 iL5Length;

  vSkipLengths(iL0Length,iL1Length,iL2Length,iL3Length,iL4Length,iL5Length);
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("EntBuffer: %p (buffer %lld): items: %lld used: %lld(B) freespace %lld(B)\n"_Stz,
         this,iBufferNo(),nItems(),nUsedBytes(),nFreeBytes());
  
  vTrace("  higherBuffer %p (L%s) prevBuffer %p nextBuffer %p\n"_Stz,
         pHigherEntBuffer(),sHigherBuffer.szStz(),pPrevEntBuffer(),pNextEntBuffer());

  vTrace("  index: %s partition: %lld height: %lld\n"_Stz,
         m_pIndexTree && m_pIndexTree->pIndex() ? m_pIndexTree->pIndex()->szIndexNm() : C_NULL,m_tLglPtn.iAsI8(),m_iBufferHeight);
  SkipRoots* aSkipRoots = this->aSkipRoots();
  vTrace("  skipRoots: %p[%lld, %lld, %lld, %lld, %lld, %lld]\n"_Stz,
         aSkipRoots,
         (i8)aSkipRoots->tNextEntry(0) ,
         (i8)aSkipRoots->tNextEntry(1) ,
         (i8)aSkipRoots->tNextEntry(2) ,
         (i8)aSkipRoots->tNextEntry(3) ,
         (i8)aSkipRoots->tNextEntry(4) ,
         (i8)aSkipRoots->tNextEntry(5)
         );

  vTrace("  listLengths: [%lld, %lld, %lld, %lld, %lld, %lld]\n"_Stz,
         iL0Length,iL1Length,iL2Length,iL3Length,iL4Length,iL5Length);

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  if (! bEntries)
    return;
  
  vTrace("\n"_Stz);
  vTrace("Arguments (by headEntry vector[0]):\n"_Stz);
  
  tET tEntry = aSkipRoots->tNextEntry(0);
  while (C_ENT_END_TICKET != tEntry) {
    Ent* aEnt = aGetEntry(tEntry);
    vShowEntry(aEnt);
    if (aMarkEntry == aEnt)
      vTrace("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"_Stz);
    
    tEntry = aEnt->tNextEntry(0);
  }
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show short form of buffer                                                  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vTrace("[t%lld h%lld: "_Stz,
         iBufferNo(),
         pHigherEntBuffer() ? pHigherEntBuffer()->iBufferNo() : 0);
  
  tET tEntry = aSkipRoots()->tNextEntry(0);
  while (C_ENT_END_TICKET != tEntry) {
    Ent* aEnt = aGetEntry(tEntry);
    
    Key* aKey = aEnt->aKey();
    i8 iVal;
    
    if (KE_INT == m_eEntKind)
      iVal = *(i8*)aKey;
    else
      iVal = aKey->iGetIntElm();
    
    if (m_iBufferHeight)
      vTrace("%lld(%lld)"_Stz,iVal,EntBuffer::pRefdEntBuffer(aEnt)->iBufferNo());
    else
      vTrace("%lld|"_Stz,iVal);
    
    tEntry = aEnt->tNextEntry(0);
    if (C_ENT_END_TICKET != tEntry)
      vTrace(","_Stz);
  }
  vTrace("]"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show sublist of buffers                                                    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pThisH0 = this;
  EntBuffer* pThisH1 = pThisH0->pHigherEntBuffer();
  
  vTrace("{"_Stz);
  
  if (pThisH0->pPrevEntBuffer())
    vTrace("<-"_Stz);
  
  EntBuffer* pH0;
  for (pH0 = pThisH0;
       pH0 && pH0->pHigherEntBuffer() == pThisH1;
       pH0 = pH0->pNextEntBuffer()) {
    
    if (pH0 != pThisH0)
      vTrace(","_Stz);
    
    vTrace("%lld"_Stz,pH0->iBufferNo());
  }
  
  if (pH0)
    vTrace("->"_Stz);
  
  vTrace("}\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show contents of all buffers in index tree                                 */
/*----------------------------------------------------------------------------*/
void EntBuffer::vShowTree(bool bDumpBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (! pHigherEntBuffer() )
    vTrace("\nIndex Tree %p:\n"_Stz,this);
  
  vTrace("Height %lld "_Stz,m_iBufferHeight);
  
//  for (EntBuffer* pBuffer = this;
//       pBuffer;
//       pBuffer = pBuffer->pNextBuffer()) {
//    
//    pBuffer->vShowShort();
//    
//    ASSERT(! pBuffer->pPrevBuffer() || pBuffer == pBuffer->pPrevBuffer()->pNextBuffer());
//    ASSERT(! pBuffer->pNextBuffer() || pBuffer == pBuffer->pNextBuffer()->pPrevBuffer());
//  }
  
  vTrace("\n"_Stz);
  
  if (bDumpBuffer) {
    for (EntBuffer* pBuffer = this;
         pBuffer;
         pBuffer = pBuffer->pNextEntBuffer())
      pBuffer->vShow(true);
  }
  
  if ( ! m_iBufferHeight )
    return;
  
  Ent* aFirstEntry = this->aLeadEntry();
  if (! aFirstEntry)
    return;
  
  Rrf xRef;
  xRef = aFirstEntry->rxRef();
  i8 i_Buffer = xRef.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
  
  EntBuffer* pChildBuffer =
  reinterpret_cast<EntBuffer*>(pHeapBuffer->aFirstChunk());
  
  pChildBuffer->vShowTree(bDumpBuffer);
}

/*----------------------------------------------------------------------------*/
/* get the length of a skip list                                              */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::nSkipLength(i8 i_Level)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
 
  i8 iLength = 0;

  for (Ent* aEnt = aNextEntry(aSkipRoots(),i_Level);
       aEnt;
       aEnt = aNextEntry(aEnt,i_Level))
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get the lengths of the three skip lists                                    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vSkipLengths(i8& iL0Length_, i8& iL1Length_,
                             i8& iL2Length_, i8& iL3Length_,
                             i8& iL4Length_, i8& iL5Length_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  iL0Length_ = nSkipLength(0);
  iL1Length_ = nSkipLength(1);
  iL2Length_ = nSkipLength(2);
  iL3Length_ = nSkipLength(3);
  iL4Length_ = nSkipLength(4);
  iL5Length_ = nSkipLength(5);
}

/*----------------------------------------------------------------------------*/
/* splice the given buffer after this buffer                                  */
/*----------------------------------------------------------------------------*/
void EntBuffer::vSpliceAfter(EntBuffer* pSplicedBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pThisBuffer = this;
  EntBuffer* pNextBuffer = pThisBuffer->pNextEntBuffer();
  
  // the spliced buffer is always inserted after this buffer
  pThisBuffer->vNextEntBuffer(pSplicedBuffer);
  pSplicedBuffer->vNextEntBuffer(pNextBuffer);
  pSplicedBuffer->vPrevEntBuffer(pThisBuffer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (a), no next buffer
  
  if (! pNextBuffer)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (b), next buffer
  
  pNextBuffer->vPrevEntBuffer(pSplicedBuffer);
}

/*----------------------------------------------------------------------------*/
/* convert entBuffer to srtBuffer                                             */
/*----------------------------------------------------------------------------*/
//SrtBuffer* EntBuffer::pToSrtBuffer(i8 iKeyCol)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
//  
//  i8 nRowRefs = 1;
//  
//  HV_RowSingle hvRowSingle(m_pIndex->pTable()->pStreamDef(), C_ALL);
//  SrtBuffer* pSrtBuffer = new SrtBuffer(nRowRefs, m_tPtnNo);
//  
//  Ent* aEntry = aLeadEntry();
//  SrtSet* pSrtSet;
//  
//  while (aEntry) {
//    
//    pSrtSet = pSrtBuffer->aAllotSrtSet();
//    if ( ! pSrtSet ) {
//      
//      // create a new SrtBuffer
//      SrtBuffer* pNewSrtBuffer = new SrtBuffer(nRowRefs, m_tPtnNo);
//      pNewSrtBuffer->vNextBuffer(pSrtBuffer);
//      pSrtBuffer = pNewSrtBuffer;
//      pSrtSet = pSrtBuffer->aAllotSrtSet();
//      
//    }
//    ASSERT(pSrtSet);
//    
//    // get the left (this) rowBuffer
//    CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEntry->xRef());
//    
//    // get the dataRow
//    Crw* aCrw = pCrwBuffer->aCrw(aEntry->iItem());
//    
//    // set the rowData in the srtSet
//    pSrtSet->vRowRef(0, (i8)aCrw);
//  
//    // get the key from the left buffer row
//    RowInflator::vInflate_OneColumn_into_RowSingle(hvRowSingle, aCrw, iKeyCol);
//    
//    // set the key in the srtSet
//    pSrtSet->vKey(hvRowSingle->m_waColVal[iKeyCol].m_iWrd);
//    
//    aEntry = aNextEntry(aEntry,0);
//  }
//  
//  return pSrtBuffer;
//}

/*----------------------------------------------------------------------------*/
/* get tree debug mode                                                        */
/*----------------------------------------------------------------------------*/
bool EntBuffer::bTreeDebug(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return m_pIndexTree->bTreeDebug();
}

/*----------------------------------------------------------------------------*/
/* get running delete count                                                   */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::nTreeDeleteCnt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return m_pIndexTree->nTreeDeleteCnt();
}

/*----------------------------------------------------------------------------*/
/* get running insert count                                                   */
/*----------------------------------------------------------------------------*/
i8 EntBuffer::nTreeInsertCnt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  return m_pIndexTree->nTreeInsertCnt();
}

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EntBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pNextBuffer;
  
  if (m_iBufferHeight != 0) {
    
    // recursively delete the lower level buffers
    for (Ent* aEnt = aLeadEntry();
         aEnt != nullptr;
         aEnt = aNextEntry(aEnt, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      
      RASSERT(pNextBuffer);
      RASSERT(pNextBuffer->eGetClassNo() == CN_ENTBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
}

/*----------------------------------------------------------------------------*/
/* unlink the sublist of EB's following this L0 EB within this H1             */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pUnlinkSubList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(0 == m_iBufferHeight);
  
  EntBuffer* pThisH0 = this;
  EntBuffer* pThisH1 = pThisH0->pHigherEntBuffer();
  EntBuffer* pNextH0 = pThisH0->pNextEntBuffer();
  EntBuffer* pNextH1 = pThisH1->pNextEntBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (a) no more L0
  
  if (! pNextH0)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (b) only one L0 under H1
  
  if (pNextH0->pHigherEntBuffer() != pHigherEntBuffer())
    return nullptr;
  
  //----------------------------------------------------------------------------
  //
  // - there is a sublist
  //
  //----------------------------------------------------------------------------
  
  EntBuffer* pThisH0SubList = pNextH0;
  
  // the sublist no longer points back to this L0
  pThisH0SubList->vPrevEntBuffer(nullptr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (c) last H1, there is a sublist
  
  if (! pNextH1) {
    
    // last H1, therefore the next chain terminates here
    pThisH0->vNextEntBuffer(nullptr);
    
    return pThisH0SubList;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // circumstance (d) middle H1, there is a sublist
  
  EntBuffer* pFirstH0ofNextH1 = pNextH1->pFirstChildBuffer();
  EntBuffer* pLastH0ofThisH1 = pFirstH0ofNextH1->pPrevEntBuffer();
  
  // the next chain points to first L0 buffer of next H1
  pThisH0->vNextEntBuffer(pFirstH0ofNextH1);
  
  // the prev chain of first L0 of next H1 is this L0
  pFirstH0ofNextH1->vPrevEntBuffer(pThisH0);
  
  // the prev chain of last L0 of sublist terminates
  pLastH0ofThisH1->vNextEntBuffer(nullptr);
  
  return pThisH0SubList;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
