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
#ifdef WITH_NODE_SKIPLIST
Node_I::Node_I(void)
: Object(CN_NODE_I)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  m_i_Node = C_UNKNOWN;
  m_pCursor = nullptr;

  _checkInitialised_(CN_NODE_I);
  
  vFirst();
    
  _postcondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}
/*----------------------------------------------------------------------------*/
#else
Node_I::Node_I(void)
: Object(CN_NODE_I)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  m_i_Node = 0;

  // newcode
  //printf("Node_I::Node_I  iStipulate:%lld\n", iStipulate);
  m_itNode = AM::pGrid()->itBeginNodes(); // begin
  m_itEndNode = AM::pGrid()->itEndNodes(); // end

  _checkInitialised_(CN_NODE_I);
  
  vFirst();
  
  _postcondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}
#endif

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
Node_I::~Node_I(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}
/*----------------------------------------------------------------------------*/
#else
Node_I::~Node_I(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}
#endif

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as Node*                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
Node_I::operator Node*(void)
{
  _precondition_(_MAGIC_, CN_NODE_I, _FOOTER_);

  if (! m_pCursor)
    return nullptr;
    
  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}
/*----------------------------------------------------------------------------*/
#else
Node_I::operator Node*(void) {
  _precondition_(_MAGIC_, CN_NODE_I, _FOOTER_);
  // newcode
  if (m_itNode != AM::pGrid()->itEndNodes()) {
    return *m_itNode;
  }

  return nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as Node*                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
Node* Node_I::operator *(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  if (! m_pCursor)
    return nullptr;
    
  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}
/*----------------------------------------------------------------------------*/
#else
Node* Node_I::operator *(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  // newcode
  if (m_itNode != AM::pGrid()->itEndNodes())
    return *m_itNode;

  return nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/* first node                                                                 */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
void Node_I::vFirst(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  Grid* pGrid = AM::pGrid();

  m_pCursor = pGrid->m_pNodes_skipList->pFirst(0);
}
/*----------------------------------------------------------------------------*/
#else
void Node_I::vFirst(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  // newcode
  m_itNode = AM::pGrid()->itBeginNodes(); // begin
}
#endif

/*----------------------------------------------------------------------------*/
/* more?                                                                      */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
bool Node_I::bMore(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  if (m_pCursor)
    return true;
  else
    return false;
}
/*----------------------------------------------------------------------------*/
#else
bool Node_I::bMore(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  // newcode
  if (m_itNode != m_itEndNode) {
    return true;
  }
  else {
    return false;
  }
}
#endif

/*----------------------------------------------------------------------------*/
/* next node                                                                  */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
void Node_I::vNext(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  Grid* pGrid = AM::pGrid();
  
  m_pCursor = pGrid->m_pNodes_skipList->pNext(m_pCursor,0);
}
/*----------------------------------------------------------------------------*/
#else
void Node_I::vNext(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);


  if (m_itNode != m_itEndNode) {
    ++m_itNode;
  }

  ++ m_i_Node;
}
#endif

/*----------------------------------------------------------------------------*/
/* get i_Node cursor                                                          */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
i8 Node_I::i_Node(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  return m_i_Node;
}
/*----------------------------------------------------------------------------*/
#else
i8 Node_I::i_Node(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  return m_i_Node;
}
#endif

/*----------------------------------------------------------------------------*/
/* get current pNode                                                          */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NODE_SKIPLIST
Node* Node_I::pNode(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  Node* pNode = _pNode(m_pCursor->m_pObject);
  
  return pNode;
}
/*----------------------------------------------------------------------------*/
#else
Node* Node_I::pNode(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  // newcode
  if (m_itNode != AM::pGrid()->itEndNodes())
    return *m_itNode;

  return nullptr;
}
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
