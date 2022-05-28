/*============================================================================*/
/* Chain_t.hpp -- Endo-Chain of Items                                         */
/*============================================================================*/
#pragma once

namespace PRIMITIVES
{
  template <eCN CLASSNO, class ITEM>
  class Chain_t : public Object
  {
  protected:
    Lock  m_okLock;  // lock
    i8    m_nItems;  // number of items
    ITEM* m_pHead;   // head item

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Chain_t(eCN eClass);
   ~Chain_t(void) noexcept(false);

    void  vDeleteAll(void);
    i8    nItems(void);
    ITEM* pPop(void);
    void  vPush(ITEM* pItem);
    void  vRemoveAll(void);
  
    void  vReadLock(void);
    void  vReadUnlock(void);

    ITEM* pHead(void);
    
    Footer* aFooter(void);
  };
}

using namespace PRIMITIVES;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void* Chain_t<CLASSNO,ITEM>::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CLASSNO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void Chain_t<CLASSNO,ITEM>::operator delete(void* aAddr)
{
  Chain_t<CLASSNO,ITEM>* pChain_t =
    (Chain_t<CLASSNO,ITEM>*)(aAddr);
  
  Heaps::vFree(pChain_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
Chain_t<CLASSNO,ITEM>::Chain_t(eCN eClass)
: Object(eClass)
, m_okLock(this)
{
  m_nItems = 0;
  m_pHead  = nullptr;
  
  _checkInitialisedSize_(sizeof(Chain_t<CLASSNO,ITEM>));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
Chain_t<CLASSNO,ITEM>::~Chain_t(void) noexcept(false)
{
  vDeleteAll();
  
  RASSERT(! m_nItems);
}

/*----------------------------------------------------------------------------*/
/* delete all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void Chain_t<CLASSNO,ITEM>::vDeleteAll(void)
{
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    delete pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
i8 Chain_t<CLASSNO,ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from head of chain                                         */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* Chain_t<CLASSNO,ITEM>::pPop(void)
{
  m_okLock.vGrabWrite();
  
  if ( ! m_pHead ) {
    ASSERT( ! m_nItems );
    m_okLock.vDropWrite();
    return nullptr;
  }

  ITEM* pItem = m_pHead;

  m_pHead = m_pHead->pNextItem();
  
  m_nItems--;
  if (m_nItems) {
    ASSERT(m_pHead);
  }
  else {
    ASSERT(!m_pHead);
  }
  m_okLock.vDropWrite();
  
  pItem->vNextItem(nullptr);
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* push item onto head of chain                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void Chain_t<CLASSNO,ITEM>::vPush(ITEM* pItem)
{
  ASSERT(NULL == pItem->pNextItem());
  
  m_okLock.vGrabWrite();
  
  pItem->vNextItem(m_pHead);
    
  m_pHead = pItem;
  
  m_nItems++;
  ASSERT(m_pHead);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* remove all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void Chain_t<CLASSNO,ITEM>::vRemoveAll(void)
{
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    {;}
}

/*----------------------------------------------------------------------------*/
/* get read lock on list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void Chain_t<CLASSNO,ITEM>::vReadLock(void)
{
  m_okLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* release read lock on list                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void Chain_t<CLASSNO,ITEM>::vReadUnlock(void)
{
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get head item of list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* Chain_t<CLASSNO,ITEM>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
Footer* Chain_t<CLASSNO,ITEM>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
