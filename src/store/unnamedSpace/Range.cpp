/*============================================================================*/
/* Range.cpp -- Range values                                                  */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

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
  void* pObject = HeapMem::pAllocate(CN_RANGE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Range::operator delete(void* aAddr)
{
  Range* pRange = reinterpret_cast<Range*>(aAddr);
  
  HeapMem::vFree(pRange);
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
  TokenVec *pTknVec = TokenVec::pTokenise(sRangeColumns);

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

  // get the column type
  eColumnType = pIndex->pIndexDef()->pGetColumn(0)->eColumnType();

  pTkn = pTknVec->pNextToken();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now get he range
  while (pTkn) {

    pTkn = pTknVec->pCurrToken();

    // first token should be a value
    if (pTkn->bIsNumeric()) {

      if ( eColumnType == CT_VARCHAR ||
          eColumnType == CT_CHAR) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      iValue = pTkn->iToI8();
    }
    else if (pTkn->bIsString()) {
      sValue = pTkn->azChars();

      if ( eColumnType != CT_VARCHAR &&
           eColumnType != CT_CHAR) {
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
      eLower = EO_EQUAL;
    }
    else if (pTkn->bSame(">="_Stz)) {
      if (eLower != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eLower = EO_GREATEREQUAL;
    }
    else if (pTkn->bSame(">"_Stz)) {
      if (eLower != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eLower = EO_GREATERTHAN;
    }
    else if (pTkn->bSame("<"_Stz)) {
      if (eUpper != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eUpper = EO_LESSTHAN;
    }
    else if (pTkn->bSame("<="_Stz)) {
      if (eUpper != EO_NONE) {
        eError = EC_INVALID_COMMAND;
        break;
      }
      eUpper = EO_LESSEQUAL;
    }
    else {
      eError = EC_INVALID_COMMAND;
      break;
    }

    if (eLower != EO_NONE) {
      iLowerValue = iValue;
      sLowerValue = sValue;
    }
    else if (eUpper != EO_NONE){
      iUpperValue = iValue;
      sUpperValue = sValue;
    }

    pTkn = pTknVec->pNextToken();
  }

  if (eLower == EO_EQUAL) {
    if (eUpper != EO_NONE) {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the macro for the range

  // equal ?
  if (eLower == EO_EQUAL) {
    if (eColumnType == CT_VARCHAR ||
        eColumnType == CT_CHAR)
      m_oTagSet.vInsert(C_TAG_LITERAL_VALUE, (Stz*)sValue);
    else
      m_oTagSet.vInsert(C_TAG_LITERAL_VALUE, iValue);

    m_eMacro = AM_INDEX_EQ;
    goto L_RETURN;
  }

  // range ?
  if (eLower != EO_NONE && eUpper != EO_NONE) {

    if (eColumnType == CT_VARCHAR ||
        eColumnType == CT_CHAR)
      m_oTagSet.vInsert(C_TAG_LBOUND_VALUE, (Stz*)sLowerValue);
    else
      m_oTagSet.vInsert(C_TAG_LBOUND_VALUE, iLowerValue);

    if (eColumnType == CT_VARCHAR ||
        eColumnType == CT_CHAR)
      m_oTagSet.vInsert(C_TAG_UBOUND_VALUE, (Stz*)sUpperValue);
    else
      m_oTagSet.vInsert(C_TAG_UBOUND_VALUE, iUpperValue);

    m_oTagSet.vInsert(C_TAG_RELATIONAL_UPPER, eUpper);
    if (eLower == EO_GREATERTHAN) {
      m_eMacro = AM_INDEX_RANGE_LOWER_GT;
    }
    else {
      m_eMacro = AM_INDEX_RANGE_LOWER_GE;
    }
    goto L_RETURN;
  }

  // lower bound ?
  if (eLower == EO_GREATERTHAN ||
      eLower == EO_GREATEREQUAL) {

    if (eColumnType == CT_VARCHAR ||
       eColumnType == CT_CHAR)
      m_oTagSet.vInsert(C_TAG_LBOUND_VALUE, (Stz*)sLowerValue);
    else
      m_oTagSet.vInsert(C_TAG_LBOUND_VALUE, iLowerValue);

    if (eLower == EO_GREATERTHAN)
      m_eMacro = AM_INDEX_LOWER_BOUND_GT;
    else
      m_eMacro = AM_INDEX_LOWER_BOUND_GE;
  }

  // upper bound ?
  ASSERT(eUpper != EO_NONE);

  if (eColumnType == CT_VARCHAR ||
      eColumnType == CT_CHAR)
    m_oTagSet.vInsert(C_TAG_UBOUND_VALUE, (Stz*)sUpperValue);
  else
    m_oTagSet.vInsert(C_TAG_UBOUND_VALUE, iUpperValue);

  m_oTagSet.vInsert(C_TAG_RELATIONAL_UPPER, eUpper);
  m_eMacro = AM_INDEX_UPPER_BOUND;

  L_RETURN:
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Range);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
