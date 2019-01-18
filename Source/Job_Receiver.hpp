/*============================================================================*/
/* Job_Receiver.hpp -- Socket Receiver Job                                    */
/*============================================================================*/
#pragma once

namespace GridModel
{
  class Node;
}

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Receiver : public Job
  {
  private:
    i8      m_iConnNo;  // which logical connection
    GridModel::Node* m_pNode; // the peer node on the other side of the socket
  
     // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Receiver(Interface::Session* pSession);
    ~Job_Receiver(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    i8 iConnNo(void);
    void vConnNo(i8 iConnNo);

    GridModel::Node* pNode(void);
    void vNode(GridModel::Node* pNode);
  
    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Receiver* _pJob_Receiver(void* aAddr);
  
  Job* pJob_Receiver_new(Job* pArg);
  void vJob_Receiver_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Receiver_show(Job* pJob);
  
  JobClass* pJob_Receiver_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
