/*============================================================================*/
/* FreeItem.cpp -- free entry in buffer                                      */
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
FreeItem* MiniHeaps::_aFreeItem(void* aAddr)
{
  FreeItem* aFreeEntry = reinterpret_cast<FreeItem*>(aAddr);
  
  return aFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeItem::operator new(size_t)
{
  vFatal("fatal: FreeItem class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeItem::operator new(size_t, Item* aItem)
{
  return reinterpret_cast<FreeItem*>(aItem);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FreeItem::operator new(size_t, FreeItem* aFreeEntry)
{
  return aFreeEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FreeItem::FreeItem(i8 nSizeItem)
: Item(CN_FREEENTRY,nSizeItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY);
  
  m_aNextFree = nullptr;
  
  aFooter()->vSetFooter();
  
  _checkInitialisedSize_(nSizeItem);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FreeItem::~FreeItem(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert given entry into free entry                                        */
/*----------------------------------------------------------------------------*/
FreeItem* FreeItem::aMorphFreeItem(Item* aItem)
{
  i8 nSizeItem = aItem->nSizeItem();
  
  FreeItem* aFreeItem = new(aItem) FreeItem(nSizeItem);
  
  return aFreeItem;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assert valid free entry                                                    */
/*----------------------------------------------------------------------------*/
void FreeItem::vAssertFreeEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test for valid free entry                                                  */
/*----------------------------------------------------------------------------*/
bool FreeItem::bValidFreeEntry(void)
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
FreeItem* FreeItem::aNextFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  return m_aNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free entry                                                        */
/*----------------------------------------------------------------------------*/
void FreeItem::vNextFree(FreeItem* aNextFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY,_FOOTER_);
  
  m_aNextFree = aNextFree;
}

/*----------------------------------------------------------------------------*/
/* get address after this freeEntry                                           */
/*----------------------------------------------------------------------------*/
char* FreeItem::aAddressAfter(void)
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
Footer* FreeItem::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEENTRY);
  
  i8 iFooterOffset = nSizeItem() - sizeof(Footer);
  
  Footer* aFooter = aAdd<Footer*>(this,iFooterOffset);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
