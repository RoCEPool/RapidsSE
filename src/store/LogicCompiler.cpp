
/*============================================================================*/
/* LogicCompiler.cpp -- Logic Compiler                                        */
/*============================================================================*/

#include "BASE.h"

#include "LogicCompiler.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"

#include "TagConstants.h"

#include "TinyC.hpp"

extern EoArg EoArgs[];

const i8 C_MACRO_FILES = 12;
char gszaAmMacroFiles[C_MACRO_FILES][128] = {
    "../asm/AM_CrwBuffer.macro",
    "../asm/AM_DataPulse.macro",
    "../asm/AM_DefaultHandlers.macro",
    "../asm/AM_LeftOuterMarker.macro",
    "../asm/AM_EosMarker.macro",
    "../asm/AM_Index.macro",
    "../asm/AM_JoinRows.macro",
    "../asm/AM_LoopJoin.macro",
    "../asm/AM_PayLoad.macro",
    "../asm/AM_QueryMisc.macro",
    "../asm/AM_RowBuffer.macro",
    "../asm/AM_StreamDef.macro"
};

#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,LogicCompiler,CN_LOGICCOMPILER)
__aNewFunction(LogicCompiler,CN_LOGICCOMPILER)
__vDeleteFunction(LogicCompiler,CN_LOGICCOMPILER)
__aFooterFunction(LogicCompiler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogicCompiler::LogicCompiler(LogicUnit* pLogicUnit)
: Object(CN_LOGICCOMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caSource,0xFF,C_LOGIC_SOURCE,CN_LOGICCOMPILER);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caSource,sizeof(m_caSource),CN_LOGICCOMPILER);
#endif

  m_caSource[0] = '\0';
  m_azSrcCrs    = m_caSource;

  m_pTinyC = new TinyC;
  
  m_pLogicUnit  = pLogicUnit;
  
  _checkInitialised_(CN_LOGICCOMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicCompiler::~LogicCompiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  delete m_pTinyC;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the logicCompiler                                                   */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRepair(Object* pObject)
{
  _static_

  LogicCompiler* pLogicCompiler = _pLogicCompiler(pObject);

  // delete the logicCompiler
  delete pLogicCompiler;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* acquire compiler resources                                                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vAcquire(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  m_pTinyC->vAcquire(this, vTinyError);
}

/*----------------------------------------------------------------------------*/
/* release compiler resources                                                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRelease(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  //delete m_pTinyC;
  //m_pTinyC = nullptr;
}

/*----------------------------------------------------------------------------*/
/* relocate object to machine code                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRelocate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  // relocate the code
  if ( ! m_pTinyC->bRelocate(m_pLogicUnit->m_aTinyFunc) )
    RASSERT(0);
  
  m_pLogicUnit->m_afDynFun =
    (i8 (*)(void*,i8,i8))m_pTinyC->pFindSymbol("aiDynFun");

  RASSERT(m_pLogicUnit->m_afDynFun);
}

/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
Stz* LogicCompiler::szSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  return (Stz*)m_caSource;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tiny-c callback for error printing                                         */
/*----------------------------------------------------------------------------*/
void vTinyError(void *aOpaque, const char *azMsg)
{
  LogicCompiler* pLogicCompiler = _pLogicCompiler(aOpaque);
  
  vTrace("Dynamic Source Code: \n\n%s\n"_Stz,pLogicCompiler->szSource());

  vTrace("TinyError: %s\n"_Stz,azMsg);
  
  vDebugPoint();
  
  NEVER_REACH;
}



//  void* pLabel = && L_Label;
//
//  goto *pLabel;
//L_Label: ;


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
