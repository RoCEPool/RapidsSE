/*============================================================================*/
/* CollectionNode_t.hpp - template for collection node mixin                  */
/*============================================================================*/

#pragma once

#include "DataStructures.h"

namespace DataStructures
{
  template <class OBJECT>
  class CollectionNode_t
  {
    OBJECT* m_pPrevNode;   // prev node
    OBJECT* m_pNextNode;   // next node
    
  public:
    void* operator new(size_t);
    
    CollectionNode_t<OBJECT>(void);
    
    OBJECT* pNextNode(void);
    void    vNextNode(OBJECT* pNext);
    OBJECT* pPrevNode(void);
    void    vPrevNode(OBJECT* pPrev);
  };
}

////////////////////////////////////////////////////////////////////////////////

using namespace DataStructures;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class OBJECT>
void* CollectionNode_t<OBJECT>::operator new(size_t iSize)
{
  vFatal("fatal: ListNode_t class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------*/
/* constructor                                                               */
/*---------------------------------------------------------------------------*/
template <class OBJECT>
CollectionNode_t<OBJECT>::CollectionNode_t(void)
{
  m_pNextNode = nullptr;
  m_pPrevNode = nullptr;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/