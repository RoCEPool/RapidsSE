/*============================================================================*/
/* SP_Word.cpp - Word sprint helper                              (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "SP_Word.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Word::SP_Word(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Word::~SP_Word(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Word::sz(Word* pWord)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  if (! pWord) {
    StrLib::vSprint(m_caResult,"%p"_Stz,pWord);
    return (Stz*) &m_caResult[0];
  }
  
  //  COLON /__dicRoot/std_recoveryWord=0x2048061b0440 \word
  
  StrLib::vSprint(m_caResult,
                  "%p: %s /%s/%s "
                  "%s"
                  "%s"_Stz
                  
                  ,pWord
                  ,aWK[pWord->eWord()]
                  ,pWord->pDic()->szDicNm()
                  ,pWord->szWordNm()
                  
                  ,pWord->pCategory()->szWordNm()
                  
                  ,pWord->bImmediate() ? " immediate" : ""
                  
                  ,C_VARGS_END
                  );
  
  if (* pWord->szExitHook()->azChar())
    StrLib::vSprintCat((Stz*)m_caResult,
                       " exitHook=%s"_Stz,
                       pWord->szExitHook());
  
  return (Stz*) &m_caResult[0];
}
/*----------------------------------------------------------------------------*/
//Stz* SP_Word::sz(Word* pWord)
//{
//  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
//
//  StrLib::vSprint(m_caResult,
//                  "/%s/%s %p %s %s"
//                  "%s"_Stz,
//
//                  pWord->pDic()->szDicNm(),
//                  pWord->szWordNm(),
//                  pWord,
//                  aWK[pWord->eWord()],
//                  pWord->pCategory()->szWordNm(),
//
//
//                  pWord->bImmediate() ? " immediate" : "",
//
//                  C_VARGS_END
//                  );
//
//  if (* pWord->szExitHook()->azChar())
//    StrLib::vSprintCat((Stz*)m_caResult,
//                      " exitHook %s"_Stz,
//                       pWord->szExitHook());
//
//
//  return (Stz*) &m_caResult[0];
//}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Word::szSeeDoes(Word* pWord)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  StrLib::vSprint(m_caResult,
                  "%p: Does %s %s %s"
                  "%s"_Stz,
                  
                  pWord,
                  pWord->szWordNm(),
                  aWK[pWord->eWord()],
                  pWord->pCategory()->szWordNm(),
                  
                  
                  pWord->bImmediate() ? " immediate" : "",
                  
                  C_VARGS_END
                  );
  
  if (* pWord->szExitHook()->azChar())
    StrLib::vSprintCat((Stz*)m_caResult,
                       " exitHook %s"_Stz,
                       pWord->szExitHook());
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Word)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
