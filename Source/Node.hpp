/*============================================================================*/
/* Node.hpp - Node                                                            */
/*============================================================================*/
#pragma once

namespace GridModel
{
  class Node : public Object,
               public ListNode_t<Node>
  {
  private:
    i8    m_iNodeNo;
    s256  m_sNodeName;
    s256  m_sNodeIP;
    i8    m_iPortNo;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Node* pNode);
    void  operator delete(void*);
    
    Node(void);
    Node(i8 iNodeNo, Stz* aName, Stz* aIpAddress, i8 iPort);
    ~Node(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vInit(void);
    Stz* aNodeIP(void);
    Stz* aNodeName(void);
    i8   iNodeNo(void);
    void vNodeNo(i8 iNodeNo);
    i8   iNodePortNo(void);
    
    
    Footer* aFooter(void);
  };
  
  Node* _pNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
