/*============================================================================*/
/* Collection_t.hpp -- Template for Collections                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class OBJECT> 
  class Collection_t
  {
  private:
    Lock    m_zLock;   // lock
    i8      m_iCount;  // number of objects in list
    OBJECT* m_pHead;   // head node of list
    OBJECT* m_pTail;   // tail node of list
    OBJECT* m_pFree;   // head node of free chain

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    Collection_t(void);
        
    OBJECT* pAllocObject(void);
    void    vContentDump(Stz* aTitle);
    void    vFreeObject(OBJECT* pObject);
    
    void    vAppendList(OBJECT*& pNode);
    OBJECT* pPopFree(void);
    void    vPushFree(OBJECT* pNode);
    void    vRemoveList(OBJECT*& pNode);

    void    vReadLock(void);
    void    vReadUnlock(void);
    
    OBJECT* pHead(void);
    OBJECT* pTail(void);
    
    Footer* aFooter(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* Collection_t<OBJECT>::operator new(size_t)
{
  vFatal("fatal: Collection_t class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Collection_t<OBJECT>::Collection_t(void)
{
  m_zLock.vInitLock(LK_COLLECTION);
  
  m_iCount = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  m_pFree  = nullptr;
  
  _checkInitialisedSize_(sizeof(Collection_t<OBJECT>));
}

/*----------------------------------------------------------------------------*/
/* allocate object & append to collection                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pAllocObject(void)
{
  OBJECT* pObject = pPopFree();
  
  if (pObject == nullptr)
    return nullptr;
  
  vAppendList(pObject);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* dump each object in collection                                             */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vContentDump(Stz* aTitle)
{
  vReadLock();
  
  CollectionCursor_t<OBJECT> xCursor(this);
  
  vTrace("%s\n"_Stz,aTitle);
  for (xCursor.vFirst(); xCursor.pObject(); xCursor.vNext()) {
    OBJECT* pObject = xCursor.pObject();
    pObject->vContentDump();
  }
  
  vReadUnlock();
}

/*----------------------------------------------------------------------------*/
/* remove object from collection and push onto free chain                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vFreeObject(OBJECT* pObject)
{
  vRemoveList(pObject);

  vPushFree(pObject);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vAppendList(OBJECT*& pNode)
{
  m_zLock.vGrabWrite();
  
  if (m_pTail)
    m_pTail->vNextNode(pNode);
  
  pNode->vPrevNode(m_pTail);
  pNode->vNextNode(nullptr);
  
  m_pTail = pNode;
  
  if (! m_pHead)
    m_pHead = pNode;
  
  m_iCount++;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* pop object from head of free chain                                         */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pPopFree(void)
{
  m_zLock.vGrabWrite();
  
  if ( ! m_pFree ) {
    m_zLock.vDropWrite();
    return nullptr;
  }

  OBJECT* pTopObject = m_pFree;

  m_pFree = m_pFree->pNextNode();
  
  pTopObject->vPrevNode(nullptr);
  pTopObject->vNextNode(nullptr);
  
  m_zLock.vDropWrite();
  
  return pTopObject;
}

/*----------------------------------------------------------------------------*/
/* push object onto free chain                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vPushFree(OBJECT* pObject)
{
  m_zLock.vGrabWrite();
  
  pObject->vNextNode(m_pFree);
  m_pFree = pObject;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vRemoveList(OBJECT*& pNode)
{
  m_zLock.vGrabWrite();
  
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
  
  m_iCount--;
  
  m_zLock.vDropWrite();
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
}

/*----------------------------------------------------------------------------*/
/* get read lock on collection                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vReadLock(void)
{
  m_zLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* release read lock on collection                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void Collection_t<OBJECT>::vReadUnlock(void)
{
  m_zLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* Collection_t<OBJECT>::pTail(void)
{
  return m_pTail;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Footer* Collection_t<OBJECT>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
