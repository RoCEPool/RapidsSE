/*============================================================================*/
/* Job_InsertRow.hpp -- Insert Row Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Job_InsertTable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_InsertRow : public Job,
                        public Job_WriteBehind
  {
  public:
    Table*     m_pTable;             // table pointer
    i8         m_iPartitionId;       // partition id
    s48K       m_aRecord;            // formatted row
    i8         m_nRecordBytes;       // length of record
    Formatters m_xFormatters;        // record formatters
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertRow(Interface::Session* pSession);
    ~Job_InsertRow(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bFormatRow(Stz* aRawData, i8 iRawLength);
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InsertRow* _pJob_InsertRow(void* aAddr);
  
  Job* pJob_InsertRow_pre(Interface::Session* pSession);
  void vJob_InsertRow_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_InsertRow_new(Job* pArg);
  void vJob_InsertRow_delete(Interface::Session* pSession, Job* pJob);
  void vJob_InsertRow_show(Job* pJob);
  
  JobClass* pJob_InsertRow_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
