/*============================================================================*/
/* Job_DropTable.hpp -- Drop Table Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropTable : public Job,
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
  
    Job_DropTable(Interface::SEitf* pSEitf);
    ~Job_DropTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DropTable* _pJob_DropTable(void* aAddr);
  
  bool bJob_DropTable_pre(TokenVec* pTokenVec);
  void vJob_DropTable_post(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
