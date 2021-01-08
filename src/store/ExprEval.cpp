/*============================================================================*/
/* ExprEval.cpp -- ExprEval column                                        */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

#include "ExprEval.hpp"

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

  Stz* azCrs = m_sAsmText.szStz();

  for (i8 iCol = 0; iCol < pStreamDefIn->nTotalColumns(); iCol++) {
    Column* pColumn = pStreamDefIn->pGetColumn(iCol);
    //Stz* azCrs = m_sAsmText[iCol].szStz();

    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {
      StrLib::vSprintCat(azCrs, "  EO_PUSH_LOCALS %lld\n"_Stz, iCol);
      StrLib::vSprintCat(azCrs, "  EO_ROW_DST_POPCOLS %lld\n"_Stz, iCol);
    }
    else {
      StrLib::vSprintCat(azCrs, "  EO_PUSH_LOCALI %lld\n"_Stz, iCol);
      StrLib::vSprintCat(azCrs, "  EO_ROW_DST_POPCOLI %lld\n"_Stz, iCol);
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
/* add EO code for pop column                                                 */
/*----------------------------------------------------------------------------*/
void ExprEval::vAddPopCol(eCT eColumnType, i8 iColumn) {

  // swtich on column type
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
/* collect source columns needed for projection                               */
/*----------------------------------------------------------------------------*/
eEC ExprEval::eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_EXPREVAL, _FOOTER_);

  eEC eError = EC_NONE;
  Token* pTkn;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);

    // skip alias ?
    if (pTkn->bSame("as"_Stz)) {
      i_Tkn++;
      pTkn = pTknVec->pGet(i_Tkn);
      continue;
    }

    // skip any arithmetic operators
    if (pTkn->bIsArithmetic())
      continue;

    // skip any logical operators
    if (pTkn->bIsLogical()) {
      continue;
    }

    // skip any numeric numbers
    if (pTkn->bIsNumeric()) {
      continue;
    }

    // skip any relational operators
    if (pTkn->bIsRelational()) {
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

    if (pTkn->bIsName() && pTkn->bSame(","_Stz)) {
      continue;
    }

    // look for the column in the streamDefIn
    Column *pColumn = nullptr;
    bool bFound = false;
    i8 nColumns = pStreamDefIn->nColumns(C_ALL);
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
  Column *pColumn = nullptr;
  Column *pColumnExpr =  new Column;
  TagSet oTagSet;

  Token* pTkn = pTknVec->pCurrToken();

  //Stz* azCrs = m_sAsmText[i_DstColumn].szStz();
  Stz* azCrs = m_sAsmText.szStz();

  while ( pTkn ) {

    if (pTkn->bSame(","_Stz)) {

      i_DstColumn++;

      // discovered a comma, add the column to the streamDefOut
      if (pStreamDefOut) {
        vUpdateStreamDefOut(pStreamDefOut, pColumn ? pColumn : pColumnExpr, sAliasName);
      }

      // add EO code for pop column
      vAddPopCol(pColumn ? pColumn->eColumnType() : pColumnExpr->eColumnType(), i_DstColumn);

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

    // arithmetic operators
    if (pTkn->bSame("+"_Stz)) {
      //vAddCode(i_DstColumn, EO_ADD);
      StrLib::vSprintCat(azCrs, "  EO_ADD\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("/"_Stz)) {
      //vAddCode(i_DstColumn, EO_DIVIDE);
      StrLib::vSprintCat(azCrs, "  EO_DIVIDE\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("%"_Stz)) {
      //vAddCode(i_DstColumn, EO_MODULO);
      StrLib::vSprintCat(azCrs, "  EO_MODULO\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("*"_Stz)) {
      //vAddCode(i_DstColumn, EO_MULTIPLY);
      StrLib::vSprintCat(azCrs, "  EO_MULTIPLY\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("-"_Stz)) {
      //vAddCode(i_DstColumn, EO_SUBTRACT);
      StrLib::vSprintCat(azCrs, "  EO_SUBTRACT\n"_Stz);
      continue;
    }

    if (pTkn->bIsRelational()) {
      if (pTkn->bSame(">"_Stz)) {
        StrLib::vSprintCat(azCrs, "  EO_GREATERTHAN\n"_Stz);
        pTkn = pTknVec->pNextToken();
        continue;
      } else if (pTkn->bSame(">="_Stz)) {
        StrLib::vSprintCat(azCrs, "  EO_GREATEREQUAL\n"_Stz);
        pTkn = pTknVec->pNextToken();
        continue;
      } else if (pTkn->bSame("<"_Stz)) {
        StrLib::vSprintCat(azCrs, "  EO_LESSTHAN\n"_Stz);
        pTkn = pTknVec->pNextToken();
        continue;
      } else if (pTkn->bSame("<="_Stz)) {
        StrLib::vSprintCat(azCrs, "  EO_LESSEQUAL\n"_Stz);
        pTkn = pTknVec->pNextToken();
        continue;
      } else if (pTkn->bSame("=="_Stz)) {
        StrLib::vSprintCat(azCrs, "  EO_EQUAL\n"_Stz);
        pTkn = pTknVec->pNextToken();
        continue;
      }
    }

    // is it an integer ?
    if (pTkn->bIsInteger()) {
      StrLib::vSprintCat(azCrs, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
      pColumnExpr->vSet("NONE"_Stz, CT_INTEGER, 19, 0);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if (!pTkn->bIsName()) {
      eError = EC_INVALID_COMMAND;
      break;
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
      if (BitLib::bEachBit(m_uExprEvalColumns, iBit))
        if (iBit < pColumn->iColumnNo())
          iLocal++;
        else
          break;
    }

    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR)
      StrLib::vSprintCat(azCrs, "  EO_PUSH_LOCALS %lld\n"_Stz, iLocal);
    else
      StrLib::vSprintCat(azCrs, "  EO_PUSH_LOCALI %lld\n"_Stz, iLocal);
    pTkn = pTknVec->pNextToken();
  }

    i_DstColumn++;

    // discovered a comma, add the column to the streamDefOut
    if (pStreamDefOut) {
      vUpdateStreamDefOut(pStreamDefOut, pColumn ? pColumn : pColumnExpr, sAliasName);
    }

    // add EO code for pop column
    vAddPopCol(pColumn ? pColumn->eColumnType() : pColumnExpr->eColumnType(), i_DstColumn);

  L_RETURN:
    delete pColumnExpr;
    return eError;
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
