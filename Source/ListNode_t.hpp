/*============================================================================*/
/* ListNode_t.hpp - template for list node mixin                              */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  template <class OBJECT>
  class ListNode_t
  {
    OBJECT* m_pPrevNode;   // prev node
    OBJECT* m_pNextNode;   // next node
    
  public:
    void* operator new(size_t);
    
    ListNode_t<OBJECT>(void);
    
    OBJECT* pNextNode(void);
    void    vNextNode(OBJECT* pNext);
    OBJECT* pPrevNode(void);
    void    vPrevNode(OBJECT* pPrev);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* ListNode_t<OBJECT>::operator new(size_t iSize)
{
  vFatal("fatal: ListNode_t class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
ListNode_t<OBJECT>::ListNode_t(void)
{
  m_pNextNode = nullptr;
  m_pPrevNode = nullptr;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
