/*============================================================================*/
/* ExprEval.cpp -- Expression Evaluator                                       */
/*============================================================================*/

#include "BASE.h"

#include "StreamDef.hpp"

#include "ExprEval.hpp"
#include "SN_Dte.hpp"
#include "eEO.h"

#include "MathOp.h"

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
  void* pObject = Heaps::pAllocate(CN_EXPREVAL);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ExprEval::operator delete(void* aAddr)
{
  ExprEval* pExprEval = reinterpret_cast<ExprEval*>(aAddr);
  
  Heaps::vFree(pExprEval);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExprEval::ExprEval(void)
: Definition(CN_EXPREVAL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  m_uExprEvalColumns = 0;
  m_nPayLoadBytes = 0;
  m_iStreamInColumns = 0;

  m_bHasAgg = false;
  m_bHasGby = false;

  //m_sAsmText;
  //m_sAsmGbyIdx;
  //m_sAsmGbyKey;
  //m_sPayLoadKey;
  //m_sPayLoadStore;
  //m_sPayLoadToBuffer;

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
eEC ExprEval::eEvaluate(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, Stz* sExprEvalColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // tokenise the projectColumns
  TokenVec *pTknVec = TokenVec::pTokenise(sExprEvalColumns);

  eEC eError = EC_NONE;
  Token* pTkn;

  // first collect all the StreamDefIn's source columns needed for projected
  eError = eCollectSourceColumns(pStreamDefIn, pTknVec);

  if (eError != EC_NONE)
    goto L_RETRUN;

  pTkn = pTknVec->pNextToken();

  // now evaluate the expressions
  eError = eExprEval(pStreamDefIn, pStreamDefOut, pTknVec);

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

  eEO eCode;
  eDM eParamType1;
  eDM eParamType2;
  eDM eResultType;

  if (pTkn->bSame("+"_Stz)) {
    eParamType1 = (eDM)m_xTypeStack.iPop();
    eParamType2 = (eDM)m_xTypeStack.iPop();
    eEO eAddCode = eMathOp_AddCode[eParamType1][eParamType2];
    StrLib::vSprintCat(m_sAsmText, "  %s\n"_Stz, aEO[eAddCode]);
    eResultType = eMathOp_AddResult[eParamType1][eParamType2];
    m_xTypeStack.vPush((i8)eResultType);
  }
  else if (pTkn->bSame("/"_Stz)) {
    eParamType1 = (eDM)m_xTypeStack.iPop();
    eParamType2 = (eDM)m_xTypeStack.iPop();
    eEO eDivCode = eMathOp_DivCode[eParamType1][eParamType2];
    StrLib::vSprintCat(m_sAsmText, "  %s\n"_Stz, aEO[eDivCode]);
    eResultType = eMathOp_DivResult[eParamType1][eParamType2];
    m_xTypeStack.vPush((i8)eResultType);
  }
  else if (pTkn->bSame("%"_Stz)) {
    eParamType1 = (eDM)m_xTypeStack.iPop();
    eParamType2 = (eDM)m_xTypeStack.iPop();
    eEO eModCode = eMathOp_ModCode[eParamType1][eParamType2];
    StrLib::vSprintCat(m_sAsmText, "  %s\n"_Stz, aEO[eModCode]);
    eResultType = eMathOp_ModResult[eParamType1][eParamType2];
    m_xTypeStack.vPush((i8)eResultType);
  }
  else if (pTkn->bSame("*"_Stz)) {
    eParamType1 = (eDM)m_xTypeStack.iPop();
    eParamType2 = (eDM)m_xTypeStack.iPop();
    eEO eMulCode = eMathOp_MulCode[eParamType1][eParamType2];
    StrLib::vSprintCat(m_sAsmText, "  %s\n"_Stz, aEO[eMulCode]);
    eResultType = eMathOp_MulResult[eParamType1][eParamType2];
    m_xTypeStack.vPush((i8)eResultType);

    if (pColumn && pColumn->eColumnType() == CT_DECIMAL) {
      StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER 100\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DIV_INTINT\n"_Stz);
    }
  }
  else if (pTkn->bSame("-"_Stz)) {
    eParamType1 = (eDM) m_xTypeStack.iPop();
    eParamType2 = (eDM) m_xTypeStack.iPop();
    eEO eSubCode = eMathOp_SubCode[eParamType1][eParamType2];
    StrLib::vSprintCat(m_sAsmText, "  %s\n"_Stz, aEO[eSubCode]);
    eResultType = eMathOp_SubResult[eParamType1][eParamType2];
    m_xTypeStack.vPush((i8) eResultType);
  }
  else {
    ASSERT(pTkn->bSame("negate"_Stz));
    eParamType1 = (eDM) m_xTypeStack.iPop();
    eEO eNegCode = eMathOp_NegCode[eParamType1];
    StrLib::vSprintCat(m_sAsmText, "  %s\n"_Stz, aEO[eNegCode]);
    eResultType = eMathOp_NegResult[eParamType1];
    m_xTypeStack.vPush((i8) eResultType);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for count                                                      */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddCount(eCT eColumnType, i8 iColumn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sAsmText, "  EO_ISNULL\n"_Stz);
      //StrLib::vSprintCat(m_sAsmText, "  EO_JMP_TRUE COUNT_DONE\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_RETURN_TRUE\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER 1\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_ADD_INTINT\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_POPCOLI %lld\n"_Stz, iColumn);
      //StrLib::vSprintCat(m_sAsmText, "  Label COUNT_DONE\n"_Stz);
      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
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

  m_xTypeStack.vPush(DM_DTE);
  StrLib::vSprintCat(m_sAsmText, "  EO_DATE \"%s\"\n"_Stz, pTkn->szChars());
  //pColumnExpr->vSet("NONE"_Stz, CT_DATE, 0, 0);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for decimals                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddDecimal(Token* pTkn, Column* pColumnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;
  i8 iDecimal;

  char* szToken = (char*)pTkn->szChars();

  bool bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 19, 2);
  if ( ! bResult ) {
    eError = EC_INVALID_COMMAND;
  }
  else {
    m_xTypeStack.vPush(DM_BCD);
    StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER %lld\n"_Stz, iDecimal);
    //pColumnExpr->vSet("NONE"_Stz, CT_DECIMAL, 19, 2);
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for groupBy                                                    */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddGby(Column* pColumn, i8 iGby)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  UNUSED(iGby);

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
      StrLib::vSprintCat(m_sAsmGbyIdx, "  EO_STRING \"int%lld\"\n"_Stz, iLocal);
      StrLib::vSprintCat(m_sAsmGbyIdx, "  EO_PUSH_COLUMN_TYPE %s\n"_Stz,
                         aCT_long[pColumn->eColumnType()]);
      StrLib::vSprintCat(m_sAsmGbyIdx, "  EO_PUSH_RLU_INDEXDEF\n"_Stz);
      StrLib::vSprintCat(m_sAsmGbyIdx, "  EO_RLU_INDEXDEF_ADDCOLUMN\n"_Stz);

      m_xTypeStack.vPush((i8)DM_INT);
      StrLib::vSprintCat(m_sPayLoadKey, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);

      StrLib::vSprintCat(m_sPayLoadKey, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadKey, "  EO_ENTWRK_SETINTELM\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadKey, "  EO_RETURN\n"_Stz);

      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for logical operators                                          */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddLogical(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame("and"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_AND\n"_Stz);
  }
  else if (pTkn->bSame("isNull"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_ISNULL\n"_Stz);
  }
  else if (pTkn->bSame("not"_Stz)) {
    ASSERT(pTkn->bSame("not"_Stz));
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_NOT\n"_Stz);
  }
  else if (pTkn->bSame("or"_Stz)) {
    ASSERT(pTkn->bSame("not"_Stz));
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_OR\n"_Stz);
  }
  else {
    ASSERT(pTkn->bSame("xor"_Stz));
    StrLib::vSprintCat(m_sAsmText, "  EO_LGL_XOR\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for numerics                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddNumeric(Token* pTkn, Column* pColumnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush((i8)DM_INT);
  StrLib::vSprintCat(m_sAsmText, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
  //pColumnExpr->vSet("NONE"_Stz, CT_INTEGER, 19, 0);
}

/*----------------------------------------------------------------------------*/
/* add EO code for pop column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPopCol(eCT eColumnType, i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
    case CT_TIME:
    case CT_TIMESTAMP:
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_POPCOLI %lld\n"_Stz, iColumn);
      m_xTypeStack.iPop();
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_POPCOLS %lld\n"_Stz, iColumn);
      m_xTypeStack.iPop();
      break;

    case CT_UNKNOWN:
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_POPCOLI %lld\n"_Stz, iColumn);
      m_xTypeStack.iPop();
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for relational operators                                       */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddRelational(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame(">"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_GT_INTINT\n"_Stz);
  }
  else if (pTkn->bSame(">="_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_GE_INTINT\n"_Stz);
  }
  else if (pTkn->bSame("<"_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LT_INTINT\n"_Stz);
  }
  else if (pTkn->bSame("<="_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_LE_INTINT\n"_Stz);
  }
  else if (pTkn->bSame("!="_Stz)) {
    StrLib::vSprintCat(m_sAsmText, "  EO_NE_INTINT\n"_Stz);
  }
  else {
    ASSERT(pTkn->bSame("=="_Stz));
      StrLib::vSprintCat(m_sAsmText, "  EO_EQ_INTINT\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column                                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddStoreCol(eCT eColumnType, i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sPayLoadStore, "  EO_PAY_STORE_COLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer,"  EO_PAY_FETCH_COLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadToBuffer,"  EO_DEVROW_POPCOLI %lld\n"_Stz, iColumn);

      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(m_sPayLoadStore, "  EO_PAY_STORE_COLS %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sPayLoadToBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadToBuffer,"  EO_DEVROW_POPCOLS %lld\n"_Stz, iColumn);
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

  m_xTypeStack.vPush((i8)DM_VCH);
  StrLib::vSprintCat(m_sAsmText, "  EO_STR_LITERAL %s\n"_Stz, szToken);
  //pColumnExpr->vSet("NONE"_Stz, CT_VARCHAR, 128, 0);    // ALKA: come back to this
}

/*----------------------------------------------------------------------------*/
/* add EO code for sum                                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddSum(eCT eColumnType, i8 iColumn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sAsmText, "  EO_ADD_INTINT\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_DEVROW_POPCOLI %lld\n"_Stz, iColumn);
      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for sum                                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddSumGby(eCT eColumnType, i8 iColumn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_FLOAT:
      StrLib::vSprintCat(m_sAsmText, "  EO_PAY_FETCH_COLI %lld\n"_Stz, iColumn);
      StrLib::vSprintCat(m_sAsmText, "  EO_ADD_INTINT\n"_Stz);
      StrLib::vSprintCat(m_sAsmText, "  EO_PAY_STORE_COLI %lld\n"_Stz, iColumn);
      return EC_NONE;

    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      return EC_INVALID_COMMAND;

    default:
      RASSERT(0);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for unknown                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddUnknown(Token* pTkn, Column* pColumnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush((i8)DM_UNK);
  StrLib::vSprintCat(m_sAsmText, "  EO_PUSHNULL\n"_Stz);
  //pColumnExpr->vSet("NONE"_Stz, CT_UNKNOWN, 19, 0);
}

/*----------------------------------------------------------------------------*/
/* collect source ordinal columns needed for projection                       */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;
  Token* pTkn;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);

    if (pTkn->bIsName() && pTkn->bSame("SUM"_Stz)) {
      m_bHasAgg = true;
      continue;
    }

    if (pTkn->bIsName() && pTkn->bSame("COUNT"_Stz)) {
      m_bHasAgg = true;
      continue;
    }

    if (pTkn->bIsName() && pTkn->bSame("GROUPBY"_Stz)) {
      m_bHasGby = true;
      continue;
    }

    if (pTkn->bIsName() && pTkn->szChars()->bContains("$"_Stz)) {

      Stz* sOrdinalColumn = pTkn->szChars() + 1;
      i8 i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

      // look for the column in the streamDefIn
      i8 nColumns = pStreamDefIn->nColumns();
      if (i_ColumnOrdinal > nColumns) {
        eError = EC_INVALID_COMMAND;
        break;
      }

      // already selected column ?
      if (!BitLib::bIsOn(m_uExprEvalColumns, i_ColumnOrdinal)) {

        // set bit on for the column in m_uExprEvalColumns
        BitLib::vSetOn(m_uExprEvalColumns, i_ColumnOrdinal);
        m_iStreamInColumns++;
      }
    }

  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEval(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);
  UNUSED(pStreamDefIn);
  
  eEC eError = EC_NONE;

  i8 i_DstColumn = C_UNKNOWN;
  i8 i_GbyColumn = C_UNKNOWN;
  Column *pColumn = nullptr;
  Column *pColumnExpr =  new Column;
  eCT eExprType;
  TagSet oTagSet;
  bool bSum = false;
  bool bCount = false;
  bool bGroupBy = false;
  s32 sColumnNm;
  i8 iColumn = 0;

  sColumnNm.vSprint("col%lld"_Stz, i_DstColumn+1);
  pColumnExpr->vColumnNm(sColumnNm);

  Token* pTkn = pTknVec->pCurrToken();

  while ( pTkn ) {


    if (pTkn->bSame(","_Stz)) {

      i_DstColumn++;

      // discovered a comma, add the column to the streamDefOut
      if (pStreamDefOut) {
        eError = eHandleOutColumn(pStreamDefOut,
                                  pColumnExpr,
                                  i_DstColumn, bSum, bCount);
        if (eError == EC_INVALID_COMMAND)
          goto L_RETURN;
        bSum = false;
        bCount = false;
      }

      sColumnNm.vSprint("col%lld"_Stz, i_DstColumn+1);
      pColumnExpr->vColumnNm(sColumnNm);

      pColumn = nullptr;
      pTkn = pTknVec->pNextToken();
    }

    // type ?
    if (pTkn->bSame("type"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      // extract expr type
      eExprType = StreamDef::eColumn(pTkn->szChars());
      pColumnExpr->vColumnType(eExprType);

      // HACK!!!
      if (eExprType == CT_DECIMAL) {
        pColumnExpr->vColumnScale(2);
        pColumnExpr->vColumnLength(19);
      }
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // alias ?
    if (pTkn->bSame("as"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      // extract column name
      pColumnExpr->vColumnNm(pTkn->szChars());
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
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {
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

//    if (pColumnExpr->eColumnType() == CT_NONE ) {
//      eError = EC_INVALID_COMMAND;
//      goto L_RETURN;
//    }

    if (pTkn->bSame("negate"_Stz) ) {
      vAddArithmetic(pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("null"_Stz) ) {
      vAddUnknown(pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ) {
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

    if (pTkn->bSame("SUM"_Stz) ) {

      bSum = true;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {

      bCount = true;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("GROUPBY"_Stz) ) {

      i_GbyColumn++;
      bGroupBy = true;

      ASSERT(m_bHasGby);

      i_DstColumn++;

      // discovered a comma, add the column to the streamDefOut
      if (pStreamDefOut)
        eError = eHandleOutColumn(pStreamDefOut,
                                  pColumnExpr,
                                  i_DstColumn, bSum, bCount);
        if ( eError == EC_INVALID_COMMAND)
          goto L_RETURN;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // found a src column
    if ( ! pTkn->szChars()->bContains("$"_Stz)) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }

    Stz *sOrdinalColumn = pTkn->szChars() + 1;
    i8 i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

    // check how many bits are on till this i_StreamInCol bit
    i8 iLocal = 0;
    i8 iBit = -1;
    for (i8 i = 0; i < i_ColumnOrdinal; i++) {
      if (BitLib::bEachBit(m_uExprEvalColumns, iBit)) {
        if (iBit < i_ColumnOrdinal) {
          iLocal++;
        } else {
          break;
        }
      }
    }

    pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

    if ( ! bGroupBy ) {

      if (m_bHasGby) {
        if (pColumnExpr->eColumnType() == CT_VARCHAR ||
            pColumnExpr->eColumnType() == CT_CHAR) {
          m_xTypeStack.vPush((i8)DM_VCH);
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
          StrLib::vSprintCat(m_sPayLoadStore, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
          //StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, iLocal);
        } else {
          m_xTypeStack.vPush((i8)DM_INT);
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
          StrLib::vSprintCat(m_sPayLoadStore, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
          //StrLib::vSprintCat(m_sPayLoadToBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, iLocal);
        }

      } else {
        if (pColumnExpr->eColumnType() == CT_VARCHAR ||
            pColumnExpr->eColumnType() == CT_CHAR) {
          m_xTypeStack.vPush((i8) DM_VCH);
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
        }
        else {
          m_xTypeStack.vPush((i8)DM_INT);
          StrLib::vSprintCat(m_sAsmText, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
        }
      }
    }
    else {
      eError = eHandleGbyColumn(pColumnExpr, i_GbyColumn);
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
      eError = eHandleOutColumn(pStreamDefOut,
                                pColumnExpr,
                                i_DstColumn, bSum, bCount);
  }

  L_RETURN:
    //RASSERT(m_xTypeStack.nItems() == 0);
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
eEC ExprEval::eHandleOutColumn(StreamDef* pStreamDefOut, Column* pColumn, i8 i_DstColumn, bool bSum, bool bCount) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;

  vUpdateStreamDefOut(pStreamDefOut, pColumn);

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
    else if (bCount) {
      eError = eAddCount(pColumn->eColumnType(), i_DstColumn);
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
void ExprEval::vUpdateStreamDefOut(StreamDef* pStreamDefOut, Column* pColumn) {
    pStreamDefOut->vAddColumn(pColumn->szColNm(),
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
