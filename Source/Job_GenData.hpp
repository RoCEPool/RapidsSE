/*============================================================================*/
/* Job_GenData.hpp -- Generate Data Job                                       */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  static const i8 C_GEN_MIN = 1; // generate a min value
  static const i8 C_GEN_MID = 2; // generate a mid value
  static const i8 C_GEN_MAX = 3; // generate a max value
  static const i8 C_GEN_NUL = 4; // generate a null value

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_GenData : public Job,
                      public JobFileName
  {
  public:
    Table*  m_pTable;  // table having data generate
    File    m_xFile;   // file being generated
  
    bool    m_bIsSchemaTest;     // is a schema test generated file?
    i8      m_nLeadingDontCare;  // number of leading dont care columns
    i8      m_nCareColumns;      // number of care columns
    i8      m_nTrailingDontCare; // number of trailing dont care columns

    Column* m_pCareColumn[C_MAX_COLUMNS]; // the care columns
    i8      m_iCareState[C_MAX_COLUMNS];  // the generate state of care columns
    i8      m_iMaxState[C_MAX_COLUMNS];   // the maximum state of care columns

    s48K    m_sRecord;  // created record
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_GenData(Interface::Session* pSession);
    ~Job_GenData(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void   vShow(void);
  
    void   vAddBcd(Column* pColumn, i8 iState);
    void   vAddBol(Column* pColumn, i8 iState);
    void   vAddDte(Column* pColumn, i8 iState);
    void   vAddFlt(Column* pColumn, i8 iState);
    void   vAddInt(Column* pColumn, i8 iState);
    void   vAddIvl(Column* pColumn, i8 iState);
    void   vAddStr(Column* pColumn, i8 iState);
    void   vAddTme(Column* pColumn, i8 iState);
    void   vAddTsp(Column* pColumn, i8 iState);

    void   vAppendCare(i8 i_Col);
    void   vAppendLeads(void);
    void   vAppendTails(void);
    void   vAppendValue(Column* pColumn, i8 iState);
    void   vCreateRecord(void);
    void   vDumpCombination(void);
    void   vFirstCombination(void);
    void   vGenerate(void);
    void   vIterateCombinations(void);
    void   vLoadColumns(void);
    bool   bNextCombination(void);
    Table* pTable(void);
    void   vTable(Table* pTable);
    void   vWriteRecord(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_GenData* _pJob_GenData(void* aAddr);
  
  Job* pJob_GenData_pre(Interface::Session* pSession);
  void vJob_GenData_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_GenData_new(Job* pArg);
  void vJob_GenData_delete(Interface::Session* pSession, Job* pJob);
  void vJob_GenData_show(Job* pJob);
  
  JobClass* pJob_GenData_class(pFUN** aaFun);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
