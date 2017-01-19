/*============================================================================*/
/* Node.cpp -- Node                                                           */
/*============================================================================*/

#include "GridModel.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Node* GridModel::_pNode(void* pAddr)
{
  Node* pNode = reinterpret_cast<Node*>(pAddr);
  
  RASSERT( !pNode || CN_NODE == pNode->eClass() );
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Node::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_NODE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Node::operator new(size_t iSize, Node* pNode)
{
  RASSERT(iSize);
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Node::operator delete(void* pNodeAddress)
{
  Object* pObject = reinterpret_cast<Node*>(pNodeAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(void)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_iNodeNo = -1;
  m_sName = "";
  m_sIpAddress = "";
  m_iPortNo = -1;
  
  _checkInitialised_(CN_NODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(i8 iNodeNo, char* pName, char* pIpAddress, i8 iPortNo)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_iNodeNo = iNodeNo;
  m_sName = pName;
  m_sIpAddress = pIpAddress;
  m_iPortNo = iPortNo;
  
  _checkInitialised_(CN_NODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Node::~Node(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Node::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Node::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_NODE);
  ASSERT(pObject->eClass() == CN_NODE);
  
  Node* pNode = reinterpret_cast<Node*>(pObject);

  vTrace("no %lld prev %p next %p\n",
         pNode->m_iPortNo,
         pNode->pPrevNode(),
         pNode->pNextNode());
}

/*----------------------------------------------------------------------------*/
/* get node name                                                            */
/*----------------------------------------------------------------------------*/
char* Node::pName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  return m_sName.a();
}

/*----------------------------------------------------------------------------*/
/* get node number                                                            */
/*----------------------------------------------------------------------------*/
i8 Node::iNodeNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  return m_iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* set node number                                                            */
/*----------------------------------------------------------------------------*/
void Node::vNodeNo(i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  m_iNodeNo = iNodeNo;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Node::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
