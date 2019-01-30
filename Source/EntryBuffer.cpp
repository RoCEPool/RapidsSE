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
