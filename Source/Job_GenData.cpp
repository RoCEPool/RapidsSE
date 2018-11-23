/*============================================================================*/
/* Job_GenData.cpp -- Generate Data Job                                       */
/*============================================================================*/

// Job Short: GenData

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_GenData =
{
  "Job_GenData"_Stz,     // name of the job class
  CN_JOB_GENDATA,        // class enum
  JK_LOCAL,              // job kind
  "generate data"_Stz,   // comment about the job
  "gendata"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "genData_req"_Stz,     // the _req task seed
  "genData_rcv"_Stz,     // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_GenData_pre,      // prompt pre-process function
  vJob_GenData_post,     // prompt post-process function
  pJob_GenData_new,      // job new function
  vJob_GenData_delete,   // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_GenData_show,     // job show function
  
  // help and example
  true,
  "gendata <tableNm>"_Stz,
  "gendata X0_T1_Z0;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_GenData_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertBuffer_class;
  
  return &xJobClass_GenData;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_GenData* JobsModel::_pJob_GenData(void* aAddress)
{
  Job_GenData* pJob_GenData = reinterpret_cast<Job_GenData*>(aAddress);
  
  RASSERT_TYPE(pJob_GenData,CN_JOB_GENDATA);
  
  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_GenData::operator new(size_t)
{
  Job_GenData* pJob_GenData =
    static_cast<Job_GenData*>(HeapMem::pAllocate(CN_JOB_GENDATA));
  
  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_GenData::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_GenData::operator delete(void* aAddress)
{
  Job_GenData* pJob_GenData = static_cast<Job_GenData*>(aAddress);
  
  HeapMem::vFree(pJob_GenData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_GenData::Job_GenData(Interface::Session* pSession)
: Job(CN_JOB_GENDATA, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  m_pTable = nullptr;
  //m_xFile
  
  m_bIsSchemaTest     = false;
  m_nLeadingDontCare  = 0;
  m_nCareColumns      = 0;
  m_nTrailingDontCare = 0;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    m_pCareColumn[i_Col] = nullptr;
    m_iCareState[i_Col]  = 0;
    m_iMaxState[i_Col]   = 0;
  }

  m_sRecord = ""_Stz;
  
  _checkInitialised_(CN_JOB_GENDATA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_GenData::~Job_GenData(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_GenData::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_GenData* pJob_GenData = _pJob_GenData(pObject);

  pJob_GenData->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_GenData::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_GenData::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_GenData_pre(Interface::Session* pSession)
{
  IntVal xBcd;
  
  xBcd.vMinValue();  vTrace("Bcd min: %lld\n"_Stz,xBcd.iAsI8());
  xBcd.vMidValue();  vTrace("Bcd mid: %lld\n"_Stz,xBcd.iAsI8());
  xBcd.vMaxValue();  vTrace("Bcd max: %lld\n"_Stz,xBcd.iAsI8());
  xBcd.vNullValue(); vTrace("Bcd nul: %lld\n"_Stz,xBcd.iAsI8());
  
  BolVal xBol;
  
  xBol.vMinValue();  vTrace("Bol min: %lld\n"_Stz,xBol.iAsI8());
  xBol.vMidValue();  vTrace("Bol mid: %lld\n"_Stz,xBol.iAsI8());
  xBol.vMaxValue();  vTrace("Bol max: %lld\n"_Stz,xBol.iAsI8());
  xBol.vNullValue(); vTrace("Bol nul: %lld\n"_Stz,xBol.iAsI8());
  
  DteVal xDte;
  
  xDte.vMinValue();  vTrace("Dte min: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
  xDte.vMidValue();  vTrace("Dte mid: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
  xDte.vMaxValue();  vTrace("Dte max: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
  xDte.vNullValue(); vTrace("Dte nul: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
  
  FltVal xFlt;
  
  xFlt.vMinValue();  vTrace("Flt min: %f\n"_Stz,xFlt.dAsD8());
  xFlt.vMidValue();  vTrace("Flt mid: %f\n"_Stz,xFlt.dAsD8());
  xFlt.vMaxValue();  vTrace("Flt max: %f\n"_Stz,xFlt.dAsD8());
  xFlt.vNullValue(); vTrace("Flt nul: %f\n"_Stz,xFlt.dAsD8());
  
  IntVal xInt;

  xInt.vMinValue();  vTrace("Int min: %lld\n"_Stz,xInt.iAsI8());
  xInt.vMidValue();  vTrace("Int mid: %lld\n"_Stz,xInt.iAsI8());
  xInt.vMaxValue();  vTrace("Int max: %lld\n"_Stz,xInt.iAsI8());
  xInt.vNullValue(); vTrace("Int nul: %lld\n"_Stz,xInt.iAsI8());

  IvlVal xIvl;
  
  xIvl.vMinValue();  vTrace("Ivl min: %lld.%lld\n"_Stz,
                            xIvl.iSecond(),xIvl.iMilli());
  xIvl.vMidValue();  vTrace("Ivl mid: %lld.%lld\n"_Stz,
                            xIvl.iSecond(),xIvl.iMilli());
  xIvl.vMaxValue();  vTrace("Ivl max: %lld.%lld\n"_Stz,
                            xIvl.iSecond(),xIvl.iMilli());
  xIvl.vNullValue(); vTrace("Ivl nul: %lld.%lld\n"_Stz,
                            xIvl.iSecond(),xIvl.iMilli());
  
  TmeVal xTme;
  
  xTme.vMinValue();  vTrace("Tme min: %ll02d:%ll02d:%ll02d\n"_Stz,
                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
  xTme.vMidValue();  vTrace("Tme mid: %ll02d:%ll02d:%ll02d\n"_Stz,
                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
  xTme.vMaxValue();  vTrace("Tme max: %ll02d:%ll02d:%ll02d\n"_Stz,
                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
  xTme.vNullValue(); vTrace("Tme nul: %ll02d:%ll02d:%ll02d\n"_Stz,
                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
  
  TspVal xTsp;
  
  xTsp.vMinValue();  vTrace("Dte min: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
  xTsp.vMidValue();  vTrace("Dte mid: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
  xTsp.vMaxValue();  vTrace("Dte max: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
  xTsp.vNullValue(); vTrace("Dte nul: %ll04d-%ll02d-%ll02d\n"_Stz,
                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
  
  s32 xStr;
  
  xStr.vMinValue();  vTrace("Str min: %s\n"_Stz,xStr.aChar());
  xStr.vMidValue();  vTrace("Str mid: %s\n"_Stz,xStr.aChar());
  xStr.vMaxValue();  vTrace("Str max: %s\n"_Stz,xStr.aChar());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 nTokens;

  Job_GenData* pJob_GenData = new Job_GenData(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens != 3)
    return pJob_GenData;

  // set the table name
  pJob_GenData->m_sObjectName = pTknVec->pGet(1)->aText();

  // set the file name
  pJob_GenData->m_sFileName = pTknVec->pGet(2)->aText();

  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_GenData_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_GenData_new(Job* pArg)
{
  Job_GenData* pJob_GenData = new Job_GenData(nullptr);
  
  return pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_GenData_delete(Interface::Session* pSession, Job* pJob)
{
  Job_GenData* pJob_GenData = _pJob_GenData(pJob);
  
  delete pJob_GenData;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_GenData_show(Job* pJob)
{
  Job_GenData* pJob_GenData = _pJob_GenData(pJob);

  pJob_GenData->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddBcd(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  BcdVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddBol(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  BolVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddDte(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  DteVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddFlt(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  FltVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddInt(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  IntVal xVal;

  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddIvl(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  IvlVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddStr(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  s32 xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL:
      m_sRecord.vConcat("null"_Stz);
      return;
    default:
      NEVER_REACH;
  }
  
  m_sRecord.vConcat(xVal);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddTme(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  TmeVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAddTsp(i8 i_Col, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  TspVal xVal;
  
  switch (iState) {
    case C_GEN_MIN: xVal.vMinValue();  break;
    case C_GEN_MID: xVal.vMidValue();  break;
    case C_GEN_MAX: xVal.vMaxValue();  break;
    case C_GEN_NUL: xVal.vNullValue(); break;
    default:
      NEVER_REACH;
  }
  
  s32 sWsp;
  xVal.vToString(sWsp);
  
  m_sRecord.vConcat(sWsp);
}

/*----------------------------------------------------------------------------*/
/* append value for given column                                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vAppendValue(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);

  if (i_Col)
    m_sRecord.vAppend(',');
  
  i8 iState = m_iCareState[i_Col];

  Column* pColumn = m_pCareColumn[i_Col];
  eCT eColType = pColumn->eColumnType();
  
  switch (eColType) {
    case CT_BOOLEAN:   vAddBol(i_Col,iState); break;
    case CT_DATE:      vAddDte(i_Col,iState); break;
    case CT_DECIMAL:   vAddBcd(i_Col,iState); break;
    case CT_FLOAT:     vAddFlt(i_Col,iState); break;
    case CT_INTEGER:   vAddInt(i_Col,iState); break;
    case CT_INTERVAL:  vAddIvl(i_Col,iState); break;
    case CT_TIME:      vAddTme(i_Col,iState); break;
    case CT_TIMESTAMP: vAddTsp(i_Col,iState); break;
    case CT_VARCHAR:   vAddStr(i_Col,iState); break;

    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* create record using the column x value matrix                              */
/*----------------------------------------------------------------------------*/
void Job_GenData::vCreateRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  m_sRecord = ""_Stz;
  
  for (i8 i_Col = 0; i_Col < m_nCareColumns; i_Col++)
    vAppendValue(i_Col);
}

/*----------------------------------------------------------------------------*/
/* dump matrix combination                                                    */
/*----------------------------------------------------------------------------*/
void Job_GenData::vDumpCombination(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  for (i8 i_Col = 0; i_Col < m_nCareColumns; i_Col++) {
    
    i8 iState = m_iCareState[i_Col];
    
    switch (iState) {
      case C_GEN_MIN: vTrace("Min "_Stz); break;
      case C_GEN_MID: vTrace("Mid "_Stz); break;
      case C_GEN_MAX: vTrace("Max "_Stz); break;
      case C_GEN_NUL: vTrace("Nul "_Stz); break;
      default:
        NEVER_REACH;
    }
    
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* set first combination of the matrix                                        */
/*----------------------------------------------------------------------------*/
void Job_GenData::vFirstCombination(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  // load initial state of matrix
  for (i8 i_Col = 0; i_Col < m_nCareColumns; i_Col++)
    m_iCareState[i_Col] = 1;
}

/*----------------------------------------------------------------------------*/
/* generate data for table                                                    */
/*----------------------------------------------------------------------------*/
void Job_GenData::vGenerate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
 vTrace("Generating data for %s into %s\n"_Stz,
         m_sObjectName.aStr(), m_sFileName.aStr());
  
  bool bOpened = m_xFile.bOpen(m_sFileName.aStr(), C_OPEN_WRITE);
  if (! bOpened) {
    vTrace("Unable to open file: %s\n"_Stz,m_sFileName.aStr());
    return;
  }
  
  vLoadColumns();
  
  vIterateCombinations();
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* iterate column x value                                                     */
/*----------------------------------------------------------------------------*/
void Job_GenData::vIterateCombinations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  vFirstCombination();
  do {
    vDumpCombination();
    vCreateRecord();
    vWriteRecord();
  } while (bNextCombination());
}

/*----------------------------------------------------------------------------*/
/* load columns                                                               */
/*----------------------------------------------------------------------------*/
void Job_GenData::vLoadColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);

  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  i8 nCols = pColVec->nItems();
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pCol = pColVec->pGet(i_Col);
 
    if (pCol->sColumnName().bSame("hidden_column"_Stz)) {
      vTrace("skipping: %s\n"_Stz,pCol->sColumnName().aChar());
      continue;
    }
    
    // use first character of column name to determine column kind
    char cKind = *pCol->sColumnName().aChar();
    
    if ('X' == cKind) {
      vTrace("X: %s\n"_Stz,pCol->sColumnName().aChar());
      m_nLeadingDontCare++;
    }
    else
    if ('Z' == cKind) {
      vTrace("Z: %s\n"_Stz,pCol->sColumnName().aChar());
      m_nTrailingDontCare++;
    }
    else {
      vTrace("C: %s\n"_Stz,pCol->sColumnName().aChar());
      m_pCareColumn[m_nCareColumns] = pCol;
      if (pCol->bIsNullable())
        m_iMaxState[m_nCareColumns] = C_GEN_NUL;
      else
        m_iMaxState[m_nCareColumns] = C_GEN_MAX;
      m_nCareColumns++;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* advance the state of the matrix to next combination                        */
/*----------------------------------------------------------------------------*/
bool Job_GenData::bNextCombination(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  i8 i_LastCol = m_nCareColumns - 1;
  
  i8 iState = m_iCareState[i_LastCol];
  i8 iMaxState = m_iMaxState[i_LastCol];

  if (iState < iMaxState) {
    m_iCareState[i_LastCol] = iState + 1;
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_IncCol;
  for (i_IncCol = i_LastCol - 1; i_IncCol >= 0; i_IncCol--)
    if (m_iCareState[i_IncCol] < m_iMaxState[i_IncCol])
      break;
  
  if (i_IncCol < 0)
    return false;
  
  iState = m_iCareState[i_IncCol];
  m_iCareState[i_IncCol] = iState + 1;
  
  for (i8 i_ResetCol = i_IncCol + 1; i_ResetCol < m_nCareColumns; i_ResetCol++)
    m_iCareState[i_ResetCol] = C_GEN_MIN;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_GenData::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_GenData::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* write generated record                                                     */
/*----------------------------------------------------------------------------*/
void Job_GenData::vWriteRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENDATA,_FOOTER_);
  
  m_xFile.vPrint("%s\n"_Stz,m_sRecord.aChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
