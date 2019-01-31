/*============================================================================*/
/* FreeEntry.cpp -- free entry in buffer                                      */
/*============================================================================*/

#include "MiniHeaps.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FreeEntry* MiniHeaps::_aFreeEntry(void* aAddr)
{
  FreeEntry* aFreeEntry = reinterpret_cast<FreeEntry*>(aAddr);
  
  return aFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeEntry::operator new(size_t)
{
  vFatal("fatal: FreeEntry class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeEntry::operator new(size_t, Entry* aEntry)
{
  return reinterpret_cast<FreeEntry*>(aEntry);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeEntry::operator new(size_t, FreeEntry* aFreeEntry)
{
  return aFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FreeEntry::FreeEntry(i8 nSizeItem)
: Item(CN_FREEENTRY,nSizeItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY);
  
  m_aNextFree = nullptr;
  m_aPrevFree = nullptr;
  
  aFooter()->vSetFooter();
  
  _checkInitialisedSize_(nSizeItem);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FreeEntry::~FreeEntry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assert valid free entry                                                    */
/*----------------------------------------------------------------------------*/
void FreeEntry::vAssertFreeEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test for valid free entry                                                  */
/*----------------------------------------------------------------------------*/
bool FreeEntry::bValidFreeEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  if (! bValidMagic())
    return false;
  
  if (CN_FREEENTRY != eClass())
    return false;

  if (! aFooter()->bValidFooter())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next free entry                                                        */
/*----------------------------------------------------------------------------*/
FreeEntry* FreeEntry::aNextFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  return m_aNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free entry                                                        */
/*----------------------------------------------------------------------------*/
void FreeEntry::vNextFree(FreeEntry* aNextFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  m_aNextFree = aNextFree;
}

/*----------------------------------------------------------------------------*/
/* get prev free entry                                                        */
/*----------------------------------------------------------------------------*/
FreeEntry* FreeEntry::aPrevFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  return m_aPrevFree;
}

/*----------------------------------------------------------------------------*/
/* set prev free entry                                                        */
/*----------------------------------------------------------------------------*/
void FreeEntry::vPrevFree(FreeEntry* aPrevFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  m_aPrevFree = aPrevFree;
}

/*----------------------------------------------------------------------------*/
/* get address after this freeEntry                                           */
/*----------------------------------------------------------------------------*/
char* FreeEntry::aAddressAfter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nFreeEntryBytes = nSizeItem();
  
  char* aAfter = aAdd<char*>(this,nFreeEntryBytes);
  
  return aAfter;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of footer                                                      */
/*----------------------------------------------------------------------------*/
Footer* FreeEntry::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY);
  
  i8 iFooterOffset = nSizeItem() - sizeof(Footer);
  
  Footer* aFooter = aAdd<Footer*>(this,iFooterOffset);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
