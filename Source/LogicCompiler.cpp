/*============================================================================*/
/* LogicCompiler.cpp -- Logic Compiler                                        */
/*============================================================================*/

#include "Expressions.h"

#ifdef __TINYC
#include "libtcc.h"
#endif


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
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caSource,0xFF,C_LOGIC_SOURCE,CN_LOGICCOMPILER);
#endif
  
  m_caSource[0] = '\0';
  
  m_azSrcCrs = m_caSource;
  
  m_aTccState = nullptr;
  
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
/* call                                                                       */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iCompile(i8 nCodes, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  i8 iResult = iToSource(nCodes,eaCode);
  
  vAcquire();
  
  bCompile();

  vRelease();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vAcquire(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  RASSERT(! m_aTccState);
  
#ifdef __TINYC
  m_aTccState = tcc_new();
  RASSERT(m_aTccState);
  
  tcc_set_output_type((TCCState*)m_aTccState,TCC_OUTPUT_MEMORY);
  
  tcc_set_error_func((TCCState*)m_aTccState, nullptr, vTinyError);
#endif
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  if (-1 == tcc_compile_string((TCCState*)m_aTccState,m_caSource))
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRelease(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  tcc_delete((TCCState*)m_aTccState);
#endif

  m_aTccState = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSymbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tiny-c callback for error printing                                         */
/*----------------------------------------------------------------------------*/
void vTinyError(void *aOpaque, const char *azMsg)
{
  vTrace("TinyError: %s\n"_Stz,azMsg);
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_preliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "typedef long long i8; xyz \n"
  ""
  "static const i8 C_LOGIC_STACK = 4096; \n"
  ""
  "i8 iCompute(void) \n"
  "{ \n"
  "  i8 iaDataStack[C_LOGIC_STACK]; \n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_concluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  ""
  "  return 0; \n"
  "} \n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_INTEGER(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_ADD(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_PRINT(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_RETURN(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* to source code                                                             */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iToSource(i8 nCodes, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  v_preliminary();
  
  for (i8 i_Code = 0; i_Code < nCodes; i_Code++) {
    
    eEO eCode = eaCode[i_Code];

    switch (eCode) {
        
      case EO_INTEGER: v_EO_INTEGER (i_Code,eaCode); break;
      case EO_ADD:     v_EO_ADD     (i_Code,eaCode); break;
      case EO_PRINT:   v_EO_PRINT   (i_Code,eaCode); break;
      case EO_RETURN:  v_EO_RETURN  (i_Code,eaCode); break;

      default:
        break;
    }
  }

  v_concluding();
  
  vTrace("c:: %s\n"_Stz,m_caSource);

  return 1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
