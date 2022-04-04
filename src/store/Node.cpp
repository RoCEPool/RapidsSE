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

__pCastFunction(STORE,Node,CN_NODE)
__aNewFunction(Node,CN_NODE)
__vDeleteFunction(Node,CN_NODE)
__aFooterFunction(Node)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Node::Node(i8 iNodeId, Stz* szNodeNm, i8 nPtnsInNode,
           Stz* szIpAddr, i8 iPortNo, Stz* szRegionNm)
: Object(CN_NODE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
    
  m_iNodeId     = iNodeId;
  m_sNodeNm     = szNodeNm;
  m_nPtnsInNode = nPtnsInNode;
  m_sIPAddr     = szIpAddr;
  m_iPortNo     = iPortNo;
  
  m_eState      = NS_INACTIVE;
  
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* print distributed state vector                                             */
/*----------------------------------------------------------------------------*/
void Node::vDsv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);

  vTrace("  [%lld] Node %s : ptnsInNode %d IPAddr %s portNo %lld %s\n"_Stz,
         m_iNodeId,
         m_sNodeNm.szStz(),
         m_nPtnsInNode,
         m_sIPAddr.szStz(),
         m_iPortNo,
         aNS[m_eState]);
}


/*----------------------------------------------------------------------------*/
/* get dump info                                                              */
/*----------------------------------------------------------------------------*/
s16KB Node::szDumpInfo(s16KB szBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);

  szBuffer.vCatSprint("    [%lld] Node %s : ptnsInNode %d IPAddr %s portNo %lld %s\n"_Stz,
         m_iNodeId,
         m_sNodeNm.szStz(),
                      m_nPtnsInNode,
         m_sIPAddr.szStz(),
         m_iPortNo,
         aNS[m_eState]);

  return szBuffer;
}

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
/* get heartbeat                                                              */
/*----------------------------------------------------------------------------*/
Heartbeat Node::xGetHeartbeat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  Heartbeat tempHeartbeat = m_atmHeartbeat.load(std::memory_order_relaxed);
  return tempHeartbeat;
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
/* get saved pid from heartbeat                                               */
/*----------------------------------------------------------------------------*/
i8   Node::iGetPid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  Heartbeat tempHeartbeat = m_atmHeartbeat.load(std::memory_order_relaxed);
  return (tempHeartbeat.m_iPid);
}

/*----------------------------------------------------------------------------*/
/* get previous saved user usec                                               */
/*----------------------------------------------------------------------------*/
i8 Node::iGetPrevUserUsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  return m_iPrevUserUsec;
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
/* get node id                                                                */
/*----------------------------------------------------------------------------*/
i8 Node::iNodeId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_iNodeId;
}

/*----------------------------------------------------------------------------*/
/* set node id                                                               */
/*----------------------------------------------------------------------------*/
void Node::vNodeId(i8 iNodeId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  m_iNodeId = iNodeId;
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
/* get node port                                                              */
/*----------------------------------------------------------------------------*/
i8 Node::iPortNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_iPortNo;
}

/*----------------------------------------------------------------------------*/
/* get degree of parallelism                                                  */
/*----------------------------------------------------------------------------*/
i8 Node::nPtnsInNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_nPtnsInNode;
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
/* get region name                                                            */
/*----------------------------------------------------------------------------*/
Stz* Node::szRegionNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  
  return m_sRegionNm;
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
/* set node status                                                            */
/*----------------------------------------------------------------------------*/
void Node::vSetNodeStatus(i1 iStatus)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NODE,_FOOTER_);
  m_iNodeStatus = iStatus;
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two nodes by their nodeNm                                          */
/*----------------------------------------------------------------------------*/
bool STORE::bNodeCompare(void* aNodeInsert, void* aNodeCursor, bool& bMatch)
{
  Node* pNodeInsert = _pNode(aNodeInsert);
  Node* pNodeCursor = _pNode(aNodeCursor);
    
  if (! pNodeCursor)
    return false;
  
  // do the node names match?
  if (pNodeInsert->szNodeNm()->bEqual(pNodeCursor->szNodeNm()))
    bMatch = true;
  else
    bMatch = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this node being inserted lexicography after the cursor?
  
  if (pNodeInsert->szNodeNm()->bGreater(pNodeCursor->szNodeNm()))
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
