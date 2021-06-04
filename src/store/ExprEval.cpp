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

  m_iProjections = 0;

  m_iDstColumn = C_UNKNOWN;

  //m_xTypeStack
  //m_xStoreStack
  //m_xAggStack
  
  //m_sProjectAsm;
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
    eError = eExprEvalAggGby(pStreamDefIn, pStreamDefOut, pTknVec);
  else
    eError = eExprEvalProject(pStreamDefIn, pStreamDefOut, pTknVec);

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
void ExprEval::vAddArithmetic(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  eEO eCode;
  eDM eParamType1;
  eDM eParamType2;
  eDM eResultType;

  if (pTkn->bSame("negate"_Stz)) {

    // pop one param type
    eParamType1 = (eDM) pTypeStack->iPop();

    eCode = eMathOp_NegCode[eParamType1];
    eResultType = eMathOp_NegResult[eParamType1];
  }
  else {

    // pop two param types
    eParamType2 = (eDM) pTypeStack->iPop();
    eParamType1 = (eDM) pTypeStack->iPop();

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
  pTypeStack->vPush((i8) eResultType);
}

/*----------------------------------------------------------------------------*/
/* add EO code for logical operators                                          */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddLogical(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  pTypeStack->vPush((i8)DM_BOL);

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
/* add EO code for relational operators                                       */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddRelational(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  eEO eCode;

  // pop two param types
  eDM eParamType2 = (eDM) pTypeStack->iPop();
  eDM eParamType1 = (eDM) pTypeStack->iPop();

  // get result type
  eDM eResultType = eMathOp_CmpResult[eParamType1][eParamType2];

  if (pTkn->bSame(">"_Stz)) {
    eCode = eMathOp_GtCode[eParamType1][eParamType2];
  }
  else if (pTkn->bSame(">="_Stz)) {
    eCode = eMathOp_GeCode[eParamType1][eParamType2];
  }
  else if (pTkn->bSame("<"_Stz)) {
    eCode = eMathOp_LtCode[eParamType1][eParamType2];
  }
  else if (pTkn->bSame("<="_Stz)) {
    eCode = eMathOp_LeCode[eParamType1][eParamType2];
  }
  else if (pTkn->bSame("!="_Stz)) {
    eCode = eMathOp_NeCode[eParamType1][eParamType2];
  }
  else {
    ASSERT(pTkn->bSame("=="_Stz));
    eCode = eMathOp_EqCode[eParamType1][eParamType2];
  }

  StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
  pTypeStack->vPush((i8) eResultType);
}

/*----------------------------------------------------------------------------*/
/* add column to streamDefOut                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAppendColumnToStreamDef(StreamDef* pStreamDefOut, Column* pColumn) {

  pStreamDefOut->vAddColumn(pColumn->szColNm(),
                            pColumn->eColumnType(),
                            pColumn->iColumnLength(),
                            pColumn->iColumnScale());
}

/*----------------------------------------------------------------------------*/
/* collect source ordinal columns needed for projection                       */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;
  Token* pTkn;
  bool bHasAgg = false;

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
      bHasAgg = true;
      m_bHasAgg = true;
      continue;
    }

    if (pTkn->bIsName() && pTkn->bSame(","_Stz)) {
      if (m_bHasGby) {
        m_eProjectKind[m_iProjections++] = PK_GBY;
      }
      else {
        m_eProjectKind[m_iProjections++] = bHasAgg ? PK_AGG : PK_EXP;
      }
      bHasAgg = false;
    }

    if (pTkn->bIsName() && pTkn->bSame("GROUPBY"_Stz)) {
      m_eProjectKind[m_iProjections++] = bHasAgg ? PK_AGG : PK_EXP;
      m_bHasGby = true;
      continue;
    }

    if (pTkn->bIsName() && pTkn->szChars()->bContains("$"_Stz)) {

      // is it a column relative to the key ?
      if (pTkn->szChars()->bContains("K"_Stz))
        continue;

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

  if (m_bHasGby) {
    m_eProjectKind[m_iProjections++] = PK_GBY;
  }
  else {
    m_eProjectKind[m_iProjections++] = bHasAgg ? PK_AGG : PK_EXP;
  }

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for creating the gby idx col                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vCreateGbyIdxCol(Stz* szAsmBuffer, eDM eDatum, i8 i_IdxCol) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eCT eColumnType = Column::eColumnType(eDatum);
  StrLib::vSprintCat(szAsmBuffer, "  EO_STRING \"col_local%lld\"\n"_Stz, i_IdxCol);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_COLUMN_TYPE %s\n"_Stz,
                     aCT_long[eColumnType]);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_INDEXDEF\n"_Stz);
  StrLib::vSprintCat(szAsmBuffer, "  EO_RLU_INDEXDEF_ADDCOLUMN\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add EO code for creating the pay load key                                  */
/*----------------------------------------------------------------------------*/
void ExprEval::vCreatePayLoadKey(Stz* szAsmBuffer, IntStack* pTypeStack, i8 i_Local) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eDM eDatum = (eDM)pTypeStack->iPop();

  // switch on column type
  switch (eDatum) {
    case DM_BOL:
    case DM_BCD:
    case DM_DTE:
    case DM_FLT:
    case DM_INT:
    case DM_TME:
    case DM_TSP:
    case DM_UNK:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_SETINTELM\n"_Stz);
      break;

    case DM_VCH:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_SETSTZELM\n"_Stz);
      break;

    case DM_IVL:
    case DM_IVY:
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEvalProject(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);
  UNUSED(pStreamDefIn);

  eCT eExprType = CT_NONE;
  eAG eAggType  = AG_NONE;
  eEC eError    = EC_NONE;

  s32 sColumnNm;
  i8 iProjection = 0;
  bool bInAgg = false;

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  // we should have a  pStreamDefOut
  //RASSERT(pStreamDefOut);

  Stz* szAsmBuffer = m_sProjectAsm;

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  // start with col1 as the first column
  m_iDstColumn++;
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
  pColumnExpr->vColumnNm(sColumnNm);

  //StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
  //StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);

  if ( ! pStreamDefOut ) {
    if (m_bHasAgg)
      return EC_INVALID_COMMAND;
  }

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      // discovered a comma, generate code for the column
      vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
      vPopDstCol(szAsmBuffer, &m_xTypeStack);

      szAsmBuffer = m_sProjectAsm;

      m_iDstColumn++;
      iProjection++;
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
      if (bInAgg)
        vAddArithmetic(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vAddArithmetic(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // logical operator ?
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {
      if (bInAgg)
        vAddLogical(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vAddLogical(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // relational operator ?
    if (pTkn->bIsRelational()) {
      if (bInAgg)
        vAddRelational(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vAddRelational(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a numeric ?
    if (pTkn->bIsNumeric()) {
      if (bInAgg)
        vPushInteger(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vPushInteger(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {
      if (bInAgg)
        vPushDecimal(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vPushDecimal(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {
      if (bInAgg)
        vPushString(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vPushString(m_sProjectAsm, &m_xTypeStack, pTkn);
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
      if (bInAgg)
        vAddArithmetic(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vAddArithmetic(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // it is a null value ?
    if (pTkn->bSame("null"_Stz) ) {
      if (bInAgg)
        vPushUnknown(m_sAggAsm, &m_xAggStack, pTkn);
      else
        vPushUnknown(m_sProjectAsm, &m_xTypeStack, pTkn);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {
        if (bInAgg)
          vPushDate(m_sAggAsm, &m_xAggStack, pTkn);
        else
          vPushDate(m_sProjectAsm, &m_xTypeStack, pTkn);
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
        if (bInAgg)
          vPushTime(m_sAggAsm, &m_xAggStack, pTkn);
        else
          vPushTime(m_sProjectAsm, &m_xTypeStack, pTkn);
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
        if (bInAgg)
          vPushTimestamp(m_sAggAsm, &m_xAggStack, pTkn);
        else
          vPushTimestamp(m_sProjectAsm, &m_xTypeStack, pTkn);
        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bSame(")"_Stz) ) {
      bInAgg = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("AVG"_Stz) ) {

      eAggType = AG_AVG;
      bInAgg = false;

      vGenerateAggregateCode(eAggType, m_sAggAsm, m_sAggInitAsm, &m_xAggStack);
      eAggType = AG_NONE;

      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      m_xTypeStack.vPush(eAggDatum);

      vPushDstCol(m_sProjectAsm, &m_xTypeStack);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {

      eAggType = AG_CNT;
      bInAgg = false;

      vGenerateAggregateCode(eAggType, m_sAggAsm, m_sAggInitAsm, &m_xAggStack);
      eAggType = AG_NONE;

      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      m_xTypeStack.vPush(eAggDatum);

      vPushDstCol(m_sProjectAsm, &m_xTypeStack);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz) ) {

      eAggType = AG_ALL;
      bInAgg = false;

      vGenerateAggregateCode(eAggType, m_sAggAsm, m_sAggInitAsm, &m_xAggStack);
      eAggType = AG_NONE;

      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      m_xTypeStack.vPush(eAggDatum);

      vPushDstCol(m_sProjectAsm, &m_xTypeStack);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz) ) {

      eAggType = AG_MIN;
      bInAgg = false;

      vGenerateAggregateCode(eAggType, m_sAggAsm, m_sAggInitAsm, &m_xAggStack);
      eAggType = AG_NONE;

      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      m_xTypeStack.vPush(eAggDatum);

      vPushDstCol(m_sProjectAsm, &m_xTypeStack);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz) ) {

      eAggType = AG_MAX;
      bInAgg = false;

      vGenerateAggregateCode(eAggType, m_sAggAsm, m_sAggInitAsm, &m_xAggStack);
      eAggType = AG_NONE;

      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      m_xTypeStack.vPush(eAggDatum);

      vPushDstCol(m_sProjectAsm, &m_xTypeStack);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz) ) {

      eAggType = AG_SUM;
      bInAgg = false;

      vGenerateAggregateCode(eAggType, m_sAggAsm, m_sAggInitAsm, &m_xAggStack);
      eAggType = AG_NONE;

      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      m_xTypeStack.vPush(eAggDatum);

      vPushDstCol(m_sProjectAsm, &m_xTypeStack);

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

    if (m_eProjectKind[iProjection] == PK_EXP)
      vPushLclCol(m_sProjectAsm, &m_xTypeStack, pColumn->eColumnType(), i_Local);
    else
      vPushLclCol(m_sAggAsm, &m_xAggStack, pColumn->eColumnType(), i_Local);

    pTkn = pTknVec->pNextToken();
  }

  // add the last column to the streamDefOut (if filter we don't have a streamDefOut)
  if (pStreamDefOut) {
    vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
    vPopDstCol(m_sProjectAsm, &m_xTypeStack);
  }

  L_RETURN:
//  RASSERT(m_xTypeStack.nItems() == 0);
//  RASSERT(m_xAggStack.nItems() == 0);
  delete pColumnExpr;
  return eError;
}

/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eExprEvalAggGby(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);
  UNUSED(pStreamDefIn);

  eCT eExprType = CT_NONE;
  eAG eAggType  = AG_NONE;
  eEC eError    = EC_NONE;

  s32 sColumnNm;
  i8 i_ColumnOrdinal = C_UNKNOWN;
  i8 i_Local = C_UNKNOWN;
  i8 iProjection = 0;

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  // should have pStreamDefOut
  RASSERT(pStreamDefOut);

  // get the current token
  Token* pTkn = pTknVec->pCurrToken();

  // start with col1 as the first column
  m_iDstColumn++;
  sColumnNm.vSprint("col%lld"_Stz, m_iDstColumn);
  pColumnExpr->vColumnNm(sColumnNm);

  bool bPushedLocal = false;
  bool bInAgg = false;
  eDM eKeyDatm;

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame(","_Stz)) {

      switch (m_eProjectKind[iProjection]) {
        case PK_AGG:
          vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
          vPopDstCol(m_sPayLoadToBufferAsm, &m_xTypeStack);
          m_nPayLoadBytes += 8;
          break;

        case PK_EXP: {
          vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
          eDM eExprDatum = (eDM) m_xStoreStack.iPeek(m_xStoreStack.nItems() - 1);
          vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eExprDatum);
          bPushedLocal = true;
          vPopDstCol(m_sPayLoadToBufferAsm, &m_xTypeStack);
          m_nPayLoadBytes += 8;
        }
          break;

        case PK_GBY: {
          eDM eKeyDatm = (eDM) m_xKeyStack.iPeek(m_xKeyStack.nItems() - 1);
          vCreateGbyIdxCol(m_sGbyIdxAsm, eKeyDatm, iProjection);
          vCreatePayLoadKey(m_sPayLoadKeyAsm, &m_xKeyStack, i_Local);
        }
          break;

        default:
          NEVER_REACH;
      }

      if (bPushedLocal) {
        vPayStoreCol(m_sPayLoadStoreAsm, &m_xStoreStack, pColumnExpr->eColumnType());
        bPushedLocal = false;
      }

      eAggType = AG_NONE;

      m_iDstColumn++;
      iProjection++;
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

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vAddArithmetic(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vAddArithmetic(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vAddArithmetic(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vAddArithmetic(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vAddArithmetic(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // logical operator ?
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vAddLogical(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vAddLogical(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vAddLogical(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vAddLogical(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vAddLogical(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // relational operator ?
    if (pTkn->bIsRelational()) {

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vAddRelational(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vAddRelational(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vAddRelational(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vAddRelational(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vAddArithmetic(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a numeric ?
    if (pTkn->bIsNumeric()) {

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vPushInteger(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vPushInteger(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vPushInteger(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vPushInteger(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vPushInteger(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a decimal no ?
    if (pTkn->bIsDecimalNo()) {

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vPushDecimal(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vPushDecimal(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vPushDecimal(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vPushDecimal(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vPushDecimal(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a string ?
    if (pTkn->bIsString()) {

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vPushString(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vPushString(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vPushString(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vPushString(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vPushString(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a tag ?
    if (pTkn->eToken() == TN_TAG && pTkn->bSame("<>"_Stz)) {
      StrLib::vSprintCat(m_sPayLoadStoreAsm, "  EO_RETURN_FALSE\n"_Stz);
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

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vAddArithmetic(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vAddArithmetic(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vAddArithmetic(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vAddArithmetic(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vAddArithmetic(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // it is a null value ?
    if (pTkn->bSame("null"_Stz) ) {

      switch (m_eProjectKind[iProjection]) {
        case PK_GBY:
          vPushUnknown(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
          break;
        case PK_AGG:
          if (bInAgg) {
            vPushUnknown(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            vPushUnknown(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
          }
          else {
            vPushUnknown(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
          }
          break;
        case PK_EXP:
          vPushUnknown(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
          break;
        default:
          NEVER_REACH;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a date ?
    if (pTkn->bSame("DATE"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      if (pTkn->bIsString()) {

        switch (m_eProjectKind[iProjection]) {
          case PK_GBY:
            vPushDate(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
            break;
          case PK_AGG:
            if (bInAgg) {
              vPushDate(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
              vPushDate(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
            }
            else {
              vPushDate(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
            }
            break;
          case PK_EXP:
            vPushDate(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            break;
          default:
            NEVER_REACH;
        }

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

        switch (m_eProjectKind[iProjection]) {
          case PK_GBY:
            vPushTime(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
            break;
          case PK_AGG:
            if (bInAgg) {
              vPushTime(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
              vPushTime(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
            }
            else {
              vPushTime(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
            }
            break;
          case PK_EXP:
            vPushTime(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            break;
          default:
            NEVER_REACH;
        }

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

        switch (m_eProjectKind[iProjection]) {
          case PK_GBY:
            vPushTimestamp(m_sPayLoadKeyAsm, &m_xKeyStack, pTkn);
            break;
          case PK_AGG:
            if (bInAgg) {
              vPushTimestamp(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
              vPushTimestamp(m_sPayLoadAggAsm, &m_xAggStack, pTkn);
            }
            else {
              vPushTimestamp(m_sPayLoadToBufferAsm, &m_xTypeStack, pTkn);
            }
            break;
          case PK_EXP:
            vPushTimestamp(m_sPayLoadStoreAsm, &m_xStoreStack, pTkn);
            break;
          default:
            NEVER_REACH;
        }

        pTkn = pTknVec->pNextToken();
        continue;
      }
      else
        pTkn = pTknVec->pPrevToken();
    }

    if (pTkn->bSame(")"_Stz) ) {
      bInAgg = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("AVG"_Stz) ) {
      eAggType = AG_AVG;
      ASSERT(m_bHasAgg);

      vGenerateAggregateCode(eAggType, m_sPayLoadAggAsm, m_sPayLoadStoreAsm, &m_xAggStack);
      eAggType = AG_NONE;
      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eAggDatum);
      bInAgg = false;
      bPushedLocal = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz) ) {
      eAggType = AG_CNT;
      ASSERT(m_bHasAgg);

      vGenerateAggregateCode(eAggType, m_sPayLoadAggAsm, m_sPayLoadStoreAsm, &m_xAggStack);
      eAggType = AG_NONE;
      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eAggDatum);
      bInAgg = false;
      bPushedLocal = false;
      m_xStoreStack.iPop();
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz) ) {
      eAggType = AG_ALL;
      ASSERT(m_bHasAgg);

      vGenerateAggregateCode(eAggType, m_sPayLoadAggAsm, m_sPayLoadStoreAsm, &m_xAggStack);
      eAggType = AG_NONE;
      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eAggDatum);
      bInAgg = false;
      bPushedLocal = false;
      //m_xStoreStack.iPop();
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz) ) {
      eAggType = AG_MIN;
      ASSERT(m_bHasAgg);

      vGenerateAggregateCode(eAggType, m_sPayLoadAggAsm, m_sPayLoadStoreAsm, &m_xAggStack);
      eAggType = AG_NONE;
      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eAggDatum);
      bInAgg = false;
      bPushedLocal = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz) ) {
      eAggType = AG_MAX;
      ASSERT(m_bHasAgg);

      vGenerateAggregateCode(eAggType, m_sPayLoadAggAsm, m_sPayLoadStoreAsm, &m_xAggStack);
      eAggType = AG_NONE;
      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eAggDatum);
      bInAgg = false;
      bPushedLocal = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz) ) {

      eAggType = AG_SUM;
      ASSERT(m_bHasAgg);

      vGenerateAggregateCode(eAggType, m_sPayLoadAggAsm, m_sPayLoadStoreAsm, &m_xAggStack);
      eAggType = AG_NONE;
      eDM eAggDatum = (eDM)m_xAggStack.iPop();
      vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eAggDatum);
      bInAgg = false;
      bPushedLocal = true;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz) ) {

      ASSERT(m_bHasGby);


      switch (m_eProjectKind[iProjection]) {
        case PK_AGG:
          vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
          vPopDstCol(m_sPayLoadToBufferAsm, &m_xTypeStack);
          m_nPayLoadBytes += 8;
          break;

        case PK_EXP: {
          vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
          eDM eExprDatum = (eDM) m_xStoreStack.iPeek(m_xStoreStack.nItems() - 1);
          vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eExprDatum);
          bPushedLocal = true;
          vPopDstCol(m_sPayLoadToBufferAsm, &m_xTypeStack);
          m_nPayLoadBytes += 8;
        }
          break;

        case PK_GBY:
          vCreatePayLoadKey(m_sPayLoadKeyAsm, &m_xKeyStack, i_Local);
          break;

        default:
          NEVER_REACH;
      }

      if (bPushedLocal) {
        vPayStoreCol(m_sPayLoadStoreAsm, &m_xStoreStack, pColumnExpr->eColumnType());
        bPushedLocal = false;
      }

      m_iDstColumn++;
      iProjection++;

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

    bool bRelativeToKey = false;

    Column *pColumn;
    // is it a column relative to the group by key ?
    if (pTkn->szChars()->bContains("K"_Stz)) {
      Stz *sOrdinalColumn = pTkn->szChars() + 2;
      i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;
      RASSERT(m_eProjectKind[iProjection] == PK_EXP);
      bRelativeToKey = true;
    }
    else {
      Stz *sOrdinalColumn = pTkn->szChars() + 1;
      i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;
      i_Local = iOrdinalToLocal(i_ColumnOrdinal);
      pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);
    }


    switch (m_eProjectKind[iProjection]) {
      case PK_GBY: {
        //eDM eKeyDatm = (eDM)m_xKeyStack.iPeek(m_xKeyStack.nItems()-1);
        //vCreateGbyIdxCol(m_sGbyIdxAsm, eKeyDatm, iProjection);
        vPushLclCol(m_sPayLoadKeyAsm, &m_xKeyStack, pColumn->eColumnType(), i_Local);
      }
        break;
      case PK_AGG:
        vPushLclCol(m_sPayLoadAggAsm, &m_xAggStack, pColumn->eColumnType(), i_Local);
        vPushLclCol(m_sPayLoadStoreAsm, &m_xStoreStack, pColumn->eColumnType(), i_Local);
        bPushedLocal = true;
        break;
      case PK_EXP: {
        if (bRelativeToKey) {
          vPushKeyElm(m_sPayLoadStoreAsm, &m_xStoreStack, pColumnExpr->eColumnType(), i_ColumnOrdinal);
        }
        else
          vPushLclCol(m_sPayLoadStoreAsm, &m_xStoreStack, pColumn->eColumnType(), i_Local);
//        eDM eExprDatum = (eDM) m_xStoreStack.iPeek(m_xStoreStack.nItems()-1);
//        vPayFetchCol(m_sPayLoadToBufferAsm, &m_xTypeStack, eExprDatum);
//        bPushedLocal = true;
      }
        break;
      default:
        NEVER_REACH;
    }

    pTkn = pTknVec->pNextToken();
  }

  eKeyDatm = (eDM)m_xKeyStack.iPeek(m_xKeyStack.nItems()-1);
  vCreateGbyIdxCol(m_sGbyIdxAsm, eKeyDatm, iProjection);
  vCreatePayLoadKey(m_sPayLoadKeyAsm, &m_xKeyStack, i_Local);

  L_RETURN:
//  RASSERT(m_xAggStack.nItems() == 0);
//  RASSERT(m_xTypeStack.nItems() == 0);
//  RASSERT(m_xStoreStack.nItems() == 0);
//  RASSERT(m_xKeyStack.nItems() == 0);

  delete pColumnExpr;

  return eError;
}

/*----------------------------------------------------------------------------*/
/* add EO code for count                                                      */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateCount(Stz* szAsmBuffer, Stz* sAggInit, IntStack* pTypeStack)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)pTypeStack->iPop();

  // push result type
  //pTypeStack->vPush(DM_INT);

  // generate code for agg initialization
  if (m_bHasGby) {

    // generate initialization code
    if (eParamType == DM_VCH)
      StrLib::vSprintCat(sAggInit, "  EO_ISNULL_STR\n"_Stz);
    else
      StrLib::vSprintCat(sAggInit, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_JMP_FALSE COUNT_CONTINUE\n"_Stz);

    StrLib::vSprintCat(sAggInit, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER 0\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAggInit, "  EO_JMP COUNT_DONE\n"_Stz);

    StrLib::vSprintCat(sAggInit, "  Label COUNT_CONTINUE\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  DeleteLabel COUNT_CONTINUE\n"_Stz);

    //StrLib::vSprintCat(sAggInit, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(sAggInit, "  Label COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  DeleteLabel COUNT_DONE\n"_Stz);

    // generate aggregation code
    if (eParamType == DM_VCH)
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL_STR\n"_Stz);
    else
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE COUNT_CONTINUE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP COUNT_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label COUNT_CONTINUE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel COUNT_CONTINUE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ADD_INTINT\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(szAsmBuffer, "  Label COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel COUNT_DONE\n"_Stz);

  }
  else {

    // generate initialization code
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate aggregation code
    if (eParamType == DM_VCH)
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL_STR\n"_Stz);
    else
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE COUNT_CONTINUE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_JMP COUNT_DONE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  Label COUNT_CONTINUE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel COUNT_CONTINUE\n"_Stz);

    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER 1\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_ADD_INTINT\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(szAsmBuffer, "  Label COUNT_DONE\n"_Stz);
    StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel COUNT_DONE\n"_Stz);
  }

  // push result type
  m_xAggStack.vPush(DM_INT);
}

/*----------------------------------------------------------------------------*/
/* add EO code for count(*)                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateCountAll(Stz* szAsmBuffer, Stz* sAggInit, IntStack* pTypeStack)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // push result type
  //pTypeStack->vPush(DM_INT);

  if (m_bHasGby) {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 1);
    StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
    //m_xStoreStack.iPop();

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

  // push result type
  m_xAggStack.vPush(DM_INT);
}

/*----------------------------------------------------------------------------*/
/* add EO code for max                                                        */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateMax(Stz* szAsmBuffer, Stz* sAggInit, IntStack* pTypeStack)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)pTypeStack->iPop();
  eEO eCode = eMathOp_MaxCode[eParamType];

  // push result type
  eDM eResultType = eMathOp_MaxResult[eParamType];
  //pTypeStack->vPush(eResultType);

  if (m_bHasGby) {

    if (eParamType == DM_VCH) {
      // generate code for agg initialization
      //StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLS %lld\n"_Stz, m_iDstColumn);
      //m_xStoreStack.iPop();

      // generate code for agg max
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_DUP_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, m_iDstColumn);
      StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLS %lld\n"_Stz, m_iDstColumn);

      StrLib::vSprintCat(szAsmBuffer, "  Label MAX_DONE\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MAX_DONE\n"_Stz);
    }
    else {

      // generate code for agg initialization
      //StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
      //m_xStoreStack.iPop();

      // generate code for agg max
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
  }
  else {

    if (eParamType == DM_VCH) {

      s32 sMinValue;

      // get the min value
      vMathOp_MinValue(sMinValue);

      // generate code for agg initialization
      StrLib::vSprintCat(sAggInit, "  EO_STRING \"%s\"\n"_Stz, sMinValue.azChar());
      StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);

      // generate code for agg max
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL_STR\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MAX\n"_Stz);

      //StrLib::vSprintCat(szAsmBuffer, "  EO_DUMP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DUP_STR\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLS %lld\n"_Stz, m_iDstColumn);

      //StrLib::vSprintCat(szAsmBuffer, "  EO_DUMP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);

      StrLib::vSprintCat(szAsmBuffer, "  Label MAX_DONE\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MAX_DONE\n"_Stz);
    }
    else {

      // get the min value
      i8 iMinValue = iMathOp_MinValue(eParamType);

      // generate code for agg initialization
      StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, iMinValue);
      StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

      // generate code for agg max
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

  // push result type
  m_xAggStack.vPush(eResultType);
}

/*----------------------------------------------------------------------------*/
/* add EO code for min                                                        */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateMin(Stz* szAsmBuffer, Stz* sAggInit, IntStack* pTypeStack)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)pTypeStack->iPop();
  eEO eCode = eMathOp_MinCode[eParamType];

  // push result type
  eDM eResultType = eMathOp_MinResult[eParamType];
  //pTypeStack->vPush(eResultType);

  if (m_bHasGby) {

    if (eParamType == DM_VCH) {
      // generate code for agg initialization
      //StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLS %lld\n"_Stz, m_iDstColumn);
      //m_xStoreStack.iPop();

      // generate code for agg min
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MIN\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MIN\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MIN\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DUP_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, m_iDstColumn);
      StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MIN\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MIN\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MIN\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_STORE_COLS %lld\n"_Stz, m_iDstColumn);

      StrLib::vSprintCat(szAsmBuffer, "  Label MIN_DONE\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MIN_DONE\n"_Stz);
    }
    else {

      // get the max value
      i8 iMaxValue = iMathOp_MaxValue(eParamType);

      // generate code for agg initialization
      //StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
      //m_xStoreStack.iPop();

      // generate code for agg min
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
  }
  else {

    if (eParamType == DM_VCH) {

      s32 sMaxValue;

      // get the min value
      vMathOp_MaxValue(sMaxValue);

      // generate code for agg initialization
      StrLib::vSprintCat(sAggInit, "  EO_STRING \"%s\"\n"_Stz, sMaxValue.azChar());
      StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);

      // generate code for agg max
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL_STR\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MAX\n"_Stz);

      //StrLib::vSprintCat(szAsmBuffer, "  EO_DUMP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DUP_STR\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLS %lld\n"_Stz, m_iDstColumn);

      //StrLib::vSprintCat(szAsmBuffer, "  EO_DUMP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_TRUE NEW_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP_STR\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MAX_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label NEW_MAX\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel NEW_MAX\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);

      StrLib::vSprintCat(szAsmBuffer, "  Label MAX_DONE\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel MAX_DONE\n"_Stz);
    } else {

      // get the max value
      i8 iMaxValue = iMathOp_MaxValue(eParamType);

      // generate code for agg initialization
      StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, iMaxValue);
      StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

      // generate code for agg min
      StrLib::vSprintCat(szAsmBuffer, "  EO_ISNULL\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP_FALSE CONTINUE_MIN\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  EO_POP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_JMP MIN_DONE\n"_Stz);

      StrLib::vSprintCat(szAsmBuffer, "  Label CONTINUE_MIN\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  DeleteLabel CONTINUE_MIN\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DUP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      //StrLib::vSprintCat(szAsmBuffer, "  EO_DUMP\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
      //StrLib::vSprintCat(szAsmBuffer, "  EO_DUMP\n"_Stz);
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

  // push result type
  m_xAggStack.vPush(eResultType);
}

/*----------------------------------------------------------------------------*/
/* generate EO code for sum                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vGenerateSum(Stz* sAsmBuffer, Stz* sAggInit, IntStack* pTypeStack)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // pop the param type
  eDM eParamType = (eDM)pTypeStack->iPop();
  eDM eResultType = eMathOp_SumResult[eParamType];
  eEO eCode = eMathOp_SumCode[eParamType];

  if (m_bHasGby) {
    // generate code for agg initialization
    //StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    //StrLib::vSprintCat(sAggInit, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);
    //m_xStoreStack.iPop();

    // generate the code
    StrLib::vSprintCat(sAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_JMP_FALSE CONTINUE_SUM\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_JMP SUM_DONE\n"_Stz);

    StrLib::vSprintCat(sAsmBuffer, "  Label CONTINUE_SUM\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  DeleteLabel CONTINUE_SUM\n"_Stz);

    StrLib::vSprintCat(sAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
    StrLib::vSprintCat(sAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(sAsmBuffer, "  EO_PAY_STORE_COLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(sAsmBuffer, "  Label SUM_DONE\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  DeleteLabel SUM_DONE\n"_Stz);
  }
  else {
    // generate code for agg initialization
    StrLib::vSprintCat(sAggInit, "  EO_INTEGER %lld\n"_Stz, 0);
    StrLib::vSprintCat(sAggInit, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAggInit, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    // generate the code
    StrLib::vSprintCat(sAsmBuffer, "  EO_ISNULL\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_JMP_FALSE CONTINUE_SUM\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_POP\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_JMP SUM_DONE\n"_Stz);

    StrLib::vSprintCat(sAsmBuffer, "  Label CONTINUE_SUM\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  DeleteLabel CONTINUE_SUM\n"_Stz);

    StrLib::vSprintCat(sAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(sAsmBuffer, "  %s\n"_Stz, aEO[eCode]);
    StrLib::vSprintCat(sAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);

    StrLib::vSprintCat(sAsmBuffer, "  Label SUM_DONE\n"_Stz);
    StrLib::vSprintCat(sAsmBuffer, "  DeleteLabel SUM_DONE\n"_Stz);
  }

  // push result type
  m_xAggStack.vPush(eResultType);

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
/* add EO code for fetching column from payload                               */
/*----------------------------------------------------------------------------*/
void ExprEval::vPayFetchCol(Stz* szAsmBuffer, IntStack* pTypeStack, eDM eDatum)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  switch (eDatum) {
    case DM_BOL:
    case DM_BCD:
    case DM_DTE:
    case DM_FLT:
    case DM_INT:
    case DM_TME:
    case DM_TSP:
    case DM_UNK:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLI %lld\n"_Stz, m_iDstColumn);
      break;

    case DM_VCH:
      StrLib::vSprintCat(szAsmBuffer, "  EO_PAY_FETCH_COLS %lld\n"_Stz, m_iDstColumn);
      break;

    case DM_IVL:
    case DM_IVY:
    default:
      NEVER_REACH;
  }

  pTypeStack->vPush(eDatum);
}

/*----------------------------------------------------------------------------*/
/* add EO code for store column in payload                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vPayStoreCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  eDM eDatum = (eDM)pTypeStack->iPop();

  //RASSERT(Column::eDatum(eColumnType) == eDatum);

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
/* add EO code for pop column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vPopDstCol(Stz* szAsmBuffer, IntStack* pTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPREVAL,_FOOTER_);

  eDM eDatum = (eDM)pTypeStack->iPop();
  
  // switch on column type
  switch (eDatum) {
    case DM_BOL:
    case DM_BCD:
    case DM_DTE:
    case DM_FLT:
    case DM_INT:
    case DM_TME:
    case DM_TSP:
    case DM_UNK:
      StrLib::vSprintCat(szAsmBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer,"  EO_DEVROW_POPCOLI %lld\n"_Stz, m_iDstColumn);
      break;

    case DM_VCH:
      StrLib::vSprintCat(szAsmBuffer,"  EO_PUSH_DEVROW_DST\n"_Stz);
      StrLib::vSprintCat(szAsmBuffer,"  EO_DEVROW_POPCOLS %lld\n"_Stz, m_iDstColumn);
      break;

    case DM_IVL:
    case DM_IVY:
    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* add EO code for dates                                                      */
/*----------------------------------------------------------------------------*/
void ExprEval::vPushDate(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  pTypeStack->vPush(DM_DTE);
  StrLib::vSprintCat(szAsmBuffer, "  EO_DATE \"%s\"\n"_Stz, pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add EO code for decimals                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vPushDecimal(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  i8 iDecimal;

  char* szToken = (char*)pTkn->szChars();

  bool bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 19, 2);
  RASSERT(bResult);
  pTypeStack->vPush(DM_BCD);
  StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER %lld\n"_Stz, iDecimal);
}

/*----------------------------------------------------------------------------*/
/* add EO code for push destination column                                    */
/*----------------------------------------------------------------------------*/
void ExprEval::vPushDstCol(Stz* szAsmBuffer, IntStack* pTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVROW_DST\n"_Stz);

  eDM eDatumType = (eDM)pTypeStack->iPop();

  // switch on column type
  switch (eDatumType) {
    case DM_BOL:
      pTypeStack->vPush((i8) DM_BOL);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;
    case DM_INT:
      pTypeStack->vPush((i8) DM_INT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;
    case DM_BCD:
      pTypeStack->vPush((i8) DM_BCD);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;
    case DM_DTE:
      pTypeStack->vPush((i8) DM_DTE);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;
    case DM_FLT:
      pTypeStack->vPush((i8) DM_FLT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;
    case DM_TME:
      pTypeStack->vPush((i8) DM_TME);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;
    case DM_TSP:
      pTypeStack->vPush((i8) DM_TSP);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;

    case DM_VCH:
      pTypeStack->vPush((i8) DM_VCH);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLS %lld\n"_Stz, m_iDstColumn);
      break;

    case DM_UNK:
      pTypeStack->vPush((i8) DM_UNK);
      StrLib::vSprintCat(szAsmBuffer, "  EO_DEVROW_PUSHCOLI %lld\n"_Stz, m_iDstColumn);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for numerics                                                   */
/*----------------------------------------------------------------------------*/
void ExprEval::vPushInteger(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  pTypeStack->vPush((i8)DM_INT);
  StrLib::vSprintCat(szAsmBuffer, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
}

/*----------------------------------------------------------------------------*/
/* add EO code for push group by key element                                  */
/*----------------------------------------------------------------------------*/
void ExprEval:: vPushKeyElm(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_KeyElm) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVIXT_TMP\n"_Stz);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);

  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
      pTypeStack->vPush((i8) DM_BOL);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;
    case CT_INTEGER:
      pTypeStack->vPush((i8) DM_INT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;
    case CT_DECIMAL:
      pTypeStack->vPush((i8) DM_BCD);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;
    case CT_DATE:
      pTypeStack->vPush((i8) DM_DTE);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;
    case CT_FLOAT:
      pTypeStack->vPush((i8) DM_FLT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;
    case CT_TIME:
      pTypeStack->vPush((i8) DM_TME);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;
    case CT_TIMESTAMP:
      pTypeStack->vPush((i8) DM_TSP);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMI %lld\n"_Stz, i_KeyElm);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      pTypeStack->vPush((i8) DM_VCH);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELMS %lld\n"_Stz, i_KeyElm);
      break;

    case CT_UNKNOWN:
      pTypeStack->vPush((i8) DM_UNK);
      StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PUSHELM %lld\n"_Stz, i_KeyElm);
      break;

    default:
      RASSERT(0);
  }

//  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_DEVIXT_TMP\n"_Stz);
//  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_RLU_ENTWRK\n"_Stz);
//  StrLib::vSprintCat(szAsmBuffer, "  EO_ENTWRK_PRINT\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* add EO code for push column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval:: vPushLclCol(Stz* szAsmBuffer, IntStack* pTypeStack, eCT eColumnType, i8 i_Local) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  // switch on column type
  switch (eColumnType) {
    case CT_BOOLEAN:
      pTypeStack->vPush((i8) DM_BOL);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_INTEGER:
      pTypeStack->vPush((i8) DM_INT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_DECIMAL:
      pTypeStack->vPush((i8) DM_BCD);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_DATE:
      pTypeStack->vPush((i8) DM_DTE);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_FLOAT:
      pTypeStack->vPush((i8) DM_FLT);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_TIME:
      pTypeStack->vPush((i8) DM_TME);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;
    case CT_TIMESTAMP:
      pTypeStack->vPush((i8) DM_TSP);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;

    case CT_CHAR:
    case CT_VARCHAR:
      pTypeStack->vPush((i8) DM_VCH);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALS %lld\n"_Stz, i_Local);
      break;

    case CT_UNKNOWN:
      pTypeStack->vPush((i8) DM_UNK);
      StrLib::vSprintCat(szAsmBuffer, "  EO_PUSH_LOCALI %lld\n"_Stz, i_Local);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* add EO code for strings                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval:: vPushString(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  char* szToken = (char*)pTkn->szChars();

  pTypeStack->vPush((i8)DM_VCH);
  StrLib::vSprintCat(szAsmBuffer, "  EO_STRING \"%s\"\n"_Stz, szToken);
}

/*----------------------------------------------------------------------------*/
/* add EO code for time                                                       */
/*----------------------------------------------------------------------------*/
void ExprEval:: vPushTime(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  pTypeStack->vPush(DM_TME);
  StrLib::vSprintCat(szAsmBuffer, "  EO_TIME \"%s\"\n"_Stz, pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add EO code for timestamp                                                  */
/*----------------------------------------------------------------------------*/
void ExprEval:: vPushTimestamp(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  pTypeStack->vPush(DM_TSP);
  StrLib::vSprintCat(szAsmBuffer, "  EO_TIMESTAMP \"%s\"\n"_Stz, pTkn->szChars());
}

/*----------------------------------------------------------------------------*/
/* add EO code for unknown                                                    */
/*----------------------------------------------------------------------------*/
void ExprEval:: vPushUnknown(Stz* szAsmBuffer, IntStack* pTypeStack, Token* pTkn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  pTypeStack->vPush((i8)DM_UNK);
  StrLib::vSprintCat(szAsmBuffer, "  EO_PUSHNULL\n"_Stz);
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
void ExprEval::vGenerateAggregateCode(eAG eAggType, Stz* szAsmBuffer, Stz* szAsmInitBuffer, IntStack* pTypeStack) {

  switch (eAggType) {
    case AG_ALL:
      vGenerateCountAll(szAsmBuffer, szAsmInitBuffer, pTypeStack);
      break;
    case AG_CNT:
      vGenerateCount(szAsmBuffer, szAsmInitBuffer, pTypeStack);
      break;
    case AG_MIN:
      vGenerateMin(szAsmBuffer,szAsmInitBuffer, pTypeStack);
      break;
    case AG_MAX:
      vGenerateMax(szAsmBuffer,szAsmInitBuffer, pTypeStack);
      break;
    case AG_SUM:
      vGenerateSum(szAsmBuffer, szAsmInitBuffer, pTypeStack);
      break;
    default:
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(ExprEval);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
