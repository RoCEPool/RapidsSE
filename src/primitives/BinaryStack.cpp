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
  this->m_ayStackHead = nullptr;            // TODO XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
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
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Adds a new segment to the chain and returns it                             */
/*----------------------------------------------------------------------------*/
BinaryStack* BinaryStack::pAddSegmentFor(i8 nBytes)
{
  ASSERT(nBytes > 0);
  ASSERT(this == m_pHead);
  ASSERT(this->m_nItems > 0);

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
  ASSERT(this == m_pHead || m_pHead);

  switch (m_eSegmentSize) {
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
  ASSERT(this == m_pHead || m_pHead);

  BinaryStack* pSeg = this;
  while (m_pNext) {
    pSeg = m_pNext;
  }

  return pSeg;
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
  m_aySegmentHead = (byte*) m_xBytes.szBoundary();

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
  m_aySegmentHead = (byte*) m_xBytes.szBoundary();

  _checkInitialisedSize_(sizeof(BinaryStackSegment<SEGMENT_SIZE, AREA>));
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eSS SEGMENT_SIZE, i8 AREA>
BinaryStackSegment<SEGMENT_SIZE, AREA>::~BinaryStackSegment(void) noexcept(false)
{

}

