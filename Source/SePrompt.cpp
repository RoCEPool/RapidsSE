/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Arguments     gxArguments;
extern pthread_key_t giTLS_ThreadKey;

PromptCatalog gxPromptCatalog;
extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t iSize)
{
  RASSERT(iSize);
  SePrompt* pSePrompt = (SePrompt*)Heap::pAllocate(CN_SEPROMPT);
  
  return pPoolCheck<void*>(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::operator delete(void* pSePromptAddress)
{
  SePrompt* pSePrompt = static_cast<SePrompt*>(pSePromptAddress);
  
  Heap::vFree(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(void)
: Object(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;
  m_bContinue = true;

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,iPartition, /*trace*/ 0);
  ASSERT(EC_NONE == eResult);
  
  m_sResponse  = ""_Stz;
  
  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SEPROMPT);
  RASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("SePrompt %p seitf %p \n"_Stz,
         pSePrompt,
         pSePrompt->m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/*-- Task Primitives ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptFun* pPromptFun =
    gxPromptCatalog.pFindFun(pTkn(0)->aText(), pTkn(1)->aText());
  
  if (! pPromptFun)
    return false;
  
  (*pPromptFun->m_fPrompt)(this);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get command processor                                                      */
/*----------------------------------------------------------------------------*/
CommandProcessor* SePrompt::pCommandProcessor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf->m_pCommandProcessor;
}

/*----------------------------------------------------------------------------*/
/* get continue                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bContinue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_bContinue;
}

/*----------------------------------------------------------------------------*/
/* set continue                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vContinue(bool bContinue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_bContinue = bContinue;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  CommandProcessor* pCommandProcessor = this->pCommandProcessor();
  
  pCommandProcessor->vGetCommand();
}

/*----------------------------------------------------------------------------*/
/* get the seitf                                                              */
/*----------------------------------------------------------------------------*/
SEitf* SePrompt::pSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf->m_pCommandProcessor->pTknVec();
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePrompt::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SePrompt* Prompt::_pSePrompt(void* aAddr)
{
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(aAddr);
  
  RASSERT( !pSePrompt || CN_SEPROMPT == pSePrompt->eClass() );
  
  return pSePrompt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer xTimer;
  Stz* aResponse = nullptr;
  
  xTimer.vStart();
  
  pTknVec()->vSetTokens();
  
  //! make this part of verbose??
  //m_pCommandProcessor->m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
  
  TokenVec* pTokenVec = pTknVec();

  bool bResult = RapidsSE_ExecuteRequest(m_pSEitf,*pTokenVec,&aResponse);
  
  if (pTknVec()->bDetachedRequest())
    return bResult;
  
  if (bResult)
    m_sResponse = aResponse ? (Stz*)aResponse : ""_Stz;
  else
    m_sResponse = aResponse ? (Stz*)aResponse : ""_Stz;
  
  xTimer.vUpdate();
  
  return bResult;
}


/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bReturnTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  pTkn(0)->vCopy("rettbl"_Stz);
  pTkn(1)->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n"_Stz, m_sResponse.aChar());
    return false;
  }
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Intercepted Commands ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
/*
bool SePrompt::bCmd_BeginInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("begin"_Stz));
  ASSERT(pTkn(1)->bSame("insert"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  pTkn(0)->vCopy("gettbl"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bBeginInsert : gettbl FAILED!!!\n"_Stz);
    return false;
  }
  
  // begin a transaction
  pTkn(0)->vCopy("txBeg"_Stz);
  pTkn(1)->vCopy(""_Stz);
  if (!bExecuteRequest()) {
     vTrace("SePrompt::bBeginInsert : txBegin FAILED!!!\n"_Stz);
     m_pSEitf->vSetLastError(EC_TX_BEGIN_FAILED);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}
*/

/*----------------------------------------------------------------------------*/
/* load a table from a file  (distributed and parallel)                       */
/*----------------------------------------------------------------------------*/
/*
bool SePrompt::bCmd_DistParLoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bResult = false;
  i8 iIndex;
  i8 iCurrentIndex;
  bool bSameMachine = false;
  
  ASSERT(pTkn(0)->bSame("dpld"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be at least 3 tokens
  if (iNumTokens < 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    goto resetFormatter;
  }
  
  iIndex = 0;
  pTkn(iIndex++)->vCopy(pTkn2(0));
  pTkn(iIndex++)->vCopy(pTkn2(1));
  pTkn(iIndex++)->vCopy(pTkn2(2));
  
  iCurrentIndex = 3;
  
  while (iNumTokens > iCurrentIndex) {
    
    //--------------------------------------------------------------------------
    // C_FIELD_DELIMITER
    if (pTkn2(iCurrentIndex)->bSame(C_FIELD_DELIMITER)) {
      
      if (iNumTokens < iCurrentIndex + 2) {
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
      
      m_pSEitf->m_xFormatters.m_cFieldDelimiter = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cFieldDelimiter == '0')
        m_pSEitf->m_xFormatters.m_cFieldDelimiter = '\0';
    }
    //--------------------------------------------------------------------------
    // C_SAME_MACHINE
    else if (pTkn2(iCurrentIndex)->bSame(C_SAME_MACHINE)) {
      bSameMachine = true;
    }
    else {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    iCurrentIndex++;
  }
  
  pTkn(iIndex++)->vCopy(bSameMachine ? "1"_Stz : "0"_Stz);
  
  //m_pSEitf->m_xFormatters.vShow();
  
  //
  // clear the last error
  m_pSEitf->vClearLastError();
  
  if (!bExecuteRequest())
    goto resetFormatter;
  
  bResult = true;
  
resetFormatter:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset seitf formatters
  
  m_pSEitf->m_xFormatters.vReset(&gpAttachMap->m_xFormatters);
  return bResult;
}
*/
/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
/*
bool SePrompt::bCmd_EndInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("end"_Stz));
  ASSERT(pTkn(1)->bSame("insert"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  Grid* pGrid = gpAttachMap->m_pGrid;
  
  // before sending buffers set no of remaining acks to no of partitons
  m_pSEitf->vSetAcksRemaining(pGrid->nSlots() * pGrid->nNodes());
  
  for (i8 iPartition = 0; iPartition < pGrid->m_nPartitions; iPartition++) {
    bool bResult = m_pSEitf->bInsertLastBuffer(iPartition);
    
    //vTrace("bInsertLastBuffer returned %lld %lld\n"_Stz, iPartition, bResult);
    
    // if rowBuffer request was appended to requestQ, then wait for resonse
    if (bResult) {
      // get response from se
      eEC eResult = eGetResponse(m_pSEitf);
      
      if (eResult != EC_NONE) {
        m_pSEitf->vSetLastError(eResult);
        bResult = bReturnTable();
        if (!bResult)
          return false;
      }
    }
  }

  // now wait till acks from all partitions have been received
  while (*m_pSEitf->m_ziAcksRemaining != 0) {
    //vTrace("*pSEitf->m_ziAcksRemaining %lld ...\n"_Stz, *pSEitf->m_ziAcksRemaining);
    ;
  }
  
  // now return the table
  bool bResult = bReturnTable();
  if (!bResult)
    return false;
  
  ASSERT(m_pSEitf->m_pCurrentTable == nullptr);
  
  // commit a transaction
  pTkn(0)->vCopy("txCmt"_Stz);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::EndInsert : txCmt FAILED!!!\n"_Stz);
    m_pSEitf->vSetLastError(EC_TX_COMMIT_FAILED);
    return false;
  }
  
  // begin a transaction
  pTkn(0)->vCopy("txEnd"_Stz);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::EndInsert : txEnd FAILED!!!\n"_Stz);
    m_pSEitf->vSetLastError(EC_TX_END_FAILED);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}
*/
/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_Help(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  vTrace("se commands:\n"_Stz);
  vTrace("antbl      -- analyze table\n"_Stz);
  vTrace("bye        -- quit se prompt\n"_Stz);
  vTrace("cf         -- change folder\n"_Stz);
  vTrace("create     -- create tenant|catalog|schema|table|index\n"_Stz);
  vTrace("dex        -- distributed export table t\n"_Stz);
  vTrace("drop       -- drop tenant|catalog|schema|table|index\n"_Stz);
  vTrace("ex         -- export table t\n"_Stz);
  vTrace("fetch      -- fetch t where ...\n"_Stz);
  vTrace("keyselidx  -- exact key select\n"_Stz);
  vTrace("ld         -- load table t\n"_Stz);
  vTrace("lf         -- list folder contents\n"_Stz);
  vTrace("ls         -- list directory contents\n"_Stz);
  vTrace("lscat      -- list catalogs contents\n"_Stz);
  vTrace("lscol      -- list columns\n"_Stz);
  vTrace("lsidx      -- list indices\n"_Stz);
  vTrace("lsidxcol   -- list columns of indiex\n"_Stz);
  vTrace("lssch      -- list schemas contents\n"_Stz);
  vTrace("lstbl      -- list tables contents\n"_Stz);
  vTrace("lsten      -- list tenants contents\n"_Stz);
  vTrace("lwrselidx  -- lower bound select\n"_Stz);
  vTrace("print      -- print string\n"_Stz);
  vTrace("pwf        -- print working folder\n"_Stz);
  vTrace("rgeselidx  -- range select\n"_Stz);
  vTrace("sel        -- select * from table\n"_Stz);
  vTrace("space      -- pool space\n"_Stz);
  vTrace("status     -- grid status\n"_Stz);
  vTrace("sz         -- size of table\n"_Stz);
  vTrace("szidx      -- size of an index\n"_Stz);
  vTrace("tableSizes -- size of all tables\n"_Stz);
  vTrace("tr         -- truncate table\n"_Stz);
  vTrace("uprselidx  -- upper bound select\n"_Stz);
  vTrace("usesch     -- use schema\n"_Stz);
  vTrace("version    -- print SE version\n"_Stz);
  vTrace("?          -- print this help\n"_Stz);
  vTrace("help       -- print this help\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
/*
bool SePrompt::bCmd_InsertRow(void)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  bool bBlankLine = false;
  
  ASSERT(pTkn(0)->bSame("insert"_Stz));
  ASSERT(pTkn(1)->bSame("row"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  if (!m_pSEitf->pCurrentTable()) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return false;
  }
  
  Stz* pRow = pTkn2(2)->aText();
  s48K s;
  i8 iRowLength;
  
  // format the record into row format
  bool bValid = Row::bFormatRow(
                  m_pSEitf->m_pCurrentTable->pColumnVec(),
                  &m_pSEitf->m_xFormatters,
                  pRow, StrLib::iSize(pRow->aChar()),
                  s, iRowLength, bBlankLine,
                  false ?* allowNonPrintable *?);
  
  if (! bValid) {
    m_pSEitf->vSetLastError(EC_INVALID_VALUE);
    return false;
  }
  
  // get the partition this row hashes to
  i8 iRowPartition = m_pSEitf->pCurrentTable()->iRowPartition(s);
  
  bool bResult = m_pSEitf->bInsertRow(s, iRowLength, iRowPartition);
  
  if (!bResult) {
    m_pSEitf->vSetLastError(m_pSEitf->m_eErrorCode);
    return bResult;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}
 */
/*----------------------------------------------------------------------------*/
/* load a table from a file                                                   */
/*----------------------------------------------------------------------------*/
/*
bool SePrompt::bCmd_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bResult = false;
  i8 iIndex;
  i8 iCurrentIndex;
  
  ASSERT(pTkn(0)->bSame("ld"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());

  // - - - - - - - - - - - - - - - - - - - - -
  // there should be at least 3 tokens
  if (iNumTokens < 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    goto resetFormatter;
  }
  
  iIndex = 0;
  pTkn(iIndex++)->vCopy(pTkn2(0));
  pTkn(iIndex++)->vCopy(pTkn2(1));
  pTkn(iIndex++)->vCopy(pTkn2(2));
  
  iCurrentIndex = 3;
    
  while (iNumTokens > iCurrentIndex) {
    
    if (iNumTokens < iCurrentIndex + 2) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    
    //--------------------------------------------------------------------------
    // C_RECORD_TERMINATOR
    if (pTkn2(iCurrentIndex)->bSame(C_RECORD_TERMINATOR)) {
      m_pSEitf->m_xFormatters.m_cRecordTerminator = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cRecordTerminator == '0')
        m_pSEitf->m_xFormatters.m_cRecordTerminator = '\0';
    }
    //--------------------------------------------------------------------------
    // C_RECORD_ENDS
    else if (pTkn2(iCurrentIndex)->bSame(C_RECORD_ENDS)) {
      char cRecordEnds = *pTkn2(++iCurrentIndex)->sText().aChar();
      
      // is record_ends 1 character ?
      if (pTkn2(iCurrentIndex)->iLength() == 1) {
        if (cRecordEnds == '0') {
          vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          goto resetFormatter;
        }
        else if (cRecordEnds == '\n') {
          m_pSEitf->m_xFormatters.m_eRecordEnds = RE_NL;
        }
        else if (cRecordEnds == '\r') {
          m_pSEitf->m_xFormatters.m_eRecordEnds = RE_CR;
        }
      }
      // record_ends is 2 characters
      else if (pTkn2(iCurrentIndex)->iLength() == 2) {
        
        if (pTkn2(iCurrentIndex)->bSame("\r\n"_Stz)) {
          m_pSEitf->m_xFormatters.m_eRecordEnds = RE_CRNL;
        }
        else {
          vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          goto resetFormatter;
        }
      }
      else {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
    }
    //--------------------------------------------------------------------------
    // C_FIELD_DELIMITER
    else if (pTkn2(iCurrentIndex)->bSame(C_FIELD_DELIMITER)) {
      m_pSEitf->m_xFormatters.m_cFieldDelimiter = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cFieldDelimiter == '0')
        m_pSEitf->m_xFormatters.m_cFieldDelimiter = '\0';
    }
    //--------------------------------------------------------------------------
    // C_ENCLOSED_BY_LEADING
    else if (pTkn2(iCurrentIndex)->bSame(C_ENCLOSED_BY_LEADING)) {
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading =
        *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading == '0')
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading = '\0';
      
      // default set trailing same as leading
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing =
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading;
    }
    //--------------------------------------------------------------------------
    // C_ENCLOSED_BY_TRAILING
    else if (pTkn2(iCurrentIndex)->bSame(C_ENCLOSED_BY_TRAILING)) {
      
      // if trailing is specified, leading should also have been specified
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading == '\0') {
        if ( ! gpAttachMap->m_bMetadata)
          vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing =
        *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing == '0')
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing = '\0';
    }
//
//    DATE_FORMAT='YYYY.MM.DD’
//
//
    //--------------------------------------------------------------------------
    // C_DATE_FORMAT
    else if (pTkn2(iCurrentIndex)->bSame(C_DATE_FORMAT)) {
      // extract date format
      if ( ! m_pSEitf->bExtractDateFormat(pTkn2(++iCurrentIndex)->sText()) ) {
        if ( ! gpAttachMap->m_bMetadata)
          vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
    }
    
    //--------------------------------------------------------------------------
    // C_ESCAPE_CHAR
    else if (pTkn2(iCurrentIndex)->bSame(C_ESCAPE_CHAR)) {
      m_pSEitf->m_xFormatters.m_cEscapeChar = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEscapeChar == '0')
        m_pSEitf->m_xFormatters.m_cEscapeChar = '\0';
    }
    else {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    iCurrentIndex++;
  }
  
  //m_pSEitf->m_xFormatters.vShow();
  
  //
  // clear the last error
  m_pSEitf->vClearLastError();

  if (!bExecuteRequest())
    goto resetFormatter;
  
  bResult = true;
  
resetFormatter:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset seitf formatters
  
  m_pSEitf->m_xFormatters.vReset(&gpAttachMap->m_xFormatters);
  return bResult;
}
*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* iterate entry buffers frim the seitf queue                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 nPartition;
  m_pSEitf->m_iRowsProcessed = 0;
  
  if (gpAttachMap->m_bMetadata ) {
    vTrace("metadata: %s\n"_Stz, m_pSEitf->m_sMetadata.aStr());
  }
  
  if (m_pSEitf->m_bDisplay) {
    
    while (m_pSEitf->bNextRow2(nPartition)) {
      
      m_pSEitf->m_iRowsProcessed++;
      
      //Entry* pEntry = m_pSEitf->m_pEntry;
      //Row* aRow = pEntry->aRow();
      Row* aRow = m_pSEitf->m_aRow;
      aRow->vAsBinaryString(m_pSEitf->m_sResponse,
                            m_pSEitf->m_pCurrentTable->pColumnVec());
      if (gpAttachMap->m_bMetadata )
        vTrace("row: (%lld, %s)\n"_Stz, nPartition, m_pSEitf->m_sResponse.aChar());
      else
        vTrace("row: (%s)\n"_Stz, m_pSEitf->m_sResponse.aChar());
    }
    if (gpAttachMap->m_bMetadata )
      vTrace("success: %lld rows affected\n"_Stz, m_pSEitf->m_iRowsProcessed);
    else
      vTrace("rows selected %lld\n"_Stz, m_pSEitf->m_iRowsProcessed);
  }
  else {
    // pop the buffers
    m_pSEitf->vPopBufferQueue();
    vTrace("rows selected %lld\n"_Stz, m_pSEitf->m_iRowsProcessed);
  }
  m_pSEitf->m_iRowsProcessed = 0;
  
  m_pSEitf->vEndFetch();
  
  bReturnTable();
  
}
