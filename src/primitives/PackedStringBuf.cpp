/*============================================================================*/
/* PackedStringBuf.cpp -- A packed, segmented container for storing strings   */
/*============================================================================*/

#include "BASE.h"
#include "PackedStringBuf.hpp"

using namespace PRIMITIVES;

__pCastFunction(PRIMITIVES,PackedStringSegmentTiny,CN_PACKEDSTRINGBUFTINY)
__pCastFunction(PRIMITIVES,PackedStringSegmentSmall,CN_PACKEDSTRINGBUFSMALL)
__pCastFunction(PRIMITIVES,PackedStringSegmentMedium,CN_PACKEDSTRINGBUFMEDIUM)
__pCastFunction(PRIMITIVES,PackedStringSegmentLarge,CN_PACKEDSTRINGBUFLARGE)


/*----------------------------------------------------------------------------*/
/* Create a PackedStringBuf                                                   */
/*----------------------------------------------------------------------------*/
PackedStringBuf* PackedStringBuf::pCreate(eSIZE eFirstSize)
{
  switch (eFirstSize) {

    case TINY:    return new PackedStringSegmentTiny;
    case SMALL:   return new PackedStringSegmentSmall;
    case MEDIUM:  return new PackedStringSegmentMedium;
    case LARGE:   return new PackedStringSegmentLarge;
  }
}

/*----------------------------------------------------------------------------*/
/* Deallocate PackedStringBuf                                                 */
/*----------------------------------------------------------------------------*/
void PackedStringBuf::vDestroy(PackedStringBuf *pBuf)
{
  if (!pBuf) {
    return;
  }

  switch (pBuf->eGetClassNo()) {

    case CN_PACKEDSTRINGBUFTINY:    delete   _pPackedStringSegmentTiny(pBuf); break;
    case CN_PACKEDSTRINGBUFSMALL:   delete  _pPackedStringSegmentSmall(pBuf); break;
    case CN_PACKEDSTRINGBUFMEDIUM:  delete _pPackedStringSegmentMedium(pBuf); break;
    case CN_PACKEDSTRINGBUFLARGE:   delete  _pPackedStringSegmentLarge(pBuf); break;
    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Constructor of PackedStringBuf class                                       */
/*----------------------------------------------------------------------------*/
PackedStringBuf::PackedStringBuf(BASE::eCN eClassNo)
: Object (eClassNo)
{
  this->m_pHead = this;
  this->m_pNext = nullptr;
  this->m_nItems = 0;
  this->u.m_pVoid = this;

  //_checkInitialised_(this->eGetClassNo());
}

/*----------------------------------------------------------------------------*/
/* Constructor of PackedStringBuf class                                       */
/*----------------------------------------------------------------------------*/
PackedStringBuf::PackedStringBuf(BASE::eCN eClassNo, PackedStringBuf* pHead)
: Object (eClassNo)
{
  this->m_pHead = pHead;
  this->m_pNext = nullptr;
  this->m_nItems = 0;
  this->u.m_pVoid = this;

  //_checkInitialised_(this->eGetClassNo());
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PackedStringBuf::~PackedStringBuf(void) noexcept(false)
{
  if (m_pNext) {
    switch (m_pNext->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    delete   _pPackedStringSegmentTiny(m_pNext); break;
      case CN_PACKEDSTRINGBUFSMALL:   delete  _pPackedStringSegmentSmall(m_pNext); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  delete _pPackedStringSegmentMedium(m_pNext); break;
      case CN_PACKEDSTRINGBUFLARGE:   delete  _pPackedStringSegmentLarge(m_pNext); break;
      default:
        NEVER_REACH
    }

    m_pNext = nullptr;
  }

//  // Call the destructor of the derived class to ensure that the bytespace
//  // is cleaned up. This is necessary because in many cases it is the base class
//  // pointer that is being deleted, and this would normally result in a leak
//  // of the bytespace object in the derived class.
//  switch (this->eGetClassNo()) {
//    case CN_PACKEDSTRINGBUFTINY:      _pPackedStringSegmentTiny(this)->vDestroy();   break;
//    case CN_PACKEDSTRINGBUFSMALL:    _pPackedStringSegmentSmall(this)->vDestroy();  break;
//    case CN_PACKEDSTRINGBUFMEDIUM:  _pPackedStringSegmentMedium(this)->vDestroy(); break;
//    case CN_PACKEDSTRINGBUFLARGE:    _pPackedStringSegmentLarge(this)->vDestroy();  break;
//    default:
//      NEVER_REACH
//  }
}

/*----------------------------------------------------------------------------*/
/* add an item to the packed string buffer                                    */
/*----------------------------------------------------------------------------*/
Stz* PackedStringBuf::pAdd(Stz* szItem)
{
  ASSERT(m_pHead == this);

  return pAdd(szItem, szItem->nSize());
}

/*----------------------------------------------------------------------------*/
/* add an item to the packed string buffer. Size must include null char       */
/*----------------------------------------------------------------------------*/
Stz* PackedStringBuf::pAdd(Stz* szItem, i8 iSize)
{
  ASSERT(m_pHead == this || this->m_pHead); // This function is recursive

  if (this->bCanFitInCurrentSeg(iSize)) {
    Stz* szDst;
    switch (this->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    szDst =   _pPackedStringSegmentTiny(this)->pAddToSeg(szItem, iSize); break;
      case CN_PACKEDSTRINGBUFSMALL:   szDst =  _pPackedStringSegmentSmall(this)->pAddToSeg(szItem, iSize); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  szDst = _pPackedStringSegmentMedium(this)->pAddToSeg(szItem, iSize); break;
      case CN_PACKEDSTRINGBUFLARGE:   szDst =  _pPackedStringSegmentLarge(this)->pAddToSeg(szItem, iSize); break;
      default:
        NEVER_REACH
    }
    ++(this->m_pHead->m_nItems);
    return szDst;
  }

  // If there is a next segment then can we fit it in there?
  if (this->m_pNext) {
    Stz* szDst = m_pNext->pAdd(szItem, iSize);  // recursive
    ASSERT(szDst);
    return szDst;
  }

  // There is no next segment that will fit this string so we need to create one
  PackedStringBuf* pSegment = pAddSegmentFor(iSize);
  ASSERT(pSegment);
  Stz* szDst;
  switch (pSegment->eGetClassNo()) {
    case CN_PACKEDSTRINGBUFTINY:    szDst =   _pPackedStringSegmentTiny(pSegment)->pAddToSeg(szItem, iSize); break;
    case CN_PACKEDSTRINGBUFSMALL:   szDst =  _pPackedStringSegmentSmall(pSegment)->pAddToSeg(szItem, iSize); break;
    case CN_PACKEDSTRINGBUFMEDIUM:  szDst = _pPackedStringSegmentMedium(pSegment)->pAddToSeg(szItem, iSize); break;
    case CN_PACKEDSTRINGBUFLARGE:   szDst =  _pPackedStringSegmentLarge(pSegment)->pAddToSeg(szItem, iSize); break;
    default:
      NEVER_REACH
  }

  ASSERT(szDst);
  return szDst;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given address falls within range of one of the segments*/
/*----------------------------------------------------------------------------*/
bool PackedStringBuf::bContainsAddr(Stz* szAddr)
{
  ASSERT(this == m_pHead);
  ASSERT(szAddr);

  PackedStringBuf* pSegment = this;
  do {
    bool bResult = false;

    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    bResult = _pPackedStringSegmentTiny(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFSMALL:   bResult = _pPackedStringSegmentSmall(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  bResult = _pPackedStringSegmentMedium(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFLARGE:   bResult = _pPackedStringSegmentLarge(pSegment)->bSegContainsAddr(szAddr); break;
      default:
        NEVER_REACH
    }

    if (bResult) {
      return true;
    }

    pSegment = pSegment->m_pNext;
  } while (pSegment);

  return false;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PackedStringBuf::aFooter(void)
{
  switch (this->eGetClassNo()) {
    case CN_PACKEDSTRINGBUFTINY:    return _pPackedStringSegmentTiny(this)->aFooter();
    case CN_PACKEDSTRINGBUFSMALL:   return _pPackedStringSegmentSmall(this)->aFooter();
    case CN_PACKEDSTRINGBUFMEDIUM:  return _pPackedStringSegmentMedium(this)->aFooter();
    case CN_PACKEDSTRINGBUFLARGE:   return _pPackedStringSegmentLarge(this)->aFooter();
    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* get number of items in all chained packed string buffer segments           */
/*----------------------------------------------------------------------------*/
i8 PackedStringBuf::nItems(void)
{
  ASSERT(this->m_pHead);

  if (this == this->m_pHead) {
    return m_nItems;
  } else {
    return this->m_pHead->m_nItems;
  }
}

/*----------------------------------------------------------------------------*/
/* Clear the buffer and all extra chained segments                            */
/*----------------------------------------------------------------------------*/
void PackedStringBuf::vReset(void)
{
  ASSERT(m_pHead == this);

//  if (m_pNext){
//    i8 iDebug = 1;
//    UNUSED(iDebug);
//  }

  PackedStringBuf::vDestroy(m_pNext); // deletes all chained segments
  m_pNext = nullptr;
  m_nItems = 0;

  // Reset the byte space in the current segment
  switch (this->eGetClassNo()) {
    case CN_PACKEDSTRINGBUFTINY:     _pPackedStringSegmentTiny(this)->vResetSeg(); break;
    case CN_PACKEDSTRINGBUFSMALL:   _pPackedStringSegmentSmall(this)->vResetSeg(); break;
    case CN_PACKEDSTRINGBUFMEDIUM: _pPackedStringSegmentMedium(this)->vResetSeg(); break;
    case CN_PACKEDSTRINGBUFLARGE:   _pPackedStringSegmentLarge(this)->vResetSeg(); break;
    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Remove a string from the buffer space and wipe the original contents       */
/*----------------------------------------------------------------------------*/
void PackedStringBuf::vRemove(Stz* szItem)
{
  ASSERT(m_pHead == this);
  ASSERT(szItem);

  // Find which segment this string belongs to
  PackedStringBuf* pSegment = pSegmentOf(szItem);
  ASSERT(pSegment);

  bool bIsEmpty = false;

  switch (pSegment->eGetClassNo()) {
    case CN_PACKEDSTRINGBUFTINY:     _pPackedStringSegmentTiny(pSegment)->vRemoveFromSeg(szItem, &bIsEmpty); break;
    case CN_PACKEDSTRINGBUFSMALL:   _pPackedStringSegmentSmall(pSegment)->vRemoveFromSeg(szItem, &bIsEmpty); break;
    case CN_PACKEDSTRINGBUFMEDIUM: _pPackedStringSegmentMedium(pSegment)->vRemoveFromSeg(szItem, &bIsEmpty); break;
    case CN_PACKEDSTRINGBUFLARGE:   _pPackedStringSegmentLarge(pSegment)->vRemoveFromSeg(szItem, &bIsEmpty); break;
    default:
      NEVER_REACH
  }

  --(this->m_pHead->m_nItems);


  // If this segment doesn't contain any strings and it's not the head
  // then remove it from the chain
  if (bIsEmpty && pSegment != pSegment->m_pHead) {

    // Find the parent segment of the one we want to remove
    ASSERT(this->m_pHead == this);

    PackedStringBuf* pParentSegment = this;
    while (pParentSegment != nullptr && pParentSegment->m_pNext != pSegment) {
      pParentSegment = pParentSegment->m_pNext;
    }

    ASSERT(pParentSegment);
    ASSERT(pParentSegment->m_pNext == pSegment);
    // Remove the empty segment from the chain
    pParentSegment->m_pNext = pSegment->m_pNext;
    pSegment->m_pNext = nullptr;
    PackedStringBuf::vDestroy(pSegment);
  }
}

/*----------------------------------------------------------------------------*/
/* return the number of chained segments                                      */
/*----------------------------------------------------------------------------*/
i8 PackedStringBuf::nSegments(void)
{
  ASSERT(m_pHead == this);
  i8 nSegments = 0;

  PackedStringBuf* pSegment = this;
  do {
    ++nSegments;
    pSegment = pSegment->m_pNext;
  } while (pSegment != nullptr);

  return nSegments;
}

/*----------------------------------------------------------------------------*/
/* return the total area of all chained segments                              */
/*----------------------------------------------------------------------------*/
i8 PackedStringBuf::iTotalArea(void)
{
  ASSERT(m_pHead == this);
  i8 iTotArea = 0;

  PackedStringBuf* pSegment = this;
  do {

    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    iTotArea +=   PackedStringSegmentTiny::nArea(); break;
      case CN_PACKEDSTRINGBUFSMALL:   iTotArea +=  PackedStringSegmentSmall::nArea(); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  iTotArea += PackedStringSegmentMedium::nArea(); break;
      case CN_PACKEDSTRINGBUFLARGE:   iTotArea +=  PackedStringSegmentLarge::nArea(); break;
      default:
        NEVER_REACH
    }

    pSegment = pSegment->m_pNext;
  } while (pSegment != nullptr);

  return iTotArea;
}

/*----------------------------------------------------------------------------*/
/* Replace szOldItem with szNewItem being copied into a buffer                */
/*----------------------------------------------------------------------------*/
Stz* PackedStringBuf::pUpdate(Stz* szOldItem, Stz* szNewItem)
{
  ASSERT(m_pHead == this);

  // Find the segment that szOldItem is in
  PackedStringBuf* pSegment = pSegmentOf(szOldItem);
  ASSERT(pSegment);

  // Optimisation, if length(szNewItem) is <= length(szOldItem) and szOldItem
  // is not the last string in the segment then we can write over szOldItem.
  // If it is the last string in the segment then we want to call vRemove()
  // as the new size might be significantly smaller than the old size and we
  // could potentially remove a segment out of the chain.
  i8 iOldSize = szOldItem->nSize();
  i8 iNewSize = szNewItem->nSize();

  if (iNewSize <= iOldSize) {
    i8 nEntriesInSeg;

    // Get the number of entries in this segment
    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    nEntriesInSeg =   _pPackedStringSegmentTiny(pSegment)->nEntriesInSeg(); break;
      case CN_PACKEDSTRINGBUFSMALL:   nEntriesInSeg =  _pPackedStringSegmentSmall(pSegment)->nEntriesInSeg(); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  nEntriesInSeg = _pPackedStringSegmentMedium(pSegment)->nEntriesInSeg(); break;
      case CN_PACKEDSTRINGBUFLARGE:   nEntriesInSeg =  _pPackedStringSegmentLarge(pSegment)->nEntriesInSeg(); break;
      default:
        NEVER_REACH
    }

    if (nEntriesInSeg == 1) {
      switch (pSegment->eGetClassNo()) {
        case CN_PACKEDSTRINGBUFTINY:     _pPackedStringSegmentTiny(pSegment)->vUpdateInPlace(szOldItem, iOldSize, szNewItem); break;
        case CN_PACKEDSTRINGBUFSMALL:   _pPackedStringSegmentSmall(pSegment)->vUpdateInPlace(szOldItem, iOldSize, szNewItem); break;
        case CN_PACKEDSTRINGBUFMEDIUM: _pPackedStringSegmentMedium(pSegment)->vUpdateInPlace(szOldItem, iOldSize, szNewItem); break;
        case CN_PACKEDSTRINGBUFLARGE:   _pPackedStringSegmentLarge(pSegment)->vUpdateInPlace(szOldItem, iOldSize, szNewItem); break;
        default:
          NEVER_REACH
      }

      return szOldItem;
    }

      // fall out
  }

  vRemove(szOldItem);
  Stz* szDst = pAdd(szNewItem);
  return szDst;
}



/*----------------------------------------------------------------------------*/
/*-- Private/Protected Functions----------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Can a given string size fit in the current segment                         */
/*----------------------------------------------------------------------------*/
bool PackedStringBuf::bCanFitInCurrentSeg(i8 iSize)
{
  ASSERT(m_pHead == this || m_pHead);

  switch (this->eGetClassNo()) {
    case CN_PACKEDSTRINGBUFTINY:    return   _pPackedStringSegmentTiny(this)->bCanFitInSeg(iSize);
    case CN_PACKEDSTRINGBUFSMALL:   return  _pPackedStringSegmentSmall(this)->bCanFitInSeg(iSize);
    case CN_PACKEDSTRINGBUFMEDIUM:  return _pPackedStringSegmentMedium(this)->bCanFitInSeg(iSize);
    case CN_PACKEDSTRINGBUFLARGE:   return  _pPackedStringSegmentLarge(this)->bCanFitInSeg(iSize);
    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns the segment that contains the given string address, or nullptr     */
/*----------------------------------------------------------------------------*/
PackedStringBuf* PackedStringBuf::pSegmentOf(Stz* szItem)
{
  ASSERT(szItem);
  ASSERT(this == m_pHead);

  PackedStringBuf* pSegment = this;

  do {
    bool bResult;

    switch (this->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    bResult =   _pPackedStringSegmentTiny(this)->bSegContainsAddr(szItem); break;
      case CN_PACKEDSTRINGBUFSMALL:   bResult =  _pPackedStringSegmentSmall(this)->bSegContainsAddr(szItem); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  bResult = _pPackedStringSegmentMedium(this)->bSegContainsAddr(szItem); break;
      case CN_PACKEDSTRINGBUFLARGE:   bResult =  _pPackedStringSegmentLarge(this)->bSegContainsAddr(szItem); break;
      default:
        NEVER_REACH
    }

    if (bResult) {
      break;
    }
  } while ((pSegment = pSegment->m_pNext) != nullptr);

  return pSegment;
}


/*----------------------------------------------------------------------------*/
/* Adds a segment of a given minimum size to the end of the chain             */
/*----------------------------------------------------------------------------*/
PackedStringBuf* PackedStringBuf::pAddSegmentFor(i8 iSize)
{
  ASSERT(iSize > 0);
  ASSERT(this == m_pHead || m_pHead);

  PackedStringBuf* pNewSegment;

  if (iSize <= PackedStringSegmentTiny::nArea()) {
    pNewSegment = new PackedStringSegmentTiny(this->m_pHead);

  } else if (iSize <= PackedStringSegmentSmall::nArea()) {
    pNewSegment = new PackedStringSegmentSmall(this->m_pHead);

  } else if (iSize <= PackedStringSegmentMedium::nArea()) {
    pNewSegment = new PackedStringSegmentMedium(this->m_pHead);

  } else if (iSize <= PackedStringSegmentLarge::nArea()) {
    pNewSegment = new PackedStringSegmentLarge(this->m_pHead);

  } else {
    NEVER_REACH
  }

  // Attach this segment to the chain
  PackedStringBuf* pSeg = this;
  while (pSeg->m_pNext) {
    pSeg = pSeg->m_pNext;
  }

  pSeg->m_pNext = pNewSegment;
  return pNewSegment;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void* PackedStringSegment<CLASSNO, AREA>::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CLASSNO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringSegment<CLASSNO, AREA>::operator delete(void* aAddr)
{
  PackedStringSegment<CLASSNO, AREA>* pPackedStringBuf =
    (PackedStringSegment<CLASSNO, AREA>*)(aAddr);

  ASSERT(pPackedStringBuf->m_pNext == nullptr);
  Heaps::vFree(pPackedStringBuf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringSegment<CLASSNO, AREA>::PackedStringSegment()
  : PackedStringBuf(CLASSNO)
{
  m_nSegEntries = 0;
  m_xBytes.vWipe();

  _checkInitialisedSize_(sizeof(PackedStringSegment<CLASSNO, AREA>));
}

/*----------------------------------------------------------------------------*/
/* private constructor                                                        */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringSegment<CLASSNO, AREA>::PackedStringSegment(PackedStringBuf* pHead)
  : PackedStringBuf(CLASSNO, pHead)
{
  m_nSegEntries = 0;
  m_xBytes.vWipe();

  _checkInitialisedSize_(sizeof(PackedStringSegment<CLASSNO, AREA>));
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringSegment<CLASSNO, AREA>::~PackedStringSegment(void) noexcept(false)
{

}


/*----------------------------------------------------------------------------*/
/* add an item to the packed string buffer. Size must include null char       */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Stz* PackedStringSegment<CLASSNO, AREA>::pAddToSeg(Stz* szItem, i8 iSize)
{
  ASSERT(m_pHead == this || this->m_pHead);
  ASSERT(this->bCanFitInSeg(iSize));
  ASSERT(m_nSegEntries >= 0);

  Stz* szDst = m_xBytes.szLatter();
  m_xBytes.iAppend(szItem);
  ++m_nSegEntries;
  return szDst;
}

/*----------------------------------------------------------------------------*/
/* returns the buffer size of this packed string buffer segment               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
i8 PackedStringSegment<CLASSNO, AREA>::nArea()
{
  return AREA;
}

/*----------------------------------------------------------------------------*/
/* returns true if the current segment can fit a string of size iSize         */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
bool PackedStringSegment<CLASSNO, AREA>::bCanFitInSeg(i8 iSize)
{
  return m_xBytes.iRemaining() >= iSize;
}

/*----------------------------------------------------------------------------*/
/* Do any cleanup for this object is being deleted                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringSegment<CLASSNO, AREA>::vDestroy(void)
{
  // none necessary as the byte array is not dynamically allocated in this object
}

/*----------------------------------------------------------------------------*/
/* returns the number of entries in this segment                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
i8 PackedStringSegment<CLASSNO, AREA>::nEntriesInSeg()
{
  return m_nSegEntries;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given address falls within range of one of the segments*/
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
bool PackedStringSegment<CLASSNO, AREA>::bSegContainsAddr(Stz* szAddr)
{
  ASSERT(szAddr);
  return m_xBytes.bContains(szAddr);
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Footer* PackedStringSegment<CLASSNO, AREA>::aFooter(void)
{
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* Clear the buffer and all extra chained segments                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringSegment<CLASSNO, AREA>::vResetSeg(void)
{
  m_nSegEntries = 0;
  m_xBytes.vReset();
  m_xBytes.vWipe();
}

/*----------------------------------------------------------------------------*/
/* Remove a string from the buffer space and wipe the original contents       */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringSegment<CLASSNO, AREA>::vRemoveFromSeg(Stz* szItem, bool* pbIsEmpty)
{
  ASSERT(szItem);
  ASSERT(m_xBytes.bContains(szItem));
  ASSERT(pbIsEmpty);
  ASSERT(m_nSegEntries > 0);

  i8 nItemSize = szItem->nSize();

  // Write nulls over the existing string. We don't try to compact or move
  // any data because this adds significant complexity for not much benefit.
  // However we write the nulls to reduce confusion in debugging and potentially
  // for compaction later (if we deem this necessary).
  m_xBytes.vWipe(szItem);

  // If it is the last string allocated then return space back to the byte array
  if (m_xBytes.szLatter() == aAdd<Stz*>(szItem, nItemSize + 1)) {
    m_xBytes.vGive(nItemSize);
  }

  *pbIsEmpty = m_xBytes.bIsEmpty();

  --m_nSegEntries;
}

/*----------------------------------------------------------------------------*/
/* Replace szOldItem with szNewItem being copied into a buffer                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringSegment<CLASSNO, AREA>::vUpdateInPlace(Stz* szOldItem, i8 iOldSize, Stz* szNewItem)
{
  ASSERT(szNewItem->nSize() <= iOldSize);
  ASSERT(m_xBytes.bContains(szOldItem));
  ASSERT(m_nSegEntries > 0);

  // Zero out the entire old string before copying the new one
  m_xBytes.vWipe(szOldItem);
  StrLib::vCopy(szOldItem->azChar(), iOldSize, szNewItem->azChar());
}


/*----------------------------------------------------------------------------*/
/*-- Private/Protected Functions----------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
