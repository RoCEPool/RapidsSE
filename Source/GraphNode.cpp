/*============================================================================*/
/* GraphNode.cpp -- Node in Graph of Nodes                                    */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GraphNode* Structures::_pGraphNode(void* pAddr)
{
  GraphNode* pGraphNode = reinterpret_cast<GraphNode*>(pAddr);
  
  return pGraphNode;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* GraphNode::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = gpSmallHeap->pAllocObject(CN_GRAPHNODE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* GraphNode::operator new(size_t iSize, GraphNode* pGraphNode)
{
  RASSERT(iSize);
  
  return pGraphNode;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void GraphNode::operator delete(void* pGraphNodeAddress)
{
  GraphNode* pGraphNode = reinterpret_cast<GraphNode*>(pGraphNodeAddress);
  
  gpSmallHeap->vFreeObject(pGraphNode);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GraphNode::GraphNode(void)
: Object(CN_GRAPHNODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE,_FOOTER_);
  
  m_pParent = nullptr;
  m_pNext   = nullptr;
  m_pPrev   = nullptr;
  m_pChild  = nullptr;
  
  _checkInitialised_(CN_GRAPHNODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GraphNode::~GraphNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump the graph node                                                        */
/*----------------------------------------------------------------------------*/
void GraphNode::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  vTrace("%sGraphNode: %p\n", pTab, pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get child of this node                                                     */
/*----------------------------------------------------------------------------*/
GraphNode* GraphNode::pChild(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  return m_pChild;
}

/*----------------------------------------------------------------------------*/
/* set child of this node                                                     */
/*----------------------------------------------------------------------------*/
void GraphNode::vChild(GraphNode* pChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/* get next sibling of this node                                              */
/*----------------------------------------------------------------------------*/
GraphNode* GraphNode::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next of this node                                                      */
/*----------------------------------------------------------------------------*/
void GraphNode::vNext(GraphNode* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* get parent of this node                                                    */
/*----------------------------------------------------------------------------*/
GraphNode* GraphNode::pParent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  return m_pParent;
}

/*----------------------------------------------------------------------------*/
/* set parent of this node                                                    */
/*----------------------------------------------------------------------------*/
void GraphNode::vParent(GraphNode* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  m_pParent = pParent;
}

/*----------------------------------------------------------------------------*/
/* get prev sibling of this node                                              */
/*----------------------------------------------------------------------------*/
GraphNode* GraphNode::pPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next of this node                                                      */
/*----------------------------------------------------------------------------*/
void GraphNode::vPrev(GraphNode* pPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  m_pPrev = pPrev;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* GraphNode::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHNODE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/