/*============================================================================*/
/* SP_Word.cpp - Word sprint helper                              (on c stack) */
/*============================================================================*/

#include "Grids.h"

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
  
  StrLib::vSprint(m_caResult,
                  "%s %p %s %s"
                  "%s"_Stz,
                  
                  pWord->szWordNm(),
                  pWord,
                  aWK[pWord->eWord()],
                  pWord->pCategory()->szWordNm(),
                  

                  pWord->bImmediate() ? " immediate" : "",

                  C_VARGS_END
                  );
  
  if (* pWord->szExitHook()->azChar())
    StrLib::vSprintCat((Stz*)m_caResult,
                      " exitHook %s"_Stz,
                       pWord->szExitHook());

  if (* pWord->szExceptionHook()->azChar())
    StrLib::vSprintCat((Stz*)m_caResult,
                       " exceptionHook %s"_Stz,
                       pWord->szExceptionHook());
  

  return (Stz*) &m_caResult[0];
}

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
  
  if (* pWord->szExceptionHook()->azChar())
    StrLib::vSprintCat((Stz*)m_caResult,
                       " exceptionHook %s"_Stz,
                       pWord->szExceptionHook());
  
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Word::szSeeWord(Word* pWord)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  StrLib::vSprint(m_caResult,
                  "%p: Word %s %s %s"
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
  
  if (* pWord->szExceptionHook()->azChar())
    StrLib::vSprintCat((Stz*)m_caResult,
                       " exceptionHook %s"_Stz,
                       pWord->szExceptionHook());
  
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Word)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
