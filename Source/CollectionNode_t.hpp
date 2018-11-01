/*============================================================================*/
/* CollectionNode_t.hpp -- Template for Collection Node                       */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class OBJECT>
  class CollectionNode_t
  {
    OBJECT* m_pPrevNode;   // prev node
    OBJECT* m_pNextNode;   // next node
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    CollectionNode_t<OBJECT>(void);
    
    OBJECT* pNextNode(void);
    void    vNextNode(OBJECT* pNext);
    OBJECT* pPrevNode(void);
    void    vPrevNode(OBJECT* pPrev);
    
    Footer* aFooter(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* CollectionNode_t<OBJECT>::operator new(size_t)
{
  vFatal("fatal: ListNode_t class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
CollectionNode_t<OBJECT>::CollectionNode_t(void)
{
  m_pNextNode = nullptr;
  m_pPrevNode = nullptr;
  
  _checkInitialisedSize_(sizeof(CollectionNode_t<OBJECT>));
}

/*---------------------------------------------------------------------------*/
/* get next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* CollectionNode_t<OBJECT>::pNextNode(void)
{
  return m_pNextNode;
}

/*---------------------------------------------------------------------------*/
/* set next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionNode_t<OBJECT>::vNextNode(OBJECT* pNext)
{
  m_pNextNode = pNext;
}

/*---------------------------------------------------------------------------*/
/* get prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* CollectionNode_t<OBJECT>::pPrevNode(void)
{
  return m_pPrevNode;
}

/*---------------------------------------------------------------------------*/
/* set prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void CollectionNode_t<OBJECT>::vPrevNode(OBJECT* pPrev)
{
  m_pPrevNode = pPrev;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Footer* CollectionNode_t<OBJECT>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
