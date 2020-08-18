
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
/* symbols for dynamically generated code                                     */
/*----------------------------------------------------------------------------*/
DynamicSymbol gxaDynamicSymbol[]
{
  { "iDev_ALT_COL_PUSH"_Stz,      (const void *) iDev_ALT_COL_PUSH      },
  { "vDev_ALT_COL_PUSHONE"_Stz,     (const void *) vDev_ALT_COL_PUSHONE     },
  { "vDev_SRC_COL_PUSHONE"_Stz,     (const void *) vDev_SRC_COL_PUSHONE     },
  
  { "vDev_COL_NOT_PROJECT"_Stz,   (const void *) vDev_COL_NOT_PROJECT   },

  { "vDev_CRW_BEGIN"_Stz,         (const void *) vDev_CRW_BEGIN         },
  { "vDev_CRW_DEREF"_Stz,         (const void *) vDev_CRW_DEREF         },
  { "iDev_CRW_NEXT"_Stz,          (const void *) iDev_CRW_NEXT          },
  { "vDev_CRW_NULL"_Stz,          (const void *) vDev_CRW_NULL          },
  { "iDev_CRW_TEST"_Stz,          (const void *) iDev_CRW_TEST          },
  { "vDev_CRW_TORSB"_Stz,         (const void *) vDev_CRW_TORSB         },

  { "iDev_ENT_COMPARE"_Stz,       (const void *) iDev_ENT_COMPARE       },
  { "iDev_ENT_NEXT"_Stz,          (const void *) iDev_ENT_NEXT          },
  { "vDev_ENT_POSPRIOR"_Stz,      (const void *) vDev_ENT_POSPRIOR      },
  { "vDev_ENT_PRINTKEYS"_Stz,     (const void *) vDev_ENT_PRINTKEYS     },
  { "aDev_ENT_PUSH"_Stz,          (const void *) aDev_ENT_PUSH          },
  { "aDev_ENT_PUSHUPPER"_Stz,     (const void *) aDev_ENT_PUSHUPPER     },
  { "iDev_ENT_PUSHELMI0"_Stz,     (const void *) iDev_ENT_PUSHELMI0     },
  { "szDev_ENT_PUSHELMS0"_Stz,    (const void *) szDev_ENT_PUSHELMS0    },
  { "iDev_ENT_PUSHOFS"_Stz,       (const void *) iDev_ENT_PUSHOFS       },
  
  { "vDev_ENTWRK_APPENDELM"_Stz,  (const void *) vDev_ENTWRK_APPENDELM  },
  { "vDev_ENTWRK_APPENDELMI"_Stz, (const void *) vDev_ENTWRK_APPENDELMI },
  { "vDev_ENTWRK_APPENDELMS"_Stz, (const void *) vDev_ENTWRK_APPENDELMS },
  { "vDev_ENTWRK_CREATEPAY"_Stz,  (const void *) vDev_ENTWRK_CREATEPAY  },
  { "vDev_ENTWRK_POPOFS"_Stz,     (const void *) vDev_ENTWRK_POPOFS     },
  { "vDev_ENTWRK_PRINT"_Stz,      (const void *) vDev_ENTWRK_PRINT      },
  { "vDev_ENTWRK_RESET"_Stz,      (const void *) vDev_ENTWRK_RESET      },
  { "vDev_ENTWRK_SETINTELM"_Stz,  (const void *) vDev_ENTWRK_SETINTELM  },

  { "iDev_IXT_CMP_SRCALT"_Stz,    (const void *) iDev_IXT_CMP_SRCALT    },
  { "iDev_IXT_CMP_WRKALT"_Stz,    (const void *) iDev_IXT_CMP_WRKALT    },
  { "vDev_IXT_DEL_ENT"_Stz,       (const void *) vDev_IXT_DEL_ENT       },
  { "vDev_IXT_DUMP"_Stz,          (const void *) vDev_IXT_DUMP          },
  { "vDev_IXT_ENT_PRINT"_Stz,     (const void *) vDev_IXT_ENT_PRINT     },
  { "vDev_IXT_INS_ENT"_Stz,       (const void *) vDev_IXT_INS_ENT       },
  { "iDev_IXT_TMP_ENTRIES"_Stz,   (const void *) iDev_IXT_TMP_ENTRIES   },

  { "aDev_IXT_POS_ALTBYSRC"_Stz,  (const void *) aDev_IXT_POS_ALTBYSRC  },
  { "aDev_IXT_POS_ALTBYWRK"_Stz,  (const void *) aDev_IXT_POS_ALTBYWRK  },
  { "aDev_IXT_POS_LBOUND"_Stz,    (const void *) aDev_IXT_POS_LBOUND    },
  { "aDev_IXT_POS_UBOUND"_Stz,    (const void *) aDev_IXT_POS_UBOUND    },
  { "aDev_IXT_POS_UBOUND_LT"_Stz, (const void *) aDev_IXT_POS_UBOUND_LT },
  { "aDev_IXT_TRV_BEGIN"_Stz,     (const void *) aDev_IXT_TRV_BEGIN     },
  { "aDev_IXT_TRV_NEXT"_Stz,      (const void *) aDev_IXT_TRV_NEXT      },
      
  { "iDev_PAY_FETCH_COLI"_Stz,    (const void *) iDev_PAY_FETCH_COLI    },
  { "szDev_PAY_FETCH_COLS"_Stz,   (const void *) szDev_PAY_FETCH_COLS   },
  { "vDev_PAY_STORE_COLI"_Stz,    (const void *) vDev_PAY_STORE_COLI    },
  { "szDev_PAY_STORE_COLS"_Stz,   (const void *) szDev_PAY_STORE_COLS   },
      
  { "vDev_PIX_STM_PRINT"_Stz,     (const void *) vDev_PIX_STM_PRINT     },
  
  { "vDev_RSB_BEGIN"_Stz,         (const void *) vDev_RSB_BEGIN         },
  { "iDev_RSB_NEW"_Stz,           (const void *) iDev_RSB_NEW           },
  { "iDev_RSB_NEXT"_Stz,          (const void *) iDev_RSB_NEXT          },
  { "vDev_RSB_POPCOLI"_Stz,       (const void *) vDev_RSB_POPCOLI       },
  { "vDev_RSB_POPCOLS"_Stz,       (const void *) vDev_RSB_POPCOLS       },
  { "vDev_RSB_PRINT"_Stz,         (const void *) vDev_RSB_PRINT         },
  { "iDev_RSB_PUSHCOLI"_Stz,      (const void *) iDev_RSB_PUSHCOLI      },
  { "szDev_RSB_PUSHCOLS"_Stz,     (const void *) szDev_RSB_PUSHCOLS     },
  { "iDev_RSB_PUSHOFS"_Stz,       (const void *) iDev_RSB_PUSHOFS       },
  { "vDev_RSB_RESET"_Stz,         (const void *) vDev_RSB_RESET         },
  { "vDev_RSB_SETBYOFS"_Stz,      (const void *) vDev_RSB_SETBYOFS      },
  { "iDev_RSB_TEST"_Stz,          (const void *) iDev_RSB_TEST          },
  { "vDev_RSB_ZERO"_Stz,          (const void *) vDev_RSB_ZERO          },

  { "iDev_RSB_COL_PUSH"_Stz,      (const void *) iDev_RSB_COL_PUSH      },
  { "vDev_RSB_COL_PUSHONE"_Stz,   (const void *) vDev_RSB_COL_PUSHONE   },
 
  { "vLib_ASSERT"_Stz,            (const void *) vLib_ASSERT            },
  { "vLib_BREAKPOINT"_Stz,        (const void *) vLib_BREAKPOINT        },
  { "iLib_DTE_NOW"_Stz,           (const void *) iLib_DTE_NOW           },
  { "szLib_DTE_PRINT"_Stz,        (const void *) szLib_DTE_PRINT        },
  { "iLib_DTE_YEAR"_Stz,          (const void *) iLib_DTE_YEAR          },
  { "vLib_ERROR"_Stz,             (const void *) vLib_ERROR             },
  { "iLib_GET_LOCAL"_Stz,         (const void *) iLib_GET_LOCAL         },
  { "sLib_GET_LOCALS"_Stz,        (const void *) sLib_GET_LOCALS        },
  { "vLib_HSH_ADD_HASHI"_Stz,     (const void *) vLib_HSH_ADD_HASHI     },
  { "vLib_PTN_CALC"_Stz,          (const void *) vLib_PTN_CALC          },
  { "vLib_PTN_RUNNING"_Stz,       (const void *) vLib_PTN_RUNNING       },
  { "iLib_RANDOM"_Stz,            (const void *) iLib_RANDOM            },
  { "vLib_SET_LOCAL"_Stz,         (const void *) vLib_SET_LOCAL         },
  { "vLib_SLEEP"_Stz,             (const void *) vLib_SLEEP             },
  { "iLib_STR_COMPARE"_Stz,       (const void *) iLib_STR_COMPARE       },
    
  { "aRlu_INIT"_Stz,              (const void *) aRlu_INIT              },
  { "vRlu_BEGINTMR"_Stz,          (const void *) vRlu_BEGINTMR          },
  { "vRlu_DUMP"_Stz,              (const void *) vRlu_DUMP              },
  { "vRlu_ENDTMR"_Stz,            (const void *) vRlu_ENDTMR            },
  { "vRlu_ENTER"_Stz,             (const void *) vRlu_ENTER             },
  { "vRlu_EXIT"_Stz,              (const void *) vRlu_EXIT              },
  { "vRlu_TMR_AGG_ADD"_Stz,       (const void *) vRlu_TMR_AGG_ADD       },
  { "iRlu_TMR_AGG_NSECS"_Stz,     (const void *) iRlu_TMR_AGG_NSECS     },
  { "vRlu_TMR_AGG_RESET"_Stz,     (const void *) vRlu_TMR_AGG_RESET     },
  { "iRlu_TMR_NSECS"_Stz,         (const void *) iRlu_TMR_NSECS         },
  { "iRlu_TMR_SINCE"_Stz,         (const void *) iRlu_TMR_SINCE         },
  { "vRlu_TMR_START"_Stz,         (const void *) vRlu_TMR_START         },
  { "vRlu_TMR_UPDATE"_Stz,        (const void *) vRlu_TMR_UPDATE        },

  { nullptr, nullptr },
};

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
  
  AM::pTccLock()->vGrabWrite();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  v_DynPreliminary();
  vSbrDeclaration();
  vSbrSource();
  vRtnSource();
  v_DynConcluding();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_TRACE_LOGICUNIT
  vTrace("Dynamic C code: %s\n"_Stz,m_caSource);
#endif
  
  vAcquire();
  bool b = bCompile();
  vSymbols();
  vRelocate();

  //vRelease();

  AM::pTccLock()->vDropWrite();
  
  m_pLogicUnit->m_pLogicTimer = new LogicTimer(m_pLogicUnit->m_xRluCommon.m_iSql);

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

  m_pLogicUnit->m_afDynFun(aDynArg,0,C_UNKNOWN);
  
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
  
  m_pLogicUnit->m_afDynFun =
    (i8 (*)(void*,i8,i8))tcc_get_symbol((TCCState*)m_aTccState,"aiDynFun");
  RASSERT(m_pLogicUnit->m_afDynFun);
  
#endif
}

/*----------------------------------------------------------------------------*/
/* add in any symbols                                                         */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSymbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
    
  for (i8 i_Symbol = 0; gxaDynamicSymbol[i_Symbol].m_afSymbol; i_Symbol++)
    tcc_add_symbol((TCCState*)m_aTccState,
                   (char*)gxaDynamicSymbol[i_Symbol].m_szSymbolNm,
                   gxaDynamicSymbol[i_Symbol].m_afSymbol);
  
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
  
  i8 iArg;
  i8 i_Label;
  
  s4K sReplaced;
  char caInt[32];
  s4K sString;
  
  s4K sPrint;

  for (i8 i_Code = i_CodeFrom; i_Code <= i_CodeTill; i_Code++) {
    
    eEO eCode = m_pLogicUnit->eCode(i_Code);

    i_Label = m_pLogicUnit->iFindLabel(i_Code);
    if (C_UNKNOWN != i_Label && m_pLogicUnit->bLabelIsDst(i_Label)) {
      StrLib::vAppendString(&m_azSrcCrs,"L_");
      StrLib::vAppendDecimal(&m_azSrcCrs,i_Code);
      StrLib::vAppendString(&m_azSrcCrs,": // ");
      StrLib::vAppendString(&m_azSrcCrs,(char*)m_pLogicUnit->szLabelNm(i_Label));
      StrLib::vAppendString(&m_azSrcCrs,"\n");
    }
    
    // eo_ trace
//    sPrint.vSprint(
//                   "    printf(\"  *: %s "_Stz,
//
//                   aEO[eCode]->azChar());
//    sPrint.vConcat(" %llx\\n\",aiDstk);\n"_Stz);
//
//    StrLib::vAppendString(&m_azSrcCrs,sPrint.azChar());

    
    
    // timer begin
//    sPrint.vSprint("\n    vRlu_BEGINTMR(pLU);\n "_Stz);
//    StrLib::vAppendString(&m_azSrcCrs,sPrint.azChar());
    

    if (EO_CALL == eCode) {
      
      iArg = m_pLogicUnit->iCodeArg(i_Code+1);
      i_Label = m_pLogicUnit->iFindLabel(iArg);

      v_CallSbr(i_Label);

      i_Code += EoArgs[eCode].m_nArgBytes;
      //vTrace("%s<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"_Stz,m_caSource);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    StrLib::vAppendString(&m_azSrcCrs,"    // ");
    StrLib::vAppendString(&m_azSrcCrs,aEO[eCode]->azChar());
    StrLib::vAppendString(&m_azSrcCrs,":\n");
    StrLib::vAppendString(&m_azSrcCrs,"       ");

    char* azFrag = (char*)gszaEoFragments[eCode];
    
    if (EA_DATE   == EoArgs[eCode].m_eArgument ||
        EA_INT    == EoArgs[eCode].m_eArgument ||
        EA_LABEL  == EoArgs[eCode].m_eArgument ||
        EA_SUBSCR == EoArgs[eCode].m_eArgument   ) {
      i8 iArg = m_pLogicUnit->iCodeArg(i_Code+1);
      StrLib::bReplace(sReplaced.azChar(),azFrag,
                       "iArg",StrLib::wToString(iArg,caInt));
      StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
      i_Code += EoArgs[eCode].m_nArgBytes;
    }
    else
    if (EA_STRING == EoArgs[eCode].m_eArgument) {
      char* azStr = (char*)m_pLogicUnit->aCodeArg(i_Code+1);
      i8 nSize = StrLib::nSize(azStr);

      sString.vSprint("\"%s\""_Stz,azStr);
      StrLib::bReplace(sReplaced.azChar(),azFrag,
                       "iArg",sString.azChar());
      StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());

      i_Code += nSize;
    }
    else {
      StrLib::vAppendString(&m_azSrcCrs,azFrag);
      i_Code += EoArgs[eCode].m_nArgBytes;
    }
    
    // timer end
//    sPrint.vSprint("    vRlu_ENDTMR(pLU,%s);\n\n "_Stz,aEO[eCode]->azChar());
//    StrLib::vAppendString(&m_azSrcCrs,sPrint.azChar());

#ifdef WITH_TRACE_LOGICUNIT
    // count no of newlines in the fragment
    i8 nNewLines = StrLib::iOccurrence(&m_caSource[0],'\n');
    
    // add the line number
    sPrint.vSprint("    /* %lld */\n "_Stz,nNewLines);
    StrLib::vAppendString(&m_azSrcCrs,sPrint.azChar());
#endif
    
    //vTrace("%s<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"_Stz,m_caSource);
    continue;
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
/* add source code fragments to declare all subroutines                       */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSbrDeclaration(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  i8 i_Label    = -1;
  i8 i_CodeFrom = -1;
  i8 i_CodeTill = -1;

  s32 sFun;
  
  for (;;) {
    bool bFound = bEachSbr(i_Label,i_CodeFrom,i_CodeTill);
    if (! bFound)
      break;
    
    i8 i_Label = m_pLogicUnit->iFindLabel(i_CodeFrom);
    sFun = m_pLogicUnit->szLabelNm(i_Label);
    StrLib::vStrip(sFun.azChar(),':');
  
    v_SbrDeclaration(sFun);
  }
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
  "\n  /* Code for Qf%4lld                                                    */"
  "\n  /*====================================================================*/"
  "\n"
  "\n  #include <tcclib.h>" // include libc header for TCC
  "\n"
  "\n  typedef unsigned char eEO;"
  "\n  typedef char i1;"
  "\n  typedef unsigned short u2;"
  "\n  typedef long long i8;"
  "\n  typedef unsigned long long m8;"
  "\n  typedef unsigned long long u8;"
  "\n  typedef char Stz;"
  "\n  typedef unsigned short tET;"
  "\n"
  "\n  static const i8 C_1MB = 1048576;"
  "\n"
  "\n  static const void* C_ADR_REGION = (void*)0x204800000000;"
  "\n  static const i8 C_DATUM_NULL = -9223372036854775807;"
  "\n  static const i8 C_CRW_HEADBYTES = 32;"
  "\n  static const i8 C_CRW_FIRSTHEAD = 0x210;"
  "\n  static const tET C_ENT_END_TICKET = 0xFFFF;"
  "\n"
  "\n  #define C_MAX_LVL_LEVELS 6"
  "\n"
  "\n  #include \"../Source/EnumGroups.h\""
  "\n  #include \"../Source/Ent_int.h\""
  "\n  #include \"../Source/LIB_interface.h\""
  "\n  #include \"../Source/DEV_interface.h\""
  "\n  #include \"../Source/RLU_interface.h\""
  "\n  #include \"../Source/RluCommon.h\""
  "\n  #include \"../Source/eEO.h\""
  "\n"
  "\n  enum eKC { KC_BEFORE=1, KC_SAME, KC_AFTER };"
  "\n"
  "\n  #define ArgNil() "
  "\n  #define ArgGet(x) "
  "\n  #define dLU0(x) "
  "\n  #define dLU1(x,a) "
  "\n  #define dLU2(x,a,b) "
  "\n  #define dLU3(x,a,b,c) "
  "\n  #define nullptr 0"
  "\n"
  "\n  #define GetStr(__azStr)"
//  "      nSize = strlen(__azStr) + 1;"
//  "      szRes = (char*)(++aiDstk);"
//  "      strcpy(szRes,__azStr);"
//  "      aiDstk += (nSize / 8) + 1;"
//  "      *(++aiDstk) = nSize;"
  "\n"
  "\n  #define S_PopStr(__azDst)"
  "      nSize = *(aiDstk--);"
  "      azSrc = (char*)(aiDstk - ((nSize / 8) + 1));"
  "      strcpy(__azDst,azSrc);"
  "      aiDstk -= (nSize / 8) + 2;"
  "\n"
  "\n  #define S_PushStr(__azStr)"
  "      nSize = strlen(__azStr) + 1;"
  "      azDst = (char*)(++aiDstk);"
  "      strcpy(azDst,__azStr);"
  "      aiDstk += (nSize / 8) + 1;"
  "      *(++aiDstk) = nSize;"
  "\n"
  "\n  #define S_UseStr(__szStz)"
  "      nSize = *(aiDstk--);"
  "      __szStz = (Stz*)(aiDstk - ((nSize / 8) + 1));"
  "\n"
  "\n  #define S_UseEnd()"
  "      aiDstk -= (nSize / 8) + 2;"
  "\n"
  "\n  #define S_StrAdr() ( (char*) ( (aiDstk-1) - ((*aiDstk / 8) + 1) ) )"
  "\n"
  "\n  #define S_StrDrp()"
  "      nSize = *(aiDstk--);"
  "      aiDstk -= (nSize / 8) + 2;"
  "\n"
  "\n  //#define JumpTo(__iArg,__EO) { vRlu_ENDTMR(pLU,(i8)__EO); goto L_##__iArg; }"
  "\n  #define JumpTo(__iArg,__EO) goto L_##__iArg;"
  "\n"
  "";
  
  s4K sReplaced;

  sReplaced.vSprint((Stz*)azFrag,m_pLogicUnit->iQueryFragment());
  
  StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
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
  "\n  i8* aiDynFun(void* pLU, i8 nOuterArgs, i8 iOuterArg0)"
  "\n  {"
  "\n    struct RluCommon* aRluCommon = nullptr;"
  "\n"
  "\n    i8* aiDstk = nullptr;"
  "\n"
  "\n    i8 iTopA;"
  "\n    i8 iTopB;"
  "\n    i8 iCmp;"
  "\n    i8 iRes;"
  "\n    Stz* szTopA;"
  "\n    Stz* szTopB;"
  "\n    Stz* szRes;"
  "\n    i8 nSize;"
  "\n    char* azSrc;"
  "\n    char* azDst;"
  "\n    char* azArg;"
  "\n    i8 iBuffer;"
  "\n    i8 iItem;"
  "\n    struct Ent_int* aEnt_int;"
  "\n"
//  "\n    printf(\"Sizeof Ent_Int   : %d\n\",sizeof(struct Ent_int));"
//  "\n    printf(\"Sizeof RluCommon : %d\n\",sizeof(struct RluCommon));"
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
/* the preliminary code fragment for a subroutine declaration                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_SbrDeclaration(Stz* szSbrNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  i8* %s(void* pLU, i8* aiDstk, struct RluCommon* aRluCommon);"
  "\n"
  "";
  
  s4K sReplaced;

  sReplaced.vSprint((Stz*)azFrag,szSbrNm->azChar());
  
  StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
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
  "\n    i8 iCmp;"
  "\n    i8 iRes;"
  "\n    Stz* szTopA;"
  "\n    Stz* szTopB;"
  "\n    Stz* szRes;"
  "\n    i8 nSize;"
  "\n    char* azSrc;"
  "\n    char* azDst;"
  "\n    char* azArg;"
  "\n    i8 iBuffer;"
  "\n    i8 iItem;"
  "\n    struct Ent_int* aEnt_int;"
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



//  void* pLabel = && L_Label;
//
//  goto *pLabel;
//L_Label: ;


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
