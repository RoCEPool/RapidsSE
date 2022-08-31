/*============================================================================*/
/* Formats.cpp -- Best Practive Example Class                                */
/*============================================================================*/

#include "BASE.h"

#include "Formats.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Formats,CN_EXEMPLAR)
__aNewFunction(Formats,CN_EXEMPLAR)
__vDeleteFunction(Formats,CN_EXEMPLAR)
__aFooterFunction(Formats)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Formats::Formats(void)
: Object(CN_EXEMPLAR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXEMPLAR,_FOOTER_);

  _checkInitialised_(CN_EXEMPLAR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXEMPLAR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Formats::~Formats(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_EXEMPLAR,_FOOTER_);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
