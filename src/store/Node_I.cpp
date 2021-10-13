/*============================================================================*/
/* Node_I.cpp -- Node Iterator                                                */
/*============================================================================*/

#include "BASE.h"

#include "Node_I.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Node_I,CN_NODE_I)
__aNewFunction(Node_I,CN_NODE_I)
__vDeleteFunction(Node_I,CN_NODE_I)
__aFooterFunction(Node_I);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node_I::Node_I(void)
: Object(CN_NODE_I)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  m_pGrid   = AM::pGrid();
  m_i_Node  = C_UNKNOWN;
  m_pCursor = nullptr;

  _checkInitialised_(CN_NODE_I);
  
  vFirst();
    
  _postcondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Node_I::~Node_I(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* arrow operator                                                             */
/*----------------------------------------------------------------------------*/
Node* Node_I::operator ->(void)
{
  _precondition_(_MAGIC_, CN_NODE_I, _FOOTER_);

  if (! m_pCursor)
    THROW("Node_I::operator ->(void) : m_pCursor member is null"_Stz);

  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as Node*                                        */
/*----------------------------------------------------------------------------*/
Node_I::operator Node*(void)
{
  _precondition_(_MAGIC_, CN_NODE_I, _FOOTER_);

  if (! m_pCursor)
    return nullptr;
    
  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as Node*                                        */
/*----------------------------------------------------------------------------*/
Node* Node_I::operator *(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  if (! m_pCursor)
    return nullptr;
    
  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* first node                                                                 */
/*----------------------------------------------------------------------------*/
void Node_I::vFirst(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  Grid* pGrid = AM::pGrid();

  m_pCursor = pGrid->m_pNodes->pFirst(0);
}

/*----------------------------------------------------------------------------*/
/* more?                                                                      */
/*----------------------------------------------------------------------------*/
bool Node_I::bMore(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  if (m_pCursor)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* next node                                                                  */
/*----------------------------------------------------------------------------*/
void Node_I::vNext(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  Grid* pGrid = AM::pGrid();
  
  m_pCursor = pGrid->m_pNodes->pNext(m_pCursor,0);
}

/*----------------------------------------------------------------------------*/
/* get i_Node cursor                                                          */
/*----------------------------------------------------------------------------*/
i8 Node_I::i_Node(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  return m_i_Node;
}

/*----------------------------------------------------------------------------*/
/* get current pNode                                                          */
/*----------------------------------------------------------------------------*/
Node* Node_I::pNode(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
