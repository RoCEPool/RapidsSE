/*============================================================================*/
/* ExprEval.cpp -- Expression Evaluator                                       */
/*============================================================================*/

#include "BASE.h"

#include "StreamDef.hpp"

#include "ExprEval.hpp"
#include "SN_Dte.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ExprEval* STORE::_pExprEval(void* aAddr)
{
  ExprEval* pExprEval = reinterpret_cast<ExprEval*>(aAddr);
  
  RASSERT_TYPE(pExprEval,CN_EXPREVAL);
  
  return pExprEval;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ExprEval::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_EXPREVAL);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ExprEval::operator delete(void* aAddr)
{
  ExprEval* pExprEval = reinterpret_cast<ExprEval*>(aAddr);
  
  HeapMem::vFree(pExprEval);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExprEval::ExprEval(void)
: Definition(CN_EXPREVAL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  m_iStreamInColumns = 0;
  m_uExprEvalColumns = 0;
  //m_sAsmText;
  //m_sAsmGbyIdx;
  //m_sAsmGbyKey;
  m_bHasAgg = false;
  m_bHasGby = false;
  m_nPayLoadBytes = 0;

  //m_pStreamInCol;

  //_checkInitialised_(CN_EXPREVAL);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExprEval::~ExprEval(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column vec from stream def                                             */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEvalAll(StreamDef* pStreamDefIn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  for (i8 iCol = 0; iCol < pStreamDefIn->nColumns(); iCol++) {
    Column* pColumn = pStreamDefIn->pGetColumn(iCol);
    //Stz* m_sAsmText = m_sAsmText[iCol].szStz();

    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALS %lld\n"_Stz, iCol);
      StrLib::vSprintCat(m_sAsmText, "  EO_ROW_DST_POPCOLS %lld\n"_Stz, iCol);
    }
    else {
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALI %lld\n"_Stz, iCol);
      StrLib::vSprintCat(m_sAsmText, "  EO_ROW_DST_POPCOLI %lld\n"_Stz, iCol);
    }
  }

  L_RETRUN:
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add column vec from stream def                                             */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eEvaluate(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, Stz* sExprEvalColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // tokenise the projectColumns
  TokenVec *pTknVec = TokenVec::pTokenise(sExprEvalColumns);

  eEC eError = EC_NONE;
  Token* pTkn;
  i8 i_Tkn;

  // first collect all the StreamDefIn's source columns needed for projected
  eError = eCollectSourceColumns(pStreamDefIn, pTknVec);

  if (eError != EC_NONE)
    goto L_RETRUN;

  pTkn = pTknVec->pNextToken();

  // now evaluate the expressions
  eError = eExprEvalColumn(pStreamDefIn, pStreamDefOut, pTknVec);

  L_RETRUN:
    delete pTknVec;
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* add EO code for arithmetic operators                                       */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddArithmetic(Token* pTkn, Column* pColumn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame("+"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_ADD\n"_Stz);
  }
  else if (pTkn->bSame("/"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_DIVIDE\n"_Stz);
  }
  else if (pTkn->bSame("%"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_MODULO\n"_Stz);
  }
  else if (pTkn->bSame("*"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_MULTIPLY\n"_Stz);
    if (pColumn && pColumn->eColumnType() == CT_DECIMAL) {
      StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER 100\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DIVIDE\n"_Stz);
    }
  }
  else {
    ASSERT(pTkn->bSame("-"_Stz));
    StrLib::vSprintCat(m_sAsmText, "  EO_SUBTRACT\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for dates                                                      */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddDate(Token* pTkn, Column* pColumnExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  i8 iDate;
  char* szToken = (char*)pTkn->szChars();

  SN_Dte snDate;

  // validate the date
  try {
    iDate = snDate.i(&szToken, AM::pFmt());
  } catch (...) {
    return EC_INVALID_COMMAND;
  }

  StrLib::vSprintCat(m_sAsmText, "  EO_DATE \"%s\"\n"_Stz, pTkn->szChars());
  pColumnExpr->vSet("NONE"_Stz, CT_DATE, 0, 0);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for decimals                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddDecimal(Token* pTkn, Column* pColumnExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;
  i8 iDecimal;

  char* szToken = (char*)pTkn->szChars();

  bool bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 19, 2);
  if ( ! bResult ) {
    eError = EC_INVALID_COMMAND;
  }
  else {
    StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER %lld\n"_Stz, iDecimal);
    pColumnExpr->vSet("NONE"_Stz, CT_DECIMAL, 19, 2);
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for groupBy                                                    */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddGby(Column* pColumn, i8 iGby) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  i8 iLocal = 0;
  i8 iBit = -1;
  for (i8 i = 0; i < pColumn->iColumnNo(); i++) {
    if (BitLib::bEachBit(m_uExprEvalColumns, iBit)) {
      if (iBit < pColumn->iColumnNo()) {
        iLocal++;
      } else {
        break;
      }
    }
  }

  // switch on column type
  switch (pColumn->eColumnType()) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sAsmGbyIdx, "  EO_PUSH_COLUMN_TYPE %s\n"_Stz,
                         aCT_long[pColumn->eColumnType()]);
      StrLib::vSprintCat(m_sAsmGbyIdx, "  EO_REG_INDEXDEF_ADDCOLUMN \"int%lld\"\n"_Stz, iLocal);

      StrLib::vSprintCat(m_sPayLoadKey, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      StrLib::vSprintCat(m_sPayLoadKey, "  EO_ENTWRK_SETINTELM\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadKey, "  EO_RETURN\n"_Stz);

      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for logical operators                                          */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddLogical(Token* pTkn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame("and"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_AND\n"_Stz);
  }
  else if (pTkn->bSame("or"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_OR\n"_Stz);
  }
  else {
    ASSERT(pTkn->bSame("not"_Stz));
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_NOT\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for numerics                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddNumeric(Token* pTkn, Column* pColumnExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
  pColumnExpr->vSet("NONE"_Stz, CT_INTEGER, 19, 0);
}

/*----------------------------------------------------------------------------*/
/* add EO code for pop column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPopCol(eCT eColumnType, i8 iColumn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_DATE:
      StrLib::vSprintCat(m_sAsmText, "  EO_ROW_DST_POPCOLI %lld\n"_Stz, iColumn);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(m_sAsmText, "  EO_ROW_DST_POPCOLS %lld\n"_Stz, iColumn);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for relational operators                                       */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddRelational(Token* pTkn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame(">"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_GREATERTHAN\n"_Stz);
  }
  else if (pTkn->bSame(">="_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_GREATEREQUAL\n"_Stz);
  }
  else if (pTkn->bSame("<"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LESSTHAN\n"_Stz);
  }
  else if (pTkn->bSame("<="_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LESSEQUAL\n"_Stz);
  }
  else {
    ASSERT(pTkn->bSame("=="_Stz));
      StrLib::vSprintCat(m_sAsmText, "  EO_EQUAL\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column                                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddStoreCol(eCT eColumnType, i8 iColumn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_DATE:
      StrLib::vSprintCat(m_sPayLoadStore, "  EO_PAY_STORE_COLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_ROW_DST_POPCOLI %lld\n"_Stz, iColumn);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(m_sPayLoadStore, "  EO_PAY_STORE_COLS %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_ROW_DST_POPCOLS %lld\n"_Stz, iColumn);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for strings                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddString(Token* pTkn, Column* pColumnExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  char* szToken = (char*)pTkn->szChars();

  StrLib::vSprintCat(m_sAsmText, "  EO_STRING %s\n"_Stz, szToken);
  pColumnExpr->vSet("NONE"_Stz, CT_VARCHAR, 128, 0);    // ALKA: come back to this
}

/*----------------------------------------------------------------------------*/
/* add EO code for sum                                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddSum(eCT eColumnType, i8 iColumn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sAsmText, "  EO_ROW_DST_PUSHCOLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sAsmText, "  EO_ADD\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_ROW_DST_POPCOLI %lld\n"_Stz, iColumn);
      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for sum                                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddSumGby(eCT eColumnType, i8 iColumn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sAsmText, "  EO_PAY_FETCH_COLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sAsmText, "  EO_ADD\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_PAY_STORE_COLI %lld\n"_Stz, iColumn);
      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* collect source columns needed for projection                               */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;
  Token* pTkn;
  bool bOpenBraces = false;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);

    // skip alias ?
    if (pTkn->bSame("as"_Stz)) {
      i_Tkn++;
      continue;
    }

    // skip any arithmetic operators
    if (pTkn->bIsArithmetic())
      continue;

    // skip any logical operators
    if (pTkn->bIsLogical()) {
      continue;
    }

    // skip any relational operators
    if (pTkn->bIsRelational()) {
      continue;
    }

    // skip any numeric numbers
    if (pTkn->bIsNumeric()) {
      continue;
    }

    // skip any decimal numbers
    if (pTkn->bIsDecimalNo()) {
      continue;
    }

    // skip any strings
    if (pTkn->bIsString()) {
      continue;
    }

    // skip any tags
    if (pTkn->eToken() == TN_TAG && pTkn->bSame("<>"_Stz)) {
      continue;
    }

    // it should be a name
    if (!pTkn->bIsName()) {
      eError = EC_INVALID_COMMAND;
      break;
    }

    if (pTkn->bIsName()  && pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pGet(i_Tkn+1);

      // skip if its a date string
      if (pTkn->bIsString())
        i_Tkn++;
      continue;
    }

    if (pTkn->bIsName()  && pTkn->bSame("SUM"_Stz) ) {

      // next token should be (
      pTkn = pTknVec->pGet(++i_Tkn);

      if ( ! pTkn->bSame("("_Stz) ) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      m_bHasAgg = true;
      bOpenBraces = true;
      continue;
    }

    if (pTkn->bIsName()  && pTkn->bSame("GROUP"_Stz) ) {

      // next token should be BY
      pTkn = pTknVec->pGet(++i_Tkn);

      if ( ! pTkn->bSame("BY"_Stz) ) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      m_bHasGby = true;

      continue;
    }

    if (pTkn->bIsName()  && pTkn->bSame(")"_Stz) ) {

      // // should have seen a (

      if ( ! bOpenBraces ) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      bOpenBraces = false;
      continue;
    }

    // skip any strings
    if (pTkn->bIsString()) {
      continue;
    }

    if (pTkn->bIsName() && pTkn->bSame(","_Stz)) {
      continue;
    }

    // look for the column in the streamDefIn
    Column *pColumn = nullptr;
    bool bFound = false;
    i8 nColumns = pStreamDefIn->nColumns();
    Stz* sColumnNm = pTkn->szChars();

    for (i8 i_StreamInCol = 0; i_StreamInCol < nColumns; i_StreamInCol++) {
      pColumn = pStreamDefIn->pGetColumn(i_StreamInCol);
      if (pColumn->szColNm()->bSame(sColumnNm)) {
        bFound = true;

        // already selected column ?
        if ( ! BitLib::bIsOn(m_uExprEvalColumns, i_StreamInCol) ) {

          // set bit on for the column in m_uExprEvalColumns
          BitLib::vSetOn(m_uExprEvalColumns, i_StreamInCol);
          m_pStreamInCol[m_iStreamInColumns] = pColumn;
          m_iStreamInColumns++;
        }
        break;
      }
    }
    m_pStreamInCol[m_iStreamInColumns] = nullptr;

    if (!bFound) {
      eError = EC_INVALID_COMMAND;
      break;
    }

    continue;
  }

  L_RETRUN:
    return eError;
}


/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEvalColumn(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;

  Stz *sAliasName = ""_Stz;
  i8 i_DstColumn = C_UNKNOWN;
  i8 i_GbyColumn = C_UNKNOWN;
  Column *pColumn = nullptr;
  Column *pColumnExpr =  new Column;
  TagSet oTagSet;
  bool bSum = false;
  bool bGroupBy = false;

  Token* pTkn = pTknVec->pCurrToken();

  while ( pTkn ) {

    if (pTkn->bSame(","_Stz)) {

      i_DstColumn++;

      // discovered a comma, add the column to the streamDefOut
      if (pStreamDefOut) {
        eError = eHandleOutColumn(pStreamDefOut, pColumn ? pColumn : pColumnExpr,
                                  sAliasName, i_DstColumn, bSum);
        if (eError == EC_INVALID_COMMAND)
          goto L_RETURN;
        bSum = false;
      }

      pColumn = nullptr;
      pTkn = pTknVec->pNextToken();
    }

    // alias ?
    if (pTkn->bSame("as"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      // extract column name
      sAliasName = pTkn->szChars();
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // arithmetic operator ?
    if (pTkn->bIsArithmetic()) {
      vAddArithmetic(pTkn, pColumn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // logical operator ?
    if (pTkn->bIsLogical()) {
      vAddLogical(pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // relational operator ?
    if (pTkn->bIsRelational()) {
      vAddRelational(pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a numeric ?
    if (pTkn->bIsNumeric()) {
      vAddNumeric(pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      eError = eAddDecimal(pTkn, pColumnExpr);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsString()) {
      vAddString(pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->eToken() == TN_TAG && pTkn->bSame("<>"_Stz)) {
      StrLib::vSprintCat(m_sAsmText, "  EO_RETURN_FALSE\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (!pTkn->bIsName()) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }

    if (pTkn->bIsName()  && pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        eError = eAddDate(pTkn, pColumnExpr);
        if (eError == EC_INVALID_COMMAND) {
          goto L_RETURN;
        }
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bIsName()  && pTkn->bSame("SUM"_Stz) ) {

      bSum = true;
      ASSERT(m_bHasAgg);

      // skip next token (
      pTkn = pTknVec->pNextToken();
      ASSERT(pTkn->bSame("("_Stz));

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsName()  && pTkn->bSame(")"_Stz)) {
      ASSERT(bSum);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsName()  && pTkn->bSame("GROUP"_Stz) ) {

      i_GbyColumn++;
      bGroupBy = true;

      ASSERT(m_bHasGby);

      // skip next token BY
      pTkn = pTknVec->pNextToken();
      ASSERT(pTkn->bSame("BY"_Stz));

      i_DstColumn++;

      // discovered a comma, add the column to the streamDefOut
      if (pStreamDefOut)
        eError = eHandleOutColumn(pStreamDefOut, pColumn ? pColumn : pColumnExpr,
                                  sAliasName, i_DstColumn, bSum);
        if ( eError == EC_INVALID_COMMAND)
          goto L_RETURN;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // found a src column
    Stz *sColumnNm = pTkn->szChars();

    // get in column from m_iStreamInCol ~ to out column
    pColumn = nullptr;
    i8 i_StreamInCol;
    for (i_StreamInCol = 0; m_pStreamInCol[i_StreamInCol] != nullptr; i_StreamInCol++) {
      pColumn = m_pStreamInCol[i_StreamInCol];
      if (pColumn->szColNm()->bSame(sColumnNm)) {
        break;
      }
    }
    RASSERT(pColumn);

    // check how many bits are on till this i_StreamInCol bit
    i8 iLocal = 0;
    i8 iBit = -1;
    for (i8 i = 0; i < pColumn->iColumnNo(); i++) {
      if (BitLib::bEachBit(m_uExprEvalColumns, iBit)) {
        if (iBit < pColumn->iColumnNo()) {
          iLocal++;
        } else {
          break;
        }
      }
    }

    if ( ! bGroupBy ) {

      if (m_bHasGby) {
        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR) {
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
          StrLib::vSprintCat(m_sPayLoadStore, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
          //StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, iLocal);
        } else {
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
          StrLib::vSprintCat(m_sPayLoadStore, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
          //StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, iLocal);
        }

      } else {
        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR)
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
        else
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      }
    }
    else {
      eError = eHandleGbyColumn(pColumn, i_GbyColumn);
      if (eError == EC_INVALID_COMMAND)
        goto L_RETURN;
    }
    pTkn = pTknVec->pNextToken();
  }

  if (m_bHasGby) {
    //eError = eHandleGbyColumn(pColumn, i_GbyColumn);
  }
  else {
    i_DstColumn++;

    // add the last column to the streamDefOut
    if (pStreamDefOut)
      eError = eHandleOutColumn(pStreamDefOut, pColumn ? pColumn : pColumnExpr,
                                sAliasName, i_DstColumn, bSum);
  }

  L_RETURN:
    delete pColumnExpr;
    return eError;
}

/*----------------------------------------------------------------------------*/
/* handle a group by column                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eHandleGbyColumn(Column* pColumn, i8 iGby) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;

  eError = eAddGby(pColumn, iGby);
  if (eError == EC_INVALID_COMMAND)
    return eError;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* handle an out column                                                       */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eHandleOutColumn(StreamDef* pStreamDefOut, Column* pColumn, Stz* sAliasName, i8 i_DstColumn, bool bSum) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;

  vUpdateStreamDefOut(pStreamDefOut, pColumn, sAliasName);

  if (m_bHasGby) {
    if (bSum) {
      eError = eAddSumGby(pColumn->eColumnType(), i_DstColumn);
      if (eError == EC_INVALID_COMMAND)
        return eError;
    }
    vAddStoreCol(pColumn->eColumnType(), i_DstColumn);

    m_nPayLoadBytes += 8;
  }
  else {
    if (bSum) {
      eError = eAddSum(pColumn->eColumnType(), i_DstColumn);
      if (eError == EC_INVALID_COMMAND)
        return eError;
    }
    else {
      vAddPopCol(pColumn->eColumnType(), i_DstColumn);
    }
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* expr has an aggregate ?                                                    */
/*----------------------------------------------------------------------------*/
bool ExprEval::bHasAggregate(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  return m_bHasAgg;
}

/*----------------------------------------------------------------------------*/
/* expr has an aggregate with groupBy ?                                       */
/*----------------------------------------------------------------------------*/
bool ExprEval::bHasGroupBy(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  return m_bHasGby;
}

/*----------------------------------------------------------------------------*/
/* update the streamDefOut                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn, Stz* sAliasName) {
    pStreamDefOut->vAddColumn(sAliasName->bSame(""_Stz) ? pColumn->szColNm() : sAliasName,
                              pColumn->eColumnType(),
                              pColumn->iColumnLength(),
                              pColumn->iColumnScale());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ExprEval);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
