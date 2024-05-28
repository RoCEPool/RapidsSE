/*============================================================================*/
/* PackedStringBuf.hpp -- A packed, segmented container for storing strings   */
/*============================================================================*/
#pragma once

#include "Bytes_t.hpp"
#include "StrLib.hpp"

namespace PRIMITIVES
{
  class PackedStringBufBase : public Object
  {
  public:
    explicit PackedStringBufBase(eCN eClassNo);
  };

  template <eCN CLASSNO, i8 AREA>
  class PackedStringBuf : public PackedStringBufBase
  {
  protected:
    PackedStringBuf* m_pHead;
    PackedStringBuf* m_pNext;
    i8               m_nItems;
    Bytes_t<AREA>    m_xBytes;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PackedStringBuf();
    explicit PackedStringBuf(PackedStringBuf* pHead);
   ~PackedStringBuf(void) noexcept(false);

    static i8 nArea(void);

    Stz*    pAdd(Stz* szItem);
    Stz*    pAdd(Stz* szItem, i8 iSize);
    Footer* aFooter(void);
    i8      nItems(void);
    void    vReset(void);
    void    vRemove(Stz* szItem);
    Stz*    pUpdate(Stz* szOldItem, Stz* szNewItem);

  private:
    bool bCanFit(i8 iSize);
    PackedStringBufBase* pSegmentOf(Stz* szItem);
    PackedStringBufBase* pAddSegmentFor(i8 iSize);
  };



  typedef PackedStringBuf<CN_PACKEDSTRINGBUFTINY,   C_SIZE_TINY_OBJECT   - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringBufTiny;
  typedef PackedStringBuf<CN_PACKEDSTRINGBUFSMALL,  C_SIZE_SMALL_OBJECT  - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringBufSmall;
  typedef PackedStringBuf<CN_PACKEDSTRINGBUFMEDIUM, C_SIZE_MEDIUM_OBJECT - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringBufMedium;
  typedef PackedStringBuf<CN_PACKEDSTRINGBUFLARGE,  C_SIZE_LARGE_OBJECT  - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringBufLarge;

  // Note: The '6 * C_BYTES_PER_WORD' in the above template parameter is calculated
  // from the size of the SegmentedArrayNL_t class without the array of objects:
  //  Object header (derived)       1 word
  //  SegmentedArrayNL_t* m_pHead   1 word
  //  SegmentedArrayNL_t* m_pNext   1 word
  //  i8 m_nItems                   1 word
  //  Footer                        2 words
  //  ------                        -------
  //  Total                         6 words
}

using namespace PRIMITIVES;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* Constructor of base class                                                  */
/*----------------------------------------------------------------------------*/
PackedStringBufBase::PackedStringBufBase(BASE::eCN eClassNo)
: Object (eClassNo)
{

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void* PackedStringBuf<CLASSNO, AREA>::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CLASSNO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringBuf<CLASSNO, AREA>::operator delete(void* aAddr)
{
  PackedStringBuf<CLASSNO, AREA>* pPackedStringBuf =
    (PackedStringBuf<CLASSNO, AREA>*)(aAddr);

  ASSERT(pPackedStringBuf->m_pNext == nullptr);
  Heaps::vFree(pPackedStringBuf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringBuf<CLASSNO, AREA>::PackedStringBuf()
  : PackedStringBufBase(CLASSNO)
{
  m_pHead  = this;
  m_pNext  = nullptr;
  m_nItems = 0;
  m_xBytes.vWipe();

  _checkInitialisedSize_(sizeof(PackedStringBuf<CLASSNO, AREA>));
}

/*----------------------------------------------------------------------------*/
/* private constructor                                                        */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringBuf<CLASSNO, AREA>::PackedStringBuf(PackedStringBuf<CLASSNO, AREA>* pHead)
  : PackedStringBufBase(CLASSNO)
{
  m_pHead  = pHead;
  m_pNext  = nullptr;
  m_nItems = 0;
  m_xBytes.vWipe();

  _checkInitialisedSize_(sizeof(PackedStringBuf<CLASSNO, AREA>));
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringBuf<CLASSNO, AREA>::~PackedStringBuf(void) noexcept(false)
{
  if (m_pNext) {
    delete m_pNext;
    m_pNext = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* add an item to the packed string buffer                                    */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Stz* PackedStringBuf<CLASSNO, AREA>::pAdd(Stz* szItem)
{
  ASSERT(m_pHead == this);

  return pAdd(szItem, szItem->nSize());
}

/*----------------------------------------------------------------------------*/
/* add an item to the packed string buffer. Size must include null char       */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Stz* PackedStringBuf<CLASSNO, AREA>::pAdd(Stz* szItem, i8 iSize)
{
  ASSERT(m_pHead == this || this->m_pHead);

  if (this->bCanFit(iSize)) {
    Stz* szDst = m_xBytes.szLatter();
    m_xBytes.vTake(iSize);
    m_xBytes.iAppend(szItem);
    ++(this->m_pHead->m_nItems);
    return szDst;
  }

  // If there is a next segment then can we fit it in there?
  if (this->m_pNext) {
    Stz* szDst = m_pNext->pAdd(szItem, iSize);
    ASSERT(szDst);
    return szDst;
  }

  // There is no next segment that will fit this string so we need to create one
  PackedStringBuf* pSegment = pAddSegmentFor(iSize);
  ASSERT(pSegment);
  Stz* szDst = pSegment->vAdd(szItem, iSize);
  ASSERT(szDst);
  return szDst;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Footer* PackedStringBuf<CLASSNO, AREA>::aFooter(void)
{
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* get number of items in all chained packed string buffer segments           */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
i8 PackedStringBuf<CLASSNO, AREA>::nItems(void)
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
template <eCN CLASSNO, i8 AREA>
void PackedStringBuf<CLASSNO, AREA>::vReset(void)
{
  ASSERT(m_pHead == this);
  delete m_pNext; // deletes all chained segments
  m_pNext = nullptr;
  m_nItems = 0;
  m_xBytes.vReset();
  m_xBytes.vWipe();
}

/*----------------------------------------------------------------------------*/
/* Remove a string from the buffer space and wipe the original contents       */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
void PackedStringBuf<CLASSNO, AREA>::vRemove(Stz* szItem)
{
  ASSERT(m_pHead == this);
  ASSERT(szItem);

  // Find which segment this belongs to
  PackedStringBuf* pSegment = pSegmentOf(szItem);
  ASSERT(pSegment);

  // Write nulls over the existing string. We don't try to compact or move
  // any data because this adds significant complexity for not much benefit.
  pSegment->m_xBytes.vWipe(szItem);
  --(this->m_pHead->m_nItems);

  // If it is the last string allocated then return space back to the byte array
  i8 nItemSize = szItem->nSize();
  if (pSegment->m_xBytes.szLatter() == aAdd<Stz*>(szItem, nItemSize + 1)) {
    pSegment->m_xBytes.vGive(nItemSize);
  }

  // If this segment doesn't contain any strings and it's not the head
  // then remove it from the chain
  if (pSegment != pSegment->m_pHead &&
      pSegment->m_xBytes.isEmpty()) {

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
    delete pSegment;
  }
}

/*----------------------------------------------------------------------------*/
/* Replace szOldItem with szNewItem being copied into a buffer                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
Stz* PackedStringBuf<CLASSNO, AREA>::pUpdate(Stz* szOldItem, Stz* szNewItem)
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

  if (iNewSize <= iOldSize &&
      pSegment->m_xBytes.szLatter() != aAdd<Stz*>(szOldItem, iOldSize + 1)) {
    pSegment->m_xBytes.vWipe(szOldItem);
    StrLib::vCopy(szOldItem->azChar(), iOldSize, szNewItem->azChar());
    return szOldItem;
  }

  vRemove(szOldItem);
  Stz* szDst = pAdd(szNewItem);
  return szDst;
}


/*----------------------------------------------------------------------------*/
/*-- Private/Protected Functions----------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* returns the buffer size of this packed string buffer segment               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
i8 PackedStringBuf<CLASSNO, AREA>::nArea()
{
  return AREA;
}

/*----------------------------------------------------------------------------*/
/* returns true if the current segment can fit a string of size iSize         */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
bool PackedStringBuf<CLASSNO, AREA>::bCanFit(i8 iSize)
{
  return m_xBytes.iRemaining() >= iSize;
}

/*----------------------------------------------------------------------------*/
/* Returns the segment that contains the given string address                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringBufBase* PackedStringBuf<CLASSNO, AREA>::pSegmentOf(Stz* szItem)
{
  ASSERT(szItem);
  ASSERT(this == m_pHead);

  PackedStringBuf* pSegment = this;

  do {
    if (pSegment->m_xBytes.bContains(szItem)) {
      break;
    }
  } while ((pSegment = pSegment->m_pNext) != nullptr);

  return pSegment;
}

/*----------------------------------------------------------------------------*/
/* Adds a segment of a given minimum size to the end of the chain             */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, i8 AREA>
PackedStringBufBase* PackedStringBuf<CLASSNO, AREA>::pAddSegmentFor(i8 iSize)
{
  ASSERT(iSize > 0);
  ASSERT(this == m_pHead);

  PackedStringBuf* pNewSegment;

  if (iSize <= PackedStringBufTiny::nArea()) {
    pNewSegment = new PackedStringBufTiny(this->m_pHead);

  } else if (iSize <= PackedStringBufSmall::nArea()) {
    pNewSegment = new PackedStringBufSmall(this->m_pHead);

  } else if (iSize <= PackedStringBufMedium::nArea()) {
    pNewSegment = new PackedStringBufMedium(this->m_pHead);

  } else if (iSize <= PackedStringBufLarge::nArea()) {
    pNewSegment = new PackedStringBufLarge(this->m_pHead);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
