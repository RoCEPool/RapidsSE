/*============================================================================*/
/* SP_Charser.cpp - Charser sprint helper                        (on c stack) */
/*============================================================================*/

#include "LexicalAnalysis.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Charser::SP_Charser(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Charser::~SP_Charser(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Charser::sz(Charser* pCharser)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
    
  SP_Char spChar1;
  SP_Char spChar2;
  SP_String spBuff;
  
  i8 nLoaded = pCharser->nLoaded();
  AnnChar* aRegOne = pCharser->aRegOne();
  AnnChar* aRegTwo = pCharser->aRegTwo();
  char* azRawBuff = pCharser->azRawBuff();
  char* acRawChar = pCharser->acRawChar();

  StrLib::vSprint(m_caResult,
                  "R1 %s L%03lld C%02lld ; R2 %s L%03lld C%02lld [>%s<]"_Stz,
                  
                  spChar1.sz(aRegOne->m_cChar),
                  aRegOne->m_iLine,
                  aRegOne->m_iCol,
                  
                  spChar2.sz(aRegTwo->m_cChar),
                  aRegTwo->m_iLine,
                  aRegTwo->m_iCol,
                  
                  spBuff.szSample((Stz*)azRawBuff,(Stz*)acRawChar,-1)

                  );

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Charser)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
