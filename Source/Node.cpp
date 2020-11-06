/*============================================================================*/
/* Node.cpp -- Node                                                           */
/*============================================================================*/

#include "Grids.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Node* Grids::_pNode(void* aAddr)
{
  Node* pNode = reinterpret_cast<Node*>(aAddr);
  
  RASSERT_TYPE(pNode,CN_NODE);
  
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Node::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_NODE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Node::operator new(size_t, Node* pNode)
{
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Node::operator delete(void* pNodeAddress)
{
  Object* pObject = reinterpret_cast<Node*>(pNodeAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(i8 iNodeNo, Stz* szNodeNm, Stz* szIpAddr, i8 iPortNo)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_pArticle  = new Article(szNodeNm,this,Node::vArtFun);
  
  m_iNodeNo   = iNodeNo;
  m_sNodeNm   = szNodeNm;
  m_sIPAddr   = szIpAddr;
  m_iPortNo   = iPortNo;
  
  m_eState    = NS_INACTIVE;
  
  m_pRecvSocket = nullptr;
  m_pSendSocket = nullptr;
  
  m_qRegionSize  = C_UNKNOWN;
  m_qFreeSpace   = C_UNKNOWN;
  m_qFreePercent = C_UNKNOWN;

  _checkInitialised_(CN_NODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Node::~Node(void) noexcept(false)
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
  Node* pNode = _pNode(pObject);

  vTrace("no %lld prev %p next %p\n"_Stz,
         pNode->m_iPortNo,
         pNode->pPrevItem(),
         pNode->pNextItem());
}

/*----------------------------------------------------------------------------*/
/* article print function                                                     */
/*----------------------------------------------------------------------------*/
void Node::vArtFun(Object* pObject, SP& spObject)
{
  SP_Node spNode;
  
  spObject.vResult(spNode.sz(_pNode(pObject)));
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get Article                                                                */
/*----------------------------------------------------------------------------*/
Article* Node::pArticle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_pArticle;
}

/*----------------------------------------------------------------------------*/
/* get node IP                                                                */
/*----------------------------------------------------------------------------*/
Stz* Node::szIPAddr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_sIPAddr;
}

/*----------------------------------------------------------------------------*/
/* get node name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Node::szNodeNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_sNodeNm;
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
i8 Node::iPortNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_iPortNo;
}

/*----------------------------------------------------------------------------*/
/* get node state                                                             */
/*----------------------------------------------------------------------------*/
eNS Node::eState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_eState;
}

/*----------------------------------------------------------------------------*/
/* set node state                                                             */
/*----------------------------------------------------------------------------*/
void Node::vState(eNS eState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_eState = eState;
}

/*----------------------------------------------------------------------------*/
/* get recv socket from node                                                  */
/*----------------------------------------------------------------------------*/
Socket* Node::pRecvSocket(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_pRecvSocket;
}

/*----------------------------------------------------------------------------*/
/* set recv socket for node                                                   */
/*----------------------------------------------------------------------------*/
void Node::vRecvSocket(Socket* pSocket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_pRecvSocket = pSocket;
}

/*----------------------------------------------------------------------------*/
/* get send socket from node                                                  */
/*----------------------------------------------------------------------------*/
Socket* Node::pSendSocket(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_pSendSocket;
}

/*----------------------------------------------------------------------------*/
/* set send socket for node                                                   */
/*----------------------------------------------------------------------------*/
void Node::vSendSocket(Socket* pSocket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_pSendSocket = pSocket;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get known free percent                                                     */
/*----------------------------------------------------------------------------*/
i8 Node::qFreePercent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_qFreePercent;
}

/*----------------------------------------------------------------------------*/
/* set known free percent                                                     */
/*----------------------------------------------------------------------------*/
void Node::vFreePercent(i8 qFreePercent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_qFreePercent = qFreePercent;
}

/*----------------------------------------------------------------------------*/
/* get known free space                                                       */
/*----------------------------------------------------------------------------*/
i8 Node::qFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_qFreeSpace;
}

/*----------------------------------------------------------------------------*/
/* set known free space                                                       */
/*----------------------------------------------------------------------------*/
void Node::vFreeSpace(i8 qFreeSpace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_qFreeSpace = qFreeSpace;
}

/*----------------------------------------------------------------------------*/
/* get known region size                                                      */
/*----------------------------------------------------------------------------*/
i8 Node::qRegionSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_qRegionSize;
}

/*----------------------------------------------------------------------------*/
/* set known region size                                                      */
/*----------------------------------------------------------------------------*/
void Node::vRegionSize(i8 qRegionSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_qRegionSize = qRegionSize;
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
