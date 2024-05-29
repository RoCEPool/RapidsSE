/*============================================================================*/
/* PackedStringBuf.hpp -- A packed, segmented container for storing strings   */
/*============================================================================*/
#pragma once

#include "Bytes_t.hpp"
#include "StrLib.hpp"

namespace PRIMITIVES
{

  // A base class that hides the segmentation as well as size differences of each segment.
  // All user calls go through this base class and not the templated derived class.
  class PackedStringBuf : public Object
  {
  protected:
    PackedStringBuf*  m_pHead;
    PackedStringBuf*  m_pNext;
    i8                m_nItems;

  public:
    enum eSIZE { TINY, SMALL, MEDIUM, LARGE };
    static PackedStringBuf* pCreate(eSIZE eFirstSize);
    ~PackedStringBuf() noexcept(false);
  protected:
    // Constructors are protected, so they aren't instantiated directly
    explicit PackedStringBuf(eCN eClassNo);
    explicit PackedStringBuf(eCN eClassNo, PackedStringBuf* pHead);

  public:
    Stz*    pAdd(Stz* szItem);
    Stz*    pAdd(Stz* szItem, i8 iSize);
    bool    bContainsAddr(Stz* szAddr);
    Footer* aFooter(void);
    i8      nItems(void);
    void    vReset(void);
    void    vRemove(Stz* szItem);
    i8      nSegments(void);
    i8      iTotalArea(void);
    Stz*    pUpdate(Stz* szOldItem, Stz* szNewItem);

  private:
    bool bCanFitInCurrentSeg(i8 iSize);
    PackedStringBuf* pSegmentOf(Stz* szItem);
    PackedStringBuf* pAddSegmentFor(i8 iSize);
  };


  // A derived class that adds the variable sized byte array for storing the packed strings.
  template <eCN CLASSNO, i8 AREA>
  class PackedStringSegment : public PackedStringBuf
  {
    // Base class will invoke protected members of derived class
    friend class PackedStringBuf;

  protected:
    i8            m_nSegEntries;
    Bytes_t<AREA> m_xBytes;

    // must be last
    Footer m_xFooter;

  protected:
    void* operator new(size_t);
    void  operator delete(void*);

    PackedStringSegment();
    explicit PackedStringSegment(PackedStringBuf* pHead);
   ~PackedStringSegment(void) noexcept(false);

    static i8 nArea(void);

    Stz*    pAddToSeg(Stz* szItem, i8 iSize);
    bool    bCanFitInSeg(i8 iSize);
    i8      nEntriesInSeg();
    bool    bSegContainsAddr(Stz* szAddr);
    Footer* aFooter(void);
    void    vResetSeg(void);
    void    vRemoveFromSeg(Stz* szItem, bool* pbIsEmpty);
    void    vUpdateInPlace(Stz* szOldItem, i8 iOldSize, Stz* szNewItem);

  };



  typedef PackedStringSegment<CN_PACKEDSTRINGBUFTINY, C_SIZE_TINY_OBJECT - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)>     PackedStringSegmentTiny;
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFSMALL, C_SIZE_SMALL_OBJECT - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)>   PackedStringSegmentSmall;
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFMEDIUM, C_SIZE_MEDIUM_OBJECT - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringSegmentMedium;
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFLARGE, C_SIZE_LARGE_OBJECT - (6 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)>   PackedStringSegmentLarge;

  // Note: The '6 * C_BYTES_PER_WORD' in the above template parameter is calculated
  // from the size of the SegmentedArrayNL_t class without the array of objects:
  //  Object header (derived)       1 word
  //  SegmentedArrayNL_t* m_pHead   1 word
  //  SegmentedArrayNL_t* m_pNext   1 word
  //  i8 m_nItems                   1 word
  //  Footer                        2 words
  //  ------                        -------
  //  Total                         6 words

  PackedStringSegmentTiny*   _pPackedStringSegmentTiny(void* aAddr);
  PackedStringSegmentSmall*  _pPackedStringSegmentSmall(void* aAddr);
  PackedStringSegmentMedium* _pPackedStringSegmentMedium(void* aAddr);
  PackedStringSegmentLarge*  _pPackedStringSegmentLarge(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
