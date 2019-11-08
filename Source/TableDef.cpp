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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  ASSERT(pTknVec->pGet(0)->bSame("create"_Stz));
  ASSERT(pTknVec->pGet(1)->bSame("table"_Stz));

  return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_Tkn = 2;
  i8 i_Col = 0;
  i8 i_StartToken = i_Tkn;

  Token* pTkn = pTknVec->pGet(i_Tkn++);

  if (! pTkn->bSame("("_Stz))
    goto L_INVALID;
  
  
  pTkn = pTknVec->pGet(i_Tkn++);
  
  while ( ! pTkn->bSame(")"_Stz)) {
    
    if (i_Col >= C_MAX_COLUMNS) {
      return EC_MAX_COLUMNS;
    }
    
    Token* pTkn = pTknVec->pGet(i_Tkn++);
    
    Column* pCol = m_pColVec->pGet(i_Col++);

    if (i_Tkn != i_StartToken && ! pTkn->bSame(","_Stz)) {
      return EC_INVALID_COMMAND;
    }
    
    
    // set column number
    pCol->vColumnNo(i_Col);
    
    // set column name
    pCol->vColumnName(pTkn->szText());
    
    // check if column name already added
    for (i8 i_Search = 0; i_Search < i_Col; i_Search++) {
      Column* pCol_search = m_pColVec->pGet(i_Search++);
      if (pCol_search->sColumnName().bSame(pCol->sColumnName()))
        return EC_DUPLICATE_COLUMN;
    }
    
    if (pTkn->bSame("varchar"_Stz) ||
        pTkn->bSame("varbinary"_Stz) ||
        pTkn->bSame("char"_Stz)) {
      
      // extract varchar
      if ( ! bExtractVarchar(pTknVec, i_Col, i_Tkn) )
        return EC_INVALID_COMMAND;
    }
    else if (pTkn->bSame("float"_Stz)) {
      
      // extract float
      if ( ! bExtractFloat(pTknVec, i_Col, i_Tkn) )
        return EC_INVALID_COMMAND;
    }
    else if (pTkn->bSame("decimal"_Stz)) {
      
      // extract decimal
      if ( ! bExtractDecimal(pTknVec, i_Col, i_Tkn) )
        return EC_INVALID_COMMAND;
    }
    else if (pTkn->bSame("integer"_Stz)) {
      pCol->vColumnType(CT_INTEGER);
      pCol->vDatumType(DM_INT);
      pCol->vColumnLength(C_MAX_PRECISION_INT);
      i_Tkn++;
    }
    else if (pTkn->bSame("date"_Stz)) {
      pCol->vColumnType(CT_DATE);
      pCol->vDatumType(DM_DTE);
      i_Tkn++;
    }
    
    else if (pTkn->bSame("boolean"_Stz)) {
      pCol->vColumnType(CT_BOOLEAN);
      pCol->vDatumType(DM_BOL);
      i_Tkn++;
    }
    
    else if (pTkn->bSame("interval"_Stz)) {
      pCol->vColumnType(CT_INTERVAL);
      pCol->vDatumType(DM_IVL);
      i_Tkn++;
    }
    
    else if (pTkn->bSame("time"_Stz)) {
      pCol->vColumnType(CT_TIME);
      pCol->vDatumType(DM_TME);
      i_Tkn++;
    }
    
    else if (pTkn->bSame("timestamp"_Stz)) {
      pCol->vColumnType(CT_TIMESTAMP);
      pCol->vDatumType(DM_TSP);
      i_Tkn++;
    }
    
    else {
      return EC_INVALID_COMMAND;
    }
    
    // check for NOT NULL
    if (pTkn->bSame("not"_Stz)) {
      i_Tkn++;
      if (!pTknVec->pGet(i_Tkn++)->bSame("null"_Stz)) {
        return EC_INVALID_COMMAND;
      }
      pCol->vIsNullable(false);
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
    if (pTkn->bSame("unique"_Stz)) {
      i_Tkn++;
      pCol->vIsUnique(true);
    }

    
    pTkn = pTknVec->pGet(i_Tkn++);
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
bool TableDef::bExtractDecimal(TokenVec* pTknVec, i8 i_Col, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default precision and scale
  i8 iPrecision = C_MAX_PRECISION_BCD;
  i8 iScale = 0;
  
  // set column type in column
  m_pColVec->pGet(i_Col)->vColumnType(CT_DECIMAL);
  m_pColVec->pGet(i_Col)->vDatumType(DM_BCD);
  i_Tkn++;
  
  // extract scale and precison if specified
  if (pTknVec->pGet(i_Tkn)->bSame("("_Stz)) {
    
    i_Tkn++;
    
    // check if it was numeric
    if ( ! pTknVec->pGet(i_Tkn)->bIsNumeric() )
      return false;
    
    iPrecision = pTknVec->pGet(i_Tkn++)->iToI8();
    
    if (!pTknVec->pGet(i_Tkn++)->bSame(","_Stz))
      return false;
    
    // check if it was numeric
    if ( ! pTknVec->pGet(i_Tkn)->bIsNumeric() )
      return false;
    
    // get the scale
    iScale = pTknVec->pGet(i_Tkn++)->iToI8();
    
    if (!pTknVec->pGet(i_Tkn++)->bSame(")"_Stz))
      return false;
    
    if (! Column::bValidBcd(iPrecision, iScale))
      return false;
  }
  
  // set scale and length in column
  m_pColVec->pGet(i_Col)->vColumnLength(iPrecision);
  m_pColVec->pGet(i_Col)->vColumnScale(iScale);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract float                                                              */
/*----------------------------------------------------------------------------*/
bool TableDef::bExtractFloat(TokenVec* pTknVec, i8 i_Col, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set default size
  i8 iSize = C_MAX_PRECISION_FLT;
  
  // set column type
  m_pColVec->pGet(i_Col)->vColumnType(CT_FLOAT);
  m_pColVec->pGet(i_Col)->vDatumType(DM_FLT);
  i_Tkn++;
  
  // extract size if specified
  if (pTknVec->pGet(i_Tkn)->bSame("("_Stz)) {
    
    // check if it was numeric
    if ( ! pTknVec->pGet(i_Tkn++)->bIsNumeric() ) {
      return false;
    }
    
    // get size
    iSize = pTknVec->pGet(i_Tkn++)->iToI8();
    
    if (!pTknVec->pGet(i_Tkn++)->bSame(")"_Stz)) {
      return false;
    }
    
    if (! Column::bValidFlt(iSize)) {
      return false;
    }
  }
  
  // set size in column
  m_pColVec->pGet(i_Col)->vColumnLength(iSize);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract varchar                                                            */
/*----------------------------------------------------------------------------*/
bool TableDef::bExtractVarchar(TokenVec* pTknVec, i8 i_Col, i8& i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLEDEF,_FOOTER_);
  
  // set column type
  m_pColVec->pGet(i_Col)->vColumnType(CT_VARCHAR);
  m_pColVec->pGet(i_Col)->vDatumType(DM_VCH);
  i_Tkn++;
  
  // set default size
  i8 iSize = C_64K;
  
  // extract size if specified
  if (pTknVec->pGet(i_Tkn)->bSame("("_Stz)) {
    
    m_pColVec->pGet(i_Col)->vColumnLength(pTknVec->pGet(i_Tkn++)->iToI8());
    
    
    // check if it was numeric
    if ( ! pTknVec->pGet(i_Tkn)->bIsNumeric() )
      return false;
    
    // get size
    iSize = pTknVec->pGet(i_Tkn++)->iToI8();
    
    if (!pTknVec->pGet(i_Tkn++)->bSame(")"_Stz))
      return false;
    
    // valid the size
    if (! Column::bValidVch(iSize)) {
      return false;
    }
  }
  // set size in column
  m_pColVec->pGet(i_Col)->vColumnLength(iSize);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TableDef)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
