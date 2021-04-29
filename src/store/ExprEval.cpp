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

  m_iDstColumn = C_UNKNOWN;
  
  //m_sProjectAsm;
  //m_sFilterAsm;
  //m_sAggInitAsm;

  //m_sGbyIdxAsm;
  //m_sPayLoadAggAsm;
  //m_sPayLoadKeyAsm;
  //m_sPayLoadStoreAsm;
  //m_sPayLoadToBufferAsm;

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
  if (m_bHasGby)
    eError = eExprEvalGby(pStreamDefIn, pStreamDefOut, pTknVec);
  else
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
void ExprEval::vAddArithmetic(Stz* szAsmBuffer, Token* pTkn, Column* pColumn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  eEO eCode;
  eDM eParamType1;
  eDM eParamType2;
  eDM eResultType;

  if (pTkn->bSame("negate"_Stz)) {

    // pop one param type
    eParamType1 = (eDM) m_xTypeStack.iPop();

    eCode = eMathOp_NegCode[eParamType1];
    eResultType = eMathOp_NegResult[eParamType1];
  }
  else {

    // pop two param types
    eParamType1 = (eDM) m_xTypeStack.iPop();
    eParamType2 = (eDM) m_xTypeStack.iPop();

    if (pTkn->bSame("+"_Stz)) {
      eCode = eMathOp_AddCode[eParamType1][eParamType2];
      eResultType = eMathOp_AddResult[eParamType1][eParamType2];
    } else if (pTkn->bSame("/"_Stz)) {
      eCode = eMathOp_DivCode[eParamType1][eParamType2];
      eResultType = eMathOp_DivResult[eParamType1][eParamType2];
    } else if (pTkn->bSame("%"_Stz)) {
      eCode = eMathOp_ModCode[eParamType1][eParamType2];
      eResultType = eMathOp_ModResult[eParamType1][eParamType2];
    } else if (pTkn->bSame("*"_Stz)) {
      eCode = eMathOp_MulCode[eParamType1][eParamType2];
      eResultType = eMathOp_MulResult[eParamType1][eParamType2];
    } else if (pTkn->bSame("-"_Stz)) {
      eCode = eMathOp_SubCode[eParamType1][eParamType2];
      eResultType = eMathOp_SubResult[eParamType1][eParamType2];
    } else {
      RASSERT(0);
    }
  }
  StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
  m_xTypeStack.vPush((i8) eResultType);
}

/*----------------------------------------------------------------------------*/
/* add EO code for dates                                                      */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddDate(Stz* szAsmBuffer, Token* pTkn, Column* pColumnExpr) {

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
  StrLib::vSprintCat(szAsmBuffer, "  EO_DATE \"%s\"\n"_Stz, pTkn->szChars());
  //pColumnExpr->vSet("NONE"_Stz, CT_DATE, 0, 0);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for decimals                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddDecimal(Stz* szAsmBuffer, Token* pTkn, Column* pColumnExpr)
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
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER %lld\n"_Stz, iDecimal);
    //pColumnExpr->vSet("NONE"_Stz, CT_DECIMAL, 19, 2);
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for gby idx col                                                */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddGbyIdxCol(Stz* szAsmBuffer, StreamDef* pStreamDefIn, i8 i_ColumnOrdinal) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

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

  Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

  // switch on column type
  switch (pColumn->eColumnType()) {

    case CT_DECIMAL:
    case CT_INTEGER:
    case CT_FLOAT:
      StrLib::vSprintCat(szAsmBuffer, "  EO_STRING \"int%lld\"\n"_Stz, iLocal);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_COLUMN_TYPE %s\n"_Stz,
                         aCT_long[pColumn->eColumnType()]);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_INDEXDEF\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_RLU_INDEXDEF_ADDCOLUMN\n"_Stz);
      break;
    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_STRING \"str%lld\"\n"_Stz, iLocal);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_COLUMN_TYPE %s\n"_Stz,
                         aCT_long[pColumn->eColumnType()]);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_INDEXDEF\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_RLU_INDEXDEF_ADDCOLUMN\n"_Stz);
      break;
    default:
      return EC_INVALID_COMMAND;
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for logical operators                                          */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddLogical(Stz* szAsmBuffer, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame("and"_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_LGL_AND\n"_Stz);
  }
  else if (pTkn->bSame("isNull"_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
  }
  else if (pTkn->bSame("not"_Stz)) {
    ASSERT(pTkn->bSame("not"_Stz));
    StrLib::vSprintCat(szAsmBuffer, "  EO_LGL_NOT\n"_Stz);
  }
  else if (pTkn->bSame("or"_Stz)) {
    ASSERT(pTkn->bSame("not"_Stz));
    StrLib::vSprintCat(szAsmBuffer, "  EO_LGL_OR\n"_Stz);
  }
  else {
    ASSERT(pTkn->bSame("xor"_Stz));
    StrLib::vSprintCat(szAsmBuffer, "  EO_LGL_XOR\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for numerics                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddNumeric(Stz* szAsmBuffer, Token* pTkn, Column* pColumnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush((i8)DM_INT);
  StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());

//  if (bGroupBy) {
//    StrLib::vSprintCat(m_sPayLoadKey, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
//  }
//  else {
//    StrLib::vSprintCat(m_sPayLoadStore, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
//  }
  //pColumnExpr->vSet("NONE"_Stz, CT_INTEGER, 19, 0);
}

/*----------------------------------------------------------------------------*/
/* add EO code for push column                                                 */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eAddPayLoadKey(Stz* szAsmBuffer, StreamDef* pStreamDefIn, i8 i_ColumnOrdinal) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

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

  Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

  // switch on column type
  switch (pColumn->eColumnType()) {
    case CT_DECIMAL:
    case CT_FLOAT:
    case CT_INTEGER:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_SETINTELM\n"_Stz);
      break;
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_SETSTZELM\n"_Stz);
      break;
    default:
      return EC_INVALID_COMMAND;
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for pop column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPopCol(Stz* szAsmBuffer, eCT eColumnType)
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
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
      m_xTypeStack.iPop();
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);
      m_xTypeStack.iPop();
      break;

    case CT_UNKNOWN:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
      m_xTypeStack.iPop();
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for push column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPushCol(Stz* szAsmBuffer, StreamDef* pStreamDefIn, i8 i_ColumnOrdinal) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

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

  Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

  // switch on column type
  switch (pColumn->eColumnType()) {
    case CT_BOOLEAN:
      m_xTypeStack.vPush((i8) DM_BOL);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
    case CT_INTEGER:
      m_xTypeStack.vPush((i8) DM_INT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
    case CT_DECIMAL:
      m_xTypeStack.vPush((i8) DM_BCD);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
    case CT_DATE:
      m_xTypeStack.vPush((i8) DM_DTE);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
    case CT_FLOAT:
      m_xTypeStack.vPush((i8) DM_FLT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
    case CT_TIME:
      m_xTypeStack.vPush((i8) DM_TME);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
    case CT_TIMESTAMP:
      m_xTypeStack.vPush((i8) DM_TSP);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      m_xTypeStack.vPush((i8) DM_VCH);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
      break;

    case CT_UNKNOWN:
      m_xTypeStack.vPush((i8) DM_UNK);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
      break;
      break;

    default:
      RASSERT(0);
  }
}


/*----------------------------------------------------------------------------*/
/* add EO code for relational operators                                       */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddRelational(Stz* szAsmBuffer, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  if (pTkn->bSame(">"_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_GT_INTINT\n"_Stz);
  }
  else if (pTkn->bSame(">="_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_GE_INTINT\n"_Stz);
  }
  else if (pTkn->bSame("<"_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_LT_INTINT\n"_Stz);
  }
  else if (pTkn->bSame("<="_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_LE_INTINT\n"_Stz);
  }
  else if (pTkn->bSame("!="_Stz)) {
    StrLib::vSprintCat(szAsmBuffer, "  EO_NE_INTINT\n"_Stz);
  }
  else {
    ASSERT(pTkn->bSame("=="_Stz));
      StrLib::vSprintCat(szAsmBuffer, "  EO_EQ_INTINT\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for count                                                      */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateCount(Stz* szAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();

  // push result type
  m_xTypeStack.vPush(DM_INT);

  // generate code for agg initialization
  if (m_bHasGby) {

    // generate code
    StrLib::vSprintCat(sAggInit, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_JMP_TRUE COUNT_NULL\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAggInit, "  EO_JMP COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  Label COUNT_NULL\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  DeleteLabel COUNT_NULL\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER 0\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAggInit, "  Label COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  DeleteLabel COUNT_DONE\n"_Stz);

    // generate code
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ADD_INTINT\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  Label COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel COUNT_DONE\n"_Stz);

  }
  else {
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate code
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ADD_INTINT\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  Label COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel COUNT_DONE\n"_Stz);

  }


  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for count(*)                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateCountAll(Stz* szAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // push result type
  m_xTypeStack.vPush(DM_INT);

  if (m_bHasGby) {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 1);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    // generate code for count(*)
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ADD_INTINT\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
  }
  else {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate code for count(*)
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ADD_INTINT\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for max                                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateMax(Stz* szAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();
  eEO eCode = eMathOp_MaxCode[eParamType];

  // get the min value
  i8 iMinValue = iMathOp_MinValue[eParamType];

  // push result type
  m_xTypeStack.vPush(eMathOp_MaxResult[eParamType]);

  if (m_bHasGby) {

    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    // generate code for agg max
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MAX\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MAX\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(szAsmBuffer, "  Label MAX_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MAX_DONE\n"_Stz);
  }
  else {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, iMinValue);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate code for agg max
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MAX\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MAX\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MAX\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(szAsmBuffer, "  Label MAX_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MAX_DONE\n"_Stz);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for min                                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateMin(Stz* szAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();
  eEO eCode = eMathOp_MinCode[eParamType];

  // get the max value
  i8 iMaxValue = iMathOp_MaxValue[eParamType];

  // push result type
  m_xTypeStack.vPush(eMathOp_MinResult[eParamType]);

  if (m_bHasGby) {

    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    // generate code for agg min
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MIN\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MIN\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(szAsmBuffer, "  Label MIN_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MIN_DONE\n"_Stz);
  }
  else {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, iMaxValue);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate code for agg min
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MIN\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MIN\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MIN\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(szAsmBuffer, "  Label MIN_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MIN_DONE\n"_Stz);
  }
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* generate EO code for sum                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateSum(Stz* sAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();
  eDM eResultType = eMathOp_SumResult[eParamType];
  eEO eCode = eMathOp_SumCode[eParamType];

  if (DM_NONE == eResultType)
    return EC_INVALID_COMMAND;

  // push result type
  m_xTypeStack.vPush(eResultType);

  if (m_bHasGby) {
    // generate code for agg initialization
    //StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    // generate the code
    StrLib::vSprintCat(sAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(sAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
  }
  else {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate the code
    StrLib::vSprintCat(sAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(sAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
  }
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* add EO code for strings                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddString(Stz* szAsmBuffer, Token* pTkn, Column* pColumnExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  char* szToken = (char*)pTkn->szChars();

  m_xTypeStack.vPush((i8)DM_VCH);
  StrLib::vSprintCat(szAsmBuffer, "  EO_STR_LITERAL %s\n"_Stz, szToken);
  //pColumnExpr->vSet("NONE"_Stz, CT_VARCHAR, 128, 0);    // ALKA: come back to this
}

/*----------------------------------------------------------------------------*/
/* add EO code for unknown                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddUnknown(Stz* szAsmBuffer, Token* pTkn, Column* pColumnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush((i8)DM_UNK);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSHNULL\n"_Stz);
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

    if ( pTkn->bIsName() &&
         (pTkn->bSame("AVG"_Stz)   |
          pTkn->bSame("COUNT"_Stz) ||
          pTkn->bSame("COUNTALL"_Stz) ||
          pTkn->bSame("MIN"_Stz)   ||
          pTkn->bSame("MAX"_Stz)   ||
          pTkn->bSame("SUM"_Stz))
        ) {
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

  eCT eExprType = CT_NONE;
  eAG eAggType  = AG_NONE;
  eEC eError    = EC_NONE;

  s32 sColumnNm;

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  // start with col1 as the first column
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn+1);
  pColumnExpr->vColumnNm(sColumnNm);
  bool bProject = false;

  Stz* szAsmBuffer;

  // check pStreamDefOut
  if (pStreamDefOut) {
    bProject = true;
    szAsmBuffer = m_sProjectAsm;
  }
  else {
    szAsmBuffer = m_sFilterAsm;
  }

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      m_iDstColumn++;

      RASSERT(bProject);

      // discovered a comma, generate code for the column 
      eError = eGenerateColumnCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn+1);
      pColumnExpr->vColumnNm(sColumnNm);

      pTkn = pTknVec->pNextToken();
      continue;
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
      vAddArithmetic(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // logical operator ?
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {
      vAddLogical(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // relational operator ?
    if (pTkn->bIsRelational()) {
      vAddRelational(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a numeric ?
    if (pTkn->bIsNumeric()) {
      vAddNumeric(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      eError = eAddDecimal(szAsmBuffer, pTkn, pColumnExpr);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {
      vAddString(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a tag ?
    if (pTkn->eToken() == TN_TAG && pTkn->bSame("<>"_Stz)) {
      StrLib::vSprintCat(szAsmBuffer, "  EO_RETURN_FALSE\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // should be a name
    if (!pTkn->bIsName()) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }

    // is it a negate op ?
    if (pTkn->bSame("negate"_Stz) ) {
      vAddArithmetic(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // it is a null value ?
    if (pTkn->bSame("null"_Stz) ) {
      vAddUnknown(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        eError = eAddDate(szAsmBuffer, pTkn, pColumnExpr);
        if (eError == EC_INVALID_COMMAND) {
          goto L_RETURN;
        }
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bSame("AVG"_Stz) ) {

      eAggType = AG_AVG;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {

      eAggType = AG_CNT;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz) ) {

      eAggType = AG_ALL;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz) ) {

      eAggType = AG_MIN;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz) ) {

      eAggType = AG_MAX;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz) ) {

      eAggType = AG_SUM;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz) ) {

      // Never Reach
      RASSERT(0);

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

    vAddPushCol(szAsmBuffer, pStreamDefIn, i_ColumnOrdinal);

    pTkn = pTknVec->pNextToken();
  }

  m_iDstColumn++;

  // add the last column to the streamDefOut
  if ( bProject )
    eError = eGenerateColumnCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);

  L_RETURN:
    //RASSERT(m_xTypeStack.nItems() == 0);
    delete pColumnExpr;
    return eError;
}

/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEvalGby(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);
  UNUSED(pStreamDefIn);

  eCT eExprType = CT_NONE;
  eAG eAggType  = AG_NONE;
  eEC eError    = EC_NONE;

  s32 sColumnNm;

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  // start with col1 as the first column
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn+1);
  pColumnExpr->vColumnNm(sColumnNm);

  bool bGroupBy = false;

  Stz* szAsmBuffer;
  Stz* szAggInitBuffer;

  // should have pStreamDefOut
  RASSERT(pStreamDefOut);

  szAsmBuffer = m_sPayLoadStoreAsm;

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  while ( pTkn ) {

    vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      m_iDstColumn++;

      // discovered a comma, generate code for the column
      eError = eGenerateColumnCodeGby(pStreamDefOut, pColumnExpr, eAggType, bGroupBy);
      eAggType = AG_NONE;

      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn+1);
      pColumnExpr->vColumnNm(sColumnNm);

      pTkn = pTknVec->pNextToken();
      continue;
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
      vAddArithmetic(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // logical operator ?
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {
      vAddLogical(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // relational operator ?
    if (pTkn->bIsRelational()) {
      vAddRelational(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a numeric ?
    if (pTkn->bIsNumeric()) {
      vAddNumeric(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      eError = eAddDecimal(szAsmBuffer, pTkn, pColumnExpr);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {
      vAddString(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a tag ?
    if (pTkn->eToken() == TN_TAG && pTkn->bSame("<>"_Stz)) {
      StrLib::vSprintCat(szAsmBuffer, "  EO_RETURN_FALSE\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // should be a name
    if (!pTkn->bIsName()) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }

    // is it a negate op ?
    if (pTkn->bSame("negate"_Stz) ) {
      vAddArithmetic(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // it is a null value ?
    if (pTkn->bSame("null"_Stz) ) {
      vAddUnknown(szAsmBuffer, pTkn, pColumnExpr);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        eError = eAddDate(szAsmBuffer, pTkn, pColumnExpr);
        if (eError == EC_INVALID_COMMAND) {
          goto L_RETURN;
        }
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bSame("AVG"_Stz) ) {

      eAggType = AG_AVG;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {

      eAggType = AG_CNT;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz) ) {

      eAggType = AG_ALL;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz) ) {

      eAggType = AG_MIN;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz) ) {

      eAggType = AG_MAX;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz) ) {

      eAggType = AG_SUM;
      ASSERT(m_bHasAgg);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz) ) {

      ASSERT(m_bHasGby);
      m_iDstColumn++;

      eError = eGenerateColumnCodeGby(pStreamDefOut, pColumnExpr, eAggType, bGroupBy);
      eAggType = AG_NONE;
      bGroupBy = true;

      StrLib::vSprintCat(m_sPayLoadKeyAsm, "  EO_PUSH_DEVIXT_TMP\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadKeyAsm, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(m_sPayLoadKeyAsm, "  EO_ENTWRK_SETENTKND\n"_Stz);

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

    if (bGroupBy) {
      eAddGbyIdxCol(m_sGbyIdxAsm, pStreamDefIn, i_ColumnOrdinal);
      vAddPushCol(m_sPayLoadKeyAsm, pStreamDefIn, i_ColumnOrdinal);
      eAddPayLoadKey(m_sPayLoadKeyAsm, pStreamDefIn, i_ColumnOrdinal);
    }
    else {
      vAddPushCol(m_sPayLoadStoreAsm, pStreamDefIn, i_ColumnOrdinal);
      vAddPushCol(m_sPayLoadAggAsm, pStreamDefIn, i_ColumnOrdinal);
    }
    pTkn = pTknVec->pNextToken();
  }

  L_RETURN:
  //RASSERT(m_xTypeStack.nItems() == 0);
  delete pColumnExpr;
  return eError;
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
/* generate code for column                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateColumnCode(Stz* szAsmBuffer, StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType) {

  eEC eError = EC_NONE;

  switch (eAggType) {
    case AG_ALL:
      eError = eGenerateCountAll(szAsmBuffer, m_sAggInitAsm);
      break;
    case AG_CNT:
      eError = eGenerateCount(szAsmBuffer, m_sAggInitAsm);
      break;
    case AG_MIN:
      eError = eGenerateMin(szAsmBuffer,m_sAggInitAsm);
      break;
    case AG_MAX:
      eError = eGenerateMax(szAsmBuffer,m_sAggInitAsm);
      break;
    case AG_SUM:
      eError = eGenerateSum(szAsmBuffer, m_sAggInitAsm);
      break;
    default:
      vAddPopCol(szAsmBuffer, pColumn->eColumnType());
      break;
  }

  pStreamDefOut->vAddColumn(pColumn->szColNm(),
                            pColumn->eColumnType(),
                            pColumn->iColumnLength(),
                            pColumn->iColumnScale());

  return eError;
}

/*----------------------------------------------------------------------------*/
/* generate code for column                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateColumnCodeGby(StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType, bool bGroupBy) {

  eEC eError = EC_NONE;

  switch (eAggType) {
    case AG_ALL:
      eError = eGenerateCountAll(m_sPayLoadAggAsm, m_sPayLoadStoreAsm);
      break;
    case AG_CNT:
      eError = eGenerateCount(m_sPayLoadAggAsm, m_sPayLoadStoreAsm);
      break;
    case AG_MIN:
      eError = eGenerateMin(m_sPayLoadAggAsm,m_sPayLoadStoreAsm);
      break;
    case AG_MAX:
      eError = eGenerateMax(m_sPayLoadAggAsm,m_sPayLoadStoreAsm);
      break;
    case AG_SUM:
      eError = eGenerateSum(m_sPayLoadAggAsm, m_sPayLoadStoreAsm);
      break;
    default:
      if ( ! bGroupBy ) {
        vAddStoreCol(m_sPayLoadAggAsm, pColumn);
        vAddStoreCol(m_sPayLoadStoreAsm, pColumn);
      }
      break;
  }

  if ( ! bGroupBy ) {
    vAddToBufferCol(m_sPayLoadToBufferAsm, pColumn);
    m_nPayLoadBytes += 8;

    pStreamDefOut->vAddColumn(pColumn->szColNm(),
                              pColumn->eColumnType(),
                              pColumn->iColumnLength(),
                              pColumn->iColumnScale());
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column                                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddStoreCol(Stz* szAsmBuffer, Column* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (pColumn->eColumnType()) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLS %lld\n"_Stz, m_iDstColumn);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column                                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddToBufferCol(Stz* szAsmBuffer, Column* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (pColumn->eColumnType()) {
    case CT_INTEGER:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
      StrLib::vSprintCat(szAsmBuffer,"  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
      StrLib::vSprintCat(szAsmBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer,"  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, m_iDstColumn);
      StrLib::vSprintCat(szAsmBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer,"  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ExprEval);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
