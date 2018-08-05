/*============================================================================*/
/* Job_CreateTable.cpp -- Create Table Job                                    */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_CreateTable (
  "Job_CreateTable"_Stz,  // name of the job class
  CN_JOB_CREATETABLE,     // class enum
  JK_DIST2PC,             // job kind
  "create table"_Stz,     // comment about the job
  "create"_Stz,           // the prompt command
  "table"_Stz,            // the second word
  "createTable"_Stz,      // the task seed for this job
  false,                  // detached job?
  pJob_CreateTable_pre,   // prompt pre-process function
  vJob_CreateTable_post,  // prompt post-process function
  true                    // job is enabled
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateTable* JobsModel::_pJob_CreateTable(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = reinterpret_cast<Job_CreateTable*>(aAddress);
  
  RASSERT( !pJob_CreateTable || CN_JOB_CREATETABLE == pJob_CreateTable->eClass());
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateTable =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB_CREATETABLE));
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::operator delete(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = static_cast<Job_CreateTable*>(aAddress);
  
  Heap::vFree(pJob_CreateTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateTable::Job_CreateTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATETABLE), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  m_sSeed             = mJobNote_CreateTable.pJobClass()->m_aSeed;
  m_bCheckResources   = true;
  
  m_eTableType = TT_NONE;
  m_iColumns   = 0;
  
  for (i8 i_Column = 0; i_Column < C_MAX_COLUMNS; i_Column++) {
    m_xColumns[i_Column].m_iColumnNo = -1;
    m_xColumns[i_Column].m_sColumnName = ""_Stz;
    m_xColumns[i_Column].m_eColumnType = CT_NONE;
    m_xColumns[i_Column].m_iColumnLength = -1;
    m_xColumns[i_Column].m_iColumnScale = -1;
    m_xColumns[i_Column].m_bIsNullable = true;
    m_xColumns[i_Column].m_bIsUnique = false;
    m_xColumns[i_Column].m_bIsPartitionKey = false;
    m_xColumns[i_Column].m_eDatumType = DM_NONE;
  }
  
  _checkInitialised_(CN_JOB_CREATETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateTable::~Job_CreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATETABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATETABLE);
  
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pObject);

  pJob_CreateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate table                                                             */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert we got a create table
  ASSERT(pTokenVec->pGet(0)->bSame("create"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("table"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateTable(pTokenVec);
  if ( ! bValid ) {
    return false;
  }
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  // mark current token
  i8 iCurrentToken = 4;
  
  // add columns to the table
  if ( ! bAddTableColumns(pTokenVec, iCurrentToken) ) {
    return false;
  }
  
  // current token should be at ')'
  if ( ! pTokenVec->pGet(iCurrentToken++)->bSame(")"_Stz)) {
    return false;
  }
  
  // check if we have a partition by clause
  if ( iCurrentToken < iNumTokens && pTokenVec->pGet(iCurrentToken++)->bSame("partition"_Stz)) {
    
    if ( ! bPartitionTable(pTokenVec, iCurrentToken) )
      return false;
  }
  else {
    // no partition by clause
    // we will create partitioning using the first column
    m_xColumns[0].m_bIsPartitionKey = true;
  }
  
  vShow();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  vTrace("\n\nJob_CreateTable: %s\n"_Stz, m_sObjectName.aChar());
  
  //vTrace("m_sExecute = %s\n"_Stz,m_sExecute.aChar());
  vTrace("m_eTableType = %s\n"_Stz,aTT[m_eTableType]);
  vTrace("m_iColumns = %lld\n"_Stz,m_iColumns);
  
  for (i8 i_Column = 0; i_Column < m_iColumns; i_Column++) {
    vTrace("\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnNo);
    vTrace("  m_xColumns[%lld].m_iColumnNo = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnNo);
    vTrace("  m_xColumns[%lld].m_sColumnName = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_sColumnName.aChar());
    vTrace("  m_xColumns[%lld].m_eColumnType = %s\n"_Stz, i_Column, aCT[m_xColumns[i_Column].m_eColumnType]);
    vTrace("  m_xColumns[%lld].m_iColumnLength = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnLength);
    vTrace("  m_xColumns[%lld].m_iColumnScale = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnScale);
    vTrace("  m_xColumns[%lld].m_bIsNullable = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsNullable ? "true" : "false");
    vTrace("  m_xColumns[%lld].m_bIsUnique = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsUnique ? "true" : "false");
    vTrace("  m_xColumns[%lld].m_bIsPartitionKey = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsPartitionKey ? "true" : "false");
    vTrace("  m_xColumns[%lld].m_eDatumType = %s\n"_Stz, i_Column, aDT[m_xColumns[i_Column].m_eDatumType]);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* add columns of the table to the job                                        */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bAddTableColumns(TokenVec* pTokenVec, i8& iCurrentToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // now collect the column names and their types
  i8 iColumn = 0;
  i8 i_StartToken = iCurrentToken;
  
  while ( ! pTokenVec->pGet(iCurrentToken)->bSame(")"_Stz)) {
    
    if (iCurrentToken != i_StartToken && ! pTokenVec->pGet(iCurrentToken++)->bSame(","_Stz)) {
      return false;
    }
    
    // set column number
    m_xColumns[iColumn].m_iColumnNo = iColumn;
    
    // set column name
    m_xColumns[iColumn].m_sColumnName= pTokenVec->pGet(iCurrentToken++)->aText();
    
    if (pTokenVec->pGet(iCurrentToken)->bSame("varchar"_Stz) ||
        pTokenVec->pGet(iCurrentToken)->bSame("varbinary"_Stz) ||
        pTokenVec->pGet(iCurrentToken)->bSame("char"_Stz)) {
      
      // extract varchar
      if ( ! bExtractVarchar(pTokenVec, iColumn, iCurrentToken) )
        return false;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("float"_Stz)) {
      
      // extract float
      if ( ! bExtractFloat(pTokenVec, iColumn, iCurrentToken) )
        return false;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("decimal"_Stz)) {
      
      // extract decimal
      if ( ! bExtractDecimal(pTokenVec, iColumn, iCurrentToken) )
        return false;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("integer"_Stz)) {
      m_xColumns[iColumn].m_eColumnType = CT_INTEGER;
      m_xColumns[iColumn].m_eDatumType = DM_INT;
      m_xColumns[iColumn].m_iColumnLength = C_MAX_PRECISION_INT;
      iCurrentToken++;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("date"_Stz)) {
      m_xColumns[iColumn].m_eColumnType = CT_DATE;
      m_xColumns[iColumn].m_eDatumType = DM_DTE;
      iCurrentToken++;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("time"_Stz)) {
      m_xColumns[iColumn].m_eColumnType = CT_TIME;
      m_xColumns[iColumn].m_eDatumType = DM_TME;
      iCurrentToken++;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("timestamp"_Stz)) {
      m_xColumns[iColumn].m_eColumnType = CT_TIMESTAMP;
      m_xColumns[iColumn].m_eDatumType = DM_TSP;
      iCurrentToken++;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("interval"_Stz)) {
      m_xColumns[iColumn].m_eColumnType = CT_INTERVAL;
      m_xColumns[iColumn].m_eDatumType = DM_IVL;
      iCurrentToken++;
    }
    else if (pTokenVec->pGet(iCurrentToken)->bSame("boolean"_Stz)) {
      m_xColumns[iColumn].m_eColumnType = CT_BOOLEAN;
      m_xColumns[iColumn].m_eDatumType = DM_BOL;
      iCurrentToken++;
    }
    else {
     return false;
    }
    
    // check for NOT NULL
    if (pTokenVec->pGet(iCurrentToken)->bSame("not"_Stz)) {
      iCurrentToken++;
      if (!pTokenVec->pGet(iCurrentToken++)->bSame("null"_Stz)) {
        return false;
      }
      m_xColumns[iColumn].m_bIsNullable = false;
    }
    
    // check for PRIMARY KEY
    //    if (m_xTokenVec2[iCurrentToken]->bSame("primary"_Stz)) {
    //
    //      pJob->m_xColumns[iColumn].m_sPrimaryKey = m_xTokenVec[iCurrentToken++]->aText();
    //
    //      if (!m_xTokenVec2[iCurrentToken]->bSame("key"_Stz)) {
    //        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    //        return false;
    //      }
    //    }
    
    // check for UNIQUE
    if (pTokenVec->pGet(iCurrentToken)->bSame("unique"_Stz)) {
      iCurrentToken++;
      m_xColumns[iColumn].m_bIsUnique = true;
    }
    
    iColumn++;
  }
  
  // set no of columns in the job
  m_iColumns = iColumn;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract float                                                              */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bExtractDecimal(TokenVec* pTokenVec, i8 iColumn, i8& iCurrentToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // set default precision and scale
  i8 iPrecision = C_MAX_PRECISION_BCD;
  i8 iScale = 0;

  // set column type in column
  m_xColumns[iColumn].m_eColumnType = CT_DECIMAL;
  m_xColumns[iColumn].m_eDatumType = DM_BCD;
  iCurrentToken++;

  // extract scale and precison if specified
  if (pTokenVec->pGet(iCurrentToken++)->bSame("("_Stz)) {
    
    // check if it was numeric
    if ( ! pTokenVec->pGet(iCurrentToken)->bIsNumeric() )
      return false;
    
    iPrecision = pTokenVec->pGet(iCurrentToken++)->iToI8();
    
    if (!pTokenVec->pGet(iCurrentToken++)->bSame(","_Stz))
      return false;
    
    // check if it was numeric
    if ( ! pTokenVec->pGet(iCurrentToken)->bIsNumeric() )
      return false;
    
    // get the scale
    iScale = pTokenVec->pGet(iCurrentToken++)->iToI8();
    
    if (!pTokenVec->pGet(iCurrentToken++)->bSame(")"_Stz))
      return false;
    
    if (!Bcd::bIsValidSize(iPrecision, iScale))
      return false;
  }
  
  // set scale and length in column
  m_xColumns[iColumn].m_iColumnLength = iPrecision;
  m_xColumns[iColumn].m_iColumnScale  = iScale;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract float                                                              */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bExtractFloat(TokenVec* pTokenVec, i8 iColumn, i8& iCurrentToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // set default size
  i8 iSize = C_MAX_PRECISION_FLT;

  // set column type
  m_xColumns[iColumn].m_eColumnType = CT_FLOAT;
  m_xColumns[iColumn].m_eDatumType = DM_FLT;
  iCurrentToken++;

  // extract size if specified
  if (pTokenVec->pGet(iCurrentToken)->bSame("("_Stz)) {
    
    // check if it was numeric
    if ( ! pTokenVec->pGet(iCurrentToken++)->bIsNumeric() ) {
      return false;
    }
    
    // get size
    iSize = pTokenVec->pGet(iCurrentToken++)->iToI8();
    
    if (!pTokenVec->pGet(iCurrentToken++)->bSame(")"_Stz)) {
      return false;
    }
    
    if (!Flt::bIsValidSize(iSize)) {
      return false;
    }
  }
  
  // set size in column
  m_xColumns[iColumn].m_iColumnLength = iSize;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract varchar                                                            */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bExtractVarchar(TokenVec* pTokenVec, i8 iColumn, i8& iCurrentToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // set column type
  m_xColumns[iColumn].m_eColumnType = CT_VARCHAR;
  m_xColumns[iColumn].m_eDatumType = DM_VCH;
  iCurrentToken++;

  // set default size
  i8 iSize = C_64K;

  // extract size if specified
  if (pTokenVec->pGet(iCurrentToken)->bSame("("_Stz)) {
    
    m_xColumns[iColumn].m_iColumnLength = pTokenVec->pGet(iCurrentToken++)->iToI8();
    
    
    // check if it was numeric
    if ( ! pTokenVec->pGet(iCurrentToken)->bIsNumeric() )
      return false;
    
    // get size
    iSize = pTokenVec->pGet(iCurrentToken++)->iToI8();
    
    if (!pTokenVec->pGet(iCurrentToken++)->bSame(")"_Stz))
      return false;
    
    // valid the size
    if (!Vch::bIsValidSize(iSize)) {
      return false;
    }
  }
  // set size in column
  m_xColumns[iColumn].m_iColumnLength = iSize;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* partition the table                                                        */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bPartitionTable(TokenVec* pTokenVec, i8 iCurrentToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // next token should be 'by'
  if (!pTokenVec->pGet(iCurrentToken++)->bSame("by"_Stz)) {
    return false;
  }
  
  // next token should be '('
  if (!pTokenVec->pGet(iCurrentToken++)->bSame("("_Stz)) {
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  i8 i_StartToken = iCurrentToken;
  i8 iPartKeys = 0;
  bool bFound = false;
  while (iCurrentToken < iNumTokens - 1) {
    
    if ( (iCurrentToken != i_StartToken) &&
        !pTokenVec->pGet(iCurrentToken++)->bSame(","_Stz)) {
      return false;
    }
    iPartKeys++;
    
    // find column and set as partition column
    for (i8 i_Column=0; i_Column< m_iColumns; i_Column++) {
      if (m_xColumns[i_Column].m_sColumnName.bSame(pTokenVec->pGet(iCurrentToken)->aText())) {
        m_xColumns[i_Column].m_bIsPartitionKey = true;
        iCurrentToken++;
        bFound = true;
        break;
      }
    }
    if ( ! bFound) {
      return false;
    }
    bFound = false;
  }
  
  if ( ! iPartKeys ) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate a create table request                                            */
/*----------------------------------------------------------------------------*/
bool Job_CreateTable::bValidateCreateTable(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // there should be a minumum of 7 tokens
  if (iNumTokens < 7) {
    return false;
  }
  
  // token[3] should be '('
  if ( ! pTokenVec->pGet(3)->bSame("("_Stz)) {
    return false;
  }
  
  // last-1 token should be ')'
  if ( ! pTokenVec->pGet(iNumTokens-1)->bSame(")"_Stz)) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CreateTable_pre(Interface::SEitf* pSEitf)
{
  Job_CreateTable* pJob_CreateTable = new Job_CreateTable(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();

  bool bResult = pJob_CreateTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CreateTable->m_bHasError = true;
    pJob_CreateTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateTable_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJob);
  
  if ( pJob_CreateTable->m_bHasError ) {
  
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_CreateTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  
  // now delete the job
  delete pJob_CreateTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
