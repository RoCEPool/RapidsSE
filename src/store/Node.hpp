/*============================================================================*/
/* Node.hpp - Node                                                            */
/*============================================================================*/
#pragma once

namespace BASE { class Socket; } //?

#include "Heartbeat.hpp"
#include "eNS.h"

namespace STORE
{
  class Node : public Object,
               public ListItem_t<Node>
  {
  private:
 
    i8  m_iNodeId;     // node id
    s32 m_sNodeNm;     // name of the node
    s32 m_sRegionNm;   // name of shared memory region
    i8  m_nPtnsInNode; // degree of parallelism
    s32 m_sIPAddr;     // ip address of node
    i8  m_iPortNo;     // port number of node
  
    eNS m_eState;      // node state
  
    BASE::Socket* m_pRecvSocket;  // socket used for receiving from node
    BASE::Socket* m_pSendSocket;  // socket used for sending to node
    
    i8 m_qRegionSize;  // last known region size
    i8 m_qFreeSpace;   // last known free space
    i8 m_qFreePercent; // last known free percent

    // Need to change the heartbeat into atomic heartbeat
    std::atomic<Heartbeat> m_atmHeartbeat;
    //Heartbeat m_xHeartbeat; // contains heartbeat info
    i8 m_iPrevUserUsec;
    i1 m_iNodeStatus;  // One byte should be enough. enum later.
                       // 0=Good, 1=not heard, 2=CPU max, 3=Mem max, 4=Disk max, 5=dead, 99=combination of 2/3/4/5

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Node(i8 iNodeId, Stz* szNodeNm, i8 nPtnsInNode,
         Stz* szIPAddr, i8 iPortNo, Stz* szRegionNm);
    ~Node(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vDsv(void);
    s16KB szDumpInfo(s16KB szBuffer);

    i8   qFreePercent(void);
    void vFreePercent(i8 qFreePercent);
    i8   qFreeSpace(void);
    void vFreeSpace(i8 qFreeSpace);
  
    Heartbeat xGetHeartbeat(void);
    i1   iGetNodeStatus(void);
    i8   iGetPid(void);
    i8   iGetPrevUserUsec(void);
 
    Stz* szIPAddr(void);
  
    Stz* szNodeNm(void);
    i8   iNodeNo(void);
    void vNodeNo(i8 iNodeNo);
  
    i8   iPortNo(void);
    i8   nPtnsInNode(void);
    void vPutHeartbeat(const Heartbeat& xHeartbeat);

    Socket* pRecvSocket(void);
    void vRecvSocket(Socket* pSocket);
    Stz* szRegionNm(void);
    i8   qRegionSize(void);
    void vRegionSize(i8 qRegionSize);


    Socket* pSendSocket(void);
    void vSendSocket(Socket* pSocket);
    void vSetNodeStatus(i1 iStatus);
    void vSetPrevUserUsec(i8 Usec);
    void vSetStartCmd(Stz* sCmd);
    eNS  eState(void);
    void vState(eNS eState);

    Footer* aFooter(void);
  };
  
  Node* _pNode(void* aAddr);

  bool bNodeCompare(void* aNodeInsert, void* aNodeCursor, bool& bMatch);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
