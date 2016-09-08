/*============================================================================*/
/* List_t.hpp - template for list mixin                                       */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  template <class OBJECT> 
  class List_t
  {
  private:
    Lock    m_zLock;   // lock
    i8      m_iCount;  // number of objects
    OBJECT* m_pHead;   // head node
    OBJECT* m_pTail;   // tail node

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    List_t(void);
    
    void    vAppend(OBJECT* pNode);
    i8      iCount(void);
    void    vDump(const char* aTitle);
    void    vInsert(OBJECT* pNode);
    OBJECT* pPop(void);
    void    vRemove(OBJECT* pNode);
    
    void    vReadLock(void);
    void    vReadUnlock(void);

    OBJECT* pHead(void);
    OBJECT* pTail(void);
    
    Footer* pFooter(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* List_t<OBJECT>::operator new(size_t iSize)
{
  vFatal("fatal: List_t class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
List_t<OBJECT>::List_t(void)
{
  m_zLock.vCheckInit();
  m_iCount = 0;
  m_pHead  = nullptr;
  m_pTail  = nullptr;
  
  Memory::vCheckInitialised(this,sizeof(List_t<OBJECT>));
}

/*----------------------------------------------------------------------------*/
/* append object to end of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void List_t<OBJECT>::vAppend(OBJECT* pNode)
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
/* dump each object in list                                                   */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
i8 List_t<OBJECT>::iCount(void)
{
  return m_iCount;
}
/*----------------------------------------------------------------------------*/
/* dump each object in list                                                   */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void List_t<OBJECT>::vDump(const char* aTitle)
{
  vReadLock();
  
  ListCursor_t<OBJECT> xCursor(this);
  
  vTrace("%s\n",aTitle);
  for(xCursor.vFirst(); xCursor.pObject(); xCursor.vNext()) {
    OBJECT* pObject = xCursor.pObject();
    pObject->vDump();
  }
  
  vReadUnlock();
  
}

/*----------------------------------------------------------------------------*/
/* insert object at head of list                                              */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void List_t<OBJECT>::vInsert(OBJECT* pNode)
{
  ASSERT( ! pNode->pPrevNode() );
  ASSERT( ! pNode->pNextNode() );
  
  m_zLock.vGrabWrite();
  
  if (m_pHead)
    m_pTail->vHead(pNode);
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(m_pHead);
  
  m_pHead = pNode;
  
  m_iCount++;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* pop object from head of list                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* List_t<OBJECT>::pPop(void)
{
  m_zLock.vGrabWrite();
  
  if ( ! m_pHead ) {
    ASSERT( ! m_iCount );
    m_zLock.vDropWrite();
    return nullptr;
  }

  OBJECT* pNode = m_pHead;

  m_pHead = m_pHead->pNextNode();
  if (m_pHead)
    m_pHead->vPrevNode(nullptr);
  else
    m_pTail = nullptr;
  
  m_iCount--;
  
  m_zLock.vDropWrite();
  
  pNode->vPrevNode(nullptr);
  pNode->vNextNode(nullptr);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* remove object from head of list                                            */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void List_t<OBJECT>::vRemove(OBJECT* pNode)
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
/* get read lock on list                                                      */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void List_t<OBJECT>::vReadLock(void)
{
  m_zLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* release read lock on list                                                  */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void List_t<OBJECT>::vReadUnlock(void)
{
  m_zLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get head object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* List_t<OBJECT>::pHead(void)
{
  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get last object in list                                                    */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* List_t<OBJECT>::pTail(void)
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
Footer* List_t<OBJECT>::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/