/*============================================================================*/
/* MemberVar.cpp - Member Variable                                            */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MemberVar,CN_MEMBERVAR)
__aNewFunction(MemberVar,CN_MEMBERVAR)
__vDeleteFunction(MemberVar,CN_MEMBERVAR)
__aFooterFunction(MemberVar)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MemberVar::MemberVar(void)
: ListNL_t<MemberVar>(CN_MEMBERVAR)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
  
  
  _checkInitialised_(CN_MEMBERVAR);
  
  _postcondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MemberVar::~MemberVar(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_MEMBERVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if term set can be recogised as a member variable                     */
/*----------------------------------------------------------------------------*/
bool MemberVar::bIsMemberVar(Stz* szClassNm, EnameSet* pTerms)
{
  _static_
  
  Stz* szTerm;
  
  i8 nTerms = pTerms->nEnames();
  
  for (i8 i_Term = 0; i_Term < nTerms; i_Term++)
    vTrace("[%lld] %s "_Stz,i_Term,pTerms->szName(i_Term)->azChar());
  vTrace("\n"_Stz);
  
  i8 i_Term = 0;
  
  szTerm = pTerms->szName(i_Term++);
  
  // -- -- -- -- -- --
  // exclude constructor function
  if (szTerm->bSame(szClassNm) &&
      pTerms->szName(i_Term)->bSame("("_Stz)) {
    return false;
  }
  
  // -- -- -- -- -- --
  // exclude destructor function
  if (szTerm->bSame("~"_Stz))
    return false;
  
  // (*) szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = pTerms->szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = pTerms->szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a function pointer
  if (szTerm->bSame("("_Stz))
    return true;

  // -- -- -- -- -- --
  // attempt to detect a member function
  szTerm = pTerms->szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return false;
    szTerm = pTerms->szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a member variable
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
