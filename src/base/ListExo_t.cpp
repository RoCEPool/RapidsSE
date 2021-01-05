/*============================================================================*/
/* ListExo_t.cpp -- List of Items (with exo-nodes)                            */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,ListExo_ENVELOPED,CN_CLASSPLACEHOLDER)
__aNewFunction(ListExo_ENVELOPED,CN_CLASSPLACEHOLDER)
__vDeleteFunction(ListExo_ENVELOPED,CN_CLASSPLACEHOLDER)
__aFooterFunction(ListExo_ENVELOPED)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ListExo_ENVELOPED::ListExo_ENVELOPED(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLASSPLACEHOLDER,_FOOTER_);
  
   _checkInitialised_(CN_CLASSPLACEHOLDER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLASSPLACEHOLDER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ListExo_ENVELOPED::~ListExo_ENVELOPED(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLASSPLACEHOLDER,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
