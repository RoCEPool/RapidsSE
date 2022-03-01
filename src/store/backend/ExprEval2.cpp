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
  TokenVec *pTknVec = TokenVec::pTokeniseSen(sExpr);

  // get the next token
  Token* pTkn = pTknVec->pNextToken();

  while ( pTkn ) {

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

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
        (pTkn->bSame("LEFTOUTER"_Stz) ||
         pTkn->bSame("BOUND"_Stz) ||
         pTkn->bSame("UBOUND"_Stz) ||
         pTkn->bSame("LBOUND"_Stz))
        ) {
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
  i8 iGroupByCol = C_UNKNOWN;

  bool bGroupBy = false;
  eExprKind = EK_PROJECT;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the expr
  TokenVec *pTknVec = TokenVec::pTokeniseSen(sExpr);

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

    vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz)) {

      eExprKind = EK_AGGGBY;
      bGroupBy = true;
      ePreExp = PE_GBY;
      pIndexDef->vIndexNm("gby_idx"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregates list
    if (pTkn->bSame("aggregates"_Stz)) {

      bGroupBy = false;

      vAppendColumnToIndexDef(pIndexDef, pColumnExpr);
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
          pPreExpStack->vPush(ePreExp);

          sColumnNm.vSprint("COL%lld"_Stz, iProjectColumn++);
          pColumnExpr->vColumnNm(sColumnNm);

        }
        else {
          vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
          pPreExpStack->vPush(ePreExp);

          sColumnNm.vSprint("COL%lld"_Stz, iProjectColumn++);
          pColumnExpr->vColumnNm(sColumnNm);

          ePreExp = PE_EXP;
        }

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
    // skip negate op
    if (pTkn->bSame("negate"_Stz) ) {
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip date / time / timestamp
    if (pTkn->bSame("DATE"_Stz) ||  pTkn->bSame("TIME"_Stz) || pTkn->bSame("TIMESTAMP"_Stz)) {
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
      ePreExp = PE_BET;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("CASE"_Stz)) {
      ePreExp = PE_CSE;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("CAST"_Stz)) {
      ePreExp = PE_CST;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_DAY"_Stz)) {
      ePreExp = PE_XDY;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_HOUR"_Stz)) {
      ePreExp = PE_XHR;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_MIN"_Stz)) {
      ePreExp = PE_XMN;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_MONTH"_Stz)) {
      ePreExp = PE_XMT;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_SECOND"_Stz)) {
      ePreExp = PE_XSC;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("EXTRACT_YEAR"_Stz)) {
      ePreExp = PE_XYR;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("IN"_Stz)) {
      ePreExp = PE_INX;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("LIKE"_Stz)) {
      ePreExp = PE_LKE;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUBSTRING2"_Stz)) {
      ePreExp = PE_SS2;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUBSTRING3"_Stz)) {
      ePreExp = PE_SS3;
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

        i8 i_ColumnOrdinal = C_UNKNOWN;

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
          pIndexDef->vSetColSet(i_ColumnOrdinal , iProjectColumn - 1);

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
        // could be a symbolic column name

        // look for the column in the indexDef
        Column *pGbyColumn = pIndexDef ? pIndexDef->pFindColumn(pTkn->szChars()) : nullptr;

        if ( pGbyColumn ) {
          // should already be in the streamDef
          Column *pColumn = pStreamDefIn->pFindColumn(pTkn->szChars());
          RASSERT(pColumn);

          // select bit should be on
          i8 iColumn = pColumn->iColumnNo();
          RASSERT(BitLib::bIsOn(*puSelectColumns, iColumn));

          ePreExp = PE_GKY;

        }
        else {
          // look for the column in the streamDefLft
          Column *pColumn = pStreamDefIn->pFindColumn(pTkn->szChars());

          if (! pColumn ) {
            vTrace("***Invalid Column %s***\n"_Stz,pTkn->szChars());
            eError = EC_INVALID_COMMAND;
            break;
          }
          i8 iColumn = pColumn->iColumnNo();
          BitLib::vSetOn(*puSelectColumns, iColumn);
        }

      }

    }

    pTkn = pTknVec->pNextToken();
  }

  // push the predicate expression
  pPreExpStack->vPush(ePreExp);

  // add the last column to the streamDefOut
  vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);

  L_RETURN:

  // dump the PreExp stack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("PREEXPSTACK:\n"_Stz);
  i8 nExpr = pPreExpStack->nItems() - 1;

  while ( nExpr >= 0) {
    i8 iValue = pPreExpStack->iPeek(nExpr--);
    vTrace("%s\n"_Stz, aPE[iValue]);
  }

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
  Column* pColumn = pStreamDefLft->pGetColumn(pStreamDefLft->nColumns() - 1);
  Stz* szColumnNm = pColumn->szColNm();
  vGenerateColPrefix(szColumnNm, sColPrefix);

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
void ExprEval2::vGenerateColPrefix(Stz* szColNm, Stz* szColPrefix)
{

  _static_

  i8 iTableNo = 1;

  // extract the column name prefix upto .
  StrLib::vCopyTill(szColPrefix->azChar(), szColNm->azChar(), '.');

  // extract the table no from the column name prefix if any
  if (!szColPrefix->bIsEmpty() && szColPrefix->bBeginsWith("J"_Stz)) {
    iTableNo = StrLib::iToI8(szColPrefix->azChar() + 1);
    iTableNo++;
  }

  // generate the new column prefix
  StrLib::vSprint(szColPrefix, "J%d_"_Stz, iTableNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
