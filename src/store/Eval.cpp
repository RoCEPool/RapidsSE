/*============================================================================*/
/* Eval.cpp -- Evaluator                                                      */
/*============================================================================*/

#include "Eval.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//__pCastFunction(STORE,Eval,CN_CARD)
//__aNewFunction(Eval,CN_CARD)
//__vDeleteFunction(Eval,CN_CARD)
//__aFooterFunction(Eval)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Eval::Eval(eCN eClass)
: Object(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass);
      

  _postcondition_(_ADDRESS_,_MAGIC_,eClass);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Eval::~Eval(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate atom                                                              */
/*----------------------------------------------------------------------------*/
eAE Eval::eAtomEval(Stz* szAtom)
{
  _static_
  
       if (szAtom->bSame("and"_Stz))     return AE_AND;
  else if (szAtom->bSame("boolean"_Stz)) return AE_BOOLEAN;
  else if (szAtom->bSame("or"_Stz))      return AE_OR;
  else if (szAtom->bSame("type"_Stz))    return AE_TYPE;
  
  else if (szAtom->bSame("<"_Stz))       return AE_COMPARE;
  else if (szAtom->bSame("<="_Stz))      return AE_COMPARE;
  else if (szAtom->bSame("="_Stz))       return AE_COMPARE;
  else if (szAtom->bSame("=="_Stz))      return AE_COMPARE;
  else if (szAtom->bSame("!="_Stz))      return AE_COMPARE;
  else if (szAtom->bSame("<>"_Stz))      return AE_COMPARE;
  else if (szAtom->bSame(">="_Stz))      return AE_COMPARE;
  else if (szAtom->bSame(">"_Stz))       return AE_COMPARE;
  
  else return AE_OTHER;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
