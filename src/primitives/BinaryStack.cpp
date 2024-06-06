/*============================================================================*/
/* BinaryStack.hpp -- A segmented stack of arbitrary binary data              */
/*============================================================================*/

#include "BASE.h"
#include "BinaryStack.hpp"

using namespace PRIMITIVES;

// Custom cast function macro that doesn't check class number
#define __pBinaryStackCastFunction(SPACENM,CLASSNM) \
  CLASSNM* SPACENM::_p##CLASSNM(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
    return pObject; \
  }

__pBinaryStackCastFunction(PRIMITIVES,BinaryStackTiny)
__pBinaryStackCastFunction(PRIMITIVES,BinaryStackSmall)
__pBinaryStackCastFunction(PRIMITIVES,BinaryStackMedium)
__pBinaryStackCastFunction(PRIMITIVES,BinaryStackLarge)


/*----------------------------------------------------------------------------*/
/* Segment Size enum string values                                            */
/*----------------------------------------------------------------------------*/

Stz* aSS[eSS::NENUMS]
{
  "ST_TINY"_Stz,
  "ST_SMALL"_Stz,
  "ST_MEDIUM"_Stz,
  "ST_LARGE"_Stz,
};

Stz* aSSValue[eSS::NENUMS]
{
  "TINY"_Stz,
  "SMALL"_Stz,
  "MEDIUM"_Stz,
  "LARGE"_Stz,
};



/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
BinaryStack::BinaryStack(eCN eClassNo,
                         eSS eSegmentSize,
                         fStackSizeToClass fSizeToCls)
 : Object(eClassNo)

{
  this->m_pHead = this;
  this->m_pNext = nullptr;
  this->m_eSegmentSize = eSegmentSize;
  this->m_nItems = 0;

#ifdef WITH_CHECK_INITIALISED
  // The m_fStackSizeToClass leaves a word uninitialised
  MemLib::vZero(&(this->m_fStackSizeToClass), sizeof(m_fStackSizeToClass), eClassNo);
#endif

  this->m_fStackSizeToClass = fSizeToCls;
  this->u.m_pBase = this;

  //_checkInitialised_(eClassNo);  // Can't call this on an abstract base class
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
BinaryStack::BinaryStack(eCN eClassNo,
                         eSS eSegmentSize,
                         fStackSizeToClass fSizeToCls,
                         BinaryStack* pHead)
  : Object(eClassNo)
{
  this->m_pHead = pHead;
  this->m_pNext = nullptr;
  this->m_eSegmentSize = eSegmentSize;
  this->m_nItems = 0;

#ifdef WITH_CHECK_INITIALISED
  // The m_fStackSizeToClass leaves a word uninitialised
  MemLib::vZero(&(this->m_fStackSizeToClass), sizeof(m_fStackSizeToClass), eClassNo);
#endif

  this->m_fStackSizeToClass = fSizeToCls;
  this->u.m_pBase = this;

  //_checkInitialised_(eClassNo);  // Can't call this on an abstract base class
}


/*----------------------------------------------------------------------------*/
/* Destructor                                                                 */
/*----------------------------------------------------------------------------*/
BinaryStack::~BinaryStack() noexcept(false)
{
  if (m_pNext) {
    switch (m_pNext->m_eSegmentSize) {
      case TINY:    delete   _pBinaryStackTiny(m_pNext); break;
      case SMALL:   delete  _pBinaryStackSmall(m_pNext); break;
      case MEDIUM:  delete _pBinaryStackMedium(m_pNext); break;
      case LARGE:   delete  _pBinaryStackLarge(m_pNext); break;
      default:
        NEVER_REACH
    }

    m_pNext = nullptr;
  }
}


/*----------------------------------------------------------------------------*/
/* Advances the stack head by a given number of bytes                         */
/*----------------------------------------------------------------------------*/
byte* BinaryStack::pyAdvance(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(nBytes > 0);
  ASSERT(this == m_pHead);

  // Get the last segment
  BinaryStack* pSeg = pLastSegment();
  ASSERT(pSeg);

  // Will nBytes fit in this segment? If not, add a new segment.
  if (!pSeg->bCanFitInCurrentSeg(nBytes)) {
    BinaryStack* pNewSeg = pAddSegmentFor(nBytes);
    ASSERT(pSeg->m_pNext == pNewSeg);

    pSeg = pNewSeg;
  }

  // Advance the segment pointer by the given number of bytes
  byte* pyHead;

  switch (pSeg->m_eSegmentSize) {
    case TINY:   pyHead =   _pBinaryStackTiny(pSeg)->pySegAdvance(nBytes); break;
    case SMALL:  pyHead =  _pBinaryStackSmall(pSeg)->pySegAdvance(nBytes); break;
    case MEDIUM: pyHead = _pBinaryStackMedium(pSeg)->pySegAdvance(nBytes); break;
    case LARGE:  pyHead =  _pBinaryStackLarge(pSeg)->pySegAdvance(nBytes); break;
    default:      NEVER_REACH
  }

  ++m_nItems;
  return pyHead;
}


/*----------------------------------------------------------------------------*/
/* Create a BinaryStack                                                       */
/*----------------------------------------------------------------------------*/
BinaryStack* BinaryStack::pCreate(eSS eSize, fStackSizeToClass fSizeToCls)
{
  eCN eClassNo = fSizeToCls(eSize);
  ASSERT(eClassNo > CN_NONE);
  ASSERT(eClassNo < CN_ENUMS);

  switch (eSize) {
    case TINY:    return new(eClassNo)   BinaryStackTiny(eClassNo, fSizeToCls);
    case SMALL:   return new(eClassNo)  BinaryStackSmall(eClassNo, fSizeToCls);
    case MEDIUM:  return new(eClassNo) BinaryStackMedium(eClassNo, fSizeToCls);
    case LARGE:   return new(eClassNo)  BinaryStackLarge(eClassNo, fSizeToCls);
    default:
      NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Deallocate BinaryStack                                                     */
/*----------------------------------------------------------------------------*/
void BinaryStack::vDestroy(BinaryStack *pSegment)
{
  if (!pSegment) {
    return;
  }

  // Need to call delete on the derived class
  switch (pSegment->m_eSegmentSize) {

    case TINY:    delete   _pBinaryStackTiny(pSegment); break;
    case SMALL:   delete  _pBinaryStackSmall(pSegment); break;
    case MEDIUM:  delete _pBinaryStackMedium(pSegment); break;
    case LARGE:   delete  _pBinaryStackLarge(pSegment); break;
    default:
      NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns a ptr to the footer in the derived class                           */
/*----------------------------------------------------------------------------*/
Footer* BinaryStack::aFooter(void)
{
  ASSERT(this == m_pHead || m_pHead);

  switch (this->m_eSegmentSize) {
    case TINY:    return   _pBinaryStackTiny(this)->aFooter();
    case SMALL:   return  _pBinaryStackSmall(this)->aFooter();
    case MEDIUM:  return _pBinaryStackMedium(this)->aFooter();
    case LARGE:   return  _pBinaryStackLarge(this)->aFooter();
    default:      NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Get the number of items in the entire stack (not just this segment)        */
/*----------------------------------------------------------------------------*/
i8 BinaryStack::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead || m_pHead);

  return m_pHead->m_nItems;
}


/*----------------------------------------------------------------------------*/
/* Get the pointer to stack item index iItem. 0 is top of the stack.          */
/*----------------------------------------------------------------------------*/
byte* BinaryStack::pyPeekItem(i8 iItem, std::function<i8(byte*)> fStackItemSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead);
  ASSERT(iItem >= 0);
  ASSERT(iItem < m_nItems);

//  Note: The last segment contains the stack head (index 0)
//  Within a segment each stack entry is written from right to left
//  (so it can be read from left to right).
//
//
//    Segment 1          Segment 2         Segment 3
//  +-----------+    +----------------+    +------+
//  | Idx4 Idx5 | -> | Idx1 Idx2 Idx3 | -> | Idx0 |
//  +-----------+    +----------------+    +------+
//     ^                 ^                    ^
//   SegmentHead        SegmentHead         SegmentHead
//                                          StackHead

  // Work out which segment this is in
  BinaryStack* pSeg = this;

  i8 iSegEntries = pSeg->nSegmentEntries();
  i8 iFirstSegIdx = m_nItems - iSegEntries;
  i8 iLastSegIdx = m_nItems - 1;
  UNUSED(iLastSegIdx);

  while (iSegEntries == 0 || iItem < iFirstSegIdx) {
    ASSERT(pSeg->m_pNext);
    pSeg = pSeg->m_pNext;
    iSegEntries = pSeg->nSegmentEntries();
    iLastSegIdx = iFirstSegIdx - 1;
    iFirstSegIdx -= iSegEntries;
  }

  // We've got the right segment, now we just need to find it
  // within this segment. Start by getting the segment head, which
  // is pointing to the first (lowest) index in this segment
  byte* pyStackItem;

  switch (pSeg->m_eSegmentSize) {
    case TINY:   pyStackItem =   _pBinaryStackTiny(pSeg)->pySegmentHead(); break;
    case SMALL:  pyStackItem =  _pBinaryStackSmall(pSeg)->pySegmentHead(); break;
    case MEDIUM: pyStackItem = _pBinaryStackMedium(pSeg)->pySegmentHead(); break;
    case LARGE:  pyStackItem =  _pBinaryStackLarge(pSeg)->pySegmentHead(); break;
    default:      NEVER_REACH
  }

  for (i8 i = iFirstSegIdx; i != iItem; i++) {
    // Get the size of this stack entry
    i8 nBytes = fStackItemSize(pyStackItem);
    pyStackItem += nBytes;
  }

  return pyStackItem;
}



/*----------------------------------------------------------------------------*/
/* Resets a stack back to it's original empty condition                       */
/*----------------------------------------------------------------------------*/
void BinaryStack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead);

  BinaryStack::vDestroy(m_pNext);
  m_pNext = nullptr;
  m_nItems = 0;

  switch (this->m_eSegmentSize) {
    case TINY:     _pBinaryStackTiny(this)->vSegReset(); break;
    case SMALL:   _pBinaryStackSmall(this)->vSegReset(); break;
    case MEDIUM: _pBinaryStackMedium(this)->vSegReset(); break;
    case LARGE:   _pBinaryStackLarge(this)->vSegReset(); break;
    default:     NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Rewinds the stack head by a given number of bytes                          */
/*----------------------------------------------------------------------------*/
void BinaryStack::vRewind(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(nBytes > 0);
  ASSERT(this == m_pHead);

  // Get the last segment
  BinaryStack* pLastSeg = pLastSegment();
  ASSERT(pLastSeg);
  ASSERT(!pLastSeg->m_pNext);
  ASSERT(!pLastSeg->bSegmentIsEmpty());

  // Rewind the stack in this segment

  switch (pLastSeg->m_eSegmentSize) {
    case TINY:     _pBinaryStackTiny(pLastSeg)->vSegRewind(nBytes); break;
    case SMALL:   _pBinaryStackSmall(pLastSeg)->vSegRewind(nBytes); break;
    case MEDIUM: _pBinaryStackMedium(pLastSeg)->vSegRewind(nBytes); break;
    case LARGE:   _pBinaryStackLarge(pLastSeg)->vSegRewind(nBytes); break;
    default:      NEVER_REACH
  }

  // If the segment is empty then we will need to remove it and update the stack head ptr
  if (pLastSeg->bSegmentIsEmpty()) {

    if (pLastSeg == pLastSeg->m_pHead) {
      // This is the first segment. Entire stack is empty
      --m_nItems;
      ASSERT(m_nItems == 0);
      return;
    }

    // Get the second to last segment
    BinaryStack* pSecondLastSeg = m_pHead;
    while (pSecondLastSeg->m_pNext != pLastSeg) {
      pSecondLastSeg = pSecondLastSeg->m_pNext;
    }

    ASSERT(pSecondLastSeg);
    pSecondLastSeg->m_pNext = nullptr;
    BinaryStack::vDestroy(pLastSeg);
  }

  --m_nItems;
  return;
}


/*----------------------------------------------------------------------------*/
/* Get the number of segments in the chain                                    */
/*----------------------------------------------------------------------------*/
i8 BinaryStack::nSegments(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead || m_pHead);

  i8 iCount = 1;
  BinaryStack* pSeg = m_pHead;
  while (pSeg->m_pNext) {
    ++iCount;
    pSeg = pSeg->m_pNext;
  }

  return iCount;
}


/*----------------------------------------------------------------------------*/
/* Returns a pointer to the top item on the stack, or nullptr if empty        */
/*----------------------------------------------------------------------------*/
byte* BinaryStack::pyStackHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead);

  if (m_nItems == 0) {
    ASSERT(!m_pNext);
    ASSERT(bSegmentIsEmpty());

    return nullptr;
  }

  // The stack head is the segment head of the last segment
  BinaryStack* pLastSeg = pLastSegment();

  ASSERT(pLastSeg);
  ASSERT(!pLastSeg->bSegmentIsEmpty());

  byte* pStackHead;

  switch (pLastSeg->m_eSegmentSize) {
    case TINY:    pStackHead =   _pBinaryStackTiny(pLastSeg)->pySegmentHead(); break;
    case SMALL:   pStackHead =  _pBinaryStackSmall(pLastSeg)->pySegmentHead(); break;
    case MEDIUM:  pStackHead = _pBinaryStackMedium(pLastSeg)->pySegmentHead(); break;
    case LARGE:   pStackHead =  _pBinaryStackLarge(pLastSeg)->pySegmentHead(); break;
    default:      NEVER_REACH
  }

  return pStackHead;
}


/*----------------------------------------------------------------------------*/
/* Adds a new segment to the chain and returns it                             */
/*----------------------------------------------------------------------------*/
BinaryStack* BinaryStack::pAddSegmentFor(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(nBytes > 0);
  ASSERT(this == m_pHead);
  ASSERT(this->m_nItems >= 0);

  BinaryStack* pNewSegment;
  eCN eClassNo;

  if (nBytes <= BinaryStackTiny::nArea()) {
    eClassNo = m_fStackSizeToClass(TINY);
    pNewSegment = new(eClassNo) BinaryStackTiny(eClassNo, m_fStackSizeToClass, this->m_pHead);

  } else if (nBytes <= BinaryStackSmall::nArea()) {
    eClassNo = m_fStackSizeToClass(SMALL);
    pNewSegment = new(eClassNo) BinaryStackSmall(eClassNo, m_fStackSizeToClass, this->m_pHead);

  } else if (nBytes <= BinaryStackMedium::nArea()) {
    eClassNo = m_fStackSizeToClass(MEDIUM);
    pNewSegment = new(eClassNo) BinaryStackMedium(eClassNo, m_fStackSizeToClass, this->m_pHead);

  } else if (nBytes <= BinaryStackLarge::nArea()) {
    eClassNo = m_fStackSizeToClass(LARGE);
    pNewSegment = new(eClassNo) BinaryStackLarge(eClassNo, m_fStackSizeToClass, this->m_pHead);

  } else {
    NEVER_REACH
  }

  // Attach this segment to the chain
  BinaryStack* pLastSeg = pLastSegment();
  ASSERT(pLastSeg->m_pNext == nullptr);

  pLastSeg->m_pNext = pNewSegment;
  return pNewSegment;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the current segment can fit the given number of bytes      */
/*----------------------------------------------------------------------------*/
bool BinaryStack::bCanFitInCurrentSeg(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead || m_pHead);

  switch (this->m_eSegmentSize) {
    case TINY:    return   _pBinaryStackTiny(this)->bSegCanFit(nBytes);
    case SMALL:   return  _pBinaryStackSmall(this)->bSegCanFit(nBytes);
    case MEDIUM:  return _pBinaryStackMedium(this)->bSegCanFit(nBytes);
    case LARGE:   return  _pBinaryStackLarge(this)->bSegCanFit(nBytes);
    default:      NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Get the last segment in the chain                                          */
/*----------------------------------------------------------------------------*/
BinaryStack* BinaryStack::pLastSegment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead || m_pHead);

  BinaryStack* pSeg = this;
  while (pSeg->m_pNext) {
    pSeg = pSeg->m_pNext;
  }

  return pSeg;
}


/*----------------------------------------------------------------------------*/
/* Returns the number of entries in the current segment                       */
/*----------------------------------------------------------------------------*/
i8 BinaryStack::nSegmentEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead || m_pHead);

  switch (this->m_eSegmentSize) {
    case TINY:    return   _pBinaryStackTiny(this)->nSegEntries();
    case SMALL:   return  _pBinaryStackSmall(this)->nSegEntries();
    case MEDIUM:  return _pBinaryStackMedium(this)->nSegEntries();
    case LARGE:   return  _pBinaryStackLarge(this)->nSegEntries();
    default:      NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the current segment has no entries                         */
/*----------------------------------------------------------------------------*/
bool BinaryStack::bSegmentIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(this == m_pHead || m_pHead);

  switch (this->m_eSegmentSize) {
    case TINY:    return   _pBinaryStackTiny(this)->bSegIsEmpty();
    case SMALL:   return  _pBinaryStackSmall(this)->bSegIsEmpty();
    case MEDIUM:  return _pBinaryStackMedium(this)->bSegIsEmpty();
    case LARGE:   return  _pBinaryStackLarge(this)->bSegIsEmpty();
    default:      NEVER_REACH
  }
}




/*============================================================================*/
/* BinaryStackSegment.hpp                                                     */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
void* BinaryStackSegment<SEGMENT_SIZE, AREA>::operator new(size_t, eCN eClassNo)
{
  void* pObject = Heaps::pAllocate(eClassNo);

  return pObject;
}


/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
void BinaryStackSegment<SEGMENT_SIZE, AREA>::operator delete(void* aAddr)
{
  BinaryStackSegment<SEGMENT_SIZE, AREA>* pSegment =
      (BinaryStackSegment<SEGMENT_SIZE, AREA>*)(aAddr);

  ASSERT(pSegment->m_pNext == nullptr);
  Heaps::vFree(pSegment);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
BinaryStackSegment<SEGMENT_SIZE, AREA>::BinaryStackSegment(eCN eClassNo,
                                                           fStackSizeToClass fSizeToCls)
  : BinaryStack(eClassNo, SEGMENT_SIZE, fSizeToCls)
{
  m_nSegEntries = 0;
  //m_xBytes
  m_pySegmentHead = (byte*) m_xBytes.szBoundary();

  _checkInitialisedSize_(sizeof(BinaryStackSegment<SEGMENT_SIZE, AREA>));
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
BinaryStackSegment<SEGMENT_SIZE, AREA>::BinaryStackSegment(eCN eClassNo,
                                                          fStackSizeToClass fSizeToCls,
                                                          BinaryStack* pHead)
  : BinaryStack(eClassNo, SEGMENT_SIZE, fSizeToCls, pHead)
{
  m_nSegEntries = 0;
  //m_xBytes
  m_pySegmentHead = (byte*) m_xBytes.szBoundary();

  _checkInitialisedSize_(sizeof(BinaryStackSegment<SEGMENT_SIZE, AREA>));
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
BinaryStackSegment<SEGMENT_SIZE, AREA>::~BinaryStackSegment(void) noexcept(false)
{

}


/*----------------------------------------------------------------------------*/
/* Return the size of the binary buffer in bytes                              */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
i8 BinaryStackSegment<SEGMENT_SIZE, AREA>::nArea(void)
{
  return AREA;
}


/*----------------------------------------------------------------------------*/
/* Return the footer to this segment                                          */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
Footer* BinaryStackSegment<SEGMENT_SIZE, AREA>::aFooter(void)
{
  return &m_xFooter;
}


/*----------------------------------------------------------------------------*/
/* Advances the segment head ptr in this segment                              */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
byte* BinaryStackSegment<SEGMENT_SIZE, AREA>::pySegAdvance(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(nBytes > 0);
  ASSERT(bSegCanFit(nBytes));
  ASSERT(m_pySegmentHead <= (byte*) m_xBytes.szBoundary());

  m_pySegmentHead -= nBytes;
  ++m_nSegEntries;

  return m_pySegmentHead;
}


/*----------------------------------------------------------------------------*/
/* Returns the number of entries in this segment                              */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
i8 BinaryStackSegment<SEGMENT_SIZE, AREA>::nSegEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_nSegEntries;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the stack entry can fit in this segment                    */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
bool BinaryStackSegment<SEGMENT_SIZE, AREA>::bSegCanFit(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  // Byte buffer is used from back to front
  byte* pyAddr = m_pySegmentHead - nBytes;
  byte* pyLimit = (byte*) m_xBytes.szArea();

  return pyAddr >= pyLimit;
}


/*----------------------------------------------------------------------------*/
/* Returns the pointer to the head of this segment only                       */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
byte* BinaryStackSegment<SEGMENT_SIZE, AREA>::pySegmentHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_pySegmentHead;
}


/*----------------------------------------------------------------------------*/
/* Returns true if this segment contains no entries                           */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
bool BinaryStackSegment<SEGMENT_SIZE, AREA>::bSegIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(  (m_nSegEntries == 0 && m_pySegmentHead == (byte*) m_xBytes.szBoundary())
        || (m_nSegEntries  > 0 && m_pySegmentHead  < (byte*) m_xBytes.szBoundary()) );

  return m_nSegEntries == 0;
}


/*----------------------------------------------------------------------------*/
/* Returns this segment to its original empty condition                       */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
void BinaryStackSegment<SEGMENT_SIZE, AREA>::vSegReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  m_nSegEntries = 0;
  m_pySegmentHead = (byte*) m_xBytes.szBoundary();
  m_xBytes.vReset();
}


/*----------------------------------------------------------------------------*/
/* Removes the top item off the stack in this segment                         */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
void BinaryStackSegment<SEGMENT_SIZE, AREA>::vSegRewind(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ASSERT(nBytes > 0);
  ASSERT(m_nSegEntries > 0);
  ASSERT(m_pySegmentHead < (byte*) m_xBytes.szBoundary());

  m_pySegmentHead += nBytes;
  --m_nSegEntries;

  ASSERT(m_pySegmentHead <= (byte*) m_xBytes.szBoundary());
  ASSERT(m_nSegEntries >= 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


