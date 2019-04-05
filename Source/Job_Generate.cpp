/*============================================================================*/
/* Job_Generate.cpp -- Generate Data Job                                      */
/*============================================================================*/

// Job Short: GenData

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_GenData =
{
  CN_JOB_GENERATE,       // class enum
  JK_LOCAL,              // job kind
  RK_NONE,               // response kind
  "generate data"_Stz,   // comment about the job
  "generate"_Stz,        // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "generate_req"_Stz,    // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_Generate_pre,     // prompt pre-process function
  vJob_post,             // prompt post-process function
  pJob_Generate_new,     // job new function
  vJob_Generate_delete,  // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Generate_show,    // job show function
  
  // help and example
  true,
  "generate <tableNm>"_Stz,
  "gendata X0_T1_Z0;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Generate_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Health_class;
  
  return &xJobClass_GenData;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Generate* Jobs::_pJob_Generate(void* aAddress)
{
  Job_Generate* pJob_Generate = reinterpret_cast<Job_Generate*>(aAddress);
  
  RASSERT_TYPE(pJob_Generate,CN_JOB_GENERATE);
  
  return pJob_Generate;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Generate::operator new(size_t)
{
  Job_Generate* pJob_Generate =
    static_cast<Job_Generate*>(HeapMem::pAllocate(CN_JOB_GENERATE));
  
  return pJob_Generate;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Generate::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Generate::operator delete(void* aAddress)
{
  Job_Generate* pJob_Generate = static_cast<Job_Generate*>(aAddress);
  
  HeapMem::vFree(pJob_Generate);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Generate::Job_Generate(Interface::Session* pSession)
: Job(CN_JOB_GENERATE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  m_pTable      = nullptr;
  m_bIndefinite = false;
  m_nRecords    = 0;
  //m_oFile
  
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
  
  _checkInitialised_(CN_JOB_GENERATE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Generate::~Job_Generate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Generate::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Generate* pJob_Generate = _pJob_Generate(pObject);

  pJob_Generate->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Generate::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Generate::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Generate_pre(Interface::Session* pSession)
{
//  IntVal xBcd;
//
//  xBcd.vMinValue();  vTrace("Bcd min: %lld\n"_Stz,xBcd.iAsI8());
//  xBcd.vMidValue();  vTrace("Bcd mid: %lld\n"_Stz,xBcd.iAsI8());
//  xBcd.vMaxValue();  vTrace("Bcd max: %lld\n"_Stz,xBcd.iAsI8());
//  xBcd.vNullValue(); vTrace("Bcd nul: %lld\n"_Stz,xBcd.iAsI8());
//
//  BolVal xBol;
//
//  xBol.vMinValue();  vTrace("Bol min: %lld\n"_Stz,xBol.iAsI8());
//  xBol.vMidValue();  vTrace("Bol mid: %lld\n"_Stz,xBol.iAsI8());
//  xBol.vMaxValue();  vTrace("Bol max: %lld\n"_Stz,xBol.iAsI8());
//  xBol.vNullValue(); vTrace("Bol nul: %lld\n"_Stz,xBol.iAsI8());
//
//  DteVal xDte;
//
//  xDte.vMinValue();  vTrace("Dte min: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
//  xDte.vMidValue();  vTrace("Dte mid: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
//  xDte.vMaxValue();  vTrace("Dte max: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
//  xDte.vNullValue(); vTrace("Dte nul: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xDte.iYear(),xDte.iMonth(),xDte.iDay());
//
//  FltVal xFlt;
//
//  xFlt.vMinValue();  vTrace("Flt min: %f\n"_Stz,xFlt.dAsD8());
//  xFlt.vMidValue();  vTrace("Flt mid: %f\n"_Stz,xFlt.dAsD8());
//  xFlt.vMaxValue();  vTrace("Flt max: %f\n"_Stz,xFlt.dAsD8());
//  xFlt.vNullValue(); vTrace("Flt nul: %f\n"_Stz,xFlt.dAsD8());
//
//  IntVal xInt;
//
//  xInt.vMinValue();  vTrace("Int min: %lld\n"_Stz,xInt.iAsI8());
//  xInt.vMidValue();  vTrace("Int mid: %lld\n"_Stz,xInt.iAsI8());
//  xInt.vMaxValue();  vTrace("Int max: %lld\n"_Stz,xInt.iAsI8());
//  xInt.vNullValue(); vTrace("Int nul: %lld\n"_Stz,xInt.iAsI8());
//
//  IvlVal xIvl;
//
//  xIvl.vMinValue();  vTrace("Ivl min: %lld.%lld\n"_Stz,
//                            xIvl.iSecond(),xIvl.iMilli());
//  xIvl.vMidValue();  vTrace("Ivl mid: %lld.%lld\n"_Stz,
//                            xIvl.iSecond(),xIvl.iMilli());
//  xIvl.vMaxValue();  vTrace("Ivl max: %lld.%lld\n"_Stz,
//                            xIvl.iSecond(),xIvl.iMilli());
//  xIvl.vNullValue(); vTrace("Ivl nul: %lld.%lld\n"_Stz,
//                            xIvl.iSecond(),xIvl.iMilli());
//
//  TmeVal xTme;
//
//  xTme.vMinValue();  vTrace("Tme min: %ll02d:%ll02d:%ll02d\n"_Stz,
//                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
//  xTme.vMidValue();  vTrace("Tme mid: %ll02d:%ll02d:%ll02d\n"_Stz,
//                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
//  xTme.vMaxValue();  vTrace("Tme max: %ll02d:%ll02d:%ll02d\n"_Stz,
//                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
//  xTme.vNullValue(); vTrace("Tme nul: %ll02d:%ll02d:%ll02d\n"_Stz,
//                            xTme.iHour(),xTme.iMinute(),xTme.iSecond());
//
//  TspVal xTsp;
//
//  xTsp.vMinValue();  vTrace("Dte min: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
//                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
//  xTsp.vMidValue();  vTrace("Dte mid: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
//                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
//  xTsp.vMaxValue();  vTrace("Dte max: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
//                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
//  xTsp.vNullValue(); vTrace("Dte nul: %ll04d-%ll02d-%ll02d\n"_Stz,
//                            xTsp.iYear(),xTsp.iMonth(),xTsp.iDay(),
//                            xTsp.iHour(),xTsp.iMinute(),xTsp.iSecond());
//
//  s32 xStr;
//
//  xStr.vMinValue();  vTrace("Str min: %s\n"_Stz,xStr.aChar());
//  xStr.vMidValue();  vTrace("Str mid: %s\n"_Stz,xStr.aChar());
//  xStr.vMaxValue();  vTrace("Str max: %s\n"_Stz,xStr.aChar());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  i8 nTokens;

  Job_Generate* pJob_Generate = new Job_Generate(pSession);

  TokenVec* pTknVec = pSession->m_pCommand->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be at least 4 tokens
  if (nTokens < 4)
    return pJob_Generate;

  // set the table name
  pJob_Generate->m_sObjectName = pTknVec->pGet(1)->aText();
  
  // set the file name
  if (pTknVec->pGet(2)->sText().bSame("file"_Stz)) {
    pJob_Generate->m_bIndefinite = false;
    pJob_Generate->m_sFilePath = pTknVec->pGet(3)->aText();
  }
  else
  if (pTknVec->pGet(2)->sText().bSame("pipe"_Stz)) {
    pJob_Generate->m_bIndefinite = true;
    pJob_Generate->m_sFilePath = pTknVec->pGet(3)->aText();
  }
  
  if (5 == nTokens) {
    if (pTknVec->pGet(4)->eToken() != TN_INTEGER)
      return pJob_Generate;

   pJob_Generate->m_bIndefinite = false;
   pJob_Generate->m_nRecords    = pTknVec->pGet(4)->iToI8();
  }

  return pJob_Generate;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Generate_new(Job* pArg)
{
  Job_Generate* pJob_Generate = new Job_Generate(nullptr);
  
  return pJob_Generate;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Generate_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Generate* pJob_Generate = _pJob_Generate(pJob);
  
  delete pJob_Generate;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Generate_show(Job* pJob)
{
  Job_Generate* pJob_Generate = _pJob_Generate(pJob);

  pJob_Generate->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add value for given bcd column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddBcd(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given bol column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddBol(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given dte column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddDte(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given flt column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddFlt(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given int column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddInt(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given ivl column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddIvl(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given str column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddStr(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given tme column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddTme(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* add value for given tsp column                                             */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAddTsp(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
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
/* append value for given care column                                         */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAppendData(i8 i_CareCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  Column* pColumn = m_pCareColumn[i_CareCol];
  
  vAppendValue(pColumn,C_GEN_MID);
}

/*----------------------------------------------------------------------------*/
/* append leading values                                                      */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAppendLeads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  ColumnVec* pColVec = m_pTable->pColumnVec();

  for (i8 i_Lead = 0; i_Lead < m_nLeadingDontCare; i_Lead++) {
    Column* pColumn = pColVec->pGet(i_Lead);
    vAppendValue(pColumn,C_GEN_MID);
  }
}

/*----------------------------------------------------------------------------*/
/* append value for given care column                                         */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAppendPattern(i8 i_CareCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  Column* pColumn = m_pCareColumn[i_CareCol];
  i8 iState = m_iCareState[i_CareCol];
  
  vAppendValue(pColumn,iState);
}

/*----------------------------------------------------------------------------*/
/* append trailing values                                                     */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAppendTails(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  ColumnVec* pColVec = m_pTable->pColumnVec();
  i8 nCols = pColVec->nItems();
  
  i8 i_Start = m_nLeadingDontCare + m_nCareColumns;
  for (i8 i_Tail = i_Start; i_Tail < nCols; i_Tail++) {
    Column* pColumn = pColVec->pGet(i_Tail);
    vAppendValue(pColumn,C_GEN_MID);
  }
}

/*----------------------------------------------------------------------------*/
/* append value for given for given column and state                          */
/*----------------------------------------------------------------------------*/
void Job_Generate::vAppendValue(Column* pColumn, i8 iState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  eCT eColumnType = pColumn->eColumnType();
  i8 iColumnNo = pColumn->iColumnNo();
  
  if (iColumnNo)
    m_sRecord.vAppend(',');
  
  switch (eColumnType) {
    case CT_BOOLEAN:   vAddBol(pColumn,iState); break;
    case CT_DATE:      vAddDte(pColumn,iState); break;
    case CT_DECIMAL:   vAddBcd(pColumn,iState); break;
    case CT_FLOAT:     vAddFlt(pColumn,iState); break;
    case CT_INTEGER:   vAddInt(pColumn,iState); break;
    case CT_INTERVAL:  vAddIvl(pColumn,iState); break;
    case CT_TIME:      vAddTme(pColumn,iState); break;
    case CT_TIMESTAMP: vAddTsp(pColumn,iState); break;
    case CT_VARCHAR:   vAddStr(pColumn,iState); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* create record using the column x value matrix                              */
/*----------------------------------------------------------------------------*/
void Job_Generate::vCreateDataRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  m_sRecord = ""_Stz;
  
  i8 iSensor      = Random::iInteger(1,10000);
  i8 iTemperature = Random::iInteger(100,500);
  i8 iPressure    = Random::iInteger(10,25);
  i8 iVolume      = Random::iInteger(1,1000);

  s256 sComment;
  Random::vString(sComment,78);
  
  m_sRecord.vSprint("%lld,%lld,%lld,%lld,%s"_Stz,
                    iSensor,
                    iTemperature,
                    iPressure,
                    iVolume,
                    sComment.aChar()
                    );
  
//  vAppendLeads();
//
//  for (i8 i_CareCol = 0; i_CareCol < m_nCareColumns; i_CareCol++)
//    vAppendData(i_CareCol);
//
//  vAppendTails();
}

/*----------------------------------------------------------------------------*/
/* create record using the column x value matrix                              */
/*----------------------------------------------------------------------------*/
void Job_Generate::vCreatePatternRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  m_sRecord = ""_Stz;
  
  vAppendLeads();
  
  for (i8 i_CareCol = 0; i_CareCol < m_nCareColumns; i_CareCol++)
    vAppendPattern(i_CareCol);
  
  vAppendTails();
}

/*----------------------------------------------------------------------------*/
/* dump matrix combination                                                    */
/*----------------------------------------------------------------------------*/
void Job_Generate::vDumpCombination(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  for (i8 i_CareCol = 0; i_CareCol < m_nCareColumns; i_CareCol++) {
    
    i8 iState = m_iCareState[i_CareCol];
    
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
void Job_Generate::vFirstCombination(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  // load initial state of matrix
  for (i8 i_Col = 0; i_Col < m_nCareColumns; i_Col++)
    m_iCareState[i_Col] = 1;
}

/*----------------------------------------------------------------------------*/
/* generate data for table                                                    */
/*----------------------------------------------------------------------------*/
void Job_Generate::vGenerate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  vTrace("Generating data for %s into %s\n"_Stz,
         m_sObjectName.aStz(), m_sFilePath.aStz());
  
  bool bOpened = m_oFile.bOpenWrite(m_sFilePath.aStz());
  if (! bOpened) {
    vTrace("Unable to open file: %s\n"_Stz,m_sFilePath.aStz());
    return;
  }
  
  vLoadColumns();
  
  if (! m_bIndefinite && ! m_nRecords)
    vGeneratePatternRecords();
  else
    vGenerateDataRecords();
  
  m_oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* generate a specific/indefinite number of records                           */
/*----------------------------------------------------------------------------*/
void Job_Generate::vGenerateDataRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  while (m_bIndefinite || m_nRecords) {
    vCreateDataRecord();
    vWriteRecord();
    if (m_nRecords)
      m_nRecords--;
  }
}

/*----------------------------------------------------------------------------*/
/* iterate column x value                                                     */
/*----------------------------------------------------------------------------*/
void Job_Generate::vGeneratePatternRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  vFirstCombination();
  do {
    //vDumpCombination();
    vCreatePatternRecord();
    vWriteRecord();
  } while (bNextCombination());
}

/*----------------------------------------------------------------------------*/
/* load columns                                                               */
/*----------------------------------------------------------------------------*/
void Job_Generate::vLoadColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);

  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  i8 nCols = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    Column* pColumn = pColVec->pGet(i_Column);
    
    // use first character of column name to determine column kind
    char cKind  = *pColumn->sColumnName().aChar();
    char cUnder = *(pColumn->sColumnName().aChar() + 1);
    
    if (('X' == cKind || 'x' == cKind) && '_' == cUnder) {
      vTrace("X: %s\n"_Stz,pColumn->sColumnName().aChar());
      m_nLeadingDontCare++;
    }
    else
    if (('Z' == cKind || 'z' == cKind) && '_' == cUnder) {
      vTrace("Z: %s\n"_Stz,pColumn->sColumnName().aChar());
      m_nTrailingDontCare++;
    }
    else {
      vTrace("C: %s\n"_Stz,pColumn->sColumnName().aChar());
      m_pCareColumn[m_nCareColumns] = pColumn;
      if (pColumn->bIsNullable())
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
bool Job_Generate::bNextCombination(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  i8 i_LastCareCol = m_nCareColumns - 1;
  
  i8 iState = m_iCareState[i_LastCareCol];
  i8 iMaxState = m_iMaxState[i_LastCareCol];

  if (iState < iMaxState) {
    m_iCareState[i_LastCareCol] = iState + 1;
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_IncCol;
  for (i_IncCol = i_LastCareCol - 1; i_IncCol >= 0; i_IncCol--)
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
Table* Job_Generate::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Generate::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* write generated record                                                     */
/*----------------------------------------------------------------------------*/
void Job_Generate::vWriteRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_GENERATE,_FOOTER_);
  
  m_oFile.vPrint("%s\n"_Stz,m_sRecord.aChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
