/*============================================================================*/
/* ListNL_t__.hpp -- List of Items (non-locking)                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <class ITEM> 
  class ListNL_t__ 
  {
  protected:
    i8    m_nItems;  // number of items
    ITEM* m_pHead;   // head item
    ITEM* m_pTail;   // tail item

    // must be last
    Footer m_xFooter;
    
  public:
    ListNL_t__(void);
    
    void  vAppend(ITEM* pItem);
    i8    iFind(ITEM* pItem);
    ITEM* pGet(i8 i_Item);
    ITEM* pHead(void);
    void  vInsertAfter(ITEM* pInsItem, ITEM* pRef);
    void  vInsertBefore(ITEM* pInsItem, ITEM* pRef);
    i8    nItems(void);
    ITEM* pNext(ITEM* pItem);
    ITEM* pPop(void);
    ITEM* pPrev(ITEM* pItem);
    void  vPush(ITEM* pItem);
    void  vRemove(ITEM* pItem);
    ITEM* pTail(void);
    
    Footer* aFooter(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListNL_t__<ITEM>::ListNL_t__(void)
{
  m_nItems = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vAppend(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevItem());
  ASSERT(NULL == pItem->pNextItem());

  if (m_pTail)
    m_pTail->vNextItem(pItem);
  
  pItem->vPrevItem(m_pTail);
  pItem->vNextItem(nullptr);
  
  m_pTail = pItem;
  
  if (! m_pHead)
    m_pHead = pItem;
  
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* find object in list                                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListNL_t__<ITEM>::iFind(ITEM* pItem)
{
  i8 iItemNo = 0;
  for (ITEM* pScan = m_pHead;
       pScan;
       pScan = pScan->pNextItem(), iItemNo++)
    if (pScan == pItem)
      return iItemNo;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get nth object of list                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pGet(i8 i_Item)
{
  ITEM* pScan = m_pHead;
  for (i8 iItemNo = 0; pScan && iItemNo < i_Item; iItemNo++)
  pScan = pScan->pNextItem();

  return pScan;
}

/*----------------------------------------------------------------------------*/
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* Insert object after a given reference object                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vInsertAfter(ITEM* pInsItem, ITEM* pRef)
{
  if (pRef == m_pTail) {
    pInsItem->vPrevItem(pRef);
    pRef->vNextItem(pInsItem);

    m_pTail = pInsItem;
    m_nItems++;
    return;
  }

  // Verify that pRef is actually in this list
  ITEM* pItem = m_pHead;
  while (pItem) {

    if (pItem == pRef) {
      pInsItem->vNextItem(pRef->pNextItem());
      pRef->pNextItem()->vPrevItem(pInsItem);

      pRef->vNextItem(pInsItem);
      pInsItem->vPrevItem(pRef);

      m_nItems++;
      return;
    }

    pItem = pItem->pNextItem();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Insert object before a given reference object                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vInsertBefore(ITEM* pInsItem, ITEM* pRef)
{
  if (pRef == m_pHead) {
    pInsItem->vNextItem(pRef);
    pRef->vPrevItem(pInsItem);

    m_pHead = pInsItem;
    m_nItems++;
    return;
  }

  // Verify that pRef is actually in this list
  ITEM* pItem = m_pHead;
  while (pItem) {

    if (pItem == pRef) {
      pInsItem->vPrevItem(pRef->pPrevItem());
      pRef->pPrevItem()->vNextItem(pInsItem);

      pRef->vPrevItem(pInsItem);
      pInsItem->vNextItem(pRef);

      m_nItems++;
      return;
    }

    pItem = pItem->pNextItem();
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* get number of objects in list                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListNL_t__<ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next object after given object                                         */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pNext(ITEM* pItem)
{
  ITEM* pNext = pItem->pNextItem();
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pPop(void)
{
  if ( ! m_pHead ) {
    ASSERT( ! m_nItems );
    return nullptr;
  }

  ITEM* pItem = m_pHead;

  m_pHead = m_pHead->pNextItem();
  if (m_pHead)
    m_pHead->vPrevItem(nullptr);
  else
    m_pTail = nullptr;
  
  m_nItems--;
  if (m_nItems) {
    ASSERT(m_pHead);
    ASSERT(m_pTail);
  }
  else {
    ASSERT( ! m_pHead );
    ASSERT( ! m_pTail )
  }
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(nullptr);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get next object after given object                                         */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pPrev(ITEM* pItem)
{
  ITEM* pPrev = pItem->pPrevItem();
  
  return pPrev;
}

/*----------------------------------------------------------------------------*/
/* push object on top of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vPush(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevItem());
  ASSERT(NULL == pItem->pNextItem());
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(m_pHead);
  
  if (m_pHead)
    m_pHead->vPrevItem(pItem);
  else
    m_pTail = pItem;
  
  m_pHead = pItem;
  
  m_nItems++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)
}

/*----------------------------------------------------------------------------*/
/* remove object from list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::vRemove(ITEM* pItem)
{
  ITEM* pPrev = pItem->pPrevItem();
  ITEM* pNext = pItem->pNextItem();

  if (pPrev)
    pPrev->vNextItem(pNext);

  if (pNext)
    pNext->vPrevItem(pPrev);
  
  if (pItem == m_pHead)
    m_pHead = pNext;
  
  if (pItem == m_pTail)
    m_pTail = pPrev;
  
  m_nItems--;
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pTail(void)
{
  return m_pTail;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
