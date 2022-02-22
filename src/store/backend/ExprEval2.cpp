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
                                 IntStack* pAggTypeStack, eEK& eExprKind, Stz* sExpr, u8* puSelectColumns)
{
  _static_;

  eCT eColExprType = CT_NONE;
  eAG eAggType     = AG_NONE;
  eEC eError       = EC_NONE;

  Stz *sOrdinalColumn;
  s32 sColumnNm;
  i8 iProjectColumn = 1;
  i8 iGroupByCol = C_UNKNOWN;

  bool bGroupByKey = false;
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

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // comma
    if (pTkn->bSame(","_Stz)) {

        if (eExprKind != EK_AGGGBY && eExprKind != EK_ORDAGG) {
          vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);

          sColumnNm.vSprint("COL%lld"_Stz, iProjectColumn++);
          pColumnExpr->vColumnNm(sColumnNm);

          if (bGroupByKey)
            bGroupByKey = false;
          else
            pAggTypeStack->vPush(eAggType);

          eAggType = AG_NONE;
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
      eAggType = AG_CNT;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("COUNTALL"_Stz)) {
      eExprKind = EK_AGGREG;
      eAggType = AG_ALL;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MIN"_Stz)) {
      eExprKind = EK_AGGREG;
      eAggType = AG_MIN;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("MAX"_Stz)) {
      eExprKind = EK_AGGREG;
      eAggType = AG_MAX;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("SUM"_Stz)) {
      eExprKind = EK_AGGREG;
      eAggType = AG_SUM;
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bSame("CAST"_Stz)) {
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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a GROUPBY ?
    if (pTkn->bSame("GROUPBY"_Stz)) {

      // add the last column to the streamDefOut
      vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
      pAggTypeStack->vPush(eAggType);

      eExprKind = EK_AGGGBY;
      pIndexDef->vIndexNm("gby_idx"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is it a ORD_GROUPBY ?
    if (pTkn->bSame("ORD_GROUPBY"_Stz)) {

      // add the last column to the streamDefOut
      vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);
      pAggTypeStack->vPush(eAggType);

      eExprKind = EK_ORDAGG;
      pIndexDef->vIndexNm("gby_idx"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (pTkn->bIsName()) {

      if (eExprKind == EK_AGGGBY) {
        iGroupByCol++;

        // extract column name
        if (pTkn->szChars()->bContains("$"_Stz)) {
          // get the column ordinal
          Stz* sOrdinalColumn = pTkn->szChars() + 1;
          i8 i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            return eError;
          }
          Column* pColumn = pStreamDefIn->pGetColumn(i_ColumnOrdinal);
          pColumnExpr->vColumnNm(pColumn->szColNm());

          // set bit on in the select list as well
          BitLib::vSetOn(*puSelectColumns, i_ColumnOrdinal);
        }
        else
          pColumnExpr->vColumnNm(pTkn->szChars());
      }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // set bit for ordinal columns not already encountered
      else if (pTkn->bIsName() && pTkn->szChars()->bContains("$"_Stz)) {

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
          pTkn = pTknVec->pNextToken();
          bGroupByKey = true;

          sOrdinalColumn = pTkn->szChars() + 2;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          vAppendColumnToIndexDef(pIndexDef, pColumnExpr);

          continue;
        }

        // column ordinal relative, ordered aggregate relative to the group by key
        if (pTkn->szChars()->bContains("O"_Stz)) {
          pTkn = pTknVec->pNextToken();

          sOrdinalColumn = pTkn->szChars() + 2;
          i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

          // look for the column in the streamDefLft
          i8 nColumns = pStreamDefIn->nColumns();
          if (i_ColumnOrdinal > nColumns) {
            eError = EC_INVALID_COMMAND;
            break;
          }

          vAppendColumnToIndexDef(pIndexDef, pColumnExpr);

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
        // should be a symbolic column name
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

    pTkn = pTknVec->pNextToken();
  }

//  if (eExprKind == EK_AGGGBY) {
//    vAppendColumnToIndexDef(pIndexDef, pColumnExpr);
//  }
//  else {

  if (eExprKind != EK_AGGGBY && eExprKind != EK_ORDAGG) {
    // add the last column to the streamDefOut
    vAppendColumnToStreamDef(pStreamDefOut, pColumnExpr);

    if ( ! bGroupByKey)
      pAggTypeStack->vPush(eAggType);
  }

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
