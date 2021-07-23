/*============================================================================*/
/* Node.cpp -- Node                                                           */
/*============================================================================*/

#include "BASE.h"

#include "Node.hpp"
#include "SP_Node.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Node* STORE::_pNode(void* aAddr)
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
  void* pObject = Heaps::pAllocate(CN_NODE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
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
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(i8 iNodeNo, Stz* szNodeNm, i8 nDegOfPar, Stz* szIpAddr, i8 iPortNo, Stz* szRegionNm)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
    
  m_iNodeNo   = iNodeNo;
  m_sNodeNm   = szNodeNm;
  m_nDegOfPar = nDegOfPar;
  m_sIPAddr   = szIpAddr;
  m_iPortNo   = iPortNo;
  
  m_eState    = NS_INACTIVE;
  
  m_pRecvSocket = nullptr;
  m_pSendSocket = nullptr;
  
  m_qRegionSize  = C_UNKNOWN;
  m_qFreeSpace   = C_UNKNOWN;
  m_qFreePercent = C_UNKNOWN;

  Heartbeat xHeartbeat;
  MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);
  strcpy(xHeartbeat.m_acRegionName, szRegionNm->azChar());

  m_atmHeartbeat.store(xHeartbeat,std::memory_order_release);
  //MemLib::vZero(&m_xHeartbeat,sizeof(m_xHeartbeat),CN_NODE);

  m_iNodeStatus = 0;
  m_iPrevUserUsec = 0;
  //m_sStartCmd = " "_Stz;
  
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
  UNUSED(eClass);
  UNUSED(aTab);

  Node* pNode = _pNode(pObject);

  vTrace("no %lld prev %p next %p\n"_Stz,
         pNode->m_iPortNo,
         pNode->pPrevItem(),
         pNode->pNextItem());
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get degree of parallelism                                                  */
/*----------------------------------------------------------------------------*/
i8 Node::nDegOfPar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_nDegOfPar;
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
/* get region name                                                            */
/*----------------------------------------------------------------------------*/
Stz* Node::szRegionNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_sRegionNm;
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
/* print distributed state vector                                             */
/*----------------------------------------------------------------------------*/
void Node::vDsv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);

  vTrace("  [%lld] Node %s : degOfPar %d IPAddr %s portNo %lld %s\n"_Stz,
         m_iNodeNo,
         m_sNodeNm.szStz(),
         m_nDegOfPar,
         m_sIPAddr.szStz(),
         m_iPortNo,
         aNS[m_eState]);
}


/**
 * @brief returns a string buffer contains Node Info
 * @details returns a string buffer contains Node Info:
 * Node ID, Node Name, Node Address and Node Port Number
 * <p>
 * @param[in] s4K szBuffer
 * @return s4K
 *
 */
s16KB Node::szDumpInfo(s16KB szBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);

  szBuffer.vCatSprint("    [%lld] Node %s : degOfPar %d IPAddr %s portNo %lld %s\n"_Stz,
         m_iNodeNo,
         m_sNodeNm.szStz(),
         m_nDegOfPar,
         m_sIPAddr.szStz(),
         m_iPortNo,
         aNS[m_eState]);

  return szBuffer;
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
  
  if (AM::TRACE::bVerbose(C_VB_NODE)) {
    SP_Node spNode;
    vTrace("Node State: %s\n"_Stz,spNode.sz(this));
  }

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
  
  if (AM::TRACE::bVerbose(C_VB_NODE)) {
    SP_Node spNode;
    vTrace("Node RecvSocket: %s\n"_Stz,spNode.sz(this));
  }

}

/*----------------------------------------------------------------------------*/
/* get send socket from node                                                  */
/*----------------------------------------------------------------------------*/
Socket* Node::pSendSocket(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  //if (!m_pSendSocket) vThreadCallStackDump();
  return m_pSendSocket;
}

/*----------------------------------------------------------------------------*/
/* set send socket for node                                                   */
/*----------------------------------------------------------------------------*/
void Node::vSendSocket(Socket* pSocket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_pSendSocket = pSocket;
  
  if (AM::TRACE::bVerbose(C_VB_NODE)) {
    SP_Node spNode;
    vTrace("Node: SendSocket Node: %s\n"_Stz,spNode.sz(this));
  }
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
/* set heartbeat                                                              */
/*----------------------------------------------------------------------------*/
void Node::vPutHeartbeat(const Heartbeat& xHeartbeat)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);

  Heartbeat tempHeartbeat = m_atmHeartbeat.load(std::memory_order_relaxed);
  MemLib::vCopy((byte*)&tempHeartbeat,(byte*)&xHeartbeat,sizeof(tempHeartbeat),CN_NODE);
  m_atmHeartbeat.store(tempHeartbeat, std::memory_order_release);
  //MemLib::vCopy((byte*)&m_xHeartbeat,(byte*)&xHeartbeat,sizeof(m_xHeartbeat),CN_NODE);
  //m_xHeartbeat.m_iSincePosixEpoch = xHeartbeat.m_iSincePosixEpoch;
}


/*----------------------------------------------------------------------------*/
/* get heartbeat                                                              */
/*----------------------------------------------------------------------------*/
Heartbeat Node::xGetHeartbeat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  Heartbeat tempHeartbeat = m_atmHeartbeat.load(std::memory_order_relaxed);
  return tempHeartbeat;
}


/*----------------------------------------------------------------------------*/
/* set node status                                                            */
/*----------------------------------------------------------------------------*/
void Node::vSetNodeStatus(i1 iStatus)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  m_iNodeStatus = iStatus;
}


/*----------------------------------------------------------------------------*/
/* get node status                                                            */
/*----------------------------------------------------------------------------*/
i1 Node::iGetNodeStatus(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  return m_iNodeStatus;
}

/*----------------------------------------------------------------------------*/
/* get previous saved user usec                                               */
/*----------------------------------------------------------------------------*/
i8   Node::iGetPrevUserUsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  return m_iPrevUserUsec;
}

/*----------------------------------------------------------------------------*/
/* set to save user usec                                                      */
/*----------------------------------------------------------------------------*/
void   Node::vSetPrevUserUsec(i8 iUsec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  m_iPrevUserUsec = iUsec;
}

/*----------------------------------------------------------------------------*/
/* set to save start node cmd                                                 */
/*----------------------------------------------------------------------------*/
void   Node::vSetStartCmd(Stz* pCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  (void) pCmd;
  //m_sStartCmd.vCopy(pCmd);
}

/*----------------------------------------------------------------------------*/
/* get saved start node cmd                                                   */
/*----------------------------------------------------------------------------*/
Stz*   Node::pszGetStartCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  //if (m_sStartCmd.nLength() > 1)
  //  return (Stz*)m_sStartCmd;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get saved pid from heartbeat                                               */
/*----------------------------------------------------------------------------*/
i8   Node::iGetPid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  Heartbeat tempHeartbeat = m_atmHeartbeat.load(std::memory_order_relaxed);
  return (tempHeartbeat.m_iPid);
}

/*----------------------------------------------------------------------------*/
/* compare two nodes by their nodeNm                                          */
/*----------------------------------------------------------------------------*/
bool STORE::bNodeCompare(void* aNodeInsert, void* aNodeScan, bool& bMatch)
{
  Node* pNodeInsert = _pNode(aNodeInsert);
  Node* pNodeScan   = _pNode(aNodeScan);
    
  if (! pNodeScan)
    return false;
  
  // do the job id's match?
  if (pNodeInsert->szNodeNm()->bEqual(pNodeScan->szNodeNm()))
    bMatch = true;
  else
    bMatch = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this node to be inserted lexicography after the scan?
  
  if (pNodeInsert->szNodeNm()->bGreater(pNodeScan->szNodeNm()))
    return true;
  else
    return false;
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
