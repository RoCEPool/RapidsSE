/*============================================================================*/
/* FreeEntry.cpp -- free entry in buffer                                      */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FreeEntry* DBobjects::_pFreeEntry(void* aAddr)
{
  FreeEntry* pFreeEntry = reinterpret_cast<FreeEntry*>(aAddr);
  
  return pFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeEntry::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: FreeEntry class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeEntry::operator new(size_t iSize, Entry* pEntry)
{
  RASSERT(iSize);
  return reinterpret_cast<FreeEntry*>(pEntry);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeEntry::operator new(size_t iSize, FreeEntry* pFreeEntry)
{
  RASSERT(iSize);
  return pFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FreeEntry::FreeEntry(i8 nSizeItem)
: Item(CN_FREEENTRY,nSizeItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY);
  
  m_pNextFree = nullptr;
  m_pPrevFree = nullptr;
  
  _setFooter_();
  
  _checkInitialisedSize_(nSizeItem);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FreeEntry::~FreeEntry(void)
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
FreeEntry* FreeEntry::pNextFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  return m_pNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free entry                                                        */
/*----------------------------------------------------------------------------*/
void FreeEntry::vNextFree(FreeEntry* pNextFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  m_pNextFree = pNextFree;
}

/*----------------------------------------------------------------------------*/
/* get prev free entry                                                        */
/*----------------------------------------------------------------------------*/
FreeEntry* FreeEntry::pPrevFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  return m_pPrevFree;
}

/*----------------------------------------------------------------------------*/
/* set prev free entry                                                        */
/*----------------------------------------------------------------------------*/
void FreeEntry::vPrevFree(FreeEntry* pPrevFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  m_pPrevFree = pPrevFree;
}

/*----------------------------------------------------------------------------*/
/* get address after this freeEntry                                           */
/*----------------------------------------------------------------------------*/
char* FreeEntry::aAddressAfter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nFreeEntryBytes = nSizeItem();
  
  char* aAfter = pAdd<char*>(this,nFreeEntryBytes);
  
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
  
  Footer* aFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
