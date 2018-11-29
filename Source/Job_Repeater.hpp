/*============================================================================*/
/* Job_Repeater.hpp -- Repeater Job                                           */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  static const i8 C_REPEATER_WSP = C_256KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Repeater : public Job,
                       public JobFileName
  {
  public:
    s256  m_sPipeName; // pipe name
  
    File  m_xFile;     // the file
    File  m_xPipe;     // the pipe

    WspFxd<C_REPEATER_WSP> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Repeater(Interface::Session* pSession);
    ~Job_Repeater(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    void vIndefiniteCopy(void);
    void vRepeat(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Repeater* _pJob_Repeater(void* aAddr);
  
  Job* pJob_Repeater_pre(Interface::Session* pSession);
  void vJob_Repeater_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Repeater_new(Job* pArg);
  void vJob_Repeater_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Repeater_show(Job* pJob);
  
  JobClass* pJob_Repeater_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
