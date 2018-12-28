/*============================================================================*/
/* Job_InsertRow.hpp -- Insert Row Job                                        */
/*============================================================================*/
#pragma once

namespace JobModel
{
  class Job_InsertTable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_InsertRows : public Job,
                         public JobWriteBehind
  {
  public:
    Job_InsertTable* m_pJob_InsertTable;                   // insert table job
    Table*           m_pTable;                             // table pointer
    Formatters       m_xFormatters;                        // record formatters
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertRows(Interface::Session* pSession);
    ~Job_InsertRows(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vEnd(void);
    bool bFormatRow(Stz* aRawData, i8 iRawLength);
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InsertRows* _pJob_InsertRows(void* aAddr);
  
  Job* pJob_InsertRows_pre(Interface::Session* pSession);
  void vJob_InsertRows_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_InsertRows_new(Job* pArg);
  void vJob_InsertRows_delete(Interface::Session* pSession, Job* pJob);
  void vJob_InsertRows_show(Job* pJob);
  
  JobClass* pJob_InsertRows_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
