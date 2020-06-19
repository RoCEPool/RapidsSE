/*============================================================================*/
/* IndexDef.cpp -- Index Definition                                           */
/*============================================================================*/

#include "MetaData.h"


#include "Jobs.h"


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexDef* MetaData::_pIndexDef(void* pAddr)
{
  IndexDef* pIndex = reinterpret_cast<IndexDef*>(pAddr);
  
  RASSERT_TYPE(pIndex,CN_INDEXDEF);
  
  return pIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexDef::operator new(size_t)
{
  Object* pIndexDef = HeapMem::pAllocate(CN_INDEXDEF);
  
  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexDef::operator delete(void* pAddress)
{
  IndexDef* pIndexDef = reinterpret_cast<IndexDef*>(pAddress);
  
  HeapMem::vFree(pIndexDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexDef::IndexDef(void)
: Definition(CN_INDEXDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  m_sIndexNm      = C_NO_NAME;
  m_sTableNm      = C_NO_NAME;
  m_eIndexType    = IT_SKIP_LIST;
  
  m_bIsPrimaryKey = false;
  m_bIsUnique     = false;
  
  m_pIdxColVec    = new IdxColVec(this);

  _checkInitialised_(CN_INDEXDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexDef::~IndexDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  // Since the column is picked from the ColVec of table
  // we need to just remove it from the IdxColVec before deleting it
  m_pIdxColVec->vClear();
  
  delete m_pIdxColVec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the index name                                                         */
/*----------------------------------------------------------------------------*/
Stz* IndexDef::szIndexNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_sIndexNm;
}

//#define dPT() vTrace("tkn=%s\n"_Stz,pTkn->szText());
#define dPT()

/*----------------------------------------------------------------------------*/
/* parse Create Index command                                                 */
/*----------------------------------------------------------------------------*/
eEC IndexDef::eParse(Table* pTable, Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  eEC eError = EC_NONE;

  // tokenise the command
  TokenVec* pTknVec = TokenVec::pTokenise(szCmd);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: Create Index <indexNm>
  
  // -- -- -- -- -- -- -- -- -- --
  // assert we got an index table
  // -- -- -- -- -- -- -- -- -- --
  
  Token* pTkn = pTknVec->pFirstToken(); dPT();
  ASSERT(pTkn->bSame("create"_Stz));
  
  pTkn = pTknVec->pNextToken(); dPT();
  ASSERT(pTkn->bSame("unique"_Stz) || pTkn->bSame("index"_Stz));
  
  if (pTkn->bSame("unique"_Stz)) {
    
    m_bIsUnique = true;
    
    pTkn = pTknVec->pNextToken(); dPT();
    ASSERT(pTkn->bSame("index"_Stz));
  }
  
  // -- -- -- --
  // index name
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  m_sIndexNm = pTkn->szChars();
  
  // -- -- -- --
  // ON
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  ASSERT(pTkn->bSame("on"_Stz));

  // -- -- -- --
  // table name
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  m_sTableNm = pTkn->szChars();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: ( <colNm> ,... )
  
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
  for (i8 i_Col = 0; ! pTkn->bSame(")"_Stz); i_Col++) {

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
    // process column
    // -- -- -- -- --
    bool bResult = bAddIdxCol(pTable,pTkn->szChars());
    if ( ! bResult ) {
      eError = EC_INVALID_INDEX_COLUMN;
      goto L_RETURN;
    }
    // -- -- -- --
    // next token
    // -- -- -- --
    pTkn = pTknVec->pNextToken(); dPT();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
L_RETURN:
  
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  delete pTknVec;
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* is this a primary key  index                                               */
/*----------------------------------------------------------------------------*/
bool IndexDef::bIsPrimaryKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_bIsPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* set index as a primary key index                                           */
/*----------------------------------------------------------------------------*/
void IndexDef::vIsPrimaryKey(bool bPrimaryKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  m_bIsPrimaryKey = bPrimaryKey;
}

/*----------------------------------------------------------------------------*/
/* is this index unique?                                                      */
/*----------------------------------------------------------------------------*/
bool IndexDef::bIsUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_bIsUnique;
}

/*----------------------------------------------------------------------------*/
/* set index unique                                                           */
/*----------------------------------------------------------------------------*/
void IndexDef::vIsUnique(bool bUnique)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  m_bIsUnique = bUnique;
}

/*----------------------------------------------------------------------------*/
/* get index column vector                                                    */
/*----------------------------------------------------------------------------*/
IdxColVec* IndexDef::pIdxColVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  return m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/* compute the size of sort key datum                                         */
/*----------------------------------------------------------------------------*/
i8 IndexDef::iSortKeyDatum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  i8 nKeyDatumSize = 0;
  for (i8 i_IdxCol = 0; i_IdxCol < m_pIdxColVec->nItems(); i_IdxCol++) {
    //Column* pColumn = m_pIdxColVec->pGet(i_IdxCol);
    nKeyDatumSize += sizeof(i8);
  }
  
  return nKeyDatumSize;
}

/*----------------------------------------------------------------------------*/
/* check the syntax of the Create Index command                               */
/*----------------------------------------------------------------------------*/
eEC IndexDef::eSyntax(Stz* szCmd)
{
  eEC eError = EC_NONE;
  i8 iIndexColumns;
  
  // tokenise the command
  TokenVec* pTknVec = TokenVec::pTokenise(szCmd);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: Create Index <indexNm>
  
  Token* pTkn = pTknVec->pFirstToken(); dPT();
  ASSERT(pTkn->bSame("create"_Stz));
  
  pTkn = pTknVec->pNextToken(); dPT();
  ASSERT(pTkn->bSame("unique"_Stz) || pTkn->bSame("index"_Stz));
  
  if (pTkn->bSame("unique"_Stz)) {
    
    pTkn = pTknVec->pNextToken(); dPT();
    ASSERT(pTkn->bSame("index"_Stz));
  }
  
  // -- -- -- --
  // index name
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // -- -- -- --
  // ON
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn || ! pTkn->bSame("on"_Stz) ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  // -- -- -- --
  // table name
  // -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Parse: ( <colNm> ,... )
  
  // -- -- -- -- --
  // open parenth (
  // -- -- -- -- --
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn || ! pTkn->bSame("("_Stz)) {
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
  
  iIndexColumns = 0;
  for (i8 i_Col = 0; ! pTkn->bSame(")"_Stz); i_Col++) {
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // subsequent column names, skipping over comma
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    iIndexColumns++;
    
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
    
    // -- -- -- --
    // next token
    // -- -- -- --
    pTkn = pTknVec->pNextToken(); dPT();
    if ( ! pTkn ) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }
  }
  
  pTkn = pTknVec->pNextToken(); dPT();
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  if ( ! pTkn->bSame(";"_Stz) ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
  
  if ( iIndexColumns == 0 ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
L_RETURN:
  
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  delete pTknVec;
  
  return eError;
}

#undef dPT


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column to the index column vector                                      */
/*----------------------------------------------------------------------------*/
bool IndexDef::bAddIdxCol(Table* pTable, Stz* szColNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDEF,_FOOTER_);
  
  Column* pColumn = pTable->pFindColumn(szColNm);

  if (! pColumn)
    return false;
  
  m_pIdxColVec->vAdd(pColumn);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(IndexDef)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
