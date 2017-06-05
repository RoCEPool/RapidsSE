/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "SePrompt.hpp"

extern BufferPool*  gpBufferPool;
extern Heap*        gpMediumHeap;
extern Arguments    gxArguments;
extern AttachMap*   gpAttachMap;
extern Grid*        gpGrid;
extern bool         gbMetadata;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t iSize)
{
  RASSERT(iSize);
  SePrompt* pSePrompt = (SePrompt*)gpBufferPool->pAllocBuffer();
  
  return pSePrompt;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::operator delete(void* pSePromptAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pSePromptAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(void)
: Buffer(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_iPartition = 0;

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,m_iPartition, /*trace*/ 0);
  ASSERT(EC_NONE == eResult);
  
  m_bContinue = true;
  
  m_sPrompt.vSprint("[N%lld %lld] se %s%c> "_Str,
                    gpGrid->iThisNode(),
                    Process::iGetPid(),
                    RAPIDSSE_VERSION_STRING,
    #ifdef _DEBUG
                    'd'
    #else
                    'r'
    #endif
                    );
  
  m_bEcho   = false;
  
  m_bExpect = false;
  m_sExpect = ""_Str;
  
  m_sResponse  = ""_Str;
  //m_xHistory
  
  //m_wWorkarea
  
  //----
  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  if (gxArguments.m_bCommand) {
    m_pTokeniser->vUseString(gxArguments.m_sCommand);
  }
  else if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
    m_pTokeniser->vPrompt(m_sPrompt);
    m_pTokeniser->vUseStdin();
  }
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);
  
  // open the command history file
  m_xHistory.vOpen();

  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // close history file
  //m_xHistory.vClose();
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
  
  delete m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_SEPROMPT);
  ASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("partition %lld seitf %p continue %s\n"_Str,
         pSePrompt->m_iPartition,
         pSePrompt->m_pSEitf,
         pSePrompt->m_bContinue ? "true"_Str : "false"_Str);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt for commands and process them                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  // open the command history file
  //m_xHistory.vOpen();
  
  // get and process commands
  while (m_bContinue) {
    
    vGetCommand();
    
    if (0 == m_xTokenSet[0]->iLength())
      continue;
    
    bool bProcessResult = bProcessCommand();
    
    if (m_bExpect) {
      if ( ! m_sExpect.bSame(m_sResponse) ) {
        vTrace("Expected \"%s\" : Got \"%s\"\n"_Str,m_sExpect.s(),m_sResponse.s());
        PosixThread::vSleepSec(10000);
      }
    }
    
    if (m_bEcho) {
      vTrace("cmd: "_Str);
      for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
        if (m_xTokenSet[iToken]->bEqual(""_Str))
          break;
        vTrace("%s "_Str,m_xTokenSet[iToken]);
      }
      vTrace(": got: %s\n"_Str,m_sResponse.s());
      continue;
    }
    
    /*
    if (bProcessResult) {
      if ( ! m_bExpect ) {
        if ( !m_sResponse.bEqual("<ok>"_Str) ) {
          if (gbMetadata) {
            vTrace("metadata: %s\n"_Str, m_pSEitf->m_sMetadata.aStr());
            vTrace("%s"_Str,m_sResponse.s());
            vTrace("success: %lld rows affected\n"_Str, m_pSEitf->m_iRowsProcessed);
            
            // reset counters
            m_pSEitf->m_iRowsProcessed = 0;
            m_pSEitf->m_sMetadata      = ""_Str;
          }
          else {
            vTrace("%s\n"_Str,m_sResponse.s());
          }
        }
        else {
          if (gbMetadata) {
            vTrace("success: %lld rows affected\n"_Str, m_pSEitf->m_iRowsProcessed);
            m_pSEitf->m_iRowsProcessed = 0;
          }
        }
      }
    }
    else {
      if (m_pSEitf->eGetLastError() != EC_NONE)
        if ( ! m_bExpect )
         vTrace("error: %s\n"_Str,m_pSEitf->pGetLastError());
    }
    */
    
    if (bProcessResult) {
      if (gbMetadata) {
        vMetaFormat();
      }
      else {
        if ( ! m_bExpect ) {
          if ( !m_sResponse.bEqual("<ok>"_Str) ) {
              vTrace("%s\n"_Str,m_sResponse.s());
          }
        }
      }
    }
    else {
      if (m_pSEitf->eGetLastError() != EC_NONE)
        if ( ! m_bExpect )
          vTrace("error: %s\n"_Str,m_pSEitf->pGetLastError());
    }
    
    // clear the response
    m_sResponse = ""_Str;
  }
  
  // close history file
  //m_xHistory.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
}
  
/*----------------------------------------------------------------------------*/
/* execute request from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_xTokenSet[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_xTokenSet[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* execute request from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode, StrVal& sResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_xTokenSet[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_xTokenSet[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  sResponse = m_pSEitf->m_sResponse;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* execute request from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode, Str** ppRowPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_xTokenSet[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_xTokenSet[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  *ppRowPtr = m_pSEitf->m_aRowPtr;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxArguments.m_bCommand)
    vGetCommandArgument();
  else {
    vGetCommandInput();
    
    // if empty line just return
    if ( m_xTokenSet[0]->bEqual(""_Str) )
      return;

    // if its a command file just return
    if (bTestFile(m_xTokenSet[0]))
      return;
    
    // collect till ;
    vCollectStatement();
    
    // stip off ;
    m_xTokenSet.vStripSemicolon();

  }
}

/*----------------------------------------------------------------------------*/
/* get command from arguments                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommandArgument(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTokenSet.vReset();
  
  while (TK_NONE !=
           m_pTokeniser->eNextToken(m_xTokenSet[m_xTokenSet.nTokens()]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_xTokenSet[iToken]);
    
    m_xTokenSet.vIncTokens();
    ASSERT(m_xTokenSet.nTokens() < C_TOKENS);
  }
  
  m_bContinue = false;
}

/*----------------------------------------------------------------------------*/
/* get command from input                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommandInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  while (true) {
    
    m_xTokenSet.vReset();
    m_xTokenSet2.vReset();
    
    while (TK_NONE !=
           m_pTokeniser->eNextToken(m_xTokenSet[m_xTokenSet.nTokens()]->s())) {
      
      //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_xTokenSet[iToken]);
      
      m_xTokenSet.vIncTokens();
      ASSERT(m_xTokenSet.nTokens() < C_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! m_xTokenSet.nTokens() && m_pTokeniser->bEOF()) {
      bool b = m_pTokeniser->bPop();
      ASSERT(b);
      continue;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* print output in metadata format                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vMetaFormat(void)
{
  
  if ( ! m_bExpect ) {
    if ( !m_sResponse.bEqual("<ok>"_Str) ) {
      if ( ! m_pSEitf->m_sMetadata.bEqual( ""_Str))
        vTrace("metadata: %s\n"_Str, m_pSEitf->m_sMetadata.aStr());
      vTrace("%s"_Str,m_sResponse.s());
      vTrace("success: %lld rows affected\n"_Str, m_pSEitf->m_iRowsProcessed);
    }
    else {
      vTrace("success: %lld rows affected\n"_Str, m_pSEitf->m_iRowsProcessed);
    }
  }
  else {
    if (m_pSEitf->eGetLastError() != EC_NONE)
      if ( ! m_bExpect )
        vTrace("error: %s\n"_Str,m_pSEitf->pGetLastError());
  }

  // reset counters
  m_pSEitf->m_iRowsProcessed = 0;
  m_pSEitf->m_sMetadata      = ""_Str;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* process command                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bProcessCommand(bool bPostProcess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for echo prefix
  if (bCmdIs("echo"_Str))
    vSetEcho();
  else
    vClearEcho();
  
  // test for expect prefix
  if (bCmdIs("expect"_Str))
    vSetExpect();
  else
    vClearExpect();
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for builtin commands
  
  if (bCmdIs("?"_Str))          return bHelp();
  if (bCmdIs("help"_Str))       return bHelp();
  if (bCmdIs("!!"_Str))         return bRecallCommand();
  if ('!' == *m_xTokenSet[0]->s()) return bRecallCommand();
  if (bCmdIs("antbl"_Str))      return bAnalyzeTable();
  if (bCmdIs("begin"_Str)) {
    if (bOneIs("insert"_Str))   return bBeginInsert();
  }
  if (bCmdIs("bye"_Str))        return bBye();
  if (bCmdIs("cd"_Str))         return bChangeWorkingDir();
//if (bCmdIs("cf"_Str))         return bExecuteRequest();
//if (bCmdIs("connectCluster"_Str)) return bConnectCluster();
  if (bCmdIs("create"_Str)) {
    if (bOneIs("tenant"_Str))   return bCreateTenant();
    if (bOneIs("catalog"_Str))  return bCreateCatalog();
    if (bOneIs("schema"_Str))   return bCreateSchema();
    if (bOneIs("table"_Str))    return bCreateTable();
    if (bOneIs("index"_Str))    return bCreateIndex(false);
    if (bOneIs("unique"_Str))   return bCreateIndex(true);
  }
  if (bCmdIs("dantbl"_Str))     return bDanalyzeTable();
  if (bCmdIs("docol"_Str))      return bDocol();
  if (bCmdIs("drop"_Str)) {
    if (bOneIs("tenant"_Str))   return bDropTenant();
    if (bOneIs("catalog"_Str))  return bDropCatalog();
    if (bOneIs("index"_Str))    return bDropIndex();
    if (bOneIs("schema"_Str))   return bDropSchema();
    if (bOneIs("table"_Str))    return bDropTable();
  }
  if (bCmdIs("end"_Str)) {
    if (bOneIs("insert"_Str))   return bEndInsert();
  }
  if (bCmdIs("first"_Str))      return bGetFirstRow();
  if (bCmdIs("history"_Str))    return bHistory();
  if (bCmdIs("insert"_Str)) {
    if (bOneIs("row"_Str))      return bInsertRow();
  }
  if (bCmdIs("ls"_Str))         return bListDir();
  if (bCmdIs("next"_Str))       return bGetNextRow();
//if (bCmdIs("object"_Str))     return bObject();
  if (bCmdIs("partition"_Str))  return bPartitionTable();
  if (bCmdIs("pwd"_Str))        return bPrintWorkingDir();
  if (bCmdIs("sleep"_Str))      return bSleep();
//if (bCmdIs("space"_Str))      return bPoolSpace();
  if (bCmdIs("use"_Str)) {
    if (bOneIs("schema"_Str))   return bUseSchema();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append to history
  m_xHistory.vAppend(m_xTokenSet);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for command file
  if (bTestFile(m_xTokenSet[0]))
    return bRunFile();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // execute seitf request
  if (! bExecuteRequest())
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // post process requests
  if (! bPostProcess)
    return true;
  
  if (m_xTokenSet[0]->bEqual("sel"_Str) || m_xTokenSet[0]->bEqual("rgesel"_Str)) {
    vIterateRows();
    return bReturnTable();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the following never return
  
  if (m_xTokenSet[0]->bEqual("mon"_Str)) {
    while (true) {
      PosixThread::vSleepSec(10);
      if (!bExecuteRequest())
        return true;
    }
  }
  
  if (m_xTokenSet[0]->bEqual("x"_Str)) {
    while (true)
      bExecuteRequest();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set echo result                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vSetEcho(void)
{
  m_bEcho = true;
  
  vClipTokens(1);
}

/*----------------------------------------------------------------------------*/
/* clear echo result                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vClearEcho(void)
{
  m_bEcho = false;
}

/*----------------------------------------------------------------------------*/
/* collect and set expected result                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vSetExpect(void)
{
  m_sExpect = m_xTokenSet[1];
  
  m_bExpect = true;
  
  vClipTokens(2);
}

/*----------------------------------------------------------------------------*/
/* clear expected result                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vClearExpect(void)
{
  m_sExpect = ""_Str;
  
  m_bExpect = false;
}

/*----------------------------------------------------------------------------*/
/* bye                                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBye(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  m_bContinue = false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* print history                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHistory(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  m_xHistory.vPrint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bRunFile(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  vPushFile(m_xTokenSet[0]);
  m_pTokeniser->vLineMode(true);
  m_sResponse = "<ok>"_Str;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* recall command                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bRecallCommand(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  if (! m_xHistory.bRecall(m_xTokenSet))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* clip tokens                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vClipTokens(i8 nTokens)
{
  i8 iToToken = 0;
  
  for (i8 iFromToken = nTokens; iFromToken < C_TOKENS; iFromToken++) {
    
    m_xTokenSet[iToToken++]->vCopy(m_xTokenSet[iFromToken]);
    
    if (m_xTokenSet[iFromToken]->bEqual(""_Str))
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer xTimer;
  Str* aResponse = nullptr;
  
  xTimer.vStart();

  m_xTokenSet.vSetTokens();
  
                                                     //! make this part of verbose??
  //m_pTokeniser->vDumpTokens("Command : "_Str,m_xTokenSet,C_TOKENS);
  
  bool bResult = RapidsSE_ExecuteRequest(m_pSEitf,m_xTokenSet,&aResponse);
  
  if (bResult)
    m_sResponse = aResponse ? (Str*)aResponse : ""_Str;
  else
    m_sResponse = aResponse ? (Str*)aResponse : ""_Str;
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* iterate rows                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = m_sResponse.iToI8();
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iRowCount = 0;
  
  if (gbMetadata) {
    vTrace("metadata: %s\n"_Str, m_pSEitf->m_sMetadata.aStr());
    for (i8 iPartition = 0; iPartition < gxArguments.m_nSlots; iPartition++) {
      //vTrace("Partition %lld: \n"_Str, Partition::iSlotToPartition(iPartition));
      i8 iPart = Partition::iSlotToPartition(iPartition);
      pSelCursor->vBegin(iPartition);
      pSelCursor->vFirst();
      Str* pStr = pSelCursor->aRowAsStr();
      while (pStr) {
        iRowCount++;
        if (m_pSEitf->m_bDisplay)
          vTrace("row: (%lld, '%s')\n"_Str, iPart, pStr);
        pSelCursor->vNext();
        pStr = pSelCursor->aRowAsStr();
      }
    }
    m_pSEitf->m_iRowsProcessed = iRowCount;
  }
  else {
    for (i8 iPartition = 0; iPartition < gxArguments.m_nSlots; iPartition++) {
      vTrace("Partition %lld: \n"_Str, Partition::iSlotToPartition(iPartition));
      pSelCursor->vBegin(iPartition);
      pSelCursor->vFirst();
      Str* pStr = pSelCursor->aRowAsStr();
      while (pStr) {
        iRowCount++;
        if (m_pSEitf->m_bDisplay)
          vTrace("row: (%s)\n"_Str,pStr);
        pSelCursor->vNext();
        pStr = pSelCursor->aRowAsStr();
      }
    }
  }
  
  pSelCursor->vEnd();
  delete pSelCursor;
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bReturnTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTokenSet[0]->vCopy("rettbl"_Str);
  m_xTokenSet[1]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n"_Str, m_sResponse.s());
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* get the first row                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = m_sResponse.iToI8();
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vBegin(m_iPartition);
  pSelCursor->vFirst();
  Str* pStr = pSelCursor->aRowAsStr();
  
  if (pStr == nullptr) {
    pSelCursor->vEnd();
    delete pSelCursor;
    m_pSEitf->m_aRowPtr = nullptr;
    return false;
  }
  else {
    m_pSEitf->m_aRowPtr = pStr;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* get the next row                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetNextRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = m_sResponse.iToI8();
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  Str* pStr = pSelCursor->aRowAsStr();
  
  if (pStr == nullptr) {
    pSelCursor->vEnd();
    delete pSelCursor;
    m_pSEitf->m_aRowPtr = nullptr;
    
    // now return the table
    bool bResult = bReturnTable();
    ASSERT(bResult);
    return false;
  }
  else {
    m_pSEitf->m_aRowPtr = pStr;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTestFile(Str* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  File xFile;
  
  if (xFile.bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vPushFile(Str* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPush();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHelp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  vTrace("se commands:\n"_Str);
  vTrace("antbl      -- analyze table\n"_Str);
  vTrace("bye        -- quit se prompt\n"_Str);
  vTrace("cf         -- change folder\n"_Str);
  vTrace("create     -- create tenant|catalog|schema|table|index\n"_Str);
  vTrace("dex        -- distributed export table t\n"_Str);
  vTrace("drop       -- drop tenant|catalog|schema|table|index\n"_Str);
  vTrace("ex         -- export table t\n"_Str);
  vTrace("keyselidx  -- exact key select\n"_Str);
  vTrace("ld         -- load table t\n"_Str);
  vTrace("lf         -- list folder contents\n"_Str);
  vTrace("ls         -- list directory contents\n"_Str);
  vTrace("lscat      -- list catalogs contents\n"_Str);
  vTrace("lscol      -- list columns\n"_Str);
  vTrace("lsidx      -- list indices\n"_Str);
  vTrace("lsidxcol   -- list columns of indiex\n"_Str);
  vTrace("lssch      -- list schemas contents\n"_Str);
  vTrace("lstbl      -- list tables contents\n"_Str);
  vTrace("lsten      -- list tenants contents\n"_Str);
  vTrace("lwrselidx  -- lower bound select\n"_Str);
  vTrace("print      -- print string\n"_Str);
  vTrace("pwf        -- print working folder\n"_Str);
  vTrace("rgeselidx  -- range select\n"_Str);
  vTrace("sel        -- select * from table\n"_Str);
  vTrace("space      -- pool space\n"_Str);
  vTrace("status     -- grid status\n"_Str);
  vTrace("sz         -- size of table\n"_Str);
  vTrace("szidx      -- size of an index\n"_Str);
  vTrace("tableSizes -- size of all tables\n"_Str);
  vTrace("tr         -- truncate table\n"_Str);
  vTrace("tridx      -- truncate table\n"_Str);
  vTrace("uprselidx  -- upper bound select\n"_Str);
  vTrace("usesch     -- use schema\n"_Str);
  vTrace("?          -- print this help\n"_Str);
  vTrace("help       -- print this help\n"_Str);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBeginInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("begin"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("insert"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  m_xTokenSet[0]->vCopy("gettbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bBeginInsert : gettbl FAILED!!!\n"_Str);
    return false;
  }
  
  bool bResult = m_pSEitf->bBeginTx();
  
  if (!bResult) {
    m_pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
    return false;
  }
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* sleep n seconds                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSleep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("sleep"_Str));
  
  if ( ! bTokenCheck(2) )
    return false;
  
  i8 nSeconds = m_xTokenSet[1]->iToI8();

  PosixThread::vSleepSec(nSeconds);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* change working directory                                                   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bChangeWorkingDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_xTokenSet[0]->bEqual("cd"_Str));
  
  sDir = m_xTokenSet[1];
  
  i8 iToken;
  for (iToken = 2; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    sDir.vConcat(m_xTokenSet[iToken]);
  }

  if ( ! xDir.bChangeCurrent(sDir) ) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_DIRECTORY);
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bAnalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_xTokenSet[0]->bEqual("antbl"_Str));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* detailed analyze table                                                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDanalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_xTokenSet[0]->bEqual("dantbl"_Str));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("docol"_Str));
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("create"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("catalog"_Str));
  
  i8 iToken;
  
  // copy tokens from m_xTokenSet to m_xTokenSet2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a catalog
  m_xTokenSet[0]->vCopy("mkcat"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog created: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateIndex(bool bUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = bUnique ? 1 : 0;
  
  ASSERT(m_xTokenSet[0]->bEqual("create"_Str));
  ASSERT(   (  bUnique &&
               m_xTokenSet[1]->bEqual("unique"_Str) &&
               m_xTokenSet[2]->bEqual("index"_Str) )
          ||
            (! bUnique && m_xTokenSet[1]->bEqual("index"_Str))
         );
  
  // copy tokens from m_xTokenSet to m_xTokenSet2
  i8 iToken;
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  i8 iValue;
  i8 iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  
  // there should be a minumum of 7 tokens
  if (iNumTokens < 7 + iIfUnique) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be 'on'
  if (!m_xTokenSet2[3 + iIfUnique]->bEqual("on"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[5] should be '('
  if (!m_xTokenSet2[5 + iIfUnique]->bEqual("("_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!m_xTokenSet2[iNumTokens - 1]->bEqual(")"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index
  
  m_xTokenSet[0]->vCopy("mkidx"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[4 + iIfUnique]);
  m_xTokenSet[2]->vCopy(m_xTokenSet2[2 + iIfUnique]);
  
  if (!bExecuteRequest())
    return false;
  
  // change folder to table
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[4 + iIfUnique]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateIndex : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmidx"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2 + iIfUnique]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // change folder to indexes
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy("indexes"_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateIndex : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmidx"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2 + iIfUnique]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index columns
  
  // now collect the column names and their types
  i8 iCurrentToken = 6 + iIfUnique;
  while(iCurrentToken < iNumTokens - 1) {
    
    if (iCurrentToken != 6 + iIfUnique && !m_xTokenSet2[iCurrentToken++]->bEqual(","_Str)) {
      bIndexReturnTable(m_xTokenSet2[4 + iIfUnique], m_xTokenSet2[2 + iIfUnique]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }

    // add columns to the table
    i8 j = 0;
    m_xTokenSet[j++]->vCopy("addidxcol"_Str);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[4 + iIfUnique]);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[2 + iIfUnique]);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[iCurrentToken++]);
  
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(m_xTokenSet2[4 + iIfUnique], m_xTokenSet2[2 + iIfUnique]);
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // uniqueness
  
  if (bUnique) {
    
    m_xTokenSet[0]->vCopy("setidxunique"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[4 + iIfUnique]);
    m_xTokenSet[2]->vCopy(m_xTokenSet2[2 + iIfUnique]);
    m_xTokenSet[3]->vCopy("true"_Str);
    m_xTokenSet[4]->vCopy(""_Str);
    
    if (!bExecuteRequest()) {
      vTrace("SePrompt::bCreateIndex : setidxunique FAILED!!!\n"_Str);
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(m_xTokenSet2[4 + iIfUnique], m_xTokenSet2[2 + iIfUnique]);
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of make index
  
  m_xTokenSet[0]->vCopy("endmkidx"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[4 + iIfUnique]);
  m_xTokenSet[2]->vCopy(m_xTokenSet2[2 + iIfUnique]);
  m_xTokenSet[3]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateIndex : endmkidx FAILED!!!\n"_Str);
    // save error returned by addColumn
    eEC eError = m_pSEitf->eGetLastError();
    
    // drop the table that was created
    bIndexReturnTable(m_xTokenSet2[4 + iIfUnique], m_xTokenSet2[2 + iIfUnique]);
    
    // return error returned by addColumn
    m_pSEitf->vSetLastError(eError);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // restore path
  
  // change folder to tables
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateIndex : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmidx"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2 + iIfUnique]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // change folder to tables
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateIndex : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmidx"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2 + iIfUnique]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  ASSERT(m_xTokenSet[0]->bEqual("create"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("schema"_Str));
  
  i8 iToken;
  
  // copy tokens from m_xTokenSet to m_xTokenSet2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // create a schema
  m_xTokenSet[0]->vCopy("mksch"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("create"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("tenant"_Str));
  
  i8 iToken;
  
  // copy tokens from m_xTokenSet to m_xTokenSet2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a tenant
  m_xTokenSet[0]->vCopy("mkten"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Tenant created: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bSame("create"_Str));
  ASSERT(m_xTokenSet[1]->bSame("table"_Str));
  
  i8 iToken;
  
  // copy tokens from m_xTokenSet to m_xTokenSet2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // some validations
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be a minumum of 7 tokens
  if (iNumTokens < 7) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be '('
  if (!m_xTokenSet2[3]->bEqual("("_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!m_xTokenSet2[iNumTokens-1]->bEqual(")"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // create the table
  m_xTokenSet[0]->vCopy("mktbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  // change folder to table
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmtbl"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // get reference to pTable
  m_xTokenSet[0]->vCopy("gettbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : gettbl FAILED!!!\n"_Str);
    
    // remove the table
    m_xTokenSet[0]->vCopy("rmtbl"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
    
    if (!bExecuteRequest()) {      
      return false;
    }
    
    return false;
  }
  
  // now collect the column names and their types
  i8 iCurrentToken = 4;
  while(iCurrentToken < iNumTokens - 2) {
    
    if (iCurrentToken != 4 && !m_xTokenSet2[iCurrentToken++]->bEqual(","_Str)) {
      bDropReturnTable(m_xTokenSet2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    m_xTokenSet[j++]->vCopy("addcol"_Str);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[2]);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[iCurrentToken++]);
    
    if (m_xTokenSet2[iCurrentToken]->bSame("integer"_Str) ||
        m_xTokenSet2[iCurrentToken]->bSame("varchar"_Str) ||
        m_xTokenSet2[iCurrentToken]->bSame("varbinary"_Str) ||
        m_xTokenSet2[iCurrentToken]->bSame("char"_Str) ||
        m_xTokenSet2[iCurrentToken]->bSame("float"_Str)) {
      
      // copy varchar/char
      m_xTokenSet[j]->vCopy(m_xTokenSet2[iCurrentToken++]);
      
      if (m_xTokenSet2[iCurrentToken]->bEqual("("_Str)) {
        
        // str.cat '('
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
        
        // str.cat the size
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken]);
        
        // check if it was numeric
        if ( ! m_xTokenSet2[iCurrentToken++]->bIsNumeric() ) {
          // drop the table
          bDropReturnTable(m_xTokenSet2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!m_xTokenSet2[iCurrentToken]->bEqual(")"_Str)) {
          // drop the table
          bDropReturnTable(m_xTokenSet2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
        
      }
    }
    else if (m_xTokenSet2[iCurrentToken]->bSame("decimal"_Str)) {
      
      // copy varchar/char
      m_xTokenSet[j]->vCopy(m_xTokenSet2[iCurrentToken++]);
      
      if (m_xTokenSet2[iCurrentToken]->bEqual("("_Str)) {
        
        // str.cat '('
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
        
        // str.cat the size
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken]);
        
        // check if it was numeric
        if ( ! m_xTokenSet2[iCurrentToken++]->bIsNumeric() ) {
          bDropReturnTable(m_xTokenSet2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!m_xTokenSet2[iCurrentToken]->bEqual(","_Str)) {
          bDropReturnTable(m_xTokenSet2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ','
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
        
        // str.cat the size
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken]);
        
        // check if it was numeric
        if ( ! m_xTokenSet2[iCurrentToken++]->bIsNumeric() ) {
          bDropReturnTable(m_xTokenSet2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!m_xTokenSet2[iCurrentToken]->bEqual(")"_Str)) {
          bDropReturnTable(m_xTokenSet2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
      }
    }
    else if (m_xTokenSet2[iCurrentToken]->bSame("date"_Str) ||
             m_xTokenSet2[iCurrentToken]->bSame("time"_Str) ||
             m_xTokenSet2[iCurrentToken]->bSame("timestamp"_Str) ||
             m_xTokenSet2[iCurrentToken]->bSame("interval"_Str) ||
             m_xTokenSet2[iCurrentToken]->bSame("boolean"_Str)) {
      m_xTokenSet[j]->vCopy(m_xTokenSet2[iCurrentToken++]);
    }
    else {
      // drop the table that was created
      bDropReturnTable(m_xTokenSet2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // check for NOT NULL
    if (m_xTokenSet2[iCurrentToken]->bSame("not"_Str)) {
      
      m_xTokenSet[j]->vConcat(" "_Str);
      m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
      
      if (!m_xTokenSet2[iCurrentToken]->bSame("null"_Str)) {
        bDropReturnTable(m_xTokenSet2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      m_xTokenSet[j]->vConcat(" "_Str);
      m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
    }
    
    // check for PRIMARY KEY
    if (m_xTokenSet2[iCurrentToken]->bSame("primary"_Str)) {
      
      m_xTokenSet[j]->vConcat(" "_Str);
      m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
      
      if (!m_xTokenSet2[iCurrentToken]->bSame("key"_Str)) {
        bDropReturnTable(m_xTokenSet2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      m_xTokenSet[j]->vConcat(" "_Str);
      m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
    }

    // check for UNIQUE
    if (m_xTokenSet2[iCurrentToken]->bSame("unique"_Str)) {
      
      m_xTokenSet[j]->vConcat(" "_Str);
      m_xTokenSet[j]->vConcat(m_xTokenSet2[iCurrentToken++]);
    }

    
    // now we should see a column type
    // validate it
    m_xTokenSet[j]->vToLower();
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bDropReturnTable(m_xTokenSet2[2]);
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // end of make table
  m_xTokenSet[0]->vCopy("endmktbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  m_xTokenSet[2]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : endmktbl FAILED!!!\n"_Str);
  }
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);

  // change folder to back to tables
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : cf .. FAILED!!!\n"_Str);
    return false;
  }
  
  //vTrace("Table created: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vCollectStatement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(! m_xTokenSet[0]->bEqual(""_Str));

  // have we already seen the ';'
  i8 iToken = m_pTokeniser->iFindToken(";"_Str,m_xTokenSet,C_TOKENS);
  
  if (iToken >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Str,m_xTokenSet,C_TOKENS);
    return;
  }
  
  iToken = m_pTokeniser->iCountTokens(m_xTokenSet,C_TOKENS);
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  s32 sContinuation("  > "_Str);
  m_pTokeniser->vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; iToken < C_TOKENS; iToken++) {
    
    eTK eToken = m_pTokeniser->eNextToken(m_xTokenSet[iToken]->s());
    ASSERT(TK_NONE != eToken);
    
    // look for terminal
    if (m_xTokenSet[iToken]->bEqual(";"_Str))
      break;
  }
  
  // switch back to line mode
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Command : "_Str,m_xTokenSet,C_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* connect the cluster                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bConnectCluster(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("connectCluster"_Str));
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop catalog                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("drop"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("catalog"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a catalog
  m_xTokenSet[0]->vCopy("rmcat"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog dropped: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("drop"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("index"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 5 tokens
  if (iNumTokens != 5) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 4th token should be '.'
  if (!m_xTokenSet2[3] ->bEqual("."_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  m_xTokenSet[0]->vCopy("rmidx"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  m_xTokenSet[2]->vCopy(m_xTokenSet2[4]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("drop"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("schema"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  m_xTokenSet[0]->vCopy("rmsch"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema dropped: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("drop"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("table"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  m_xTokenSet[0]->vCopy("rmtbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* index return table                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bIndexReturnTable(Str* aTableName, Str* aIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  // change folder to table
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  // change folder to table
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  m_xTokenSet[0]->vCopy("rmidx"_Str);
  m_xTokenSet[1]->vCopy(aTableName);
  m_xTokenSet[2]->vCopy(aIndexName);
  m_xTokenSet[3]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    //vFatal("Error: (%s) Drop Table %s\n"_Str, m_sResponse.s(), pTableName);
    return false;
  }
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* drop return table                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropReturnTable(Str* aTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  // change folder to table
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    return false;
  }
  m_xTokenSet[0]->vCopy("rmtbl"_Str);
  m_xTokenSet[1]->vCopy(aTableName);
  m_xTokenSet[2]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    //vFatal("Error: (%s) Drop Table %s\n"_Str, m_sResponse.s(), pTableName);
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("drop"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("tenant"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a tenant
  m_xTokenSet[0]->vCopy("rmten"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  if (!bExecuteRequest()) {
    return false;
  }
  
  //vTrace("Tenant dropped: %s\n"_Str, m_xTokenSet2[2]);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bEndInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("end"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("insert"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  bool bResult = m_pSEitf->bInsertBuffer();
  
  // if rowBuffer request was appended to requestQ, then wait for resonse
  if (bResult) {
    // get response from se
    eEC eResult = eGetResponse(m_pSEitf);
    
    if (eResult != EC_NONE) {
      m_pSEitf->vSetLastError(eResult);
      return eResult;
    }
  }
  
  // now return the table
  bResult = bReturnTable();
  if (!bResult)
    return false;
  
  //eEC eResult = eExecuteRequest(m_pSEitf, "rettbl"_Str);
  //if (EC_NONE != eResult) {
  //  vTrace("SePrompt::bEndInsert : rettbl FAILED!!!\n"_Str);
  //  return eResult;
  //}
  
  // get response
  //eResult = eGetResponse(m_pSEitf);
  //if (EC_NONE != eResult) {
  //  return eResult;
  //}

  //m_pSEitf->vCurrentTable(nullptr);
  
  m_pSEitf->vCommitTx();
  m_pSEitf->vEndTx();

  return true;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("insert"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("row"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  //if (!pRow) {
  //  m_pSEitf->vSetLastError(EC_INVALID_PARAMETER);
  //  return EC_INVALID_PARAMETER;
  //}
  
  if (!m_pSEitf->pCurrentTable()) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return EC_NO_SUCH_TABLE;
  }
  
  Str* pRow = m_xTokenSet2[2];
  bool bResult = m_pSEitf->bInsertRow(pRow);
  
  if (!bResult) {
    m_pSEitf->vSetLastError(m_pSEitf->m_eErrorCode);
    return bResult;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* list directory                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir  xDir;
  s256 sDir;
  s256 sDirEntry;
  i8 iEntryType;
  
  ASSERT(m_xTokenSet[0]->bEqual("ls"_Str));
  
  if ( ! bTokenCheck(1) )
    return false;
  
  // get the current directory
  if (!xDir.bGetCurrent(sDir)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // open the current directory
  if (!xDir.bOpen()) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // read the current directory
  while(xDir.bRead()) {
    sDirEntry = xDir.sEntryName();
    iEntryType = xDir.iEntryType();
    vTrace("%c %s\n"_Str, iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.s());
  }
  
  // close the current directory
  xDir.vClose();
  
  // clear the response
  m_sResponse = ""_Str;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* send object (temp)                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("object"_Str));
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print pool space                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPoolSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("space"_Str));
    
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print node status                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bNodeStatus(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("status"_Str));
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* partition a table                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPartitionTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenSet[0]->bEqual("partition"_Str));
  
  // copy tokens from m_xTokenSet to m_xTokenSet2
  i8 iToken;
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  i8 iValue;
  i8 iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  
  // there should be a minumum of 5 tokens
  if (iNumTokens < 5) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[1] should be 'table'
  if (!m_xTokenSet2[1]->bEqual("table"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // token[3] should be 'on'
  if (!m_xTokenSet2[3]->bEqual("on"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // partition the index
  
  m_xTokenSet[0]->vCopy("parttbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  // change folder to table
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bPartitionTable : cf FAILED!!!\n"_Str);
    
    // remove partitioning
    m_xTokenSet[0]->vCopy("rmpart"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // change folder to partition
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy("partitions"_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bPartitionTable : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmpart"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  
  // now collect the column names and their types
  i8 iCurrentToken = 3;
  while(iCurrentToken < iNumTokens - 1) {
    
    if (iCurrentToken != 3 && !m_xTokenSet2[iCurrentToken++]->bEqual(","_Str)) {
      bIndexReturnTable(m_xTokenSet2[4], m_xTokenSet2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    m_xTokenSet[j++]->vCopy("addpartcol"_Str);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[2]);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[4]);
    m_xTokenSet[j++]->vCopy(m_xTokenSet2[iCurrentToken++]);
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(m_xTokenSet2[2], m_xTokenSet2[2]);
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of partition
  
  m_xTokenSet[0]->vCopy("endparttbl"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  m_xTokenSet[2]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bPartitionTable : endparttbl FAILED!!!\n"_Str);
    // save error returned by addColumn
    eEC eError = m_pSEitf->eGetLastError();
    
    // drop the table that was created
    bIndexReturnTable(m_xTokenSet2[4], m_xTokenSet2[2]);
    
    // return error returned by addColumn
    m_pSEitf->vSetLastError(eError);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // restore path
  
  // change folder to tables
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bPartitionTable : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmpart"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2 ]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // change folder to tables
  m_xTokenSet[0]->vCopy("cf"_Str);
  m_xTokenSet[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bPartitionTable : cf FAILED!!!\n"_Str);
    
    // create the table
    m_xTokenSet[0]->vCopy("rmpart"_Str);
    m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print working directory                                                    */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPrintWorkingDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir xDir;
  s256 sDir;
  
  ASSERT(m_xTokenSet[0]->bEqual("pwd"_Str));
  
  if ( ! bTokenCheck(1) )
    return false;
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s"_Str, sDir.s());
  }
  
  // clear the respnse
  m_sResponse = ""_Str;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* use schema                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bUseSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_xTokenSet[0]->bEqual("use"_Str));
  ASSERT(m_xTokenSet[1]->bEqual("schema"_Str));
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_xTokenSet[iToken]->s())
      break;
    m_xTokenSet2[iToken]->vCopy(m_xTokenSet[iToken]);
    m_xTokenSet[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  m_xTokenSet[0]->vCopy("usesch"_Str);
  m_xTokenSet[1]->vCopy(m_xTokenSet2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema changed: %s\n"_Str, m_xTokenSet2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* check for a specific number of tokens                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTokenCheck(i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if ( ! m_xTokenSet[nTokens]->bEqual(""_Str) ) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* check if command token 0 is given argument                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmdIs(Str* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_xTokenSet[0]->bEqual(aArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* check if command token 1 is given argument                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bOneIs(Str* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_xTokenSet[1]->bEqual(aArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePrompt::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
