/*============================================================================*/
/* ClassPlaceHolder.hpp -- Place Holder for a new Class                       */
/*============================================================================*/

#include "BASE.h"

#include "ClassPlaceHolder.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,ClassPlaceHolder,CN_CLASSPLACEHOLDER)
__aNewFunction(ClassPlaceHolder,CN_CLASSPLACEHOLDER)
__vDeleteFunction(ClassPlaceHolder,CN_CLASSPLACEHOLDER)
__aFooterFunction(ClassPlaceHolder)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ClassPlaceHolder::ClassPlaceHolder(void)
: Object(CN_CLASSPLACEHOLDER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLASSPLACEHOLDER,_FOOTER_);
  
   _checkInitialised_(CN_CLASSPLACEHOLDER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLASSPLACEHOLDER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ClassPlaceHolder::~ClassPlaceHolder(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLASSPLACEHOLDER,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
