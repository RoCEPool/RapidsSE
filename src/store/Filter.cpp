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

  // now evaluate the filter
  eError = eFilterColumn(pStreamDefIn, pTknVec);

  L_RETRUN:
    delete pTknVec;
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
  char* szToken;
  i8 iDecimal;

  Stz* azCrs = m_sAsmText.szStz();

  Token* pTkn = pTknVec->pCurrToken();
  bool bFound = false;
  Stz* sColumnNm;

  // collect each expression
  while (pTkn) {

    // is column an integer ?
    if (pTkn->bIsInteger()) {
      // we should have a column and column type should be CT_INTEGER
      RASSERT(pColumn && pColumn->eColumnType() == CT_INTEGER);
      StrLib::vSprintCat(azCrs, "  EO_INTEGER %lld\n"_Stz, pTkn->iToI8());
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is column a decimal ?
    if (pTkn->bIsName() && StrLib::bIsDecimalNo(pTkn->azChars())) {
      // we should have a column and column type should be CT_DECIMAL
      RASSERT(pColumn && pColumn->eColumnType() == CT_DECIMAL);
      szToken = (char*)pTkn->szChars();
      bool bResult = StrLib::bExtractDecimal(&szToken, iDecimal, 15, 2);
      if ( ! bResult ) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }
      StrLib::vSprintCat(azCrs, "  EO_INTEGER %lld\n"_Stz, iDecimal);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a logical operator ?
    if (pTkn->bSame("and"_Stz)) {
      StrLib::vSprintCat(azCrs, "  EO_LGL_AND\n"_Stz);
      pTkn = pTknVec->pNextToken();
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
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->eToken() == TN_TAG && pTkn->bSame("<>"_Stz)) {
      StrLib::vSprintCat(azCrs, "EO_JMP_FALSE CRW_FILTER_FALSE\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // is it a column name
    if ( pTkn->bIsName() ) {

      // found a src column
      sColumnNm = pTkn->szChars();

      // get in column from m_iStreamInCol
      pColumn = nullptr;
      for (i8 i_StreamInCol = 0; i_StreamInCol < pStreamDefIn->nTotalColumns(); i_StreamInCol++) {
        pColumn = pStreamDefIn->pGetColumn(i_StreamInCol);
        if (pColumn->szColNm()->bSame(sColumnNm)) {
          bFound = true;
          break;
        }
      }

      // invalid column name
      if (!bFound) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      // code to push the column
      //StrLib::vSprintCat(azCrs, "  EO_COL_SRC_PUSHONE %lld\n"_Stz, pColumn->iColumnNo());
      if (pColumn->eColumnType() == CT_VARCHAR ||
          pColumn->eColumnType() == CT_CHAR)
        StrLib::vSprintCat(azCrs,"  EO_PUSH_LOCALS %lld\n"_Stz,pColumn->iColumnNo());
      else
        StrLib::vSprintCat(azCrs,"  EO_PUSH_LOCALI %lld\n"_Stz,pColumn->iColumnNo());
      pTkn = pTknVec->pNextToken();
      continue;
    }
    else {
      eError = EC_INVALID_COMMAND;
      goto L_RETURN;
    }
  }

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
