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
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_okLock.vInitLock(LK_ENTRYBUFFER);
  
  m_bIsLast       = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_iPartitionNo  = -1;
  m_iBufferNo     = *nBufferNo();
  m_iBufferLevel  = 0;
  
  m_iaEntryOffsets = nullptr;
  m_i_FreeOffsetChain = C_ENTRY_NONE;
  
  m_aHeadEntry  = nullptr;
  m_aLastEntry  = nullptr;
  m_aFreeEntryList  = nullptr;
  
  m_aLazyChain  = nullptr;
  
  m_oWhenNewed.vNow();
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get age of buffer in seconds                                               */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iAgeInSeconds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Since oSince;
  oSince.vNow();
  
  Ivl wDiff;
  
  wDiff = oSince.wSubtract(m_oWhenNewed);
  
  i8 iAge = wDiff.iSecond();
  
  return iAge;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferLevel(i8 iBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_iBufferLevel = iBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iBufferLevel(void)
{
  return m_iBufferLevel;
}

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
  
  m_iaEntryOffsets  =
    reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_ENTRIES_L0));
  
  m_i_FreeOffsetChain = C_ENTRY_NONE;
  
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

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPartitionNo(i8 iPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_iPartitionNo = iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_iPartitionNo;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree() - sizeof(Wilderness);
  
  // we allow for only C_ENTRIES_L0 - 16 entries
  if (nItems() >= C_ENTRIES_L0 - 16) {
    return false;
  }
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (m_nItems >= gpAttachMap->m_nIndexEntries)
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on free space
    if (nFree < nBytes)
      return false;
    else
      return true;
  }
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
