/*============================================================================*/
/* ListExot_t.hpp -- List of Items (with exo-nodes)                           */
/*============================================================================*/
#pragma once



namespace BASE
{
  template <class ITEM>
  class ListExoNode_t : public Object
  {
  private:
    ListExoNode_t<ITEM>* m_pPrevItem;   // prev node/item
    ListExoNode_t<ITEM>* m_pNextItem;   // next node/item
   
  public:
    void* operator new(size_t);
    void operator delete(void* aListExoNode);

    ListExoNode_t<ITEM>(void);
    
    ListExoNode_t<ITEM>* pNextItem(void);
    void vNextItem(ListExoNode_t<ITEM>* pNext);
  
    ListExoNode_t<ITEM>* pPrevItem(void);
    void vPrevItem(ListExoNode_t<ITEM>* pPrev);
  };

}

// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §

namespace BASE
{
  template <eCN CLASSNO, class ITEM>
  class ListExo_t : public Object
  {
  protected:
    Lock  m_okLock;  // lock
    i8    m_nItems;  // number of items
    ITEM* m_pHead;   // head item
    ITEM* m_pTail;   // tail item
  
  
    ListExoNode_t<ITEM>* m_pHead_exo;
    ListExoNode_t<ITEM>* m_pTail_exo;


    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* aListExo);

    ListExo_t(void);
   ~ListExo_t(void) noexcept(false);

    void  vAppend(ITEM* pItem);
    void  vDeleteAll(void);
    i8    nItems(void);
    ITEM* pPop(void);
    void  vPush(ITEM* pItem);
    void  vRemove(ITEM* pItem);
    void  vRemoveAll(void);
  
    void  vReadLock(void);
    void  vReadUnlock(void);

    ITEM* pHead(void);
    ITEM* pTail(void);
    
    Footer* aFooter(void);
  };
}

using namespace BASE;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void* ListExo_t<CLASSNO,ITEM>::operator new(size_t)
{
  ListExo_t<CLASSNO,ITEM>* pListExo_t;
  
  pListExo_t = (ListExo_t<CLASSNO,ITEM>*)HeapMem::pAllocate(CLASSNO);
  
  return pListExo_t;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::operator delete(void* aListExo_t)
{
  ListExo_t<CLASSNO,ITEM>* pListExo_t = (ListExo_t<CLASSNO,ITEM>*)aListExo_t;

  HeapMem::vFree(pListExo_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExo_t<CLASSNO,ITEM>::ListExo_t(void)
: Object(CLASSNO)
, m_okLock(LK_LIST,nullptr)
{
  m_nItems = 0;
  
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  
  m_pHead_exo  = nullptr;
  m_pTail_exo  = nullptr;

  _checkInitialisedSize_(sizeof(ListExo_t<CLASSNO,ITEM>));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExo_t<CLASSNO,ITEM>::~ListExo_t(void) noexcept(false)
{
  vRemoveAll();
  
  RASSERT(! m_nItems);
}

/*----------------------------------------------------------------------------*/
/* append item to end of list                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vAppend(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevItem());
  ASSERT(NULL == pItem->pNextItem());
  
  m_okLock.vGrabWrite();
  
  if (m_pTail)
    m_pTail->vNextItem(pItem);
  
  pItem->vPrevItem(m_pTail);
  pItem->vNextItem(nullptr);
  
  m_pTail = pItem;
  
  if (! m_pHead)
    m_pHead = pItem;
  
  m_nItems++;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* delete all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vDeleteAll(void)
{
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    delete pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
i8 ListExo_t<CLASSNO,ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from head of list                                          */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pPop(void)
{
  m_okLock.vGrabWrite();
  
  if ( ! m_pHead ) {
    ASSERT( ! m_nItems );
    m_okLock.vDropWrite();
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
    ASSERT(!m_pHead);
    ASSERT(!m_pTail)
  }
  m_okLock.vDropWrite();
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(nullptr);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* push item onto top of list                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vPush(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevItem());
  ASSERT(NULL == pItem->pNextItem());
  
  m_okLock.vGrabWrite();
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(m_pHead);
  
  if (m_pHead)
    m_pHead->vPrev(pItem);
  else
    m_pTail = pItem;
  
  m_pHead = pItem;
  
  m_nItems++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* remove item from list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vRemove(ITEM* pItem)
{
  m_okLock.vGrabWrite();
  
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
  
  m_okLock.vDropWrite();
  
  pItem->vPrevItem(nullptr);
  pItem->vNextItem(nullptr);
}

/*----------------------------------------------------------------------------*/
/* remove all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vRemoveAll(void)
{
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    {;}
}

/*----------------------------------------------------------------------------*/
/* get read lock on list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vReadLock(void)
{
  m_okLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* release read lock on list                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vReadUnlock(void)
{
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get head item of list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get tail item of list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pTail(void)
{
  return m_pTail;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
Footer* ListExo_t<CLASSNO,ITEM>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
