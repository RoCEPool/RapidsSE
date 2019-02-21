/*============================================================================*/
/* EntryBuffer_all.cpp -- working file to sort out the fun's of Item PoolBuffer  */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* move to exact key entry                                                    */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToExactEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aItem == aSkipRoots())
    aItem = aNextEntry(aItem,0);
    
  eKC eCompare = KeyData::eCompare(aItem, xKeyWrk);
  
  // have we reached the key?
  if (eCompare == KC_SAME)
    return aItem;
  
  // have we moved beyond ?
  if (eCompare == KC_AFTER)
    return nullptr;
  
  aItem = aNextEntry(aItem,0);
  
  if ( ! aItem ) {
    
    // move to the next buffer
    *paEntryBuffer = pNextBuffer();
    
    if (*paEntryBuffer) {
      return (*paEntryBuffer)->pMoveToExactEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
    }
    
  }
  

  return aItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToGreaterThanEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aItem == aSkipRoots())
    aItem = aNextEntry(aItem,0);
  
  while (aItem) {
    eKC eCompare = KeyData::eCompare(aItem, xKeyWrk);
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aItem = aNextEntry(aItem,0);
    
    if ( ! aItem) {
      
      // move to the next buffer
      *paEntryBuffer = pNextBuffer();
      
      if (*paEntryBuffer) {
        return (*paEntryBuffer)->pMoveToGreaterThanEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
      }
      else
        break;
    }
  }
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToGreaterEqualEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (aItem == aSkipRoots())
    aItem = aNextEntry(aItem,0);
  
  while (aItem) {
    eKC eCompare = KeyData::eCompare(aItem, xKeyWrk);
    
    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      break;
    
    aItem = aNextEntry(aItem,0);
    
    if (!aItem) {
      
      // move to the next buffer
      *paEntryBuffer = pNextBuffer();
      
      if (*paEntryBuffer) {
        return (*paEntryBuffer)->pMoveToGreaterEqualEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
      }
    }
    else
      break;
  }
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* move to next key entry                                                     */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::pMoveToNextEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** paEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  aItem = aNextEntry(aItem,0);
  
  if (aItem)
    return aItem;
  
  while( ! aItem) {
    
    // move to the next buffer
    *paEntryBuffer = pNextBuffer();
    
    if (*paEntryBuffer) {
      return (*paEntryBuffer)->pMoveToNextEntry((*paEntryBuffer)->aSkipRoots(), xKeyWrk, paEntryBuffer);
    }
    else
      break;
  }
  
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* pop entry from lazy chain                                                  */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aPopLazyChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (! m_aLazyChain )
    return nullptr;
  
  Item* aPoppedLazyEntry = m_aLazyChain;
  
  i8 iNextLazyOffset = aPoppedLazyEntry->iNEO(0);
  
  if (-1 == iNextLazyOffset)
    m_aLazyChain = nullptr;
  else
    m_aLazyChain = aGetEntryOffset(iNextLazyOffset);
  
  return aPoppedLazyEntry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
