/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

bool mbWatch = false;


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer* DBobjects::_pEntryBuffer(void* aAddress)
{
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(aAddress);
  
  RASSERT_TYPE(pEntryBuffer,CN_ENTRYBUFFER);
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t)
{
  EntryBuffer* pEntryBuffer =
    static_cast<EntryBuffer*>(HeapMem::pAllocate(CN_ENTRYBUFFER));
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* aAddress)
{
  EntryBuffer* pEntryBuffer = static_cast<EntryBuffer*>(aAddress);
  
  HeapMem::vFree(pEntryBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: ItemHeap(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vInitLock(LK_ENTRYBUFFER);
  
  m_pIndex        = nullptr;
  
  m_iaEntryOffsets = nullptr;
  m_i_FreeOffsetChain = C_NO_MORE_ITEMS;
  
  m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  vHeadEntryNew();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pObject);
  
  pEntryBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pRefdEntryBuffer(Id& xId)
{
  bn nBufferNo = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBufferNo));
  
  EntryBuffer* pEntryBuffer =
  reinterpret_cast<EntryBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pEntryBuffer);
  ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  
  return pEntryBuffer;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
  
  if (! m_pIndex->bIsHidden())
    return;
  
  vInitFreeChain();
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}


////////////////////////////////////////////////////////////////////////////////
//                             buffer functions                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferCount(i8& nBuffers)
{
  EntryBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index ?
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_iLevelNo == 0);
    pNextBuffer = this->pNextBuffer();
    nBuffers++;
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->iLevelNo() == 0);
      
      //EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      nBuffers++;
    }
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  
  // count this buffer
  nBuffers++;
  
  // recursively count the lower level buffers
  if (! m_iLevelNo)
    return;
    
  for (Entry* aEntry = aFirstEntry();
       aEntry != nullptr;
       aEntry = aNextEntry(aEntry, 0)) {
    
    // get the index buffer from the entry
    pNextBuffer = aEntry->pReferencedEntryBuffer();
    
    ASSERT(pNextBuffer);
    ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
    
    pNextBuffer->vBufferCount(nBuffers);
    
  }
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return _pEntryBuffer(m_pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* set the higher level index                                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHigherBuffer(EntryBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return _pEntryBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vNextBuffer(EntryBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return _pEntryBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPrevBuffer(EntryBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pNextBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index buffers are just one level
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_iLevelNo == 0);
    pNextBuffer = this->pNextBuffer();
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->iLevelNo() == 0);
      
      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      delete pThisBuffer;
    }
    delete this;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // other index
  if (m_iLevelNo != 0) {
    
    // recursively delete the lower level buffers
    for (Entry* aEntry = aFirstEntry();
         aEntry != nullptr;
         aEntry = aNextEntry(aEntry, 0)) {
      
      // get the index buffer from the entry
      pNextBuffer = aEntry->pReferencedEntryBuffer();
      
      ASSERT(pNextBuffer);
      ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
      pNextBuffer->vTruncate();
    }
  }
  
  delete this;
}

////////////////////////////////////////////////////////////////////////////////
//                             entry functions                                //
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Entry* aFirst = aNextEntry(m_aHeadEntry,0);
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get last entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aLastEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  Entry* aLast = m_aHeadEntry;
  
  for (;;) {
    Entry* aNext = aNextEntry(aLast,0);
    if (! aNext)
      break;
    aLast = aNext;
  }
  
  return aLast;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aAllotEntry(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Entry* aEntry = reinterpret_cast<Entry*>(aAllotItem(nBytes));
  if ( ! aEntry )
    return nullptr;
  
  new(aEntry) Entry(CN_ENTRY,nBytes,0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* allot an offset array in this buffer                                       */
/*----------------------------------------------------------------------------*/
NEO* EntryBuffer::aAllotNEO(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  NEO* aNEO = reinterpret_cast<NEO*>(aAllotSpace(nBytes));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* set up the initial entry offset free chain                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vInitFreeChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_iaEntryOffsets  =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_MAX_ITEMS));
  
  m_i_FreeOffsetChain = C_NO_MORE_ITEMS;
  
  i8 L_ENTRIES_0 = gpAttachMap->m_nIndexEntries ?
    gpAttachMap->m_nIndexEntries : C_MAX_ITEMS;
  
  // L0 free chain
  for (i4 i_RowNo = 0; i_RowNo < (L_ENTRIES_0 - 1); i_RowNo++) {
    RASSERT(i_RowNo >= 0 && i_RowNo < C_MAX_ITEMS);
    m_iaEntryOffsets[i_RowNo] = - (i_RowNo+1);
  }
  
  RASSERT((L_ENTRIES_0 - 1) >= 0 && (L_ENTRIES_0 - 1) < C_MAX_ITEMS);
  m_iaEntryOffsets[L_ENTRIES_0 - 1] = - C_NO_MORE_ITEMS;
  
  m_i_FreeOffsetChain = 0;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntryBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Unit Tests --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
