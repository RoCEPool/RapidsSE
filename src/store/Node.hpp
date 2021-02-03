/*============================================================================*/
/* Node.hpp - Node                                                            */
/*============================================================================*/
#pragma once

namespace BASE { class Socket; } //?

#include <Heartbeat.hpp>
#include "eNS.h"

namespace STORE
{
  class Node : public Object,
               public ListItem_t<Node>
  {
  private:
    Article* m_pArticle;

    i8  m_iNodeNo;    // node number
    s32 m_sNodeNm;    // name of the node
    i8  m_nDegOfPar;  // degree of parallelism
    s32 m_sIPAddr;    // ip address of node
    i8  m_iPortNo;    // port number of node
  
    eNS m_eState;     // node state
  
    BASE::Socket* m_pRecvSocket;  // socket used for receiving from node
    BASE::Socket* m_pSendSocket;  // socket used for sending to node
    
    i8 m_qRegionSize;  // last known region size
    i8 m_qFreeSpace;   // last known free space
    i8 m_qFreePercent; // last known free percent


    Heartbeat m_xHeartbeat; // contains heartbeat info
    i8 m_iPrevUserUsec;
    i1 m_iNodeStatus;  // One byte should be enough. enum later.
                       // 0=Good, 1=not heard, 2=CPU max, 3=Mem max, 4=Disk max, 5=dead, 99=combination of 2/3/4/5
    String_t<1024> m_sStartCmd; // 1K is good for now

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Node* pNode);
    void  operator delete(void*);
    
    Node(i8 iNodeNo, Stz* szNodeNm, i8 nDegOfPar, Stz* szIPAddr, i8 iPortNo);
    ~Node(void) noexcept(false);
    
    static void vArtFun(Object* pObject, SP& spObject);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    Article* pArticle(void);
  
    i8   nDegOfPar(void);
    void vInit(void);
    Stz* szIPAddr(void);
    Stz* szNodeNm(void);
    i8   iNodeNo(void);
    void vNodeNo(i8 iNodeNo);
    i8   iPortNo(void);
  
    void vDsv(void);
    eNS  eState(void);
    void vState(eNS eState);

    Socket* pRecvSocket(void);
    void vRecvSocket(Socket* pSocket);
    Socket* pSendSocket(void);
    void vSendSocket(Socket* pSocket);
  
    i8   qFreePercent(void);
    void vFreePercent(i8 qFreePercent);
    i8   qFreeSpace(void);
    void vFreeSpace(i8 qFreeSpace);
    i8   qRegionSize(void);
    void vRegionSize(i8 qRegionSize);

    void vPutHeartbeat(const Heartbeat& xHeartbeat);
    Heartbeat xGetHeartbeat(void);
    void vSetNodeStatus(i1 iStatus);
    i1   iGetNodeStatus(void);
    i8   iGetPrevUserUsec(void);
    void vSetPrevUserUsec(i8 Usec);
    void vSetStartCmd(Stz* sCmd);
    Stz* szGetStartCmd(void);
    i8   iGetPid(void);

    i1   iGetPrev(void);

    Footer* aFooter(void);
  };
  
  Node* _pNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
