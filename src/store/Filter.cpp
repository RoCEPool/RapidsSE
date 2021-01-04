/*============================================================================*/
/* Filter.cpp -- Filter Rows                                                  */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

#include "Filter.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter* STORE::_pFilter(void* aAddr)
{
  Filter* pFilter = reinterpret_cast<Filter*>(aAddr);
  
  RASSERT_TYPE(pFilter,CN_PROJECT);
  
  return pFilter;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Filter::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_PROJECT);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Filter::operator delete(void* aAddr)
{
  Filter* pFilter = reinterpret_cast<Filter*>(aAddr);
  
  HeapMem::vFree(pFilter);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter::Filter(void)
: Definition(CN_PROJECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);

  //m_sAsmText;
  
  //_checkInitialised_(CN_PROJECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter::~Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);
  
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
eEC Filter::eFilterRows(StreamDef* pStreamDefIn, Stz* sFilter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);

  // tokenise the projectColumns
  TokenVec *pTknVec = TokenVec::pTokenise(sFilter);

  eEC eError = EC_NONE;
  Token* pTkn;
  i8 i_Tkn;

  pTkn = pTknVec->pNextToken();

  // now project each destination column
  while (pTkn) {
    eError = eFilterColumn(pStreamDefIn, pTknVec);
    if (eError != EC_NONE)
       break;

    pTkn = pTknVec->pCurrToken();

    if ( pTkn && ! pTkn->bSame(","_Stz) ) {
      eError = EC_INVALID_COMMAND;
      break;
    }
    if (pTkn)
      pTkn = pTknVec->pNextToken();
  }

  L_RETRUN:
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse filter                                                               */
/*----------------------------------------------------------------------------*/
eEC Filter::eFilterColumn(StreamDef* pStreamDefIn, TokenVec* pTknVec) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PROJECT, _FOOTER_);

  eEC eError = EC_NONE;

  Column *pColumn = nullptr;
  TagSet oTagSet;

  Stz* azCrs = m_sAsmText.szStz();

  Token* pTkn = pTknVec->pCurrToken();
  bool bFound = false;
  Stz* sColumnNm;

  // first should be a column name
  if ( ! pTkn->bIsName() ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }

  // found a src column
  sColumnNm = pTkn->szChars();

  // get in column from m_iStreamInCol ~ to out column
  pColumn = nullptr;
  for (i8 i_StreamInCol = 0; i_StreamInCol < pStreamDefIn->nTotalColumns(); i_StreamInCol++) {
    pColumn = pStreamDefIn->pGetColumn(i_StreamInCol);
    if (pColumn->szColNm()->bSame(sColumnNm)) {
      bFound = true;
      break;
    }
  }

  // invalid column name
  if ( ! bFound ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }

  // code to push the column
  StrLib::vSprintCat(azCrs,"  EO_SET_ITEMS 1\n"_Stz);
  StrLib::vSprintCat(azCrs,"  EO_COL_SRC_PUSHONE %lld\n"_Stz,pColumn->iColumnNo());

  // collect the expression
  while (pTkn && ! pTkn->bIsRelational()) {

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

    // is it an integer ?
    if (pTkn->bIsInteger()) {
      StrLib::vSprintCat(azCrs, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
    }

    // is it an decimal ?
    //if (pTkn->bIsDecimal()) {
    //  StrLib::vSprintCat(azCrs, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
    //}

    pTkn = pTknVec->pNextToken();
  }

  // next token should be a relational operator
  if ( ! pTkn ) {
    eError = EC_INVALID_COMMAND;
    goto L_RETURN;
  }

  if (pTkn->bSame(">"_Stz)) {
    StrLib::vSprintCat(azCrs,"  EO_GREATERTHAN\n"_Stz);
    pTkn = pTknVec->pNextToken();
  }
  else if (pTkn->bSame(">="_Stz)) {
    StrLib::vSprintCat(azCrs,"  EO_GREATEREQUAL\n"_Stz);
    pTkn = pTknVec->pNextToken();
  }
  else {
    eError = EC_INVALID_COMMAND;
  }
  StrLib::vSprintCat(azCrs,"  EO_JMP_FALSE CRW_FILTER_FALSE\n"_Stz);
  L_RETURN:
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Filter);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
