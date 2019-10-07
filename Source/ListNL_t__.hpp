/*============================================================================*/
/* ListNL_t__.hpp -- List of Items (non-locking)                              */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM> 
  class ListNL_t__ 
  {
  protected:
    i8    m_nCount;  // number of objects
    ITEM* m_pHead;   // head node
    ITEM* m_pTail;   // tail node

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ListNL_t__(eCN eClassNo);
    
    void  vAppend(ITEM* pItem);
    ITEM* pHead(void);
    i8    nObjects(void);
    ITEM* pPop(void);
    void  vPush(ITEM* pItem);
    void  vRemove(ITEM* pItem);
    ITEM* pTail(void);
    
    Footer* aFooter(void);
  };
}

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void* ListNL_t__<ITEM>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t__<ITEM>::operator delete(void* aAddress)
{
  ListNL_t__<ITEM>* pList = reinterpret_cast<ListNL_t__<ITEM>*>(aAddress);
  
  HeapMem::vFree(pList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListNL_t__<ITEM>::ListNL_t__(eCN eClassNo)
{
  m_nCount = 0;
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
  
  m_nCount++;
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
/* get number of objects in list                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListNL_t__<ITEM>::nObjects(void)
{
  return m_nCount;
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListNL_t__<ITEM>::pPop(void)
{
  if ( ! m_pHead ) {
    ASSERT( ! m_nCount );
    return nullptr;
  }

  ITEM* pItem = m_pHead;

  m_pHead = m_pHead->pNextItem();
  if (m_pHead)
    m_pHead->vPrevItem(nullptr);
  else
    m_pTail = nullptr;
  
  m_nCount--;
  if (m_nCount) {
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
    m_pHead->vPrev(pItem);
  else
    m_pTail = pItem;
  
  m_pHead = pItem;
  
  m_nCount++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
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
  
  m_nCount--;
  
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
