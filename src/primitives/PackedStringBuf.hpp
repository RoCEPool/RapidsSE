/*============================================================================*/
/* PackedStringBuf.hpp -- A packed, segmented container for storing strings   */
/*============================================================================*/
#pragma once

#include "Bytes_t.hpp"
#include "StrLib.hpp"

namespace PRIMITIVES
{

  // Forward declarations to allow the typedef
  template <eCN CLASSNO, i8 AREA> class PackedStringSegment;
  class PackedStringBuf;

  // Typedefs of the 4 actual types of buffer segments
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFTINY,   C_SIZE_TINY_OBJECT   - (8 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringSegmentTiny;
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFSMALL,  C_SIZE_SMALL_OBJECT  - (8 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringSegmentSmall;
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFMEDIUM, C_SIZE_MEDIUM_OBJECT - (8 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringSegmentMedium;
  typedef PackedStringSegment<CN_PACKEDSTRINGBUFLARGE,  C_SIZE_LARGE_OBJECT  - (8 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> PackedStringSegmentLarge;

  // Note: The '8 * C_BYTES_PER_WORD' in the above template parameter is calculated
  // from the size of the PackedStringSegment class on its own and without the
  // array of objects (which is why we can't use sizeof()). The -1 on the end is
  // because then the size of the object will equal the size of the allocated chunk
  // and it must be smaller than this.
  //    Object header                 1 word
  //    PackedStringBuf:
  //      ptr m_pHead                 1 word
  //      ptr m_pNext                 1 word
  //      union of ptrs               1 word
  //    PackedStringSegment:
  //      i8 m_nItems                 1 word
  //      i8 m_nSegEntries            1 word
  //      Footer                      2 words
  //    ------                        -------
  //    Total                         8 words


  // A base class that hides the segmentation as well as size differences of each segment.
  // All user calls go through this base class and not the templated derived class.
  class PackedStringBuf : public Object
  {
  protected:
    PackedStringBuf*  m_pHead;
    PackedStringBuf*  m_pNext;
    i8                m_nItems;
    union { // For debugging purposes
      PackedStringSegmentTiny*   m_pTiny;
      PackedStringSegmentSmall*  m_pSmall;
      PackedStringSegmentMedium* m_pMedium;
      PackedStringSegmentLarge*  m_pLarge;
      void*                      m_pVoid;
    } u;

  public:
    enum eSIZE { TINY, SMALL, MEDIUM, LARGE };
    static PackedStringBuf* pCreate(eSIZE eFirstSize);
    static void vDestroy(PackedStringBuf* pBuf);

    static bool bSerialise(PackedStringBuf* pPackedStringBuf, Bytes* pBuf);
    static PackedStringBuf* pDeserialise(Bytes* ayBuf, i8& iPos);

  protected:
    // Constructors and destructor are protected, so they aren't instantiated directly
    explicit PackedStringBuf(eCN eClassNo);
    explicit PackedStringBuf(eCN eClassNo, PackedStringBuf* pHead);
    ~PackedStringBuf() noexcept(false);

  public:
    Stz*    pAdd(Stz* szItem);
    Stz*    pAdd(Stz* szItem, i8 iSize);
    Stz*    szAt(i8 iOffset);
    bool    bContainsAddr(Stz* szAddr);
    Footer* aFooter(void);
    i8      nItems(void);
    i8      iOffsetOf(Stz* szAddr);
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
    static bool bSerialise(PackedStringSegment* pSeg, Bytes* pBuf);
    static PackedStringSegment* pDeserialise(Bytes* ayBuf, i8& iPos, PackedStringBuf* pHead);

    Stz*    pAddToSeg(Stz* szItem);
    Stz*    szAtSegOffset(i8 iSegOffset);
    bool    bCanFitInSeg(i8 iSize);
    void    vDestroy(void);
    i8      nEntriesInSeg();
    bool    bSegContainsAddr(Stz* szAddr);
    i8      iSegOffsetOf(Stz* szAddr);
    Footer* aFooter(void);
    void    vResetSeg(void);
    void    vRemoveFromSeg(Stz* szItem, bool* pbIsEmpty);
    void    vUpdateInPlace(Stz* szOldItem, i8 iOldSize, Stz* szNewItem);

  };



  // Casting
  PackedStringSegmentTiny*   _pPackedStringSegmentTiny(void* aAddr);
  PackedStringSegmentSmall*  _pPackedStringSegmentSmall(void* aAddr);
  PackedStringSegmentMedium* _pPackedStringSegmentMedium(void* aAddr);
  PackedStringSegmentLarge*  _pPackedStringSegmentLarge(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
