/*============================================================================*/
/* SP_TknVecStr.cpp - Token VEctor to String sprint helper       (on c stack) */
/*============================================================================*/

#include "Commands.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_TknVecStr::SP_TknVecStr(void)
: SP(CN_SPTKNVECSTR)
{
  _precondition_(_MAGIC_,CN_SPTKNVECSTR,_FOOTER_);
  
  _checkInitialised_(CN_SPTKNVECSTR);
  
  _postcondition_(_MAGIC_,CN_SPTKNVECSTR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_TknVecStr::~SP_TknVecStr(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPTKNVECSTR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint SP_TknVecStr and return Stz* to result                              */
/*----------------------------------------------------------------------------*/
Stz* SP_TknVecStr::sz(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_SPTKNVECSTR,_FOOTER_);
  
//  StrLib::vCopy(m_caResult,"");
//
//  i8 nTokens = pTknVec->nTokens();
//
//  // concat all the tokens together
//  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
//
//    Token* pToken = pTknVec->pGet(i_Token);
//
//    eTN eToken = pToken->eToken();
//
//    switch (eToken) {
//
//      case TN_INTEGER:
//        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
//        break;
//
//      case TN_NAME:
//        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
//        break;
//
//      case TN_OTHER:
//        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
//        break;
//
//      case TN_STRING:
//        StrLib::vConcat(m_caResult,"\"");
//        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
//        StrLib::vConcat(m_caResult,"\"");
//        break;
//
//      default:
//        NEVER_REACH;
//    }
//
//    StrLib::vConcat(m_caResult," ");
//  }
//
  
  return sz(pTknVec,0);
}

/*----------------------------------------------------------------------------*/
/* sprint SP_TknVecStr and return Stz* to result                              */
/*----------------------------------------------------------------------------*/
Stz* SP_TknVecStr::sz(TokenVec* pTknVec, i8 i_From, i8 i_Till)
{
  _precondition_(_MAGIC_,CN_SPTKNVECSTR,_FOOTER_);
  
  StrLib::vCopy(m_caResult,"");
  
  i8 nTokens = pTknVec->nTokens();
  if (C_UNKNOWN == i_Till)
    i_Till = nTokens - 1;
  
  // concat all the tokens together
  for (i8 i_Token = i_From; i_Token <= i_Till; i_Token++) {
    
    if (i_Token > i_From)
      StrLib::vConcat(m_caResult," ");

    Token* pToken = pTknVec->pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_INTEGER:
        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
        break;
        
      case TN_NAME:
        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
        break;
        
      case TN_OTHER:
        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
        break;
        
      case TN_STRING:
        StrLib::vConcat(m_caResult,"\"");
        StrLib::vConcat(m_caResult,pToken->szText()->aChar());
        StrLib::vConcat(m_caResult,"\"");
        break;
        
      default:
        NEVER_REACH;
    }
    
  }
  
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_TknVecStr)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
