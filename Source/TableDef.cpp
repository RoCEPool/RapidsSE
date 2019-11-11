/*============================================================================*/
/* TableDef.cpp -- Table Definition                                           */
/*============================================================================*/

#include "MetaData.h"
#include "AM.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TableDef* MetaData::_pTableDef(void* aAddr)
{
  TableDef* pTableDef = reinterpret_cast<TableDef*>(aAddr);
    
  RASSERT_TYPE(pTableDef,CN_TABLEDEF);
  
  return pTableDef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TableDef::operator new(size_t)
{
  Object* pTableDef = HeapMem::pAllocate(CN_TABLEDEF);
  
  return pTableDef;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TableDef::operator delete(void* pAddress)
{
  TableDef* pTableDef = reinterpret_cast<TableDef*>(pAddress);
  
  HeapMem::vFree(pTableDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TableDef::TableDef(void)
: Object(CN_TABLEDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  m_sTableNm   = C_NO_NAME;
  m_eTableType = TT_NORMAL;

  m_pColVec    = new ColumnVec;
  m_pStreamDef = new StreamDef;
  
  m_sCmd       = ""_Stz;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  _checkInitialised_(CN_TABLEDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TableDef::~TableDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  delete m_pColVec;
  delete m_pStreamDef;
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert string into column type                                            */
/*----------------------------------------------------------------------------*/
eCT TableDef::eColumn(Stz* szType)
{
  i8 nEntries = sizeof(aCT) / sizeof (Stz*);
  
  for (i8 i_Type = 0; i_Type < nEntries ; i_Type++) {
    
    if (aCT[i_Type]->bSame(szType))
      return (eCT)i_Type;

  }
  
  return (eCT)CT_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#define PT() vTrace("tkn=%s\n"_Stz,pTkn->szText());

/*----------------------------------------------------------------------------*/
/* parse Create Table command                                                 */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParse(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
 
  eEC eError = EC_NONE;

  // retain the Create Table command
  m_sCmd = szCmd;
  
  // tokenise the command
  TokenVec* pTknVec = Tokeniser::pTokenise(szCmd);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: Create Table <tableNm>
  
  // -- -- -- -- -- -- -- -- -- --
  // assert we got a create table
  // -- -- -- -- -- -- -- -- -- --

  Token* pTkn = pTknVec->pFirstToken();
  ASSERT(pTkn->bSame("create"_Stz));
  
  pTkn = pTknVec->pNextToken();
  ASSERT(pTkn->bSame("table"_Stz));
  
  // -- -- -- --
  // table name
  // -- -- -- --

  pTkn = pTknVec->pNextToken(); PT();
  m_sTableNm = pTkn->szText();

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: ( <columnDef> ,... )
  
  // -- -- -- -- --
  // open parenth (
  // -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); PT();
  if (! pTkn->bSame("("_Stz)) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // iterate from first column till )
  // -- -- -- -- -- -- -- -- -- -- --

  pTkn = pTknVec->pNextToken(); PT();
  for (i8 i_Col = 0; ! pTkn->bSame(")"_Stz); i_Col++) {
    
    // -- -- -- -- -- -- -- --
    // exceeded column limit?
    // -- -- -- -- -- -- -- --

    if (i_Col >= C_MAX_COLUMNS) {
      eError = EC_MAX_COLUMNS;
      goto L_RETURN;
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // subsequent column names, skipping over comma
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    if (i_Col) {
      if (pTkn->bSame(","_Stz)) {
        pTkn = pTknVec->pNextToken(); PT();
      }
      else {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }
    }
    
    // -- -- -- --
    // add column
    // -- -- -- --

    Column* pCol = new Column;
    m_pColVec->vAdd(pCol);

    // set column number
    pCol->vColumnNo(i_Col);
    
    // set column name
    pCol->vColumnName(pTkn->szText());
    
    // -- -- -- -- -- -- -- -- -- -- --
    // check for duplicate column name
    // -- -- -- -- -- -- -- -- -- -- --

    for (i8 i_Search = 0; i_Search < i_Col - 1; i_Search++) {
      Column* pCol_search = m_pColVec->pGet(i_Search++);
      if (pCol_search->sColumnName().bSame(pCol->sColumnName())) {
        eError = EC_DUPLICATE_COLUMN;
        goto L_RETURN;
      }
    }
    
    // -- -- -- --
    // column type
    // -- -- -- --

    pTkn = pTknVec->pNextToken(); PT();
    eCT eColumn = TableDef::eColumn(pTkn->szText());
    
    switch (eColumn) {
        
      case CT_BOOLEAN:   eError = eExtractBoolean   (pCol,pTknVec);  break;
      case CT_DATE:      eError = eExtractDate      (pCol,pTknVec);  break;
      case CT_DECIMAL:   eError = eExtractDecimal   (pCol,pTknVec);  break;
      case CT_FLOAT:     eError = eExtractFloat     (pCol,pTknVec);  break;
      case CT_INTEGER:   eError = eExtractInteger   (pCol,pTknVec);  break;
      case CT_INTERVAL:  eError = eExtractInterval  (pCol,pTknVec);  break;
      case CT_TIME:      eError = eExtractTime      (pCol,pTknVec);  break;
      case CT_TIMESTAMP: eError = eExtractTimestamp (pCol,pTknVec);  break;
      case CT_VARCHAR:   eError = eExtractVarchar   (pCol,pTknVec);  break;
        
      default:
        if (pTkn->bSame("char"_Stz) ||
            pTkn->bSame("varbinary"_Stz)) {
          eError = eExtractVarchar(pCol,pTknVec);
        }
        break;
        
    }
    
    if (EC_NONE != eError)
      goto L_RETURN;
 
    
    // -- -- --
    // not null
    // -- -- --

    pTkn = pTknVec->pNextToken(); PT();
    if (pTkn->bSame("not"_Stz)) {
      pTkn = pTknVec->pNextToken(); PT();
      if (! pTkn->bSame("null"_Stz))
        return EC_INVALID_COMMAND;
      pCol->vIsNullable(false);
      
      pTkn = pTknVec->pNextToken(); PT();
    }
    
    // check for PRIMARY KEY
//    if (m_xTokenVec[i_Tkn]->bSame("primary"_Stz)) {
//
//      pJob->pCol->m_sPrimaryKey = m_xTokenVec[i_Tkn++]->aText();
//
//      if (!m_xTokenVec[i_Tkn]->bSame("key"_Stz)) {
//        m_pSession->vSetLastError(EC_INVALID_COMMAND);
//        return false;
//      }
//    }
    
    // check for UNIQUE
//    if (pTkn->bSame("unique"_Stz)) {
//      pCol->vIsUnique(true);
//
//      pTkn = pTknVec->pNextToken(); PT();
//    }
    if (i_Col == 0)
      pCol->vIsPtnKey(true);
    m_pStreamDef->m_xaColumn[i_Col].vCopy(pCol);
    m_pStreamDef->m_nDefColumns[0]++;
    
   
  }
  
//  // check if we have a partition by clause
//  if ( iCurrentToken < iNumTokens && pTokenVec->pGet(iCurrentToken++)->bSame("partition"_Stz)) {
//
//    if ( ! bPartitionTable(pTokenVec, iCurrentToken) )
//      return EC_INVALID_COMMAND;
//  }
//  else {
//    // no partition by clause
//    // we will create partitioning using the first column
//    m_xColumns[0].vIsPtnKey(true);
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_RETURN:
  
  delete pTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract boolean                                                            */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractBoolean(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_BOOLEAN);
  pCol->vDatumType(DM_BOL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract date                                                               */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractDate(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_DATE);
  pCol->vDatumType(DM_DTE);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract decimal                                                            */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractDecimal(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default precision and scale
  i8 iPrecision = C_MAX_PRECISION_BCD;
  i8 iScale = 0;
  
  // set column type in column
  pCol->vColumnType(CT_DECIMAL);
  pCol->vDatumType(DM_BCD);
  
  Token* pTkn = pTknVec->pNextToken(); PT();

  // extract scale and precison if specified
  if (pTkn->bSame("("_Stz)) {
    
    pTkn = pTknVec->pNextToken(); PT();
    
    // check if it was numeric
    if ( ! pTkn->bIsNumeric() )
      return EC_INVALID_COMMAND;
    
    iPrecision = pTkn->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    
    if (!pTkn->bSame(","_Stz))
      return EC_INVALID_COMMAND;
    
    pTkn = pTknVec->pNextToken(); PT();
    
    // check if it was numeric
    if ( ! pTkn->bIsNumeric() )
      return EC_INVALID_COMMAND;
    
    // get the scale
    iScale = pTkn->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    if (! pTkn->bSame(")"_Stz))
      return EC_INVALID_COMMAND;
    
    if (! Column::bValidBcd(iPrecision, iScale))
      return EC_INVALID_COMMAND;
  }
  
  // set scale and length in column
  pCol->vColumnLength(iPrecision);
  pCol->vColumnScale(iScale);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract float                                                              */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractFloat(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default size
  i8 iSize = C_MAX_PRECISION_FLT;
  
  // set column type
  pCol->vColumnType(CT_FLOAT);
  pCol->vDatumType(DM_FLT);
  
  Token* pTkn = pTknVec->pNextToken(); PT();

  // extract size if specified
  if (pTkn->bSame("("_Stz)) {
    
    pTkn = pTknVec->pNextToken(); PT();

    // check if it was numeric
    if ( ! pTkn->bIsNumeric() ) {
      return EC_INVALID_COMMAND;
    }
    
    // get size
    iSize = pTknVec->pNextToken()->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    if (!pTkn->bSame(")"_Stz)) {
      return EC_INVALID_COMMAND;
    }
    
    if (! Column::bValidFlt(iSize)) {
      return EC_INVALID_COMMAND;
    }
  }
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract integer                                                            */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractInteger(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_INTEGER);
  pCol->vDatumType(DM_INT);
  pCol->vColumnLength(C_MAX_PRECISION_INT);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract interval                                                           */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractInterval(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_INTERVAL);
  pCol->vDatumType(DM_IVL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract time                                                               */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractTime(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_TIME);
  pCol->vDatumType(DM_TME);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract timestamp                                                          */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractTimestamp(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_TIMESTAMP);
  pCol->vDatumType(DM_TSP);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* extract varchar                                                            */
/*----------------------------------------------------------------------------*/
eEC TableDef::eExtractVarchar(Column* pCol, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default size
  i8 iSize = C_64K;
  
  // set column type
  pCol->vColumnType(CT_VARCHAR);
  pCol->vDatumType(DM_VCH);
  
  Token* pTkn = pTknVec->pNextToken(); PT();

  // extract size if specified
  if (pTkn->bSame("("_Stz)) {
    
    //pCol->vColumnLength(pTknVec->pNextToken()->iToI8());
    
    pTkn = pTknVec->pNextToken(); PT();

    // check if it was numeric
    if (! pTkn->bIsNumeric())
      return EC_INVALID_COMMAND;
    
    // get size
    iSize = pTkn->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    
    if (! pTkn->bSame(")"_Stz))
      return EC_INVALID_COMMAND;
    
    // valid the size
    if (! Column::bValidVch(iSize))
      return EC_INVALID_COMMAND;
  }
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return EC_NONE;
}

#undef PT


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TableDef)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
