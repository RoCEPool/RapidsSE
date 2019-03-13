/*============================================================================*/
/* Node.hpp - Node                                                            */
/*============================================================================*/
#pragma once

namespace Grids
{
  class Node : public Object,
               public ListNode_t<Node>
  {
  private:
    i8    m_iNodeNo;    // node number
    s32   m_sNodeName;  // name of the node
    s32   m_sNodeIP;    // ip address of node
    i8    m_iPortNo;    // port number of node
  
    Socket* m_pRecvSocket;  // socket used for receiving from node
    Socket* m_pSendSocket;  // socket used for sending to node
  
    i8    m_iRegionSize;  // last known region size
    i8    m_iFreeSpace;   // last known free space
    i8    m_iFreePercent; // last known free percent

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Node* pNode);
    void  operator delete(void*);
    
    Node(i8 iNodeNo, Stz* aName, Stz* aIpAddress, i8 iPort);
    ~Node(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vInit(void);
    Stz* aNodeIP(void);
    Stz* aNodeName(void);
    i8   iNodeNo(void);
    void vNodeNo(i8 iNodeNo);
    i8   iNodePortNo(void);
    
    Socket* pRecvSocket(void);
    void vRecvSocket(Socket* pSocket);
    Socket* pSendSocket(void);
    void vSendSocket(Socket* pSocket);
  
    i8   iFreePercent(void);
    void vFreePercent(i8 iFreePercent);
    i8   iFreeSpace(void);
    void vFreeSpace(i8 iFreeSpace);
    i8   iRegionSize(void);
    void vRegionSize(i8 iRegionSize);

    Footer* aFooter(void);
  };
  
  Node* _pNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
