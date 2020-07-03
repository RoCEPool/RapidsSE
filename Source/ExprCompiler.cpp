/*============================================================================*/
/* ExprCompiler.cpp -- Expression Compiler                                    */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,ExprCompiler,CN_EXPR)
__aNewFunction(ExprCompiler,CN_EXPR)
__vDeleteFunction(ExprCompiler,CN_EXPR)
__aFooterFunction(ExprCompiler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExprCompiler::ExprCompiler(void)
: Object(CN_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  _checkInitialised_(CN_EXPR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExprCompiler::~ExprCompiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
