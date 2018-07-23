/*============================================================================*/
/* Job_ChangeFolder.hpp -- Change Folder Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ChangeFolder : public Job,
                           public JobDicPath,
                           public JobObjectName,
                           public JobResult
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ChangeFolder(Interface::SEitf* pSEitf);
    ~Job_ChangeFolder(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ChangeFolder* _pJob_ChangeFolder(void* aAddr);
  
  Job* pJob_ChangeFolder_pre(Prompt::SePrompt* pPrompt);
  void vJob_ChangeFolder_post(Prompt::SePrompt* pPrompt, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
