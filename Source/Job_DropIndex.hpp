/*============================================================================*/
/* Job_DropIndex.hpp -- Drop Index Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropIndex : public Job,
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
  
    Job_DropIndex(Interface::SEitf* pSEitf);
    ~Job_DropIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DropIndex* _pJob_DropIndex(void* aAddr);
  
  Job* pJob_DropIndex_pre(TokenVec* pTokenVec);
  void vJob_DropIndex_post(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
