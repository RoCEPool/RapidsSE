/*============================================================================*/
/* TableDef.cpp -- Table Definition                                           */
/*============================================================================*/

#include "MetaData.h"



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
  
  m_sTableNm    = C_NO_NAME;
  m_eTableType  = TT_NORMAL;
  m_bDefinitionOnly = false;
  
  m_pStreamDef = new StreamDef();
  
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

//#define dPT() vTrace("tkn=%s\n"_Stz,pTkn->szText());
#define dPT()

/*----------------------------------------------------------------------------*/
/* parse Create Table command                                                 */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParse(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
 
  eEC eError = EC_NONE;
  i8 i_Col = C_UNKNOWN;
  Column* pLastCol = nullptr;
  
  // retain the Create Table command
  m_sCmd = szCmd;
  
  // tokenise the command
  TokenVec* pTknVec = TokenVec::pTokenise(szCmd);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: Create Table <tableNm>
  
  // -- -- -- -- -- -- -- -- -- --
  // assert we got a create table
  // -- -- -- -- -- -- -- -- -- --

  Token* pTkn = pTknVec->pFirstToken(); dPT();
  ASSERT(pTkn->bSame("create"_Stz) || pTkn->bSame("define"_Stz) );

  // -- -- -- -- -- --
  // table definition
  // -- -- -- -- -- --
  if (pTkn->bSame("define"_Stz)) {
    m_bDefinitionOnly = true;
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  ASSERT(pTkn->bSame("table"_Stz));
  
  // -- -- -- --
  // table name
  // -- -- -- --

  pTkn = pTknVec->pNextToken(); dPT();
  m_sTableNm = pTkn->szChars();
  m_pStreamDef->vName(m_sTableNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: ( <columnDef> ,... )
  
  // -- -- -- -- --
  // open parenth (
  // -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (! pTkn->bSame("("_Stz)) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // create the streamDef for this table def
  m_pStreamDef->m_nDefSets = 1;
  m_pStreamDef->m_i_DefStart[0] = 0;
  m_pStreamDef->m_nDefColumns[0] = 0;
  
  // -- -- -- -- -- -- -- -- -- -- --
  // iterate from first column till )
  // -- -- -- -- -- -- -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  for (i_Col = 0; ! pTkn->bSame(")"_Stz); i_Col++) {
    
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
        pTkn = pTknVec->pNextToken(); dPT();
      }
      else {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }
    }
    
    // -- -- -- -- --
    // parse column
    // -- -- -- -- --
    eError = eParseColumn(pTknVec,i_Col);
    if (EC_NONE != eError) {
      goto L_RETURN;
    }
    pTkn = pTknVec->pCurrToken(); dPT();
  }
  
  // did we get any columns ?
  if (i_Col == 0) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // mark the last column
  pLastCol = m_pStreamDef->pGetColumn(i_Col-1);
  pLastCol->vIsLastColumn(true);
  
  eError = eParseTableClauses(pTknVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_RETURN:
  
  delete pTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* check the syntax of the Create Table command                               */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntax(Stz* szCmd)
{
  eEC eError = EC_NONE;
  i8 i_Col = C_UNKNOWN;
  
  // tokenise the command
  TokenVec* pTknVec = TokenVec::pTokenise(szCmd);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: Create Table <tableNm>
  
  // -- -- -- -- -- -- -- -- -- --
  // assert we got a create table
  // -- -- -- -- -- -- -- -- -- --
  
  Token* pTkn = pTknVec->pFirstToken(); dPT();
  ASSERT(pTkn->bSame("create"_Stz) || pTkn->bSame("define"_Stz) );
  
  pTkn = pTknVec->pNextToken(); dPT();
  ASSERT(pTkn->bSame("table"_Stz));
  
  // -- -- -- -- -- --
  // table definition
  // -- -- -- -- -- --
  pTkn = pTknVec->pNextToken(); dPT();
  if (pTkn->bSame("definition"_Stz)) {
    pTkn = pTknVec->pNextToken(); dPT();
  }
  
  // -- -- -- --
  // table name
  // -- -- -- --
  
  //pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: ( <columnDef> ,... )
  
  // -- -- -- -- --
  // open parenth (
  // -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (! pTkn->bSame("("_Stz)) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // -- -- -- -- -- -- -- -- -- -- --
  // iterate from first column till )
  // -- -- -- -- -- -- -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  for (i_Col = 0; ! pTkn->bSame(")"_Stz); i_Col++) {
    
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
        pTkn = pTknVec->pNextToken(); dPT();
        if ( ! pTkn ) {
          eError = EC_INVALID_COMMAND;
          goto L_RETURN;
        }
      }
      else {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }
    }
    
    // -- -- -- -- --
    // parse column
    // -- -- -- -- --
    eError = eSyntaxColumn(pTknVec);
    if (EC_NONE != eError) {
      goto L_RETURN;
    }
    pTkn = pTknVec->pCurrToken(); dPT();
  }
  
  // did we get any columns ?
  if (i_Col == 0) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // mark the last column
  eError = eSyntaxTableClauses(pTknVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
L_RETURN:
  
  delete pTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse column                                                               */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumn(TokenVec* pTknVec, i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  eEC eError = EC_NONE;
  
  Token* pTkn = pTknVec->pCurrToken(); dPT();
  // -- -- -- --
  
  // create column
  // -- -- -- --
  Column* pCol = m_pStreamDef->pGetColumn(i_Col);
  
  // set column number
  pCol->vColumnNo(i_Col);
  
  // set column name
  pCol->vColumnNm(pTkn->szChars());
  
  // -- -- -- -- -- -- -- -- -- -- --
  // check for duplicate column name
  // -- -- -- -- -- -- -- -- -- -- --
  
  for (i8 i_Search = 0; i_Search < i_Col; i_Search++) {
    Column* pCol_search = m_pStreamDef->pGetColumn(i_Search++);
    if (pCol_search->szColNm()->bSame(pCol->szColNm())) {
      eError = EC_DUPLICATE_COLUMN;
      return eError;
    }
  }
  
  // -- -- -- --
  // column type
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  eCT eColumn = TableDef::eColumn(pTkn->szChars());
  
  switch (eColumn) {
      
    case CT_BOOLEAN:   eError = eParseColumnBoolean   (pTknVec,pCol);  break;
    case CT_DATE:      eError = eParseColumnDate      (pTknVec,pCol);  break;
    case CT_DECIMAL:   eError = eParseColumnDecimal   (pTknVec,pCol);  break;
    case CT_FLOAT:     eError = eParseColumnFloat     (pTknVec,pCol);  break;
    case CT_INTEGER:   eError = eParseColumnInteger   (pTknVec,pCol);  break;
    case CT_INTERVAL:  eError = eParseColumnInterval  (pTknVec,pCol);  break;
    case CT_TIME:      eError = eParseColumnTime      (pTknVec,pCol);  break;
    case CT_TIMESTAMP: eError = eParseColumnTimestamp (pTknVec,pCol);  break;
    case CT_VARCHAR:   eError = eParseColumnVarchar   (pTknVec,pCol);  break;
      
    default:
      if (pTkn->bSame("char"_Stz) ||
          pTkn->bSame("varbinary"_Stz)) {
        eError = eParseColumnVarchar(pTknVec,pCol);
      }
      else
        eError = EC_INVALID_COMMAND;
      break;
      
  }
  
  if (EC_NONE != eError)
    return eError;
  
  eError = eParseColumnClauses(pTknVec,pCol);
  if (EC_NONE != eError)
    return eError;

  m_pStreamDef->m_nDefColumns[0]++;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse boolean column                                                       */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnBoolean(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_BOOLEAN);
  pCol->vDatum(DM_BOL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse date column                                                          */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnDate(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_DATE);
  pCol->vDatum(DM_DTE);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse decimal column                                                       */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnDecimal(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default precision and scale
  i8 iPrecision = C_MAX_BCD_PRECISION;
  i8 iScale = 0;
  
  // set column type in column
  pCol->vColumnType(CT_DECIMAL);
  pCol->vDatum(DM_BCD);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();

  // extract scale and precison if specified
  if (! pTkn->bSame("("_Stz)) {
    pCol->vColumnLength(iPrecision);
    pCol->vColumnScale(iScale);
    
    IGNORE_RETURN(pTknVec->pPrevToken());
    
    return EC_NONE;
  }
    
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() )
    return EC_INVALID_COMMAND;
  
  iPrecision = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  if (!pTkn->bSame(","_Stz))
    return EC_INVALID_COMMAND;
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() )
    return EC_INVALID_COMMAND;
  
  // get the scale
  iScale = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (! pTkn->bSame(")"_Stz))
    return EC_INVALID_COMMAND;
  
  if (! Column::bValidBcd(iPrecision, iScale))
    return EC_INVALID_COMMAND;

  // set scale and length in column
  pCol->vColumnLength(iPrecision);
  pCol->vColumnScale(iScale);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse float column                                                         */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnFloat(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default size
  i8 iSize = C_MAX_FLT_PRECISION;
  
  // set column type
  pCol->vColumnType(CT_FLOAT);
  pCol->vDatum(DM_FLT);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();

  // extract size if specified
  if (! pTkn->bSame("("_Stz)) {
    // set size in column
    pCol->vColumnLength(iSize);
    
    IGNORE_RETURN(pTknVec->pPrevToken());
    
    return EC_NONE;
  }
    
  pTkn = pTknVec->pNextToken(); dPT();

  // check if it was numeric
  if ( ! pTkn->bIsNumeric() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  iSize = pTknVec->pNextToken()->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if (!pTkn->bSame(")"_Stz)) {
    return EC_INVALID_COMMAND;
  }
  
  if (! Column::bValidFlt(iSize)) {
    return EC_INVALID_COMMAND;
  }
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse integer column                                                       */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnInteger(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_INTEGER);
  pCol->vDatum(DM_INT);
  pCol->vColumnLength(C_MAX_INT_PRECISION);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse interval column                                                      */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnInterval(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_INTERVAL);
  pCol->vDatum(DM_IVL);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse time column                                                          */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnTime(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_TIME);
  pCol->vDatum(DM_TME);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse timestamp column                                                     */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnTimestamp(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vColumnType(CT_TIMESTAMP);
  pCol->vDatum(DM_TSP);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse varchar column                                                       */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnVarchar(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default size
  i8 iSize = C_64K;
  
  // set column type
  pCol->vColumnType(CT_VARCHAR);
  pCol->vDatum(DM_VCH);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();

  // extract size if specified
  if (! pTkn->bSame("("_Stz)) {
    // set size in column
    pCol->vColumnLength(iSize);
    
    IGNORE_RETURN(pTknVec->pPrevToken());
    
    return EC_NONE;
  }
    
  //pCol->vColumnLength(pTknVec->pNextToken()->iToI8());
  
  pTkn = pTknVec->pNextToken(); dPT();

  // check if it was numeric
  if (! pTkn->bIsNumeric())
    return EC_INVALID_COMMAND;
  
  // get size
  iSize = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  if (! pTkn->bSame(")"_Stz))
    return EC_INVALID_COMMAND;
  
  // valid the size
  if (! Column::bValidVch(iSize))
    return EC_INVALID_COMMAND;
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse column clause                                                        */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseColumnClauses(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
  
    if (pTkn->bSame("asc"_Stz)) {
      
      eError = eParseAsc(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("desc"_Stz)) {
      
      eError = eParseDesc(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("fixed"_Stz)) {
      
      eError = eParseFixed(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }

    if (pTkn->bSame("max"_Stz)) {
      
      eError = eParseMax(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }

    if (pTkn->bSame("min"_Stz)) {
      
      eError = eParseMin(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if (pTkn->bSame("not"_Stz)) {
    
      eError = eParseNotNull(pTknVec,pCol);
      if (EC_NONE != eError) {
        pCol->vIsNullable(false);
        return eError;
      }

      continue;
    }
  
    if (pTkn->bSame("random"_Stz)) {
      
      eError = eParseRandom(pTknVec,pCol);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    break;
  }
  
  
  //!
  //! ??? discuss with Alka
  //!
  
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
  //      pTkn = pTknVec->pNextToken(); dPT();
  //    }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse asc column clause                                                    */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseAsc(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vGeneration(DG_ASCENDING);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse desc column clause                                                   */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseDesc(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vGeneration(DG_DESCENDING);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse fixed column clause                                                  */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseFixed(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vGeneration(DG_FIXED);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse max column clause                                                    */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseMax(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();

  Datum_h* pDatum_max = pCol->pMaxVal();

  DatumToken::vAssign(pDatum_max,pTkn,pCol);
  
//  if (!b)
//    return EC_INVALID_COLUMN;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse min column clause                                                    */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseMin(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  Datum_h* pDatum_min = pCol->pMinVal();

  DatumToken::vAssign(pDatum_min,pTkn,pCol);

//  if (!b)
//    return EC_INVALID_COLUMN;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse not null column clause                                               */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseNotNull(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  if (! pTkn->bSame(C_NULL))
    return EC_INVALID_COMMAND;
  
  pCol->vIsNullable(false);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse random column clause                                                 */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseRandom(TokenVec* pTknVec, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  pCol->vGeneration(DG_RANDOM);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse table clauses                                                        */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseTableClauses(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);

  bool bSawPartition = false;
  
  eEC eError = EC_NONE;
  
  for (;;) {
  
    Token* pTkn = pTknVec->pNextToken(); dPT();
    
    if ( pTkn->bSame("rowsize"_Stz)) {
      
      eError = eParseRowSize(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      continue;
    }
    
    if ( pTkn->bSame("partition"_Stz)) {
      
      eError = eParsePartitionBy(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      bSawPartition = true;
      
      continue;
    }
    
    break;
  }
  
  // if no partition by clause was specified
  //   we create default partitioning using the first column

  if (! bSawPartition)
    m_pStreamDef->pGetColumn(0)->vIsPtnKey(true);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse partition clause                                                     */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParsePartitionBy(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // next token should be 'by'
  Token* pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn->bSame("by"_Stz) ) {
    return EC_INVALID_COMMAND;
  }
  
  // next token should be '('
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn->bSame("("_Stz)) {
    return EC_INVALID_COMMAND;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  
  i8 iPartKeys = 0;
  bool bFound = false;
  
  pTkn = pTknVec->pNextToken(); dPT();
  while ( ! pTkn->bSame(")"_Stz) ) {
    
    iPartKeys++;
    
    // find column and set as partition column
    for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
      Column* pCol = m_pStreamDef->pGetColumn(i_Col);
      if (pCol->szColNm()->bSame(pTkn->szChars())) {
        pCol->vIsPtnKey(true);
        bFound = true;
        break;
      }
    }
    if ( ! bFound) {
      return EC_INVALID_PARTITION_KEY;
    }
    pTkn = pTknVec->pNextToken(); dPT();
    if ( ! pTkn->bSame(","_Stz)) {
      continue;
    }
    pTkn = pTknVec->pNextToken(); dPT();
    bFound = false;
  }
  
  if ( ! iPartKeys ) {
    return EC_INVALID_COMMAND;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse rowsize clause                                                       */
/*----------------------------------------------------------------------------*/
eEC TableDef::eParseRowSize(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // next token should be integer
  Token* pTkn = pTknVec->pNextToken(); dPT();
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  i8 nRowSize = pTkn->iToI8();

  m_pStreamDef->m_nFixedSized = nRowSize;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check column syntax                                                        */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxColumn(TokenVec* pTknVec)
{
   eEC eError = EC_NONE;
  
  // -- -- -- --
  // column name
  // -- -- -- --
  Token* pTkn = pTknVec->pCurrToken(); dPT();
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    return eError;
  }
  
  // -- -- -- --
  // column type
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    return eError;
  }
  
  eCT eColumn = TableDef::eColumn(pTkn->szChars());
  
  switch (eColumn) {
      
    case CT_BOOLEAN: break;
    case CT_DATE:    break;
    case CT_DECIMAL:
      eError = eSyntaxColumnDecimal(pTknVec);
      break;
    case CT_FLOAT:
      eError = eSyntaxColumnFloat(pTknVec);
      break;
    case CT_INTEGER:  break;
    case CT_INTERVAL: break;
    case CT_TIME:     break;
    case CT_TIMESTAMP: break;
    case CT_VARCHAR:
      eError = eSyntaxColumnVarchar(pTknVec);
      break;
      
    default:
      if (pTkn->bSame("char"_Stz))
         eError = eSyntaxColumnVarchar(pTknVec);
      else {
        if (! pTkn->bSame("varbinary"_Stz))
          eError = EC_INVALID_COMMAND;
      }
      break;
  }
  
  if (EC_NONE != eError)
    return eError;
  
  eError = eSyntaxColumnClauses(pTknVec);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* check syntax of column clause                                              */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxColumnClauses(TokenVec* pTknVec)
{
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
    
    if (pTkn->bSame("asc"_Stz)) {
      continue;
    }
    
    if (pTkn->bSame("desc"_Stz)) {
      continue;
    }
    
    if (pTkn->bSame("fixed"_Stz)) {
      continue;
    }
    
    if (pTkn->bSame("max"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if ( ! pTkn ) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      continue;
    }
    
    if (pTkn->bSame("min"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if ( ! pTkn ) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      continue;
    }
    
    if (pTkn->bSame("not"_Stz)) {
      pTkn = pTknVec->pNextToken(); dPT();
      if ( ! pTkn ) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      
      if (! pTkn->bSame(C_NULL)){
        eError = EC_INVALID_COMMAND;
        break;
      }
      
      continue;
    }
    
    if (pTkn->bSame("random"_Stz)) {
      continue;
    }
    
    break;
  }
  
  
  //!
  //! ??? discuss with Alka
  //!
  
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
  //      pTkn = pTknVec->pNextToken(); dPT();
  //    }
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* check syntax of decimal column                                             */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxColumnDecimal(TokenVec* pTknVec)
{
  Token* pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // extract scale and precison if specified
  if (! pTkn->bSame("("_Stz)) {
    IGNORE_RETURN(pTknVec->pPrevToken());
    return EC_NONE;
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() )
    return EC_INVALID_COMMAND;
  
  i8 iPrecision = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  if (!pTkn->bSame(","_Stz))
    return EC_INVALID_COMMAND;
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() )
    return EC_INVALID_COMMAND;
  
  // get the scale
  i8 iScale = pTkn->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  if (! pTkn->bSame(")"_Stz))
    return EC_INVALID_COMMAND;
  
  if (! Column::bValidBcd(iPrecision, iScale))
    return EC_INVALID_COMMAND;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check syntax of float column                                               */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxColumnFloat(TokenVec* pTknVec)
{
  Token* pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // extract size if specified
  if (! pTkn->bSame("("_Stz)) {
    IGNORE_RETURN(pTknVec->pPrevToken());
    return EC_NONE;
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // check if it was numeric
  if ( ! pTkn->bIsNumeric() ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  i8 iSize = pTknVec->pNextToken()->iToI8();
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  if (!pTkn->bSame(")"_Stz)) {
    return EC_INVALID_COMMAND;
  }
  
  if (! Column::bValidFlt(iSize)) {
    return EC_INVALID_COMMAND;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check syntax of a varchar column                                           */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxColumnVarchar(TokenVec* pTknVec)
{
  
  Token* pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // is size specified ?
  if (! pTkn->bSame("("_Stz)) {
    IGNORE_RETURN(pTknVec->pPrevToken());
    return EC_NONE;
  }
  
  //pCol->vColumnLength(pTknVec->pNextToken()->iToI8());
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // check if it was numeric
  if (! pTkn->bIsNumeric())
    return EC_INVALID_COMMAND;
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  // get size
  i8 iSize = pTkn->iToI8();
  
  if (! pTkn->bSame(")"_Stz))
    return EC_INVALID_COMMAND;
  
  // valid the size
  if (! Column::bValidVch(iSize))
    return EC_INVALID_COMMAND;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check sytax of partition clause                                            */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxPartitionBy(TokenVec* pTknVec)
{
  // next token should be 'by'
  Token* pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  if ( ! pTkn->bSame("by"_Stz) ) {
    return EC_INVALID_COMMAND;
  }
  
  // next token should be '('
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  if ( ! pTkn->bSame("("_Stz)) {
    return EC_INVALID_COMMAND;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  
  i8 iPartKeys = 0;
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  while ( ! pTkn->bSame(")"_Stz) ) {
    
    iPartKeys++;
    
    pTkn = pTknVec->pNextToken(); dPT();
    if ( ! pTkn ) {
      return EC_INVALID_COMMAND;
    }
    if ( ! pTkn->bSame(","_Stz)) {
      continue;
    }
    pTkn = pTknVec->pNextToken(); dPT();
    if ( ! pTkn ) {
      return EC_INVALID_COMMAND;
    }
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    return EC_INVALID_COMMAND;
  }
  
  if ( ! pTkn->bSame(";"_Stz) ) {
    return EC_INVALID_COMMAND;
  }
  
  if ( ! iPartKeys ) {
    return EC_INVALID_COMMAND;
  }
  
  IGNORE_RETURN(pTknVec->pPrevToken());
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check syntax of table clauses                                              */
/*----------------------------------------------------------------------------*/
eEC TableDef::eSyntaxTableClauses(TokenVec* pTknVec)
{
  
  bool bSawPartition = false;
  
  eEC eError = EC_NONE;
  
  for (;;) {
    
    Token* pTkn = pTknVec->pNextToken(); dPT();
    
    if ( pTkn->bSame("rowsize"_Stz)) {
      
      // next token should be integer
      Token* pTkn = pTknVec->pNextToken(); dPT();
      if ( ! pTkn ) {
        return EC_INVALID_COMMAND;
      }
      if ( ! pTkn->bIsNumeric() ) {
        return EC_INVALID_COMMAND;
      }
      
      continue;
    }
    
    if ( pTkn->bSame("partition"_Stz)) {
      
      eError = eSyntaxPartitionBy(pTknVec);
      if (EC_NONE != eError)
        return eError;
      
      bSawPartition = true;
      
      continue;
    }
    
    break;
  }
  
  return EC_NONE;
}

#undef dPT


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TableDef)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
