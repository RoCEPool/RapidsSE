/*============================================================================*/
/* PackedStringBuf.cpp -- A packed, segmented container for storing strings   */
/*============================================================================*/

#include "BASE.h"
#include "PackedStringBuf.hpp"
#include "Bytes.hpp"

using namespace PRIMITIVES;

extern AttachMap* gpAttachMap;

static const Stz* C_BEGIN_STRBUF    = "Begin_StrBuf"_Stz;
static const Stz* C_END_STRBUF      = "End_StrBuf"_Stz;
static const Stz* C_BEGIN_STRSEG    = "Begin_StrSeg"_Stz;
static const Stz* C_END_STRSEG      = "End_StrSeg"_Stz;

static const i8 C_SIZE_BEGIN_STRBUF  = C_BEGIN_STRBUF->nLength() + 1;
static const i8 C_SIZE_END_STRBUF    = C_END_STRBUF->nLength() + 1;
static const i8 C_SIZE_BEGIN_STRSEG  = C_BEGIN_STRSEG->nLength() + 1;
static const i8 C_SIZE_END_STRSEG    = C_END_STRSEG->nLength() + 1;

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
    default:
      NEVER_REACH
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
/* serialise a PackedStringBuf to a byte buffer                               */
/*----------------------------------------------------------------------------*/
bool PackedStringBuf::bSerialise(PackedStringBuf* pPackedStringBuf, Bytes* pBuf)
{
  _static_

  ASSERT(pPackedStringBuf);
  ASSERT(pBuf);
  ASSERT(pBuf->i_Latter() >= 0);


  // (1) First write a basic human-readable string for debugging/verification purposes
  pBuf->iAppend(C_BEGIN_STRBUF);

  // (2) First write the number of items in the entire buffer
  pBuf->iAppend(pPackedStringBuf->nItems());

  // (3) Write the number of segments
  pBuf->iAppend(pPackedStringBuf->nSegments());

  // Serialise each segment
  PackedStringBuf* pSeg = pPackedStringBuf;
  do {
    RASSERT(pBuf->iRemaining() >= 0);

    // (4) Write the class number as this indicates the size of the segment
    pBuf->iAppend((i8) pSeg->eGetClassNo());

    // (5) Serialise the data in the current segment
    switch (pSeg->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:      PackedStringSegmentTiny::bSerialise(  _pPackedStringSegmentTiny(pSeg), pBuf); break;
      case CN_PACKEDSTRINGBUFSMALL:    PackedStringSegmentSmall::bSerialise( _pPackedStringSegmentSmall(pSeg), pBuf); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  PackedStringSegmentMedium::bSerialise(_pPackedStringSegmentMedium(pSeg), pBuf); break;
      case CN_PACKEDSTRINGBUFLARGE:    PackedStringSegmentLarge::bSerialise( _pPackedStringSegmentLarge(pSeg), pBuf); break;
      default:
        NEVER_REACH
    }
  } while ((pSeg = pSeg->m_pNext) != nullptr);

  // (8) Write a short human-readable footer
  pBuf->iAppend(C_END_STRBUF);

  RASSERT(pBuf->iRemaining() >= 0);
  return true;
}

/*----------------------------------------------------------------------------*/
/* deserialise a tagset from a byte buffer                                    */
/*----------------------------------------------------------------------------*/
PackedStringBuf* PackedStringBuf::pDeserialise(Bytes* pBuf, i8& iPos)
{
  _static_

  ASSERT(pBuf);
  ASSERT(pBuf->i_Latter() > 0);
  ASSERT(iPos >= 0);

  PackedStringBuf* pPackedStringBuf = nullptr;


  // (1) Read C_BEGIN_STRBUF
  ASSERT(pBuf->szGet(iPos)->bEqual(C_BEGIN_STRBUF));
  iPos += C_SIZE_BEGIN_STRBUF;

  // (2) Read the number of items in the entire buffer
  i8 nItems = pBuf->iGet(iPos);
  iPos += sizeof(i8);
  ASSERT(nItems >= 0);

  // (3) Read the number of segments
  i8 nSegments = pBuf->iGet(iPos);
  iPos += sizeof(i8);
  ASSERT(nSegments > 0);

  PackedStringBuf* pHead = nullptr;
  PackedStringBuf* pSeg;
  PackedStringBuf* pLastSeg = nullptr;

  for (i8 i=0; i<nSegments; i++) {
    // (4) Read the class number of the segment
    eCN eSegClass = (eCN) pBuf->iGet(iPos);
    iPos += sizeof(i8);

    // (5) Deserialise the segment
    switch (eSegClass) {
      case BASE::CN_PACKEDSTRINGBUFTINY:
        pSeg = PackedStringSegmentTiny::pDeserialise(pBuf, iPos, pHead);
        break;

      case BASE::CN_PACKEDSTRINGBUFSMALL:
        pSeg = PackedStringSegmentSmall::pDeserialise(pBuf, iPos, pHead);
        break;

      case BASE::CN_PACKEDSTRINGBUFMEDIUM:
        pSeg = PackedStringSegmentMedium::pDeserialise(pBuf, iPos, pHead);
        break;

      case BASE::CN_PACKEDSTRINGBUFLARGE:
        pSeg = PackedStringSegmentLarge::pDeserialise(pBuf, iPos, pHead);
        break;

      default: NEVER_REACH
    }

    if (!pHead) {
      pHead = pSeg;
    }

    if (!pPackedStringBuf) {
      pPackedStringBuf = pSeg;
      pPackedStringBuf->m_nItems = nItems;

      ASSERT(pPackedStringBuf->m_pHead == pSeg);
      ASSERT(!pPackedStringBuf->m_pNext);
      ASSERT(!pLastSeg);

    } else {
      ASSERT(pSeg->m_pHead == pPackedStringBuf);
      ASSERT(pLastSeg);
      pLastSeg->m_pNext = pSeg;
    }

    pLastSeg = pSeg;
  }

  // (6) Read C_END_STRBUF
  ASSERT(pBuf->szGet(iPos)->bEqual(C_END_STRBUF));
  iPos += C_SIZE_END_STRBUF;


  return pPackedStringBuf;
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
  this->m_pHead = (pHead == nullptr ? this : pHead);
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
}

/*----------------------------------------------------------------------------*/
/* add an item to the packed string buffer                                    */
/*----------------------------------------------------------------------------*/
Stz* PackedStringBuf::pAdd(Stz* szItem)
{
  ASSERT(m_pHead == this);

  if (!szItem) {
    szItem = ""_Stz;
  }

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
      case CN_PACKEDSTRINGBUFTINY:    szDst =   _pPackedStringSegmentTiny(this)->pAddToSeg(szItem); break;
      case CN_PACKEDSTRINGBUFSMALL:   szDst =  _pPackedStringSegmentSmall(this)->pAddToSeg(szItem); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  szDst = _pPackedStringSegmentMedium(this)->pAddToSeg(szItem); break;
      case CN_PACKEDSTRINGBUFLARGE:   szDst =  _pPackedStringSegmentLarge(this)->pAddToSeg(szItem); break;
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
    case CN_PACKEDSTRINGBUFTINY:    szDst =   _pPackedStringSegmentTiny(pSegment)->pAddToSeg(szItem); break;
    case CN_PACKEDSTRINGBUFSMALL:   szDst =  _pPackedStringSegmentSmall(pSegment)->pAddToSeg(szItem); break;
    case CN_PACKEDSTRINGBUFMEDIUM:  szDst = _pPackedStringSegmentMedium(pSegment)->pAddToSeg(szItem); break;
    case CN_PACKEDSTRINGBUFLARGE:   szDst =  _pPackedStringSegmentLarge(pSegment)->pAddToSeg(szItem); break;
    default:
      NEVER_REACH
  }

  ASSERT(szDst);
  return szDst;
}

/*----------------------------------------------------------------------------*/
/* Returns the string pointer for the given offset from the buffer beginning  */
/*----------------------------------------------------------------------------*/
Stz* PackedStringBuf::szAt(i8 iOffset) {
  ASSERT(this == m_pHead);
  ASSERT(iOffset >= 0);
  ASSERT(iOffset < iTotalArea());

  // Get the seg and the offset into that segment

  PackedStringBuf* pSegment = this;
  i8 iLower = 0;

  do {
    i8   iArea;

    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    iArea =   _pPackedStringSegmentTiny(pSegment)->nArea(); break;
      case CN_PACKEDSTRINGBUFSMALL:   iArea =  _pPackedStringSegmentSmall(pSegment)->nArea(); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  iArea = _pPackedStringSegmentMedium(pSegment)->nArea(); break;
      case CN_PACKEDSTRINGBUFLARGE:   iArea =  _pPackedStringSegmentLarge(pSegment)->nArea(); break;
      default:
        NEVER_REACH
    }

    if (iOffset >= iLower && iOffset < iLower + iArea) {
      // It's in this segment
      i8 iSegOffset = iOffset - iLower;

      switch (pSegment->eGetClassNo()) {
        case CN_PACKEDSTRINGBUFTINY:    return   _pPackedStringSegmentTiny(pSegment)->szAtSegOffset(iSegOffset);
        case CN_PACKEDSTRINGBUFSMALL:   return  _pPackedStringSegmentSmall(pSegment)->szAtSegOffset(iSegOffset);
        case CN_PACKEDSTRINGBUFMEDIUM:  return _pPackedStringSegmentMedium(pSegment)->szAtSegOffset(iSegOffset);
        case CN_PACKEDSTRINGBUFLARGE:   return  _pPackedStringSegmentLarge(pSegment)->szAtSegOffset(iSegOffset);
        default:
          NEVER_REACH
      }
      NEVER_REACH
    }

    iLower += iArea;
    pSegment = pSegment->m_pNext;
  } while (pSegment != nullptr);

  NEVER_REACH
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
      case CN_PACKEDSTRINGBUFTINY:    bResult =   _pPackedStringSegmentTiny(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFSMALL:   bResult =  _pPackedStringSegmentSmall(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  bResult = _pPackedStringSegmentMedium(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFLARGE:   bResult =  _pPackedStringSegmentLarge(pSegment)->bSegContainsAddr(szAddr); break;
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
/* Returns the size enum of this first buffer                                 */
/*----------------------------------------------------------------------------*/
PackedStringBuf::eSIZE PackedStringBuf::eFirstBufSize()
{
  ASSERT(this == m_pHead);

  switch (this->eGetClassNo()) {
    case CN_PACKEDSTRINGBUFTINY:    return TINY;
    case CN_PACKEDSTRINGBUFSMALL:   return SMALL;
    case CN_PACKEDSTRINGBUFMEDIUM:  return MEDIUM;
    case CN_PACKEDSTRINGBUFLARGE:   return LARGE;
    default:
      NEVER_REACH
  }
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
/* get the offset from the start of the buffer for a given address            */
/*----------------------------------------------------------------------------*/
i8 PackedStringBuf::iOffsetOf(Stz* szAddr)
{
  ASSERT(this->m_pHead);

  PackedStringBuf* pSegment = this;
  i8 iLower = 0;

  do {
    i8   iArea;
    bool bHasAddr;

    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    bHasAddr =   _pPackedStringSegmentTiny(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFSMALL:   bHasAddr =  _pPackedStringSegmentSmall(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  bHasAddr = _pPackedStringSegmentMedium(pSegment)->bSegContainsAddr(szAddr); break;
      case CN_PACKEDSTRINGBUFLARGE:   bHasAddr =  _pPackedStringSegmentLarge(pSegment)->bSegContainsAddr(szAddr); break;
      default:
        NEVER_REACH
    }

    if (!bHasAddr) {
      switch (pSegment->eGetClassNo()) {
        case CN_PACKEDSTRINGBUFTINY:    iArea =   _pPackedStringSegmentTiny(pSegment)->nArea(); break;
        case CN_PACKEDSTRINGBUFSMALL:   iArea =  _pPackedStringSegmentSmall(pSegment)->nArea(); break;
        case CN_PACKEDSTRINGBUFMEDIUM:  iArea = _pPackedStringSegmentMedium(pSegment)->nArea(); break;
        case CN_PACKEDSTRINGBUFLARGE:   iArea =  _pPackedStringSegmentLarge(pSegment)->nArea(); break;
        default:
          NEVER_REACH
      }

      iLower += iArea;
      pSegment = pSegment->m_pNext;
      continue;
    }

    // It's in this segment. Get the offset of this string into this specific segment
    i8 iSegOffset = 0;

    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    iSegOffset =   _pPackedStringSegmentTiny(pSegment)->iSegOffsetOf(szAddr); break;
      case CN_PACKEDSTRINGBUFSMALL:   iSegOffset =  _pPackedStringSegmentSmall(pSegment)->iSegOffsetOf(szAddr); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  iSegOffset = _pPackedStringSegmentMedium(pSegment)->iSegOffsetOf(szAddr); break;
      case CN_PACKEDSTRINGBUFLARGE:   iSegOffset =  _pPackedStringSegmentLarge(pSegment)->iSegOffsetOf(szAddr); break;
      default:
        NEVER_REACH
    }

    return iLower + iSegOffset;

  } while (pSegment != nullptr);

  NEVER_REACH
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

    switch (pSegment->eGetClassNo()) {
      case CN_PACKEDSTRINGBUFTINY:    bResult =   _pPackedStringSegmentTiny(pSegment)->bSegContainsAddr(szItem); break;
      case CN_PACKEDSTRINGBUFSMALL:   bResult =  _pPackedStringSegmentSmall(pSegment)->bSegContainsAddr(szItem); break;
      case CN_PACKEDSTRINGBUFMEDIUM:  bResult = _pPackedStringSegmentMedium(pSegment)->bSegContainsAddr(szItem); break;
      case CN_PACKEDSTRINGBUFLARGE:   bResult =  _pPackedStringSegmentLarge(pSegment)->bSegContainsAddr(szItem); break;
      default:
        NEVER_REACH
    }

    if (bResult) {
      break;
    }

    pSegment = pSegment->m_pNext;
  } while (pSegment != nullptr);

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
void* PackedStringSegment<CLASSNO, AREA>::operator new(size_t iObjSize)
{
  void* pObject;

  // This is a hack because TagSet is used before the allocators are setup
  // and ready by the SysArgs class to record command line arguments. These
  // instances, once setup, really shouldn't be changing.
  if (gpAttachMap) {
    pObject = Heaps::pAllocate(CLASSNO);
  } else {
    pObject = MemLib::aMallocBlock(iObjSize);
  }
  
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

  if (gpAttachMap && AM::pHeaps() && AM::bInBufferPool(pPackedStringBuf)) {
    Heaps::vFree(pPackedStringBuf);
  } else {
    // For the case of running the status command on the command line
    // but SE is not actually running (so no heaps exist).
    free(pPackedStringBuf);
  }
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
Stz* PackedStringSegment<CLASSNO, AREA>::pAddToSeg(Stz* szItem)
{
  ASSERT(m_pHead == this || this->m_pHead);
  ASSERT(this->bCanFitInSeg(szItem->nSize()));
  ASSERT(m_nSegEntries >= 0);

  Stz* szDst = m_xBytes.szLatter();
  m_xBytes.iAppend(szItem);
  ++m_nSegEntries;
  return szDst;
}


/*----------------------------------------------------------------------------*/
/* Get a Stz* for a given offset into this particular segment                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Stz* PackedStringSegment<CLASSNO, AREA>::szAtSegOffset(i8 iSegOffset)
{
  ASSERT(iSegOffset >= 0);
  ASSERT(iSegOffset < AREA);
  ASSERT(m_nSegEntries > 0);

  return m_xBytes.szGet(iSegOffset);
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
/* serialise a PackedStringBuf to a byte buffer                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
bool PackedStringSegment<CLASSNO, AREA>::bSerialise(PackedStringSegment<CLASSNO, AREA>* pSeg, Bytes* pBuf)
{
  _static_

  ASSERT(pSeg);
  ASSERT(pBuf);
  ASSERT(pBuf->i_Latter() >= 0);


  // (1) Write a short human-readable header text for this segment
  pBuf->iAppend(C_BEGIN_STRSEG);

  // (2) Write the number of entries in this segment
  pBuf->iAppend(pSeg->m_nSegEntries);

  // (3) Now write the size of the data in this segment
  i8 iStrSize = pSeg->m_xBytes.i_Latter();
  pBuf->iAppend(iStrSize);

  // (4) Write the data in this segment
  pBuf->iAppend(reinterpret_cast<byte*>(pSeg->m_xBytes.szArea()), iStrSize);

  // (5) Write a human-readable footer for the end of this segment's data
  pBuf->iAppend(C_END_STRSEG);

  RASSERT(pBuf->iRemaining() >= 0);
  return true;
}

/*----------------------------------------------------------------------------*/
/* deserialise a byte buffer to a PackedStringSegment                         */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringSegment<CLASSNO,AREA>* PackedStringSegment<CLASSNO, AREA>::pDeserialise(Bytes* pBuf, i8& iPos, PackedStringBuf* pHead)
{
  _static_

  ASSERT(pBuf);
  ASSERT(pBuf->i_Latter() > 0);
  ASSERT(iPos >= 0);

  PackedStringSegment<CLASSNO,AREA>* pSeg = new PackedStringSegment<CLASSNO,AREA>(pHead);

  // (1) Read C_BEGIN_STRSEG
  ASSERT(pBuf->szGet(iPos)->bEqual(C_BEGIN_STRSEG));
  iPos += C_SIZE_BEGIN_STRSEG;

  // (2) Read in the number of entries in the segment
  pSeg->m_nSegEntries = pBuf->iGet(iPos);
  iPos += sizeof(i8);
  ASSERT(pSeg->m_nSegEntries >= 0);

  // (3) Read in the size of the string data in this segment
  i8 iStrSize = pBuf->iGet(iPos);
  iPos += sizeof(i8);
  ASSERT(iStrSize >= 0);

  // (4) Read in the string data
  pSeg->m_xBytes.iAppend(reinterpret_cast<byte*>(pBuf->szGet(iPos)), iStrSize);
  iPos += iStrSize;

  // (5) Read C_END_STRSEG
  ASSERT(pBuf->szGet(iPos)->bEqual(C_END_STRSEG));
  iPos += C_SIZE_END_STRSEG;

  return pSeg;
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
/* Returns the offset of the address to this specific segment                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
i8 PackedStringSegment<CLASSNO, AREA>::iSegOffsetOf(Stz* szAddr)
{
  ASSERT(szAddr);
  i8 iSegOffset = nDifference(m_xBytes.szArea(), szAddr);

  ASSERT(iSegOffset >= 0);
  ASSERT(iSegOffset < AREA);

  return iSegOffset;
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
