/*============================================================================*/
/* Node_I.cpp -- Node Iterator                                                */
/*============================================================================*/

#include "Grids.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Grids,Node_I,CN_NODE_I)
__aNewFunction(Node_I,CN_NODE_I)
__vDeleteFunction(Node_I,CN_NODE_I)
__aFooterFunction(Node_I);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node_I::Node_I(i8 iStipulate)
: Object(CN_NODE_I)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  m_iStipulate = iStipulate;
  m_i_Node = 0;
  
  _checkInitialised_(CN_NODE_I);
  
  vFirst();
  
  _postcondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node_I::Node_I(i8 i_Node,i8 iStipulate)
: Object(CN_NODE_I)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  m_iStipulate = iStipulate;
  m_i_Node = i_Node;
  
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
/* auto conversion operator : as Node*                                        */
/*----------------------------------------------------------------------------*/
Node_I::operator Node*(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  Node* pNode = AM::pGrid()->pGetNode(m_i_Node);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as Node*                                        */
/*----------------------------------------------------------------------------*/
Node* Node_I::operator *(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);
  
  Node* pNode = AM::pGrid()->pGetNode(m_i_Node);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* first node                                                                 */
/*----------------------------------------------------------------------------*/
void Node_I::vFirst(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* more?                                                                      */
/*----------------------------------------------------------------------------*/
bool Node_I::bMore(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  if (m_iStipulate == C_ALL) {
    
    if (m_i_Node < AM::nNodes())
      return true;
    else
      return false;
    
  }
  else {
    
    if (m_i_Node < AM::nBootNodes())
      return true;
    else
      return false;
    
  }

}

/*----------------------------------------------------------------------------*/
/* next node                                                                  */
/*----------------------------------------------------------------------------*/
void Node_I::vNext(void)
{
  _precondition_(_MAGIC_,CN_NODE_I,_FOOTER_);

  ++ m_i_Node;
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

  Node* pNode = AM::pGrid()->pGetNode(m_i_Node);
  
  return pNode;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
