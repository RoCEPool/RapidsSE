/*============================================================================*/
/* LogicCompiler.cpp -- Logic Compiler                                        */
/*============================================================================*/

#include "Expressions.h"

#ifdef __TINYC
#include "libtcc.h"
#endif

extern EoArg EoArgs[];

char gcaEoFragments[C_128K];
Stz* gszaEoFragments[EO_ENUMS];

#include "DEV_interface.h"
#include "LIB_interface.h"
#include "RLU_interface.h"

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
  m_azSrcCrs    = m_caSource;
  m_aTccState   = nullptr;
  
  m_pLogicUnit  = nullptr;
  
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
/* extract the EO_ code fragments from LogicUnit interpreter                  */
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
/* extract all EO_ fragments                                                  */
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
/* extract an EO_ fragment                                                    */
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
  
  StrLib::vAppendChar(aazCrs,'\n');
  StrLib::vAppendChar(aazCrs,'\0');
}

/*----------------------------------------------------------------------------*/
/* find eEO by name                                                           */
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
/* call the dynamic generated function                                        */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* compile the EO code into machine code                                      */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iCompile(LogicUnit* pLogicUnit, void* aDynArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  m_pLogicUnit = pLogicUnit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  v_DynPreliminary();
  vSbrSource();
  vRtnSource();
  v_DynConcluding();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTrace("Dynamic C code: %s\n"_Stz,m_caSource);

  vAcquire();
  bool b = bCompile();
  vSymbols();
  vRelocate();

  vCall(aDynArg);

  vRelease();
  
  return 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* acquire compiler resources                                                 */
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
/* call the dynamic function                                                  */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vCall(void* aDynArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  i8 (*afDyn)(void* aRLU);

  afDyn = (i8 (*)(void*))tcc_get_symbol((TCCState*)m_aTccState,"aiDynCompute");
  RASSERT(afDyn);
  
  afDyn(aDynArg);
#endif
}

/*----------------------------------------------------------------------------*/
/* compile the synthesised code                                               */
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
/* release compiler resources                                                 */
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
/* relocate object to machine code                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRelocate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  // relocate the code
  if (tcc_relocate((TCCState*)m_aTccState, TCC_RELOCATE_AUTO) < 0) {
    RASSERT(0);
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* add in any symbols                                                         */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSymbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  // add external symbols
  
  tcc_add_symbol((TCCState*)m_aTccState,
                 "aRlu_INIT", (const void *)aRlu_INIT);

  tcc_add_symbol((TCCState*)m_aTccState,
                 "iLib_RANDOM", (const void *)iLib_RANDOM);

  tcc_add_symbol((TCCState*)m_aTccState, "iMul", (const void *)iMul);
  tcc_add_symbol((TCCState*)m_aTccState, "xMul", (const void *)xMul);
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add source code fragment for given rangeof eo codes                        */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vAddFragment(i8 i_CodeFrom, i8 i_CodeTill)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  s4K sReplaced;
  char caInt[32];
  
  for (i8 i_Code = i_CodeFrom; i_Code <= i_CodeTill; i_Code++) {
    
    eEO eCode = m_pLogicUnit->eCode(i_Code);

    if (EO_CALL == eCode) {
      
      i8 iArg = m_pLogicUnit->iCodeArg(i_Code+1);
      i8 i_Label = m_pLogicUnit->iFindLabel(iArg);
      
      v_CallSbr(i_Label);

      i_Code += EoArgs[eCode].m_nArgBytes;
      continue;
    }

    StrLib::vAppendString(&m_azSrcCrs,"    // ");
    StrLib::vAppendString(&m_azSrcCrs,aEO[eCode]->azChar());
    StrLib::vAppendString(&m_azSrcCrs,":\n");
    StrLib::vAppendString(&m_azSrcCrs,"       ");

    char* azFrag = (char*)gszaEoFragments[eCode];
    
    if (EA_INT == EoArgs[eCode].m_eArgument) {
      i8 iArg = m_pLogicUnit->iCodeArg(i_Code+1);
      StrLib::bReplace(sReplaced.azChar(),azFrag,
                       "iArg",StrLib::wToString(iArg,caInt));
      StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
    }
    else {
      StrLib::vAppendString(&m_azSrcCrs,azFrag);
    }
    
    i_Code += EoArgs[eCode].m_nArgBytes;
  }
  
}

/*----------------------------------------------------------------------------*/
/* step thru each subroutine                                                  */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bEachSbr(i8& ri_Label, i8& ri_CodeFrom, i8& ri_CodeTill)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  i8 nCodes  = m_pLogicUnit->nCodes();
  i8 nLabels = m_pLogicUnit->nLabels();

  for (ri_Label++; ri_Label < nLabels; ri_Label++) {
    
    if (m_pLogicUnit->bLabelIsSbr(ri_Label)) {
      
      ri_CodeFrom = m_pLogicUnit->iLabelCode(ri_Label);
      
      for (ri_CodeTill = ri_CodeFrom + 1; ri_CodeTill < nCodes; ri_CodeTill++) {
        
        i8 i_Label = m_pLogicUnit->iFindLabel(ri_CodeTill);
        if (C_UNKNOWN != i_Label && m_pLogicUnit->bLabelIsSbr(i_Label)) {
          ri_CodeTill--;
          return true;
        }
      }
      
      ri_CodeTill = nCodes - 1;
      return true;
    }
  }
  
  ri_CodeFrom = -1;
  ri_CodeTill = -1;

  return false;
}

/*----------------------------------------------------------------------------*/
/* add source code fragment for main routine                                  */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRtnSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  s4K sReplaced;
  char caInt[32];
  
  i8 nCodes = m_pLogicUnit->nCodes();
  
  i8 i_CodeTill = 0;
  while (i_CodeTill + 1 < nCodes) {
    i8 i_Label = m_pLogicUnit->iFindLabel(i_CodeTill+1);
    if (C_UNKNOWN == i_Label) {
      i_CodeTill++;
      continue;
    }
    
    if (m_pLogicUnit->bLabelIsSbr(i_Label))
      break;
    
    i_CodeTill++;
  }
  
  v_RtnPreliminary();
  vAddFragment(0,i_CodeTill);
  v_RtnConcluding();
}

/*----------------------------------------------------------------------------*/
/* add source code fragment for all subroutines                               */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSbrSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  i8 i_Label    = -1;
  i8 i_CodeFrom = -1;
  i8 i_CodeTill = -1;

  s32 sFun;
  s4K sReplaced;
  
  for (;;) {
    bool bFound = bEachSbr(i_Label,i_CodeFrom,i_CodeTill);
    if (! bFound)
      break;
    
    i8 i_Label = m_pLogicUnit->iFindLabel(i_CodeFrom);
    sFun = m_pLogicUnit->szLabelNm(i_Label);
    StrLib::vStrip(sFun.azChar(),':');
    
    v_SbrPreliminary(sFun);
    vAddFragment(i_CodeFrom,i_CodeTill);
    v_SbrConcluding();
  }
}

/*----------------------------------------------------------------------------*/
/* the preliminary code fragment for this compilation unit                    */
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
  "\n  typedef char Stz;"
  "\n"
  "\n  #include \"../Source/LIB_interface.h\""
  "\n  #include \"../Source/DEV_interface.h\""
  "\n  #include \"../Source/RluCommon.h\""
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
  "\n  #define nullptr 0"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
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
/* the preliminary code fragment for the main routine                         */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_RtnPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  /*--------------------------------------------------------------------*/"
  "\n  /* rlu compute function                                               */"
  "\n  /*--------------------------------------------------------------------*/"
  "\n  i8* aiDynCompute(void* pLU)"
  "\n  {"
  "\n    struct RluCommon* aRluCommon = nullptr;"
  "\n"
  "\n    i8* aiDstk = nullptr;"
  "\n"
  "\n    i8 iTopA;"
  "\n    i8 iTopB;"
  "\n    i8 iRes;"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for the main routine                          */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_RtnConcluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  char* azFrag = (char*)
  "    // ** Hazard **"
  "\n    return aiDstk;"
  "\n  }"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the preliminary code fragment for a subroutine                             */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_SbrPreliminary(Stz* szSbrNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  /*--------------------------------------------------------------------*/"
  "\n  /* subroutine                                                         */"
  "\n  /*--------------------------------------------------------------------*/"
  "\n  i8* %s(void* pLU, i8* aiDstk, struct RluCommon* aRluCommon)"
  "\n  { "
  "\n    i8 iTopA;"
  "\n    i8 iTopB;"
  "\n    i8 iRes;"
  "\n"
  "\n"
  "";
  
  s4K sReplaced;

  sReplaced.vSprint((Stz*)azFrag,szSbrNm->azChar());
  
  StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for a subroutine                              */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_SbrConcluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "    // ** End **"
  "\n    return aiDstk;"
  "\n  }"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the code fragment for calling a subroutine                                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_CallSbr(i8 i_Label)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "    // EO_CALL:"
  "\n        aiDstk = %s(pLU,aiDstk,aRluCommon);"
  "\n"
  "\n"
  "";

  s32 sFun;
  s4K sReplaced;

  sFun = m_pLogicUnit->szLabelNm(i_Label);
  
  StrLib::vStrip(sFun.azChar(),':');
  
  sReplaced.vSprint((Stz*)azFrag,sFun.azChar());
  
  StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
