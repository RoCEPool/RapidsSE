/*============================================================================*/
/* ListNode_t.hpp -- Template for List Node                                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  template <class ITEM>
  class ListNode_t
  {
    ITEM* m_pPrevNode;   // prev node
    ITEM* m_pNextNode;   // next node
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    ListNode_t<ITEM>(void);
    
    ITEM* pNextNode(void);
    void  vNextNode(ITEM* pNext);
    ITEM* pPrevNode(void);
    void  vPrevNode(ITEM* pPrev);
    
    Footer* aFooter(void);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* ListNode_t<OBJECT>::operator new(size_t)
{
  vFatal("fatal: ListNode_t class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
ListNode_t<OBJECT>::ListNode_t(void)
{
  m_pNextNode = nullptr;
  m_pPrevNode = nullptr;
  
  _checkInitialisedSize_(sizeof(ListNode_t<OBJECT>));
}

/*---------------------------------------------------------------------------*/
/* get next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListNode_t<OBJECT>::pNextNode(void)
{
  return m_pNextNode;
}

/*---------------------------------------------------------------------------*/
/* set next node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void ListNode_t<OBJECT>::vNextNode(OBJECT* pNextNode)
{
  m_pNextNode = pNextNode;
}

/*---------------------------------------------------------------------------*/
/* get prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
OBJECT* ListNode_t<OBJECT>::pPrevNode(void)
{
  return m_pPrevNode;
}

/*---------------------------------------------------------------------------*/
/* set prev node                                                             */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
void ListNode_t<OBJECT>::vPrevNode(OBJECT* pPrevNode)
{
  m_pPrevNode = pPrevNode;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
Footer* ListNode_t<OBJECT>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
