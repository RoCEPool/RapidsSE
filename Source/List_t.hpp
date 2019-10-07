/*============================================================================*/
/* List_t.hpp -- List of Items                                                */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM>
  class List_t
  {
  protected:
    Lock  m_okLock;  // lock
    i8    m_nItems;  // number of items
    ITEM* m_pHead;   // head item
    ITEM* m_pTail;   // tail item

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    List_t(void);
    
    void  vAppend(ITEM* pItem);
    i8    nItems(void);
    ITEM* pPop(void);
    void  vPush(ITEM* pItem);
    void  vRemove(ITEM* pItem);
    
    void  vReadLock(void);
    void  vReadUnlock(void);

    ITEM* pHead(void);
    ITEM* pTail(void);
    
    Footer* aFooter(void);
  };
}

using namespace Structures;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void* List_t<ITEM>::operator new(size_t)
{
  vFatal("fatal: List_t class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
List_t<ITEM>::List_t(void)
: m_okLock(LK_LIST,nullptr)
{
  m_nItems = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  
  _checkInitialisedSize_(sizeof(List_t<ITEM>));
}

/*----------------------------------------------------------------------------*/
/* append item to end of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void List_t<ITEM>::vAppend(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevNode());
  ASSERT(NULL == pItem->pNextNode());
  
  m_okLock.vGrabWrite();
  
  if (m_pTail)
    m_pTail->vNextNode(pItem);
  
  pItem->vPrevNode(m_pTail);
  pItem->vNextNode(nullptr);
  
  m_pTail = pItem;
  
  if (! m_pHead)
    m_pHead = pItem;
  
  m_nItems++;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 List_t<ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from head of list                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* List_t<ITEM>::pPop(void)
{
  m_okLock.vGrabWrite();
  
  if ( ! m_pHead ) {
    ASSERT( ! m_nItems );
    m_okLock.vDropWrite();
    return nullptr;
  }

  ITEM* pNode = m_pHead;

  m_pHead = m_pHead->pNextNode();
  if (m_pHead)
    m_pHead->vPrevNode(nullptr);
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
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* push item onto top of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void List_t<ITEM>::vPush(ITEM* pItem)
{
  ASSERT(NULL == pItem->pPrevNode());
  ASSERT(NULL == pItem->pNextNode());
  
  m_okLock.vGrabWrite();
  
  pItem->vPrevNode(nullptr);
  pItem->vNextNode(m_pHead);
  
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
template <class ITEM>
void List_t<ITEM>::vRemove(ITEM* pNode)
{
  m_okLock.vGrabWrite();
  
  ITEM* pPrev = pNode->pPrevNode();
  ITEM* pNext = pNode->pNextNode();

  if (pPrev)
    pPrev->vNextNode(pNext);

  if (pNext)
    pNext->vPrevNode(pPrev);
  
  if (pNode == m_pHead)
    m_pHead = pNext;
  
  if (pNode == m_pTail)
    m_pTail = pPrev;
  
  m_nItems--;
  
  m_okLock.vDropWrite();
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get read lock on list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void List_t<ITEM>::vReadLock(void)
{
  m_okLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* release read lock on list                                                  */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void List_t<ITEM>::vReadUnlock(void)
{
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get head item of list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* List_t<ITEM>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get tail item of list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* List_t<ITEM>::pTail(void)
{
  return m_pTail;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Footer* List_t<ITEM>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
