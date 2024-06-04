/*============================================================================*/
/* BinaryStack.hpp -- A segmented stack of arbitrary binary data              */
/*============================================================================*/
#pragma once

#include <functional> // for std::function
#include "Bytes_t.hpp"
#include "ClassNumbers.hpp"

namespace PRIMITIVES
{
  // Segment Size
  enum eSS {
    TINY,
    SMALL,
    MEDIUM,
    LARGE,
    NENUMS
  };

  extern Stz* aSS[NENUMS];
  extern Stz* aSSValue[NENUMS];

  // Forward declarations
  template <eSS SEGMENT_SIZE, i8 AREA> class BinaryStackSegment;
  class BinaryStack;

  // Typedefs of the 4 actual types of buffer segments
  typedef BinaryStackSegment<eSS::TINY,   C_SIZE_TINY_OBJECT   - (10 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> BinaryStackTiny;
  typedef BinaryStackSegment<eSS::SMALL,  C_SIZE_SMALL_OBJECT  - (10 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> BinaryStackSmall;
  typedef BinaryStackSegment<eSS::MEDIUM, C_SIZE_MEDIUM_OBJECT - (10 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> BinaryStackMedium;
  typedef BinaryStackSegment<eSS::LARGE,  C_SIZE_LARGE_OBJECT  - (10 * C_BYTES_PER_WORD) - sizeof(Bytes_t<1>)> BinaryStackLarge;

  // Note: The 12 * C_BYTES_PER_WORD is for the other elements in the BinaryStack and BinaryStackSegment classes:
  //   - m_pHead          (1 word)
  //   - m_pNext          (1 word)
  //   - m_eSegmentSize   (1 word)
  //   - m_nItems         (1 word)
  //   - m_fStackSizeToClass (1 word)
  //   - union u          (1 word)
  //   - m_nSegEntries    (1 word)
  //   - m_aySegmentHead  (1 word)
  //   - m_xFooter        (2 words)

  // Typedefs for functions that translate class numbers to stack sizes and vice versa
  typedef std::function<eSS(eCN eClassNo)> fClassToStackSize;
  typedef std::function<eCN(eSS eSize)>    fStackSizeToClass;


/*----------------------------------------------------------------------------*/
/* BinaryStack - a base class that hides the segmentation as well as size     */
/* differences of each segment. This base class contains all of the           */
/* user-callable methods. A parent object should be responsible for           */
/* serialising data to bytes before it is written into the stack.             */
/*----------------------------------------------------------------------------*/
  class BinaryStack : public Object
  {
  protected:
    BinaryStack*  m_pHead;
    BinaryStack*  m_pNext;
    eSS           m_eSegmentSize;
    i8            m_nItems;
    byte*         m_ayStackHead;  // Head of the overall stack, not the current segment.
                                  // Only valid if this is the head node.
    fStackSizeToClass m_fStackSizeToClass; // maps StackSize to the class number to be used

    union { // For debugging purposes only
      BinaryStackTiny*   m_pTiny;
      BinaryStackSmall*  m_pSmall;
      BinaryStackMedium* m_pMedium;
      BinaryStackLarge*  m_pLarge;
      BinaryStack*       m_pBase;
    } u;

  protected:
    // Constructors and destructor are protected, so they aren't instantiated directly
    explicit BinaryStack(eCN eClassNo, eSS eSize, fStackSizeToClass fSizeToCls);
    explicit BinaryStack(eCN eClassNo, eSS eSize, fStackSizeToClass fSizeToCls, BinaryStack* pHead);
    ~BinaryStack() noexcept(false);

  public:
    void    vAdvance(i8 nBytes);
    Footer* aFooter(void);
    void    vRewind(i8 nBytes);
    i8      nSegments(void);
    byte*   pbStackHead(void);
    i8      nItems(void);

  private:
    BinaryStack* pAddSegmentFor(i8 nBytes);
    bool         bCanFitInCurrentSeg(i8 nBytes);
    BinaryStack* pLastSegment(void);
  };



  // A derived class that adds the variable sized binary stack segment.
  template <eSS SEGMENT_SIZE, i8 AREA>
  class BinaryStackSegment : public BinaryStack
  {
    // Base class will invoke protected members of derived class
    friend class BinaryStack;

  protected:
    i8      m_nSegEntries;
    byte*   m_aySegmentHead;
    Bytes_t<AREA> m_xBytes;

    // must be last
    Footer m_xFooter;

  protected:
    void* operator new(size_t, eCN eClassNo);
    void  operator delete(void*);

    BinaryStackSegment(eCN eClassNo, fStackSizeToClass fSizeToCls);
    explicit BinaryStackSegment(eCN eClassNo, fStackSizeToClass fSizeToCls, BinaryStack* pHead);
    ~BinaryStackSegment(void) noexcept(false);

    static i8 nArea(void);
    bool bSegCanFit(i8 nBytes);

//    static i8 nArea(void);
//
//    Stz*    pAddToSeg(Stz* szItem);
//    bool    bCanFitInSeg(i8 iSize);
//    void    vDestroy(void);
//    i8      nEntriesInSeg();
//    bool    bSegContainsAddr(Stz* szAddr);
//    Footer* aFooter(void);
//    void    vResetSeg(void);
//    void    vRemoveFromSeg(Stz* szItem, bool* pbIsEmpty);
//    void    vUpdateInPlace(Stz* szOldItem, i8 iOldSize, Stz* szNewItem);

  };



  // Casting
  BinaryStackTiny*   _pBinaryStackTiny(void* aAddr);
  BinaryStackSmall*  _pBinaryStackSmall(void* aAddr);
  BinaryStackMedium* _pBinaryStackMedium(void* aAddr);
  BinaryStackLarge*  _pBinaryStackLarge(void* aAddr);
}