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

  //m_sAggAsm;

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
  else if (m_bHasAgg) {
    // Its a filter with agg (invalid)
    if (!pStreamDefOut)
      return EC_INVALID_COMMAND;
    eError = eExprEvalAgg(pStreamDefIn, pStreamDefOut, pTknVec);
  }
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
void ExprEval::vAddDate(Stz* szAsmBuffer, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush(DM_DTE);
  StrLib::vSprintCat(szAsmBuffer, "  EO_DATE \"%s\"\n"_Stz, pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add EO code for decimals                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddDecimal(Stz* szAsmBuffer, Token* pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  i8 iDecimal;

  char* szToken = (char*)pTkn->szChars();

  bool bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 19, 2);
  RASSERT(bResult);
  m_xTypeStack.vPush(DM_BCD);
  StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER %lld\n"_Stz, iDecimal);
}

/*----------------------------------------------------------------------------*/
/* add EO code for gby idx col                                                */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddGbyIdxCol(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  StrLib::vSprintCat(szAsmBuffer, "  EO_STRING \"col_local%lld\"\n"_Stz, i_Local);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_COLUMN_TYPE %s\n"_Stz,
                     aCT_long[eColumnType]);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_INDEXDEF\n"_Stz);
  StrLib::vSprintCat(szAsmBuffer, "  EO_RLU_INDEXDEF_ADDCOLUMN\n"_Stz);
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
    ASSERT(pTkn->bSame("or"_Stz));
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
void ExprEval::vAddInteger(Stz* szAsmBuffer, Token* pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush((i8)DM_INT);
  StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
}

/*----------------------------------------------------------------------------*/
/* add EO code for push column                                                */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPayLoadKey(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
    case CT_INTEGER:
    case CT_TIME:
    case CT_TIMESTAMP:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_SETINTELM\n"_Stz);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_SETSTZELM\n"_Stz);
      break;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_UNKNOWN:
    default:
      NEVER_REACH;
  }
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

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for push column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPushCol(Stz* szAsmBuffer, eCT eColumnType, i8 i_Local) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);



  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
      m_xTypeStack.vPush((i8) DM_BOL);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_INTEGER:
      m_xTypeStack.vPush((i8) DM_INT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_DECIMAL:
      m_xTypeStack.vPush((i8) DM_BCD);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_DATE:
      m_xTypeStack.vPush((i8) DM_DTE);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_FLOAT:
      m_xTypeStack.vPush((i8) DM_FLT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_TIME:
      m_xTypeStack.vPush((i8) DM_TME);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_TIMESTAMP:
      m_xTypeStack.vPush((i8) DM_TSP);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      m_xTypeStack.vPush((i8) DM_VCH);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALS %lld\n"_Stz, i_Local);
      break;

    case CT_UNKNOWN:
      m_xTypeStack.vPush((i8) DM_UNK);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
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
void ExprEval::vGenerateCount(Stz* szAsmBuffer, Stz* sAggInit)
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
}

/*----------------------------------------------------------------------------*/
/* add EO code for count(*)                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateCountAll(Stz* szAsmBuffer, Stz* sAggInit)
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
}

/*----------------------------------------------------------------------------*/
/* add EO code for max                                                        */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateMax(Stz* szAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();
  eEO eCode = eMathOp_MaxCode[eParamType];

  // get the min value
  i8 iMinValue = iMathOp_MinValue(eParamType);

  Dte dte;
  dte.vSetMin();
  i8 iMin = dte;

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
}

/*----------------------------------------------------------------------------*/
/* add EO code for min                                                        */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateMin(Stz* szAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();
  eEO eCode = eMathOp_MinCode[eParamType];

  // get the max value
  i8 iMaxValue = iMathOp_MaxValue(eParamType);

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
}

/*----------------------------------------------------------------------------*/
/* generate EO code for sum                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateSum(Stz* sAsmBuffer, Stz* sAggInit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)m_xTypeStack.iPop();
  eDM eResultType = eMathOp_SumResult[eParamType];
  eEO eCode = eMathOp_SumCode[eParamType];

  // push result type
  m_xTypeStack.vPush(eResultType);

  if (m_bHasGby) {
    // generate code for agg initialization
    //StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
   // StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

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
}

/*----------------------------------------------------------------------------*/
/* add EO code for strings                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddString(Stz* szAsmBuffer, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  char* szToken = (char*)pTkn->szChars();

  m_xTypeStack.vPush((i8)DM_VCH);
  StrLib::vSprintCat(szAsmBuffer, "  EO_STRING \"%s\"\n"_Stz, szToken);
}

/*----------------------------------------------------------------------------*/
/* add EO code for time                                                       */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddTime(Stz* szAsmBuffer, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush(DM_TME);
  StrLib::vSprintCat(szAsmBuffer, "  EO_TIME \"%s\"\n"_Stz, pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add EO code for timestamp                                                  */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddTimestamp(Stz* szAsmBuffer, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush(DM_TSP);
  StrLib::vSprintCat(szAsmBuffer, "  EO_TIMESTAMP \"%s\"\n"_Stz, pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add EO code for unknown                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddUnknown(Stz* szAsmBuffer, Token* pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  m_xTypeStack.vPush((i8)DM_UNK);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSHNULL\n"_Stz);
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
         (pTkn->bSame("AVG"_Stz)   ||
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

  // start with col1 as the first column
  m_iDstColumn++;
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
  pColumnExpr->vColumnNm(sColumnNm);

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      RASSERT(bProject);

      // discovered a comma, generate code for the column 
      vGenerateColumnCode(szAsmBuffer, pStreamDefOut, pColumnExpr);

      m_iDstColumn++;
      sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
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
      vAddInteger(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      vAddDecimal(szAsmBuffer, pTkn);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {
      vAddString(szAsmBuffer, pTkn);
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
      vAddUnknown(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddDate(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // is it a time ?
    if (pTkn->bSame("TIME"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddTime(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // is it a timestamp ?
    if (pTkn->bSame("TIMESTAMP"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddTimestamp(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // found a src column
    if ( ! pTkn->szChars()->bContains("$"_Stz)) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }

    Stz *sOrdinalColumn = pTkn->szChars() + 1;
    i8 i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;
    i8 i_Local = iOrdinalToLocal(i_ColumnOrdinal);
    Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

    vAddPushCol(szAsmBuffer, pColumn->eColumnType(), i_Local);

    pTkn = pTknVec->pNextToken();
  }

  // add the last column to the streamDefOut
  if ( bProject )
    vGenerateColumnCode(szAsmBuffer, pStreamDefOut, pColumnExpr);

  L_RETURN:
    //RASSERT(m_xTypeStack.nItems() == 0);
    delete pColumnExpr;
    return eError;
}

/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEvalAgg(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);
  UNUSED(pStreamDefIn);

  eCT eExprType = CT_NONE;
  eAG eAggType  = AG_NONE;
  eEC eError    = EC_NONE;

  s32 sColumnNm;

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  Stz* szAsmBuffer;

  // we should have a  pStreamDefOut
  RASSERT(pStreamDefOut);

  szAsmBuffer = m_sProjectAsm;

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  // start with col1 as the first column
  m_iDstColumn++;
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
  pColumnExpr->vColumnNm(sColumnNm);

  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
  StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      // discovered a comma, generate code for the column
      vGenerateColumnCode(szAsmBuffer, pStreamDefOut, pColumnExpr);

      szAsmBuffer = m_sProjectAsm;

      m_iDstColumn++;
      sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
      pColumnExpr->vColumnNm(sColumnNm);

      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);

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
      vAddInteger(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      vAddDecimal(szAsmBuffer, pTkn);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {
      vAddString(szAsmBuffer, pTkn);
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
      vAddUnknown(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddDate(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // is it a time ?
    if (pTkn->bSame("TIME"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddTime(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // is it a timestamp ?
    if (pTkn->bSame("TIMESTAMP"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddTimestamp(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bSame(")"_Stz) ) {

      m_bHasAgg = true;
      szAsmBuffer = m_sAggAsm;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("AVG"_Stz) ) {

      eAggType = AG_AVG;
      ASSERT(m_bHasAgg);

      eError = eGenerateAggregateCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;
      szAsmBuffer = m_sProjectAsm;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {

      eAggType = AG_CNT;
      ASSERT(m_bHasAgg);

      eError = eGenerateAggregateCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;
      szAsmBuffer = m_sProjectAsm;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz) ) {

      eAggType = AG_ALL;
      ASSERT(m_bHasAgg);

      eError = eGenerateAggregateCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;
      szAsmBuffer = m_sProjectAsm;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz) ) {

      eAggType = AG_MIN;
      ASSERT(m_bHasAgg);

      eError = eGenerateAggregateCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;
      szAsmBuffer = m_sProjectAsm;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz) ) {

      eAggType = AG_MAX;
      ASSERT(m_bHasAgg);

      eError = eGenerateAggregateCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;
      szAsmBuffer = m_sProjectAsm;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz) ) {

      eAggType = AG_SUM;
      ASSERT(m_bHasAgg);

      eError = eGenerateAggregateCode(szAsmBuffer, pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;
      szAsmBuffer = m_sProjectAsm;

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
    i8 i_Local = iOrdinalToLocal(i_ColumnOrdinal);
    Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

    vAddPushCol(szAsmBuffer, pColumn->eColumnType(), i_Local);

    pTkn = pTknVec->pNextToken();
  }

  // add the last column to the streamDefOut
  vGenerateColumnCode(szAsmBuffer, pStreamDefOut, pColumnExpr);

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
  i8 i_ColumnOrdinal = C_UNKNOWN;
  i8 i_Local = C_UNKNOWN;

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  bool bGroupBy = false;

  Stz* szAsmBuffer;
  Stz* szAggInitBuffer;

  // should have pStreamDefOut
  RASSERT(pStreamDefOut);

  szAsmBuffer = m_sPayLoadToBufferAsm;

  m_bHasAgg = false;

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  // start with col1 as the first column
  m_iDstColumn++;
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
  pColumnExpr->vColumnNm(sColumnNm);

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      // discovered a comma, generate code for the column
      vGenerateColumnCodeGby(pStreamDefOut, pColumnExpr);
      eAggType = AG_NONE;
      m_bHasAgg = false;

      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }

      vAddToBufferCol(m_sPayLoadToBufferAsm, pColumnExpr->eColumnType());

      m_iDstColumn++;
      sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
      pColumnExpr->vColumnNm(sColumnNm);

      //StrLib::vSprintCat(m_sPayLoadToBufferAsm, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);

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
      vAddInteger(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      vAddDecimal(szAsmBuffer, pTkn);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {
      vAddString(szAsmBuffer, pTkn);
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
      vAddUnknown(szAsmBuffer, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddDate(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // is it a time ?
    if (pTkn->bSame("TIME"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddTime(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    // is it a timestamp ?
    if (pTkn->bSame("TIMESTAMP"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        vAddTimestamp(szAsmBuffer, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bSame(")"_Stz) ) {

      m_bHasAgg = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("AVG"_Stz) ) {

      eAggType = AG_AVG;
      ASSERT(m_bHasAgg);

      eError = eGenerateGroupbyCode(pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {

      eAggType = AG_CNT;
      ASSERT(m_bHasAgg);

      eError = eGenerateGroupbyCode(pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz) ) {

      eAggType = AG_ALL;
      ASSERT(m_bHasAgg);

      eError = eGenerateGroupbyCode(pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz) ) {

      eAggType = AG_MIN;
      ASSERT(m_bHasAgg);

      eError = eGenerateGroupbyCode(pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz) ) {

      eAggType = AG_MAX;
      ASSERT(m_bHasAgg);

      eError = eGenerateGroupbyCode(pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz) ) {

      eAggType = AG_SUM;
      ASSERT(m_bHasAgg);

      eError = eGenerateGroupbyCode(pStreamDefOut, pColumnExpr, eAggType);
      if (eError == EC_INVALID_COMMAND) {
        goto L_RETURN;
      }
      eAggType = AG_NONE;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz) ) {

      ASSERT(m_bHasGby);
      bGroupBy = true;

      // discovered a column, generate code for the column
      vGenerateColumnCodeGby(pStreamDefOut, pColumnExpr);

      vAddToBufferCol(m_sPayLoadToBufferAsm, pColumnExpr->eColumnType());

      m_iDstColumn++;

      //vAddPushCol(m_sPayLoadStoreAsm, pStreamDefIn, m_iDstColumn);
      //vAddPushCol(m_sPayLoadStoreAsm, pStreamDefIn, m_iDstColumn);
      //vAddPushCol(m_sPayLoadAggAsm, pStreamDefIn, m_iDstColumn);

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
    i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;
    i_Local = iOrdinalToLocal(i_ColumnOrdinal);
    Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);

    if (bGroupBy) {
      (m_sGbyIdxAsm, pColumn->eColumnType(), i_Local);
      vAddPushCol(m_sPayLoadKeyAsm, pColumn->eColumnType(), i_Local);
      vAddPayLoadKey(m_sPayLoadKeyAsm, pColumn->eColumnType(), i_Local);
    }
    else {
      if (m_bHasAgg)
        vAddPushCol(m_sPayLoadAggAsm, pColumn->eColumnType(), i_Local);
      StrLib::vSprintCat(m_sPayLoadToBufferAsm, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
      vAddPushCol(m_sPayLoadStoreAsm, pColumn->eColumnType(), i_Local);
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
eEC ExprEval::eGenerateAggregateCode(Stz* szAsmBuffer, StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType) {

  eEC eError = EC_NONE;

  switch (eAggType) {
    case AG_ALL:
      vGenerateCountAll(szAsmBuffer, m_sAggInitAsm);
      break;
    case AG_CNT:
      vGenerateCount(szAsmBuffer, m_sAggInitAsm);
      break;
    case AG_MIN:
      vGenerateMin(szAsmBuffer,m_sAggInitAsm);
      break;
    case AG_MAX:
      vGenerateMax(szAsmBuffer,m_sAggInitAsm);
      break;
    case AG_SUM:
      vGenerateSum(szAsmBuffer, m_sAggInitAsm);
      break;
    default:
      RASSERT(0);
      break;
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* generate code for column                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateColumnCode(Stz* szAsmBuffer, StreamDef* pStreamDefOut, Column* pColumn) {

  vAddPopCol(szAsmBuffer, pColumn->eColumnType());

  pStreamDefOut->vAddColumn(pColumn->szColNm(),
                            pColumn->eColumnType(),
                            pColumn->iColumnLength(),
                            pColumn->iColumnScale());
}

/*----------------------------------------------------------------------------*/
/* generate code for column                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateColumnCodeGby(StreamDef* pStreamDefOut, Column* pColumn) {

  vAddStoreCol(m_sPayLoadStoreAsm, pColumn->eColumnType());

  m_nPayLoadBytes += 8;

  pStreamDefOut->vAddColumn(pColumn->szColNm(),
                            pColumn->eColumnType(),
                            pColumn->iColumnLength(),
                            pColumn->iColumnScale());
}

/*----------------------------------------------------------------------------*/
/* generate code for column                                                   */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eGenerateGroupbyCode(StreamDef* pStreamDefOut, Column* pColumn, eAG eAggType) {

  eEC eError = EC_NONE;

  switch (eAggType) {
    case AG_ALL:
      vGenerateCountAll(m_sPayLoadAggAsm, m_sPayLoadStoreAsm);
      break;
    case AG_CNT:
      vGenerateCount(m_sPayLoadAggAsm, m_sPayLoadStoreAsm);
      break;
    case AG_MIN:
      vGenerateMin(m_sPayLoadAggAsm,m_sPayLoadStoreAsm);
      break;
    case AG_MAX:
      vGenerateMax(m_sPayLoadAggAsm,m_sPayLoadStoreAsm);
      break;
    case AG_SUM:
      vGenerateSum(m_sPayLoadAggAsm, m_sPayLoadStoreAsm);
      break;
    default:
      RASSERT(0);
      break;
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column                                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddStoreCol(Stz* szAsmBuffer, eCT eColumnType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
    case CT_INTEGER:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_UNKNOWN:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLS %lld\n"_Stz, m_iDstColumn);
      break;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column                                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddToBufferCol(Stz* szAsmBuffer, eCT eColumnType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
    case CT_DECIMAL:
    case CT_DATE:
    case CT_FLOAT:
    case CT_INTEGER:
    case CT_TIME:
    case CT_TIMESTAMP:
      //StrLib::vSprintCat(szAsmBuffer,"  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
      StrLib::vSprintCat(szAsmBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer,"  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      //StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, m_iDstColumn);
      StrLib::vSprintCat(szAsmBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer,"  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);
      break;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_UNKNOWN:
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* convert ordinal to local                                                   */
/*----------------------------------------------------------------------------*/
i8 ExprEval::iOrdinalToLocal(i8 i_ColumnOrdinal) {

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
  return iLocal;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ExprEval);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
