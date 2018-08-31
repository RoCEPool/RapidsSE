/*============================================================================*/
/* Job_Receiver.hpp -- Socket Receiver Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Receiver : public Job
  {
  private:
    i8 m_iConnNo;
    Socket* m_pSocket;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Receiver(Interface::SEitf* pSEitf);
    ~Job_Receiver(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    i8 iConnNo(void);
    void vConnNo(i8 iConnNo);

    Socket* pSocket(void);
    void vSocket(Socket* pSocket);

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Receiver* _pJob_Receiver(void* aAddr);
  
  Job* pJob_Receiver_new(Job* pArg);
  void vJob_Receiver_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Receiver_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_Receiver_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
