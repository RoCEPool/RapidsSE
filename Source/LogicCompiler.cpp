/*============================================================================*/
/* LogicCompiler.cpp -- Logic Compiler                                        */
/*============================================================================*/

#include "Expressions.h"

#ifdef __TINYC
#include "libtcc.h"
#endif

char gcaEoFragments[C_128K];
Stz* gszaEoFragments[EO_ENUMS];

extern EoArg EoArgs[];

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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract EO_ code fragments                                                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtract(void)
{
  _static_
    
  vTrace("LogicCompiler::vExtract -- Extract EO_ Fragments\n"_Stz);
  
  File* pFile = new File(nullptr);
  
  bool b = pFile->bOpenRead("../Source/LogicUnit.cpp"_Stz);
  RASSERT(b);
  
  char* azCrs = gcaEoFragments;
  
  s4K sTok;
  while (pFile->bNextLiteTok(sTok)) {
    
    if (sTok.bSame("//+ExtractBegin"_Stz)) {
      vTrace("Found +ExtractBegin\n"_Stz);
      while (bExtractAll(pFile,&azCrs))
        {;}
    }
  }
  
  vTrace("Fragments:: %s\n"_Stz,gcaEoFragments);
  
  for (i8 i_EO = 1; i_EO < EO_ENUMS; i_EO++) {
    vTrace("Fragment:: %s\n%s\n"_Stz,aEO[i_EO],gszaEoFragments[i_EO]);
  }
  
  pFile->vClose();
  
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* extract all EO_fragments                                                   */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bExtractAll(File* pFile, char** aazCrs)
{
  _static_
  
  s4K sTok;
  while (pFile->bNextLiteTok(sTok)) {
    if (sTok.bSame("case"_Stz)) {
      bool b = pFile->bNextLiteTok(sTok);
      vExtractFragment(pFile,aazCrs);
      return true;
    }
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* extract an EO_fragment                                                     */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtractFragment(File* pFile, char** aazCrs)
{
  _static_
  
  s4K sTok;
  bool b = pFile->bNextLiteTok(sTok);
  
  StrLib::vStrip(sTok.azChar(),':');
  
  i8 i_EO = eFindEO(sTok);
  vTrace("Fragment %s [%lld]\n"_Stz,sTok.szStz(),i_EO);

  gszaEoFragments[i_EO] = (Stz*)*aazCrs;
  
  for (;;) {
    b = pFile->bNextLiteTok(sTok);
    if (!b || sTok.bSame("break;//+endCase"_Stz))
      break;
    
    StrLib::vAppendString(aazCrs,sTok.azChar());
  }
  
  //StrLib::vAppendChar(aazCrs,'\n');
  StrLib::vAppendChar(aazCrs,'\n');
  StrLib::vAppendChar(aazCrs,'\0');
}

/*----------------------------------------------------------------------------*/
/* extract an EO_fragment                                                     */
/*----------------------------------------------------------------------------*/
eEO LogicCompiler::eFindEO(Stz* szEOname)
{
  _static_
  
  for (i8 i_EO = 1; i_EO < EO_ENUMS; i_EO++)
    if (StrLib::bSame(aEO[i_EO]->azChar(), szEOname->azChar()))
      return (eEO)i_EO;
  
  return EO_NONE;
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
i8 LogicCompiler::iCompile(LogicUnit* pLogicUnit, RLU* aRLU,
                           i8 nCodes, eEO eaCode[],
                           i8 nLabels, struct JumpLabel xaJumpLabel[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  i8 iResult = iToSource(pLogicUnit,nCodes,eaCode,nLabels,xaJumpLabel);
  
  vAcquire();
  
  bCompile();
  
  vSymbols();
  
  vCall(aRLU);

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
void LogicCompiler::vCall(RLU* aRLU)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  i8 (*afDyn)(void* aRLU);

  afDyn = (i8 (*)(void*))tcc_get_symbol((TCCState*)m_aTccState,"iDynCompute");
  RASSERT(afDyn);
  
  afDyn(aRLU);
#endif
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
  
#ifdef __TINYC
  // add external symbols
  tcc_add_symbol((TCCState*)m_aTccState, "iMul", (const void *)iMul);
  tcc_add_symbol((TCCState*)m_aTccState, "xMul", (const void *)xMul);

  // relocate the code
  if (tcc_relocate((TCCState*)m_aTccState, TCC_RELOCATE_AUTO) < 0) {
    RASSERT(0);
  }
#endif
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
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* extern function test : iMul()                                              */
/*----------------------------------------------------------------------------*/
i8 iMul(i8 iTopA, i8 iTopB)
{
  return iTopA * iTopB;
}

/*----------------------------------------------------------------------------*/
/* extern function test : iMul()                                              */
/*----------------------------------------------------------------------------*/
Qty xMul(i8 iTopA, i8 iTopB)
{
  Qty xRes;
  
  xRes.m_iValue = iTopA * iTopB;
  return xRes;
}


//  void* pLabel = && L_Label;
//
//  goto *pLabel;
//L_Label: ;

/*----------------------------------------------------------------------------*/
/* to source code                                                             */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iToSource(LogicUnit* pLogicUnit,
                            i8 nCodes, eEO eaCode[],
                            i8 nLabels, struct JumpLabel xaJumpLabel[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  s4K sReplaced;
  char caInt[32];
  
  v_DynPreliminary();

  v_FunPreliminary();

  for (i8 i_Code = 0; i_Code < nCodes; i_Code++) {
    
    eEO eCode = eaCode[i_Code];

    StrLib::vAppendString(&m_azSrcCrs,"    // ");
    StrLib::vAppendString(&m_azSrcCrs,aEO[eCode]->azChar());
    StrLib::vAppendString(&m_azSrcCrs,":\n");
    StrLib::vAppendString(&m_azSrcCrs,"       ");

    char* azFrag = (char*)gszaEoFragments[eCode];
    
    if (EA_INT == EoArgs[eCode].m_eArgument) {
      i8 iArg = *(i8*)&eaCode[i_Code+1];
      StrLib::bReplace(sReplaced.azChar(),azFrag,
                       "iArg",StrLib::wToString(iArg,caInt));
      StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
    }
    else {
      StrLib::vAppendString(&m_azSrcCrs,azFrag);
    }
    
    
    i_Code += EoArgs[eCode].m_nArgBytes;
  }

  v_FunConcluding();

  v_DynConcluding();
  
  vTrace("Dynamic C code: %s\n"_Stz,m_caSource);

  return 1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_DynPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  /*====================================================================*/"
  "\n  /* Dynamically Generated Code                                         */"
  "\n  /*====================================================================*/"
  "\n"
  "\n  #include <tcclib.h>" // include libc header for TCC
  "\n"
  "\n  typedef unsigned char eEO;"
  "\n  typedef char i1;"
  "\n  typedef long long i8;"
  "\n"
  "\n  #include \"../Source/Qty.h\""
  "\n"
  "\n  static const i8 C_LOGIC_STACK = 4096;"
  "\n"
  "\n  extern i8 iMul(i8 iTopA, i8 iTopB);"
  "\n  extern struct Qty xMul(i8 iTopA, i8 iTopB);"
  "\n"
  "\n  enum eDO { DO_A=0, DO_B, DO_C };"
  "\n"
  "\n  #define ArgNil() "
  "\n  #define ArgGet(x) "
  "\n  #define dLU0(x) "
  "\n  #define dLU1(x,a) "
  "\n  #define dLU2(x,a,b) "
  "\n  #define dLU3(x,a,b,c) "
  "\n  #define SbrCall() "
  "\n  #define SbrReturn() "
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_DynConcluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  /*====================================================================*/"
  "\n  /* end                                                                */"
  "\n  /*====================================================================*/"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_FunPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  /*--------------------------------------------------------------------*/"
  "\n  /* rlu compute function                                               */"
  "\n  /*--------------------------------------------------------------------*/"
  "\n  i8 iDynCompute(void* aRLU)" // the dynamic function is: iDynCompute()
  "\n  { "
  "\n    i8 iaDataStack[C_LOGIC_STACK];"
  "\n    eEO* aaReturnStack[C_LOGIC_STACK];"
  "\n"
  "\n    i8* aiDstk = &iaDataStack[0];"
  "\n    eEO** aaeRstk = &aaReturnStack[0];"
  "\n"
  "\n    i8 iTopA;"
  "\n    i8 iTopB;"
  "\n    i8 iRes;"
  "\n    struct Qty xRes;"
  "\n"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_FunConcluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  char* azFrag = (char*)
  "    // ** Hazard **"
  "\n    return 0;"
  "\n  }"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_SbrPreliminary(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  voic v%s(void"
  "\n  { "
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_SbrConcluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  }"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
