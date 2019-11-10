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
void TableDef::operator delete(void*)
{
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
eCT TableDef::eToCT(Stz* szType)
{
  i8 nEntries = sizeof(aCT) / sizeof (Stz*);
  
  for (i8 i_Type = 0; i_Type < nEntries ; i_Type++) {
    
    if ( aCT[i_Type]->bSame(szType) )
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
bool TableDef::bParse(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // retain the Create Table command
  m_sCmd = szCmd;
  
  // tokenise the command
  TokenVec* pTknVec = Tokeniser::pTokenise(szCmd);
  
  // assert we got a create table
  ASSERT(pTknVec->pFirstToken()->bSame("create"_Stz));
  ASSERT(pTknVec->pNextToken()->bSame("table"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_Tkn = 2;
  i8 i_Col = 0;
  i8 i_StartToken = C_UNKNOWN;

  // table name -- -- -- -- -- --
  Token* pTkn = pTknVec->pNextToken(); PT();

  m_sTableNm = pTkn->szText();

  // open parenth ( -- -- -- -- -- --
  pTkn = pTknVec->pNextToken(); PT();

  if (! pTkn->bSame("("_Stz))
    goto L_INVALID;
  
  
  // column name -- -- -- -- -- --
  pTkn = pTknVec->pNextToken(); PT();
  
  i_StartToken = pTknVec->iCurrToken();

  while ( ! pTkn->bSame(")"_Stz)) {
    
    if (i_Col >= C_MAX_COLUMNS)
      return EC_MAX_COLUMNS;
    
    if (pTknVec->iCurrToken() != i_StartToken) {
      if (pTkn->bSame(","_Stz)) {
        // column name -- -- -- -- -- --
        pTkn = pTknVec->pNextToken(); PT();
      }
      else
        return EC_INVALID_COMMAND;
    }
    
    Column* pCol = new Column;
    m_pColVec->vAdd(pCol);

    // set column number
    pCol->vColumnNo(i_Col++);
    
    // set column name
    pCol->vColumnName(pTkn->szText());
    
    // check if column name already added
    for (i8 i_Search = 0; i_Search < i_Col - 1; i_Search++) {
      Column* pCol_search = m_pColVec->pGet(i_Search++);
      if (pCol_search->sColumnName().bSame(pCol->sColumnName()))
        return EC_DUPLICATE_COLUMN;
    }
    
    // column type -- -- -- -- -- --
    pTkn = pTknVec->pNextToken(); PT();
    
    eCT eColumn = eToCT(pTkn->szText());
    
    switch (eColumn) {
      case CT_BOOLEAN:
        pCol->vColumnType(CT_BOOLEAN);
        pCol->vDatumType(DM_BOL);
        break;
        
      case CT_DATE:
        pCol->vColumnType(CT_DATE);
        pCol->vDatumType(DM_DTE);
        break;
        
      case CT_DECIMAL:
        if ( ! bExtractDecimal(pCol,pTknVec) )
          return EC_INVALID_COMMAND;
        break;
        
      case CT_FLOAT:
        if ( ! bExtractFloat(pCol,pTknVec) )
          return EC_INVALID_COMMAND;
        break;
        
      case CT_INTEGER:
        pCol->vColumnType(CT_INTEGER);
        pCol->vDatumType(DM_INT);
        pCol->vColumnLength(C_MAX_PRECISION_INT);
        break;
        
      case CT_INTERVAL:
        pCol->vColumnType(CT_INTERVAL);
        pCol->vDatumType(DM_IVL);
        break;
        
      case CT_TIME:
        pCol->vColumnType(CT_TIME);
        pCol->vDatumType(DM_TME);
        break;
        
      case CT_TIMESTAMP:
        pCol->vColumnType(CT_TIMESTAMP);
        pCol->vDatumType(DM_TSP);
        break;
        
      case CT_VARCHAR:
        if ( ! bExtractVarchar(pCol, pTknVec) )
          return EC_INVALID_COMMAND;
        break;
        
      default:
        if (pTkn->bSame("varbinary"_Stz) ||
            pTkn->bSame("char"_Stz)) {
          if ( ! bExtractVarchar(pCol, pTknVec) )
            return EC_INVALID_COMMAND;
        }
        break;
        
    }
 
    /*
    if (pTkn->bSame("boolean"_Stz)) {
      pCol->vColumnType(CT_BOOLEAN);
      pCol->vDatumType(DM_BOL);
    }
    
    else if (pTkn->bSame("date"_Stz)) {
      pCol->vColumnType(CT_DATE);
      pCol->vDatumType(DM_DTE);
    }
    
    else if (pTkn->bSame("decimal"_Stz)) {
      if ( ! bExtractDecimal(pCol,pTknVec) )
        return EC_INVALID_COMMAND;
    }
    
    else if (pTkn->bSame("float"_Stz)) {
      if ( ! bExtractFloat(pCol,pTknVec) )
        return EC_INVALID_COMMAND;
    }
    
    else if (pTkn->bSame("integer"_Stz)) {
      pCol->vColumnType(CT_INTEGER);
      pCol->vDatumType(DM_INT);
      pCol->vColumnLength(C_MAX_PRECISION_INT);
    }
    
    else if (pTkn->bSame("interval"_Stz)) {
      pCol->vColumnType(CT_INTERVAL);
      pCol->vDatumType(DM_IVL);
    }
    
    else if (pTkn->bSame("time"_Stz)) {
      pCol->vColumnType(CT_TIME);
      pCol->vDatumType(DM_TME);
    }
    
    else if (pTkn->bSame("timestamp"_Stz)) {
      pCol->vColumnType(CT_TIMESTAMP);
      pCol->vDatumType(DM_TSP);
     }
    
    else if (pTkn->bSame("varchar"_Stz)   ||
             pTkn->bSame("varbinary"_Stz) ||
             pTkn->bSame("char"_Stz)) {
      if ( ! bExtractVarchar(pCol, pTknVec) )
        return EC_INVALID_COMMAND;
    }
    
    else {
      return EC_INVALID_COMMAND;
    }
    */
    
    
    pTkn = pTknVec->pNextToken(); PT();

    // check for NOT NULL  -- -- -- -- -- --
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

    
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pTknVec;
  
  return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_INVALID:
  delete pTknVec;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract float                                                              */
/*----------------------------------------------------------------------------*/
bool TableDef::bExtractDecimal(Column* pCol, TokenVec* pTknVec)
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
      return false;
    
    iPrecision = pTkn->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    
    if (!pTkn->bSame(","_Stz))
      return false;
    
    pTkn = pTknVec->pNextToken(); PT();
    
    // check if it was numeric
    if ( ! pTkn->bIsNumeric() )
      return false;
    
    // get the scale
    iScale = pTkn->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    if (! pTkn->bSame(")"_Stz))
      return false;
    
    if (! Column::bValidBcd(iPrecision, iScale))
      return false;
  }
  
  // set scale and length in column
  pCol->vColumnLength(iPrecision);
  pCol->vColumnScale(iScale);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract float                                                              */
/*----------------------------------------------------------------------------*/
bool TableDef::bExtractFloat(Column* pCol, TokenVec* pTknVec)
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
      return false;
    }
    
    // get size
    iSize = pTknVec->pNextToken()->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    if (!pTkn->bSame(")"_Stz)) {
      return false;
    }
    
    if (! Column::bValidFlt(iSize)) {
      return false;
    }
  }
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract varchar                                                            */
/*----------------------------------------------------------------------------*/
bool TableDef::bExtractVarchar(Column* pCol, TokenVec* pTknVec)
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
      return false;
    
    // get size
    iSize = pTkn->iToI8();
    
    pTkn = pTknVec->pNextToken(); PT();
    
    if (! pTkn->bSame(")"_Stz))
      return false;
    
    // valid the size
    if (! Column::bValidVch(iSize))
      return false;
  }
  
  // set size in column
  pCol->vColumnLength(iSize);
  
  return true;
}

#undef PT

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TableDef)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
