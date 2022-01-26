/*============================================================================*/
/* Range.cpp -- Range values                                                  */
/*============================================================================*/

#include "BASE.h"

#include "Index.hpp"

#include "Range.hpp"
#include "eEO.h"

#include "TagConstants.h"
/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Range* STORE::_pRange(void* aAddr)
{
  Range* pRange = reinterpret_cast<Range*>(aAddr);
  
  RASSERT_TYPE(pRange,CN_RANGE);
  
  return pRange;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Range::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_RANGE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Range::operator delete(void* aAddr)
{
  Range* pRange = reinterpret_cast<Range*>(aAddr);
  
  Heaps::vFree(pRange);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Range::Range(void)
: Definition(CN_RANGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RANGE,_FOOTER_);

  //m_oTagSet
  m_eMacro = AM_NONE;

  //_checkInitialised_(CN_RANGE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_RANGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Range::~Range(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RANGE,_FOOTER_);
  
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
eEC Range::eRangeValues(Index* pIndex, Stz* sRangeColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RANGE,_FOOTER_);

  // tokenise the RangeColumns
  TokenVec *pTknVec = TokenVec::pTokeniseSen(sRangeColumns);

  eEC eError = EC_NONE;
  Token* pTkn;

  i8 iValue;
  char* sValue;

  i8 iLowerValue;
  i8 iUpperValue;

  char* sLowerValue;
  char* sUpperValue;

  eEO eLower = EO_NONE;
  eEO eUpper = EO_NONE;

  eCT eColumnType;

  // range catering to only single column indexes
  if (pIndex->pIndexDef()->nColumns() > 1) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }

  // all a project start  for each crw row
  m_oTagSet.vInsert(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_STAR"_Stz);

  // get the column type
  eColumnType = pIndex->pIndexDef()->pGetColumn(0)->eColumnType();

  pTkn = pTknVec->pNextToken();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now get the range
  while (pTkn) {

    pTkn = pTknVec->pCurrToken();

    // first token should be a value
    if (pTkn->bIsNumeric()) {

      if ( eColumnType != CT_INTEGER &&
           eColumnType != CT_DECIMAL &&
           eColumnType != CT_FLOAT   &&
           eColumnType != CT_BOOLEAN) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      iValue = pTkn->iToI8();
    }
    else if (pTkn->bIsString()) {
      sValue = pTkn->azChars();

      if ( eColumnType != CT_VARCHAR     &&
           eColumnType != CT_CHAR        &&
           eColumnType != CT_INTERVAL_DT &&
           eColumnType != CT_INTERVAL_YM &&
           eColumnType != CT_DATE        &&
           eColumnType != CT_TIMESTAMP) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

    }
    else {
      eError = EC_INVALID_COMMAND;
      break;
    }

    // get next token
    pTkn = pTknVec->pNextToken();

    // next token should be a relational operator
    if (pTkn->bSame("=="_Stz)) {
      if (eLower != EO_NONE ||
          eUpper != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eLower = EO_EQ_INTINT;
      iLowerValue = iValue;
      sLowerValue = sValue;
    }
    else if (pTkn->bSame(">="_Stz)) {
      if (eLower != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eLower = EO_GE_INTINT;
      iLowerValue = iValue;
      sLowerValue = sValue;
    }
    else if (pTkn->bSame(">"_Stz)) {
      if (eLower != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eLower = EO_GT_INTINT;
      iLowerValue = iValue;
      sLowerValue = sValue;
    }
    else if (pTkn->bSame("<"_Stz)) {
      if (eUpper != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eUpper = EO_LT_INTINT;
      iUpperValue = iValue;
      sUpperValue = sValue;
    }
    else if (pTkn->bSame("<="_Stz)) {
      if (eUpper != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eUpper = EO_LE_INTINT;
      iUpperValue = iValue;
      sUpperValue = sValue;
    }
    else {
      eError = EC_INVALID_COMMAND;
      break;
    }

    pTkn = pTknVec->pNextToken();
  }

  if (eLower == EO_EQ_INTINT) {
    if (eUpper != EO_NONE) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the macro for the range

  // equal ?
  if (eLower == EO_EQ_INTINT) {
    vInsertLowerBound(eColumnType, sValue, iValue);

    m_eMacro = AM_INDEX_EQ;
    goto L_RETURN;
  }

  // range ?
  if (eLower != EO_NONE && eUpper != EO_NONE) {

    vInsertLowerBound(eColumnType, sLowerValue, iLowerValue);
    vInsertUpperBound(eColumnType, sUpperValue, iUpperValue);

    m_oTagSet.vInsert(C_TAG_RELATIONAL_UPPER, eUpper);
    if (eLower == EO_GT_INTINT) {
      m_eMacro = AM_INDEX_RANGE_LOWER_GT;
    }
    else {
      m_eMacro = AM_INDEX_RANGE_LOWER_GE;
    }
    goto L_RETURN;
  }

  // lower bound ?
  if (eLower == EO_GT_INTINT ||
      eLower == EO_GE_INTINT) {

    vInsertLowerBound(eColumnType, sLowerValue, iLowerValue);

    if (eLower == EO_GT_INTINT)
      m_eMacro = AM_INDEX_LOWER_BOUND_GT;
    else
      m_eMacro = AM_INDEX_LOWER_BOUND_GE;
  }

  // upper bound ?
  ASSERT(eUpper != EO_NONE);

  vInsertUpperBound(eColumnType, sUpperValue, iUpperValue);

  m_oTagSet.vInsert(C_TAG_RELATIONAL_UPPER, eUpper);
  m_eMacro = AM_INDEX_UPPER_BOUND;

  L_RETURN:
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert lower bound tags                                                    */
/*----------------------------------------------------------------------------*/
void Range::vInsertLowerBound(eCT eColumnType, char* sValue, i8 iValue) {

  switch (eColumnType) {
    case CT_BOOLEAN:
    case CT_DECIMAL:
    case CT_INTEGER:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_FLOAT:
      m_oTagSet.vInsert(C_TAG_LBOUND_TYPE, "EO_INTEGER"_Stz);
      break;

    case CT_DATE:
      m_oTagSet.vInsert(C_TAG_LBOUND_TYPE, "EO_DATE"_Stz);
      break;

    case CT_CHAR:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    default:
      RASSERT(0);
  }

  if (eColumnType == CT_VARCHAR     ||
      eColumnType == CT_CHAR        ||
      eColumnType == CT_INTERVAL_DT ||
      eColumnType == CT_INTERVAL_YM ||
      eColumnType == CT_DATE        ||
      eColumnType == CT_TIMESTAMP)
    m_oTagSet.vInsert(C_TAG_LBOUND_LITERAL, (Stz *) sValue);
  else
    m_oTagSet.vInsert(C_TAG_LBOUND_LITERAL, iValue);
}

/*----------------------------------------------------------------------------*/
/* insert upper bound tags                                                    */
/*----------------------------------------------------------------------------*/
void Range::vInsertUpperBound(eCT eColumnType, char* sValue, i8 iValue) {

  switch (eColumnType) {
    case CT_BOOLEAN:
    case CT_DECIMAL:
    case CT_INTEGER:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_FLOAT:
      m_oTagSet.vInsert(C_TAG_UBOUND_TYPE, "EO_INTEGER"_Stz);
      break;

    case CT_DATE:
      m_oTagSet.vInsert(C_TAG_UBOUND_TYPE, "EO_DATE"_Stz);
      break;

    case CT_CHAR:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    default:
      RASSERT(0);
  }

  if (eColumnType == CT_VARCHAR     ||
      eColumnType == CT_CHAR        ||
      eColumnType == CT_INTERVAL_DT ||
      eColumnType == CT_INTERVAL_YM ||
      eColumnType == CT_DATE        ||
      eColumnType == CT_TIMESTAMP)
    m_oTagSet.vInsert(C_TAG_UBOUND_LITERAL, (Stz *) sValue);
  else
    m_oTagSet.vInsert(C_TAG_UBOUND_LITERAL, iValue);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Range);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
