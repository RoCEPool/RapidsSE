/*============================================================================*/
/* Job_Generate.hpp -- Generate Data Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  static const i8 C_GEN_MIN = 1; // generate a min value
  static const i8 C_GEN_MID = 2; // generate a mid value
  static const i8 C_GEN_MAX = 3; // generate a max value
  static const i8 C_GEN_NUL = 4; // generate a null value

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Generate : public Job,
                       public JobFileName
  {
  public:
    Table*  m_pTable;      // table having data generate
    bool    m_bIndefinite; // generate continous stream?
    i8      m_nRecords;    // number of records to generate
    File    m_xFile;       // file being generated
  
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
  
    Job_Generate(Interface::Session* pSession);
    ~Job_Generate(void) noexcept(false);
  
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

    void   vAppendData(i8 i_CareCol);
    void   vAppendLeads(void);
    void   vAppendPattern(i8 i_CareCol);
    void   vAppendTails(void);
    void   vAppendValue(Column* pColumn, i8 iState);
    void   vCreateDataRecord(void);
    void   vCreatePatternRecord(void);
    void   vDumpCombination(void);
    void   vFirstCombination(void);
    void   vGenerate(void);
    void   vGenerateDataRecords(void);
    void   vGeneratePatternRecords(void);
    void   vLoadColumns(void);
    bool   bNextCombination(void);
    Table* pTable(void);
    void   vTable(Table* pTable);
    void   vWriteRecord(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Generate* _pJob_Generate(void* aAddr);
  
  Job* pJob_Generate_pre(Interface::Session* pSession);
  void vJob_Generate_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Generate_new(Job* pArg);
  void vJob_Generate_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Generate_show(Job* pJob);
  
  JobClass* pJob_Generate_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
