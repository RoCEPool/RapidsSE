/*============================================================================*/
/* ListNL_t.hpp -- Template for Non-Locking List                              */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class OBJECT> 
  class ListNL_t : public Object
  {
  protected:
    i8      m_nCount;  // number of objects
    OBJECT* m_pHead;   // head node
    OBJECT* m_pTail;   // tail node

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ListNL_t(eCN eClassNo);
    
    void    vAppend(OBJECT* pNode);
    OBJECT* pHead(void);
    i8      nObjects(void);
    OBJECT* pPop(void);
    void    vPush(OBJECT* pNode);
    void    vRemove(OBJECT* pNode);
    OBJECT* pTail(void);
    
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
template <class OBJECT>
void* ListNL_t<OBJECT>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListNL_t<OBJECT>::operator delete(void* aAddress)
{
  ListNL_t<OBJECT>* pList = reinterpret_cast<ListNL_t<OBJECT>*>(aAddress);
  
  HeapMem::vFree(pList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
ListNL_t<OBJECT>::ListNL_t(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
 m_nCount = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  
  _checkInitialisedSize_(sizeof(ListNL_t<OBJECT>));
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListNL_t<OBJECT>::vAppend(OBJECT* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(NULL == pNode->pPrevNode());
  ASSERT(NULL == pNode->pNextNode());
  
  if (m_pTail)
    m_pTail->vNextNode(pNode);
  
  pNode->vPrevNode(m_pTail);
  pNode->vNextNode(nullptr);
  
  m_pTail = pNode;
  
  if (! m_pHead)
    m_pHead = pNode;
  
  m_nCount++;
}

/*----------------------------------------------------------------------------*/
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListNL_t<OBJECT>::pHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get number of objects in list                                              */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
i8 ListNL_t<OBJECT>::nObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nCount;
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListNL_t<OBJECT>::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  if ( ! m_pHead ) {
    ASSERT( ! m_nCount );
    return nullptr;
  }

  OBJECT* pNode = m_pHead;

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
    ASSERT( ! m_pHead );
    ASSERT( ! m_pTail )
  }
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* push object on top of list                                                 */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListNL_t<OBJECT>::vPush(OBJECT* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(NULL == pNode->pPrevNode());
  ASSERT(NULL == pNode->pNextNode());
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(m_pHead);
  
  if (m_pHead)
    m_pHead->vPrev(pNode);
  else
    m_pTail = pNode;
  
  m_pHead = pNode;
  
  m_nCount++;
  ASSERT(m_pHead);
  ASSERT(m_pTail)
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void ListNL_t<OBJECT>::vRemove(OBJECT* pNode)
{
  OBJECT* pPrev = pNode->pPrevNode();
  OBJECT* pNext = pNode->pNextNode();

  if (pPrev)
    pPrev->vNextNode(pNext);

  if (pNext)
    pNext->vPrevNode(pPrev);
  
  if (pNode == m_pHead)
    m_pHead = pNext;
  
  if (pNode == m_pTail)
    m_pTail = pPrev;
  
  m_nCount--;
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListNL_t<OBJECT>::pTail(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pTail;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Footer* ListNL_t<OBJECT>::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
