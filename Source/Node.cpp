/*============================================================================*/
/* Node.cpp -- Node                                                           */
/*============================================================================*/

#include "GridModel.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Node* GridModel::_pNode(void* aAddr)
{
  Node* pNode = reinterpret_cast<Node*>(aAddr);
  
  RASSERT( !pNode || CN_NODE == pNode->eClass() );
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Node::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heaps::pAllocate(CN_NODE);
  
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
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(void)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_iNodeNo   = -1;
  m_sNodeName = ""_Stz;
  m_sNodeIP   = ""_Stz;
  m_iPortNo   = -1;
  
  _checkInitialised_(CN_NODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(i8 iNodeNo, Stz* pName, Stz* pIpAddress, i8 iPortNo)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_iNodeNo   = iNodeNo;
  m_sNodeName = (Stz*)pName;
  m_sNodeIP   = (Stz*)pIpAddress;
  m_iPortNo   = iPortNo;
  m_pSocket   = nullptr;
  
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
void Node::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_NODE);
  RASSERT(pObject->eClass() == CN_NODE);
  
  Node* pNode = reinterpret_cast<Node*>(pObject);

  vTrace("no %lld prev %p next %p\n"_Stz,
         pNode->m_iPortNo,
         pNode->pPrevNode(),
         pNode->pNextNode());
}

/*----------------------------------------------------------------------------*/
/* get node IP                                                                */
/*----------------------------------------------------------------------------*/
Stz* Node::aNodeIP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_sNodeIP;
}

/*----------------------------------------------------------------------------*/
/* get node name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Node::aNodeName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_sNodeName;
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
/* get node port                                                              */
/*----------------------------------------------------------------------------*/
i8 Node::iNodePortNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_iPortNo;
}

/*----------------------------------------------------------------------------*/
/* get socket to node                                                         */
/*----------------------------------------------------------------------------*/
Socket* Node::pSocket(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_pSocket;
}

/*----------------------------------------------------------------------------*/
/* set socket to node                                                         */
/*----------------------------------------------------------------------------*/
void Node::vSocket(Socket* pSocket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_pSocket = pSocket;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Node::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
