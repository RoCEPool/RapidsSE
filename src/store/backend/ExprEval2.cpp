/*============================================================================*/
/* ExprEval2.cpp -- Expression Evaluator                                       */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"
#include "StreamDef.hpp"

#include "ExprEval2.hpp"
#include "SN_Dte.hpp"


/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval2::eGenerateSelectColumns(StreamDefBase* pStreamDefIn, Stz* sExpr, u8* puSelectColumns)
{
  _static_;

  eEC eError       = EC_NONE;

  Stz *sOrdinalColumn;
  s32 sColumnNm;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the expr
  TokenVec *pTknVec = AM::TOKENISE::pTokeniseSen(sExpr);

  // get the next token
  Token* pTkn = pTknVec->pNextToken();

  while ( pTkn ) {

//#ifdef WITH_DEBUG_VERSION
//    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());
//#endif

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // comma
    if (pTkn->bSame(","_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // }
    if (pTkn->bSame("}"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inList
    if (pTkn->bSame("inList"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // in
    if (pTkn->bSame("in"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // type ?
    if (pTkn->bSame("type"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsLogical() ||
        pTkn->bIsRelational() ||
        pTkn->bIsInteger() ||
        pTkn->bIsDecimal() ||
        pTkn->bIsString() ||
        pTkn->bIsArithmetic() ||
        pTkn->bSame("isNull"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore index related keywords
    if (pTkn->bIsName() &&
        (pTkn->bSame("LEFT_OUTER"_Stz) ||
         pTkn->bSame("BOUND"_Stz) ||
         pTkn->bSame("UBOUND"_Stz) ||
         pTkn->bSame("LBOUND"_Stz) ||
         pTkn->bSame("!f"_Stz) ||
         pTkn->bSame("!t"_Stz))
        ) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore functions
    if (pTkn->bSame("EXTRACT_MINUTE"_Stz) || pTkn->bSame("EXTRACT_MONTH"_Stz) || pTkn->bSame("EXTRACT_SECOND"_Stz) ||
        pTkn->bSame("EXTRACT_YEAR"_Stz)   || pTkn->bSame("INLIST"_Stz)        || pTkn->bSame("IN"_Stz)             ||
        pTkn->bSame("LIKE2"_Stz)          || pTkn->bSame("LIKE3"_Stz)          || pTkn->bSame("MATCHLEADING"_Stz)  ||
        pTkn->bSame("MATCHMIDDLE"_Stz)    || pTkn->bSame("MATCHMIDDLE1"_Stz)   || pTkn->bSame("MATCHMIDDLE2"_Stz)  ||
        pTkn->bSame("MATCHTRAILING"_Stz)  || pTkn->bSame("SUBSTRING2"_Stz)     || pTkn->bSame("SUBSTRING3"_Stz)    ||
        pTkn->bSame("EXTRACT_DAY"_Stz)    || pTkn->bSame("EXTRACT_HOUR"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore other keywords
    if (pTkn->bSame("casttype"_Stz) || pTkn->bSame("isNull"_Stz) || pTkn->bSame("null"_Stz) ||
        pTkn->bSame("BETWEEN"_Stz) || pTkn->bSame("CASE"_Stz) || pTkn->bSame("CAST"_Stz) ||
        pTkn->bSame("ELSE"_Stz) || pTkn->bSame("ENDIF"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsName()) {

      // skip index column
      if (pTkn->szChars()->bContains("#"_Stz)) {
        pTkn = pTknVec->pNextToken();
        continue;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // skip date / time / timestamp
      if (pTkn->bSame("DATE"_Stz) ||  pTkn->bSame("TIME"_Stz) || pTkn->bSame("TIMESTAMP"_Stz)) {
        pTkn = pTknVec->pNextToken();
        continue;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set bit for ordinal columns not already encountered
      if (pTkn->bIsName() && pTkn->szChars()->bContains("$"_Stz)) {

        // skip index column
        if (pTkn->szChars()->bContains("I"_Stz)) {
          pTkn = pTknVec->pNextToken();
          continue;
        }

        i8 i_ColumnOrdinal = C_UNKNOWN;
        // column ordinal relative to the left join table
        if (pTkn->szChars()->bContains("L"_Stz)) {
          // get the column ordinal
          sOrdinalColumn = pTkn->szChars() + 2;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          // select left column
          //vTrace("SELECT %lld\n"_Stz, i_ColumnOrdinal);
          BitLib::vSetOn(*puSelectColumns, i_ColumnOrdinal);
          pTkn = pTknVec->pNextToken();
          continue;
        }
        else {
          // get the column ordinal from symbolic column name
          sOrdinalColumn = pTkn->szChars() + 1;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          // select left column
          // set bit on for the column in puSelectColumns
          BitLib::vSetOn(*puSelectColumns, i_ColumnOrdinal);
          pTkn = pTknVec->pNextToken();
          continue;
        }
      } else {
        // should be a symbolic column name
        // look for the column in the streamDefLft
        Column *pColumn = pStreamDefIn->pFindColumn(pTkn->szChars());

        if (! pColumn ) {
          vTrace("Invalid Column %s\n"_Stz,pTkn->szChars());
          eError = EC_INVALID_COMMAND;
          break;
        }
        i8 iColumn = pColumn->iColumnNo();
        BitLib::vSetOn(*puSelectColumns, iColumn);
      }

    }

    pTkn = pTknVec->pNextToken();
  }


  L_RETURN:
  delete pTknVec;

  return eError;
}
/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC ExprEval2::eGenerateStreamDef(StreamDefBase* pStreamDefIn, StreamDefBase* pStreamDefOut, IndexDef* pIndexDef,
                                 IntStack* pPreExpStack, eEK& eExprKind, Stz* sExpr, u8* puSelectColumns)
{
  _static_;

  eCT eColExprType = CT_NONE;
  ePE ePreExp      = PE_EXP;
  eEC eError       = EC_NONE;

  Stz *sOrdinalColumn;
  s32 sColumnNm;
  i8 iProjectColumn = 1;
  i8 i_ColumnOrdinal = C_UNKNOWN;

  bool bGroupBy = false;
  eExprKind = EK_PROJECT;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the expr
  TokenVec *pTknVec = AM::TOKENISE::pTokeniseSen(sExpr);

  // create a new columnExpr
  Column *pColumnExpr =  new Column;

  // should have pStreamDefOut
  RASSERT(pStreamDefOut);

  // get the next token
  Token* pTkn = pTknVec->pNextToken();

  // start with col1 as the first column
  sColumnNm.vSprint("COL%lld"_Stz, iProjectColumn++);
  pColumnExpr->vColumnNm(sColumnNm);

  while ( pTkn ) {

//#ifdef WITH_DEBUG_VERSION
//    vTrace("TKN: %s\n"_Stz, pTkn->azChars());
//#endif

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a FILTER ?
    if (pTkn->bSame("FILTER"_Stz)) {

      eExprKind = EK_FILTER;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz)) {

      eExprKind = EK_GBYAGG;
      bGroupBy = true;
      ePreExp = PE_GBY;
      pIndexDef->vIndexNm("gby_idx"_Stz);

      // start with idx_col1 as the first column for indexes
      // else it can clash with COL1 of streamDefOut
      sColumnNm.vSprint("IDX_COL%lld"_Stz, iProjectColumn-1);
      pColumnExpr->vColumnNm(sColumnNm);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it an ORDGROUPBY ?
    if (pTkn->bSame("ORDGROUPBY"_Stz)) {

      eExprKind = EK_AGGORD;
      bGroupBy = true;
      ePreExp = PE_GBY;
      pIndexDef->vIndexNm("ordgby_idx"_Stz);

      // start with idx_col1 as the first column for indexes
      // else it can class with COL1 of streamDefOut
      sColumnNm.vSprint("IDX_COL%lld"_Stz, iProjectColumn-1);
      pColumnExpr->vColumnNm(sColumnNm);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a SORT ?
    if (pTkn->bSame("SORT"_Stz)) {

      eExprKind = EK_SORT;
      ePreExp = PE_SRT;
      pIndexDef->vIndexNm("srt_idx"_Stz);

      // start with idx_col1 as the first column for indexes
      // else it can class with COL1 of streamDefOut
      sColumnNm.vSprint("IDX_COL%lld"_Stz, iProjectColumn-1);
      pColumnExpr->vColumnNm(sColumnNm);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a TOP ?
    if (pTkn->bSame("TOP"_Stz)) {

      eExprKind = EK_TOP;
      ePreExp = PE_TOP;
      pIndexDef->vIndexNm("top_idx"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregates list
    if (pTkn->bSame("aggregates"_Stz)) {

      bGroupBy = false;

      vAppendColumnToIndexDef(pIndexDef, pColumnExpr);

      // set the column no in the indexDef w.r to the streamDef
      pIndexDef->vSetColSet(pIndexDef->nColumns() - 1, i_ColumnOrdinal);

      pPreExpStack->vPush(ePreExp);

      iProjectColumn = 1;
      sColumnNm.vSprint("COL%lld"_Stz, iProjectColumn++);
      pColumnExpr->vColumnNm(sColumnNm);

      ePreExp = PE_EXP;

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // comma
    if (pTkn->bSame(","_Stz)) {

      if (bGroupBy) {
        vAppendColumnToIndexDef(pIndexDef, pColumnExpr);

        // set the column no in the indexDef w.r to the streamDef
        pIndexDef->vSetColSet(pIndexDef->nColumns() - 1, i_ColumnOrdinal);

        pPreExpStack->vPush(ePreExp);

      } else if (ePreExp == PE_TOP) {
        vAppendColumnToIndexDef(pIndexDef, pColumnExpr);

        // set the column no in the indexDef w.r to the streamDef
        pIndexDef->vSetColSet(pIndexDef->nColumns() - 1, i_ColumnOrdinal);

        pPreExpStack->vPush(ePreExp);
      } else {
        vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
        pPreExpStack->vPush(ePreExp);

        ePreExp = PE_EXP;
      }

      sColumnNm.vSprint("COL%lld"_Stz, iProjectColumn++);
      pColumnExpr->vColumnNm(sColumnNm);

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // type ?
    if (pTkn->bSame("type"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      // set expr type
      eColExprType = StreamDefBase::eColumn(pTkn->szChars());
      pColumnExpr->vColumnType(eColExprType);

      // HACK!!!
      if (eColExprType == CT_DECIMAL) {
        pColumnExpr->vColumnScale(2);
        pColumnExpr->vColumnLength(19);
      }
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // type ?
    if (pTkn->bSame("casttype"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip arithmetic operator
    if (pTkn->bIsArithmetic()) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip integer
    if (pTkn->bIsInteger()) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip logical operator
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip !f / !t (early exit from logical)
    if (pTkn->bSame("!f"_Stz) || pTkn->bSame("!t"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip decimal no
    if (pTkn->bIsDecimal()) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip null value
    if (pTkn->bSame("null"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip strings
    if (pTkn->bIsString()) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip negate op
    if (pTkn->bSame("negate"_Stz) ) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ascending
    if (pTkn->bSame("ASC"_Stz)) {
      vAppendColumnToIndexDef(pIndexDef, pColumnExpr);
      // set the column no in the indexDef w.r to the streamDef
      pIndexDef->vSetColSet(pIndexDef->nColumns() - 1, i_ColumnOrdinal);

      pPreExpStack->vPush(ePreExp);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // descending
    if (pTkn->bSame("DESC"_Stz)) {
      vAppendColumnToIndexDef(pIndexDef, pColumnExpr);
      // set the column no in the indexDef w.r to the streamDef
      pIndexDef->vSetColSet(pIndexDef->nColumns() - 1, i_ColumnOrdinal);

      // set the column as descending
      pIndexDef->pGetColumn(pIndexDef->nColumns() - 1)->vIsAscending(false);
      pIndexDef->vHasDescending(true);

        pPreExpStack->vPush(ePreExp);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip date / time / timestamp
    if (pTkn->bSame("DATE"_Stz) ||
        pTkn->bSame("INTERVAL_DT"_Stz) ||
        pTkn->bSame("INTERVAL_YM"_Stz) ||
        pTkn->bSame("TIME"_Stz) ||
        pTkn->bSame("TIMESTAMP"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip relational operator
    if (pTkn->bIsRelational()) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNT"_Stz)) {
      eExprKind = EK_AGGREG;
      ePreExp = PE_CNT;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz)) {
      eExprKind = EK_AGGREG;
      ePreExp = PE_ALL;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz)) {
      eExprKind = EK_AGGREG;
      ePreExp = PE_MIN;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz)) {
      eExprKind = EK_AGGREG;
      ePreExp = PE_MAX;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz)) {
      eExprKind = EK_AGGREG;
      ePreExp = PE_SUM;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("BETWEEN"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_BET;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("CASE"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_CSE;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("CAST"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_CST;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("ELSE"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("ENDIF"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_DAY"_Stz)) {
      ePreExp = PE_XDY;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_HOUR"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_XHR;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_MINUTE"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_XMN;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_MONTH"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_XMT;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_SECOND"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_XSC;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_YEAR"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_XYR;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("INLIST"_Stz) || pTkn->bSame("IN"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_INX;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("LIKE2"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_LK2;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("LIKE3"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_LK3;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MATCHLEADING"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_MLE;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MATCHMIDDLE"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_MMD;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MATCHMIDDLE1"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_MM1;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MATCHMIDDLE2"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_MM2;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MATCHTRAILING"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_MTR;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUBSTRING2"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_SS2;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUBSTRING3"_Stz)) {
      if (bGroupBy) {
        eError = EC_NOT_IMPLEMENTED;
        goto L_RETURN;
      }
      ePreExp = PE_SS3;;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("THEN"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("WHEN"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame(")"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("}"_Stz)) {
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsName()) {

      if (pTkn->szChars()->bContains("#"_Stz)) {
        pTkn = pTknVec->pNextToken();
        continue;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set bit for ordinal columns not already encountered
      if (pTkn->bIsName() && pTkn->szChars()->bContains("$"_Stz)) {

        // skip index column
        if (pTkn->szChars()->bContains("I"_Stz)) {
          pTkn = pTknVec->pNextToken();
          continue;
        }

        // ignore right columns
        if (pTkn->szChars()->bContains("R"_Stz)) {
          pTkn = pTknVec->pNextToken();
          continue;
        }

        // ignore if column ordinal relative to the group by key
        if (pTkn->szChars()->bContains("K"_Stz)) {

          sOrdinalColumn = pTkn->szChars() + 2;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // get the column in the indexDef
          i8 nColumns = pIndexDef->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          // set the column no in the indexDef w.r to the streamDef
          pIndexDef->vSetColSet(i_ColumnOrdinal , iProjectColumn - 2);

          ePreExp = PE_GKY;

          pTkn = pTknVec->pNextToken();
          continue;
        }

        // column ordinal relative to the left join table
        if (pTkn->szChars()->bContains("L"_Stz)) {
          // get the column ordinal
          sOrdinalColumn = pTkn->szChars() + 2;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          // select left column
          //vTrace("SELECT %lld\n"_Stz, i_ColumnOrdinal);
          BitLib::vSetOn(*puSelectColumns, i_ColumnOrdinal);
          pTkn = pTknVec->pNextToken();
          continue;
        }
        else {
          // get the column ordinal from symbolic column name
          sOrdinalColumn = pTkn->szChars() + 1;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          // select left column
          // set bit on for the column in puSelectColumns
          BitLib::vSetOn(*puSelectColumns, i_ColumnOrdinal);
          pTkn = pTknVec->pNextToken();
          continue;
        }
      } else {
        // look for the column in the streamDefLft
        Column *pColumn = pStreamDefIn->pFindColumn(pTkn->szChars());

        if (! pColumn ) {
          vTrace("***Invalid Column %s***\n"_Stz,pTkn->szChars());
          eError = EC_INVALID_COMMAND;
          break;
        }
        i_ColumnOrdinal = pColumn->iColumnNo();
        BitLib::vSetOn(*puSelectColumns, i_ColumnOrdinal);
      }

    }

    pTkn = pTknVec->pNextToken();
  }

  // push the predicate expression
  pPreExpStack->vPush(ePreExp);

  // add the last column to the indexDef
  if (ePreExp == PE_TOP) {
    vAppendColumnToIndexDef(pIndexDef, pColumnExpr);

    // set the column no in the indexDef w.r to the streamDef
    pIndexDef->vSetColSet(pIndexDef->nColumns() - 1, i_ColumnOrdinal);
  }

  RASSERT(pColumnExpr->eColumnType() != CT_NONE);

  // add the last column to the streamDefOut
  if (ePreExp != PE_SRT && ePreExp != PE_TOP)
    vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);

  L_RETURN:

  delete pColumnExpr;
  delete pTknVec;

  return eError;
}

/*----------------------------------------------------------------------------*/
/* generate the streamDef for the join                                        */
/*----------------------------------------------------------------------------*/
eEC ExprEval2::eJoinStreamDef(StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, StreamDef* pStreamDefOut)
{
  _static_

  eEC eError    = EC_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add columns from StreamDefLft to StreamDefOut
  pStreamDefOut->vAppendStreamDef(pStreamDefLft);

  // generate right table column prefix
  s32 sColPrefix;
  vGenerateColPrefix(pStreamDefLft, sColPrefix);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add columns from StreamDefRht to StreamDefOut using colPrefix for right tablecolumns
  pStreamDefOut->vAppendStreamDef(pStreamDefRht, sColPrefix);

  return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column to streamDefOut                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval2::vAppendColumnToIndexDef(IndexDef* pIndexDef, Column* pColumn) {

  _static_

  pIndexDef->vAddColumn(pColumn->szColNm(),
                        pColumn->eColumnType());
}

/*----------------------------------------------------------------------------*/
/* add column to streamDefOut                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval2::vAppendColumnToStreamDef(StreamDefBase* pStreamDefOut, Column* pColumn) {

  _static_

  pStreamDefOut->vAddColumn(pColumn->szColNm(),
                            pColumn->eColumnType(),
                            pColumn->iColumnLength(),
                            pColumn->iColumnScale());
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ExprEval2::vGenerateColPrefix(StreamDefBase* pStreamDef, Stz* szColPrefix)
{

  _static_

  i8 iTableNo = 0;
  i8 iTablePrefix = C_UNKNOWN;

  for (i8 iCol = 0; iCol < pStreamDef->nColumns(); iCol++) {

    // get the column
    Column* pColumn = pStreamDef->pGetColumn(iCol);

    // get the column name
    Stz* szColNm = pColumn->szColNm();

    // extract the column name prefix upto .
    StrLib::vCopyTill(szColPrefix->azChar(), szColNm->azChar(), '.');

    // extract the table no from the column name prefix if any
    if (!szColPrefix->bIsEmpty() && szColPrefix->bBeginsWith("J"_Stz)) {
      iTablePrefix = StrLib::iToI8(szColPrefix->azChar() + 1);
      if (iTablePrefix > iTableNo )
        iTableNo = iTablePrefix;
    }

  }
  // generate the new column prefix
  StrLib::vSprint(szColPrefix, "J%d_"_Stz, iTableNo+1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
