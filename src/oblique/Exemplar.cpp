/*============================================================================*/
/* Exemplar.cpp -- Best Practive Example Class                                */
/*============================================================================*/

#include "BASE.h"

#include "Exemplar.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(OBLIQUE,Exemplar,CN_EXEMPLAR)
__aNewFunction(Exemplar,CN_EXEMPLAR)
__vDeleteFunction(Exemplar,CN_EXEMPLAR)
__aFooterFunction(Exemplar)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Exemplar::Exemplar(void)
: Object(CN_EXEMPLAR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXEMPLAR,_FOOTER_);
  
  _checkInitialised_(CN_EXEMPLAR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXEMPLAR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Exemplar::~Exemplar(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXEMPLAR,_FOOTER_);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
