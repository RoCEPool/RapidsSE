/*============================================================================*/
/* LogicCompiler.hpp -- Logic Compiler                                        */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,LogicCompiler,CN_LOGICCOMPILER)
__aNewFunction(LogicCompiler,CN_LOGICCOMPILER)
__vDeleteFunction(LogicCompiler,CN_LOGICCOMPILER)
__aFooterFunction(LogicCompiler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogicCompiler::LogicCompiler(void)
: Object(CN_LOGICCOMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  _checkInitialised_(CN_LOGICCOMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicCompiler::~LogicCompiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vCompile(i8 nCodes, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
