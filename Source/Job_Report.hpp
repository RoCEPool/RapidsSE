/*============================================================================*/
/* Job_Report.hpp -- Report Job                                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Report : public Job,
                     public JobFilePath
  {
  public:
    File  m_oInFile;     // input file

    bool  m_bNamedOut;   // named output file or stdout?
    s256  m_sOutputPath; // output file name
    File  m_oOutFile;    // output file
  
    RowAgg* m_pRowAgg;   // row aggregator
  
    RowAgg* m_pRowAgg_10s; // row aggregator for 10 sec
    RowAgg* m_pRowAgg_1m;  // row aggregator for 1 min
    RowAgg* m_pRowAgg_3m;  // row aggregator for 3 min
    RowAgg* m_pRowAgg_15m; // row aggregator for 15 min
  
    Birth   m_oBegan;      // when the report job began
  
    i8      m_iLast_10s;
    i8      m_iLast_1m;
    i8      m_iLast_3m;
    i8      m_iLast_15m;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Report(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Report(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    bool bBegin(void);
    bool bTake(void);
    void vEnd(void);
    void vPrint(Stz* aLabel, RowAgg* pRowAgg);

  public:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Report* _pJob_Report(void* aAddr);
  
  Job* pJob_Report_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  Job* pJob_Report_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Report_delete(Job* pJob);
  void vJob_Report_show(Job* pJob);
  
  JobClass* pJob_Report_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
