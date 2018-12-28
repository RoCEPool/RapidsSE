/*============================================================================*/
/* Job_SubLoader.hpp -- Table SubLoader Job                                   */
/*============================================================================*/
#pragma once

namespace JobModel
{
  static const i8 C_MAX_RECORD = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubLoader : public Job,
                        public JobWriteBehind
  {
  public:
    Job_LoadTable* m_pJob_LoadTable;

    RecordLoader* m_pRecordLoader; // chunk loading buffer
 
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubLoader(Interface::Session* pSession);
    ~Job_SubLoader(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    Job_LoadTable* pJob_LoadTable(void);
    void vJob_LoadTable(Job_LoadTable* pJob_LoadTable);

    void vBegin(void);
    bool bChunk(void);
    void vEnd(void);
  
    bool bNextChunkInfo(i8& i_Seek, i8& nBytes);

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubLoader* _pJob_SubLoader(void* aAddr);
  
  Job* vJob_SubLoader_pre(Interface::Session* pSession);
  Job* pJob_SubLoader_new(Job* pArg);
  void vJob_SubLoader_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubLoader_show(Job* pJob);
  
  JobClass* pJob_SubLoader_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
