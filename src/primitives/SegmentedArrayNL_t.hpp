/*============================================================================*/
/* SegmentedArrayNL_t.hpp -- A templated, segmented array that expands automatically */
/*============================================================================*/
#pragma once

namespace PRIMITIVES
{
  // Forward declaration
  template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG> class SegmentedArrayNL_t;

  // Segmented array iterator
  template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
  class SegmentedArrayIter : public Object
  {
  protected:
    SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* m_pCurrSegment;
    i8 m_iSegment;  // 0 based segment index
    i8 m_iOffset;   // 0 based offset in the current segment

    // must be last
    Footer m_xFooter;

  public:
    SegmentedArrayIter(SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>* pSegmentedArray);
    ~SegmentedArrayIter(void) noexcept(false);


    bool  bIsAfterLast();
    bool  bIsBeforeFirst();
    bool  bHasNext();
    bool  bHasPrevious();
    ITEM* pNext();
    ITEM* pPrevious();
    ITEM* operator *();
  };

////////////////////////////////////////////////////////////////////////////////

  template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
  class SegmentedArrayNL_t : public Object
  {
    friend class SegmentedArrayIter<CLASSNO, ITEM, OBJS_PER_SEG>;

  protected:
    ITEM  m_aObjects[OBJS_PER_SEG]; // The array of objects in this segment.
    i8    m_nItems;                 // Number of items. Only valid for first segment.
    SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>* m_pNext;    // The next segment in the chain. Or nullptr.
    SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>* m_pHead;    // Pointer to the first segment in the chain.

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SegmentedArrayNL_t();
   ~SegmentedArrayNL_t(void) noexcept(false);

    void  vAdd(const ITEM& oItem);
    Footer* aFooter(void);
    ITEM* pGet(i8 iIdx);
    SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG> xIterator();
    i8    nItems(void);
    void  vReset(void);
    void  vRemove(i8 iIdx);
    void  vSet(i8 iIdx, const ITEM& oItem);


  protected:
    SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>* pAddSegment(void);
    SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>* pSegmentOf(i8 iIdx);

  };
}

using namespace PRIMITIVES;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
void* SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CLASSNO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
void SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::operator delete(void* aAddr)
{
  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSegmentedArrayNL_t =
    (SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>*)(aAddr);

  ASSERT(pSegmentedArrayNL_t->m_pNext == nullptr);
  Heaps::vFree(pSegmentedArrayNL_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::SegmentedArrayNL_t()
: Object(CLASSNO)
{
  m_nItems = 0;
  m_pNext  = nullptr;
  m_pHead  = this;
  
  _checkInitialisedSize_(sizeof(SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>));
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::~SegmentedArrayNL_t(void) noexcept(false)
{
  if (m_pNext) {
    delete m_pNext;
    m_pNext = nullptr;
  }
}


/*----------------------------------------------------------------------------*/
/* add an item to the array                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
void SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::vAdd(const ITEM& oItem)
{
  ASSERT(m_pHead == this);

  i8 iNextIdx = m_nItems;
  i8 iOffset = iNextIdx % OBJS_PER_SEG;

  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSeg = pSegmentOf(iNextIdx);
  if (!pSeg) {
    pSeg = pAddSegment();
  }

  pSeg->vAddToThisSegment(iOffset, oItem);
  ++m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
Footer* SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::aFooter(void)
{
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* get an item in the array                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
ITEM* SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::pGet(i8 iIdx)
{
  ASSERT(m_pHead == this);
  ASSERT(iIdx >= 0);

  if (iIdx >= m_nItems) {
    return nullptr;
  }

  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSeg;
  pSeg = pSegmentOf(iIdx);

  ITEM* pItem = &(pSeg->m_aObjects[iIdx % OBJS_PER_SEG]);
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG> SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::xIterator(void)
{
  ASSERT(m_pHead == this);
  SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG> xIter(this);
  return xIter;
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
i8 SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* Clear the array any all extra array segments                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
void SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::vReset(void)
{
  ASSERT(m_pHead == this);
  delete m_pNext; // deletes all segments
  m_pNext = nullptr;
  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* Clear the array any all extra array segments                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
void SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::vRemove(i8 iIdx)
{
  ASSERT(m_pHead == this);
  ASSERT(iIdx >= 0);
  ASSERT(iIdx < m_nItems);

  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSeg;
  pSeg = pSegmentOf(iIdx);

  i8 iOffset = iIdx % OBJS_PER_SEG;

  while (iIdx + 1 < m_nItems) {

    iOffset = iIdx % OBJS_PER_SEG;
    if (iOffset + 1 == OBJS_PER_SEG) {
      ASSERT(pSeg->m_pNext);
      pSeg->m_aObjects[iOffset] = pSeg->m_pNext->m_aObjects[0];
    } else {
      pSeg->m_aObjects[iOffset] = pSeg->m_aObjects[iOffset + 1];
    }

    ++iIdx;
  }

  --m_nItems;
}

/*----------------------------------------------------------------------------*/
/* Clear the array any all extra array segments                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
void SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::vSet(i8 iIdx, const ITEM& oItem)
{
  ASSERT(m_pHead == this);
  ASSERT(iIdx >= 0);
  ASSERT(iIdx < m_nItems);

  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSeg;
  pSeg = pSegmentOf(iIdx);

  pSeg->m_aObjects[iIdx % OBJS_PER_SEG] = oItem;
}


/*----------------------------------------------------------------------------*/
/*-- Private/Protected Functions----------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Add a segment to the end of the chain and return it                        */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>*
    SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::pAddSegment(void)
{
  ASSERT(m_pHead == this);
  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pNewSeg = new SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>();

  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSeg = this;
  while (pSeg->m_pNext) {
    pSeg = pSeg->m_pNext;
  }

  pSeg->m_pNext = pNewSeg;
  return pNewSeg;
}

/*----------------------------------------------------------------------------*/
/* Get the segment for a given array index                                    */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>*
    SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>::pSegmentOf(i8 iIdx)
{
  ASSERT(iIdx >= 0);
  i8 iSeg = iIdx / OBJS_PER_SEG;

  SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSeg = this;

  while (iSeg-- > 0) {
    ASSERT(pSeg);
    pSeg = pSeg->m_pNext;
  }

  return pSeg;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* Construct a new iterator before the beginning                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::SegmentedArrayIter(SegmentedArrayNL_t<CLASSNO,ITEM,OBJS_PER_SEG>* pSegmentedArray)
 : Object(CN_SEGMENTEDARRAYITER)
{
  ASSERT(pSegmentedArray == pSegmentedArray->m_pHead);
  m_pCurrSegment = pSegmentedArray;
  m_iSegment = 0;
  m_iOffset = -1;
}

/*----------------------------------------------------------------------------*/
/* Destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::~SegmentedArrayIter(void) noexcept(false)
{

}

/*----------------------------------------------------------------------------*/
/* Is the iterator beyond the last item?                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
bool SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::bIsAfterLast()
{
  i8 nItems = m_pCurrSegment->m_pHead->m_nItems;
  i8 iCurrIdx = m_iSegment * OBJS_PER_SEG + m_iOffset;

  return (iCurrIdx >= nItems);
}

/*----------------------------------------------------------------------------*/
/* Is the iterator before the first item?                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
bool SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::bIsBeforeFirst()
{
  return (m_iSegment == 0 && m_iOffset < 0);
}

/*----------------------------------------------------------------------------*/
/* Is there a next item in the array?                                         */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
bool SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::bHasNext()
{
  i8 nItems = m_pCurrSegment->m_pHead->m_nItems;
  i8 iCurrIdx = m_iSegment * OBJS_PER_SEG + m_iOffset;

  return (iCurrIdx + 1 < nItems);
}

/*----------------------------------------------------------------------------*/
/* Is there a previous item in the array?                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
bool SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::bHasPrevious()
{
  i8 nItems = m_pCurrSegment->m_pHead->m_nItems;
  i8 iCurrIdx = m_iSegment * OBJS_PER_SEG + m_iOffset;

  return (iCurrIdx - 1 >= 0 && iCurrIdx < nItems);
}

/*----------------------------------------------------------------------------*/
/* Get the next item in the array, or nullptr                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
ITEM* SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::pNext()
{
  if (!bHasNext()) {
    return nullptr;
  }

  if (++m_iOffset == OBJS_PER_SEG) {
    m_iOffset = 0;
    ++m_iSegment;

    ASSERT(m_pCurrSegment->m_pNext);
    m_pCurrSegment = m_pCurrSegment->m_pNext;
  }

  return &(m_pCurrSegment->m_aObjects[m_iOffset]);
}

/*----------------------------------------------------------------------------*/
/* Get the previous item in the array, or nullptr                             */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
ITEM* SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::pPrevious()
{
  if (!bHasPrevious()) {
    return nullptr;
  }

  if (--m_iOffset == -1) {
    m_iOffset = OBJS_PER_SEG - 1;
    --m_iSegment;


    // Find the previous segment by iterating from the head
    SegmentedArrayNL_t<CLASSNO, ITEM, OBJS_PER_SEG>* pSeg = m_pCurrSegment->m_pHead;
    ASSERT(m_pCurrSegment != m_pCurrSegment->m_pHead);

    i8 i = 0;
    while (i++ < m_iSegment) {
      ASSERT(pSeg->m_pNext);
      pSeg = pSeg->m_pNext;
    }

    m_pCurrSegment = pSeg;
  }

  return &(m_pCurrSegment->m_aObjects[m_iOffset]);
}

/*----------------------------------------------------------------------------*/
/* Get the current item that the iterator is pointing at                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 OBJS_PER_SEG>
ITEM* SegmentedArrayIter<CLASSNO,ITEM,OBJS_PER_SEG>::operator*()
{
  i8 nItems = m_pCurrSegment->m_pHead->m_nItems;
  i8 iCurrIdx = m_iSegment * OBJS_PER_SEG + m_iOffset;

  if (iCurrIdx < 0 || iCurrIdx >= nItems) {
    return nullptr;
  }

  return &(m_pCurrSegment->m_aObjects[m_iOffset]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
