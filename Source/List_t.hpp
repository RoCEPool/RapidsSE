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
    Lock    m_okLock;   // lock
    i8      m_nCount;  // number of objects
    ITEM* m_pHead;   // head node
    ITEM* m_pTail;   // tail node

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    List_t(void);
    
    void    vAppend(ITEM* mpNode);
    i8      iCount(void);
    ITEM* npPop(void);
    void    vPush(ITEM* mpNode);
    void    vRemove(ITEM* mpNode);
    
    void    vReadLock(void);
    void    vReadUnlock(void);

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
{
  m_okLock.vInitLock(LK_LIST);
  m_nCount = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  
  _checkInitialisedSize_(sizeof(List_t<ITEM>));
}

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void List_t<ITEM>::vAppend(ITEM* mpNode)
{
  ASSERT(NULL == mpNode->pPrevNode());
  ASSERT(NULL == mpNode->pNextNode());
  
  m_okLock.vGrabWrite();
  
  if (m_pTail)
    m_pTail->vNextNode(mpNode);
  
  mpNode->vPrevNode(m_pTail);
  mpNode->vNextNode(nullptr);
  
  m_pTail = mpNode;
  
  if (! m_pHead)
    m_pHead = mpNode;
  
  m_nCount++;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump each object in list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 List_t<ITEM>::iCount(void)
{
  return m_nCount;
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* List_t<ITEM>::npPop(void)
{
  m_okLock.vGrabWrite();
  
  if ( ! m_pHead ) {
    ASSERT( ! m_nCount );
    m_okLock.vDropWrite();
    return nullptr;
  }

  ITEM* pNode = m_pHead;

  m_pHead = m_pHead->pNextNode();
  if (m_pHead)
    m_pHead->vPrevNode(nullptr);
  else
    m_pTail = nullptr;
  
  m_nCount--;
  if (m_nCount) {
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
/* push object on top of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void List_t<ITEM>::vPush(ITEM* mpNode)
{
  ASSERT(NULL == mpNode->pPrevNode());
  ASSERT(NULL == mpNode->pNextNode());
  
  m_okLock.vGrabWrite();
  
  mpNode->vPrevNode(nullptr);
  mpNode->vNextNode(m_pHead);
  
  if (m_pHead)
    m_pHead->vPrev(mpNode);
  else
    m_pTail = mpNode;
  
  m_pHead = mpNode;
  
  m_nCount++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
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
  
  m_nCount--;
  
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
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* List_t<ITEM>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
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
