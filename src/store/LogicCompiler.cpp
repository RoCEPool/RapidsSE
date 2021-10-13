
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
extern i8 giaVectorSet[];

char gcaEoFragments[C_128K];
Stz* gszaEoFragments[EO_ENUMS];

char gcaAmMacros[C_128K];
Stz* gszaAmMacros[AM_ENUMS];

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

#include "DEV_interface.h"
#include "LIB_interface.h"
#include "RLU_interface.h"
#include "MathLib.h"


/*----------------------------------------------------------------------------*/
/* symbols for dynamically generated code                                     */
/*----------------------------------------------------------------------------*/
DynamicSymbol gxaDynamicSymbol[]
{
  { "giaVectorSet"_Stz,          (const void *) giaVectorSet          },

  { "vDev_IXT_COLS_TO_LOCALS"_Stz, (const void *) vDev_IXT_COLS_TO_LOCALS    },
  { "vDev_IXT_COLS_TO_ROW"_Stz,    (const void *) vDev_IXT_COLS_TO_ROW       },
  { "vDev_IXT_COLS_TO_ROW_OFS"_Stz,(const void *) vDev_IXT_COLS_TO_ROW_OFS   },

  { "vDev_IXT_COL_PUSHSET_ADR"_Stz,(const void *) vDev_IXT_COL_PUSHSET_ADR   },
  { "vDev_IXT_COL_PUSHSET_PST"_Stz,(const void *) vDev_IXT_COL_PUSHSET_PST   },

  { "vDev_IXT_CRWBUF_BEGIN"_Stz,   (const void *) vDev_IXT_CRWBUF_BEGIN      },
  { "vDev_IXT_CRWBUF_FINDCRW"_Stz, (const void *) vDev_IXT_CRWBUF_FINDCRW    },
  { "vDev_IXT_CRWBUF_NEXT"_Stz,    (const void *) vDev_IXT_CRWBUF_NEXT       },
  { "vDev_IXT_CRWBUF_NULL"_Stz,    (const void *) vDev_IXT_CRWBUF_NULL       },
  { "vDev_IXT_CRWBUF_ROWCNT"_Stz,  (const void *) vDev_IXT_CRWBUF_ROWCNT     },
  { "vDev_IXT_CRWBUF_ROWNO"_Stz,   (const void *) vDev_IXT_CRWBUF_ROWNO      },
  { "vDev_IXT_CRWBUF_TEST"_Stz,    (const void *) vDev_IXT_CRWBUF_TEST       },

  { "vDev_IXT_ENT_COMPARE"_Stz,   (const void *) vDev_IXT_ENT_COMPARE       },
  { "vDev_IXT_ENT_DEREF"_Stz,     (const void *) vDev_IXT_ENT_DEREF         },

  { "vDev_IXT_ENT_FIND"_Stz,      (const void *) vDev_IXT_ENT_FIND          },
  { "vDev_IXT_ENT_NEXT"_Stz,      (const void *) vDev_IXT_ENT_NEXT          },
  { "vDev_IXT_ENT_POSPRIOR"_Stz,  (const void *) vDev_IXT_ENT_POSPRIOR      },
  { "vDev_IXT_ENT_PRINTKEYS"_Stz, (const void *) vDev_IXT_ENT_PRINTKEYS     },
  { "vDev_IXT_ENT_PUSH"_Stz,      (const void *) vDev_IXT_ENT_PUSH          },
  { "vDev_IXT_ENT_PUSHELMI0"_Stz, (const void *) vDev_IXT_ENT_PUSHELMI0     },
  { "vDev_IXT_ENT_PUSHELMS0"_Stz, (const void *) vDev_IXT_ENT_PUSHELMS0     },
  { "vDev_IXT_ENT_PUSHUPPER"_Stz, (const void *) vDev_IXT_ENT_PUSHUPPER     },

  { "vDev_ROW_COLS_PUSHSET_PST"_Stz,(const void *) vDev_ROW_COLS_PUSHSET_PST},
  { "vDev_ROW_COLS_TO_LOCALS"_Stz, (const void *) vDev_ROW_COLS_TO_LOCALS   },
  { "vDev_ROW_COLS_TO_ROW"_Stz,    (const void *) vDev_ROW_COLS_TO_ROW      },
  { "vDev_ROW_COLS_TO_ROW_OFS"_Stz,(const void *) vDev_ROW_COLS_TO_ROW_OFS  },
  { "vDev_ROW_NULS_TO_ROW_OFS"_Stz,(const void *) vDev_ROW_NULS_TO_ROW_OFS  },

  { "vDev_ROW_ENTWRK_APPENDELMI"_Stz, (const void *) vDev_ROW_ENTWRK_APPENDELMI },
  { "vDev_ROW_ENTWRK_APPENDELMS"_Stz, (const void *) vDev_ROW_ENTWRK_APPENDELMS },
  { "vDev_ROW_ENTWRK_CREATEPAY"_Stz,  (const void *) vDev_ROW_ENTWRK_CREATEPAY  },

  { "vDev_ENTWRK_PRINT"_Stz,      (const void *) vDev_ENTWRK_PRINT      },
  { "vDev_ENTWRK_PUSHELMI"_Stz,   (const void *) vDev_ENTWRK_PUSHELMI   },
  { "vDev_ENTWRK_PUSHELMS"_Stz,   (const void *) vDev_ENTWRK_PUSHELMS   },
  { "vDev_ENTWRK_RESET"_Stz,      (const void *) vDev_ENTWRK_RESET      },
  { "vDev_ENTWRK_SETENTKND"_Stz,  (const void *) vDev_ENTWRK_SETENTKND  },
  { "vDev_ENTWRK_SETINTELM"_Stz,  (const void *) vDev_ENTWRK_SETINTELM  },
  { "vDev_ENTWRK_SETINTELM_OLD"_Stz,  (const void *) vDev_ENTWRK_SETINTELM_OLD  },
  { "vDev_ENTWRK_SETSTZELM"_Stz,  (const void *) vDev_ENTWRK_SETSTZELM  },

  { "vDev_IXT_CMP_KEYS"_Stz,      (const void *) vDev_IXT_CMP_KEYS      },
  { "vDev_IXT_IDX_DEL"_Stz,       (const void *) vDev_IXT_IDX_DEL       },
  { "vDev_IXT_IDX_DUMP"_Stz,      (const void *) vDev_IXT_IDX_DUMP      },
  { "vDev_IXT_ENT_PRINT"_Stz,     (const void *) vDev_IXT_ENT_PRINT     },
  { "vDev_IXT_INDEXNM_SET"_Stz,   (const void *) vDev_IXT_INDEXNM_SET   },
  { "vDev_IXT_INDEXTREE_SET"_Stz, (const void *) vDev_IXT_INDEXTREE_SET },
  { "vDev_IXT_IDX_INS"_Stz,       (const void *) vDev_IXT_IDX_INS       },
  { "vDev_IXT_IDX_ENTRIES"_Stz,   (const void *) vDev_IXT_IDX_ENTRIES   },

  { "vDev_IXT_POS_BYKEY"_Stz,     (const void *) vDev_IXT_POS_BYKEY     },
  { "vDev_IXT_POS_LBOUND_GE"_Stz, (const void *) vDev_IXT_POS_LBOUND_GE },
  { "vDev_IXT_POS_LBOUND_GE_OLD"_Stz, (const void *) vDev_IXT_POS_LBOUND_GE_OLD },
  { "vDev_IXT_POS_LBOUND_GT"_Stz, (const void *) vDev_IXT_POS_LBOUND_GT },
  { "vDev_IXT_POS_UBOUND_LE"_Stz, (const void *) vDev_IXT_POS_UBOUND_LE },
  { "vDev_IXT_POS_UBOUND_LT"_Stz, (const void *) vDev_IXT_POS_UBOUND_LT },
  { "vDev_IXT_POS_UBOUND_LT_OLD"_Stz, (const void *) vDev_IXT_POS_UBOUND_LT_OLD },
  { "vDev_IXT_PUSH_ENT"_Stz,      (const void *) vDev_IXT_PUSH_ENT      },
  { "vDev_IXT_IDX_BEGIN"_Stz,     (const void *) vDev_IXT_IDX_BEGIN     },
  { "vDev_IXT_IDX_NEXT"_Stz,      (const void *) vDev_IXT_IDX_NEXT      },

  { "vDev_PAY_ADDRESS"_Stz,       (const void *) vDev_PAY_ADDRESS       },

  { "vDev_RLU_ROWPIX_PRINT"_Stz,  (const void *) vDev_RLU_ROWPIX_PRINT  },

  { "vDev_POT_USE_STREAMDEFOUT"_Stz, (const void *) vDev_POT_USE_STREAMDEFOUT },

  { "vDev_ROW_ROW_NEW"_Stz,       (const void *) vDev_ROW_ROW_NEW       },
  { "vDev_ROW_ROW_PRINT"_Stz,     (const void *) vDev_ROW_ROW_PRINT     },
  { "vDev_ROW_ROW_SET"_Stz,       (const void *) vDev_ROW_ROW_SET       },
  { "vDev_ROW_ROW_TEST"_Stz,      (const void *) vDev_ROW_ROW_TEST      },
  { "vDev_ROW_ROW_ZERO"_Stz,      (const void *) vDev_ROW_ROW_ZERO      },

  { "vDev_ROW_ROWBUF_BEGIN"_Stz,  (const void *) vDev_ROW_ROWBUF_BEGIN  },
  { "vDev_ROW_ROWBUF_FINDROW"_Stz,(const void *) vDev_ROW_ROWBUF_FINDROW},
  { "vDev_ROW_ROWBUF_NEXT"_Stz,   (const void *) vDev_ROW_ROWBUF_NEXT   },
  { "vDev_ROW_ROWBUF_NULL"_Stz,   (const void *) vDev_ROW_ROWBUF_NULL   },
  { "vDev_ROW_ROWBUF_RESET"_Stz,  (const void *) vDev_ROW_ROWBUF_RESET  },
  { "vDev_ROW_ROWBUF_ROWCNT"_Stz, (const void *) vDev_ROW_ROWBUF_ROWCNT },
  { "vDev_ROW_ROWBUF_ROWNO"_Stz,  (const void *) vDev_ROW_ROWBUF_ROWNO  },
  { "vDev_ROW_ROWBUF_TEST"_Stz,   (const void *) vDev_ROW_ROWBUF_TEST  },

  { "vDev_ROW_POPCOLI"_Stz,       (const void *) vDev_ROW_POPCOLI       },
  { "vDev_ROW_POPCOLS"_Stz,       (const void *) vDev_ROW_POPCOLS       },
  { "vDev_ROW_PUSHCOLI"_Stz,      (const void *) vDev_ROW_PUSHCOLI      },
  { "szDev_ROW_PUSHCOLS"_Stz,     (const void *) szDev_ROW_PUSHCOLS     },
  { "vDev_ROW_PUSHCOLS"_Stz,      (const void *) vDev_ROW_PUSHCOLS      },

  { "vDev_ROW_TABLENM_SET"_Stz,   (const void *) vDev_ROW_TABLENM_SET   },


  { "vDev_TABLEPTN_DROPREAD"_Stz,   (const void *) vDev_TABLEPTN_DROPREAD   },
  { "vDev_TABLEPTN_GRABREAD"_Stz,   (const void *) vDev_TABLEPTN_GRABREAD   },

  { "vDev_ROW_COLS_TO_LOCALS"_Stz,  (const void *) vDev_ROW_COLS_TO_LOCALS  },

  { "vDev_POT_USE_STREAMDEFOUT"_Stz, (const void *) vDev_POT_USE_STREAMDEFOUT       },

  { "vDev_REG_EOSMARKER_NEW"_Stz,          (const void *) vDev_REG_EOSMARKER_NEW      },
  { "vDev_RLU_INDEXDEF_ADDCOLUMN"_Stz,     (const void *) vDev_RLU_INDEXDEF_ADDCOLUMN },
  { "vDev_RLU_INDEXDEF_DELETE"_Stz,        (const void *) vDev_RLU_INDEXDEF_DELETE    },
  { "vDev_RLU_INDEXDEF_NEW"_Stz,           (const void *) vDev_RLU_INDEXDEF_NEW       },

  { "vDev_RLU_INDEXTREE_NEW"_Stz,          (const void *) vDev_RLU_INDEXTREE_NEW      },

  { "vDev_REG_PTNSTREAM_CLOSE"_Stz,        (const void *) vDev_REG_PTNSTREAM_CLOSE    },
  { "vDev_REG_PTNSTREAM_DELETE"_Stz,       (const void *) vDev_REG_PTNSTREAM_DELETE   },
  { "vDev_REG_PTNSTREAM_NEW"_Stz,          (const void *) vDev_REG_PTNSTREAM_NEW      },
  { "vDev_REG_PTNSTREAM_OPEN"_Stz,         (const void *) vDev_REG_PTNSTREAM_OPEN     },
  { "vDev_REG_PTNSTREAM_READ"_Stz,         (const void *) vDev_REG_PTNSTREAM_READ     },

  { "vDev_REG_STREAMDEFOUT_NEW"_Stz,       (const void *) vDev_REG_STREAMDEFOUT_NEW },
  { "vDev_REG_STREAMDEFOUT_ADDCOLUMN"_Stz, (const void *) vDev_REG_STREAMDEFOUT_ADDCOLUMN },
  { "vDev_REG_STREAMDEF_STREAMDEFOUT"_Stz, (const void *) vDev_REG_STREAMDEF_STREAMDEFOUT },

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
  { "vLib_PRINTF"_Stz,            (const void *) vLib_PRINTF            },

  { "aRlu_INIT"_Stz,              (const void *) aRlu_INIT              },
  { "vRlu_BEGINTMR"_Stz,          (const void *) vRlu_BEGINTMR          },
  { "vRlu_ASSERTSTACK"_Stz,       (const void *) vRlu_ASSERTSTACK       },
  { "vRlu_DUMP"_Stz,              (const void *) vRlu_DUMP              },
  { "vRlu_ENDTMR"_Stz,            (const void *) vRlu_ENDTMR            },
  { "vRlu_ENTER"_Stz,             (const void *) vRlu_ENTER             },
  { "vRlu_EXIT"_Stz,              (const void *) vRlu_EXIT              },
  { "vRlu_HANDLER"_Stz,           (const void *) vRlu_HANDLER           },
  { "vRlu_QUERYFRAG"_Stz,         (const void *) vRlu_QUERYFRAG         },
  { "vRlu_TMR_AGG_ADD"_Stz,       (const void *) vRlu_TMR_AGG_ADD       },
  { "iRlu_TMR_AGG_NSECS"_Stz,     (const void *) iRlu_TMR_AGG_NSECS     },
  { "vRlu_TMR_AGG_RESET"_Stz,     (const void *) vRlu_TMR_AGG_RESET     },
  { "iRlu_TMR_NSECS"_Stz,         (const void *) iRlu_TMR_NSECS         },
  { "iRlu_TMR_SINCE"_Stz,         (const void *) iRlu_TMR_SINCE         },
  { "vRlu_TMR_START"_Stz,         (const void *) vRlu_TMR_START         },
  { "vRlu_TMR_UPDATE"_Stz,        (const void *) vRlu_TMR_UPDATE        },

  { "vRev_00"_Stz,                (const void *) vRev_00                },

  { nullptr, nullptr },
};

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
/* extract the EO_ code fragments from LogicUnit interpreter                  */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtract(void)
{
  _static_
    
  if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
    vTrace("LogicCompiler::vExtract -- Extract EO_ Fragments\n"_Stz);
  
  LocalFile oFile;
  
  bool b = oFile.bOpenRead("../src/store/LogicUnit.cpp"_Stz);
  RASSERT(b);
  
  char* azCrs = gcaEoFragments;
  
  s4K sTok;
  while (oFile.bNextLiteTok(sTok)) {
    
    if (sTok.bSame("//+ExtractBegin"_Stz)) {
      if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
        vTrace("Found +ExtractBegin\n"_Stz);
      while (bExtractAll(&oFile,&azCrs, "case"_Stz, "break;//+endCase"_Stz))
        {;}
    }
  }
  
  
  if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER)) {
    vTrace("Fragments:: %s\n"_Stz,gcaEoFragments);
    for (i8 i_EO = 1; i_EO < EO_ENUMS; i_EO++) {
      vTrace("Fragment:: %s\n%s\n"_Stz,aEO[i_EO],gszaEoFragments[i_EO]);
    }
  }
  
  oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* extract all EO_ fragments                                                  */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bExtractAll(LocalFile* pFile, char** aazCrs, Stz* startMarker, Stz* endMarker)
{
  _static_
  
  s4K sTok;
  while (pFile->bNextLiteTok(sTok)) {
    if (sTok.bSame(startMarker)) {
      bool b = pFile->bNextLiteTok(sTok);
      UNUSED(b);
      vExtractFragment(pFile,aazCrs,startMarker,endMarker);
      return true;
    }
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* extract the AM_ macro fragments from AsmMacro.txt                          */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtractAsm(void)
{
  _static_

  if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
    vTrace("LogicCompiler::vExtractAsm -- Extract AM_ Macro Fragments\n"_Stz);

  LocalFile oFile;
  char *azCrs = gcaAmMacros;

  //bool b = pFile->bOpenRead("../asm/AsmMacros.txt"_Stz);
  for (i8 iMacroFile = 0; iMacroFile < C_MACRO_FILES; iMacroFile++) {

    if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
      vTrace("File: %s\n"_Stz,gszaAmMacroFiles[iMacroFile]);
    bool b = oFile.bOpenRead((Stz*)gszaAmMacroFiles[iMacroFile]);
    RASSERT(b);

    s4K sTok;
    while (oFile.bNextLiteTok(sTok)) {

      //vTrace("Found macro\n"_Stz);
      while (bExtractAll(&oFile, &azCrs, "macro"_Stz, "endMacro"_Stz)) { ; }
    }
    oFile.vClose();
  }

   if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER)) {
     for (i8 i_AM = 1; i_AM < AM_ENUMS; i_AM++) {
       vTrace("Macro[%lld]:: %s\n%s\n"_Stz, i_AM, aAM[i_AM], gszaAmMacros[i_AM]);
    }
  }

}

/*----------------------------------------------------------------------------*/
/* extract an EO_ fragment                                                    */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtractFragment(LocalFile* pFile, char** aazCrs, Stz* startMarker, Stz* endMarker) {
  _static_

  i8 i_AM = C_UNKNOWN;

  s4K sTok;
  bool b = pFile->bNextLiteTok(sTok);

  StrLib::vStrip(sTok.azChar(), ':');

  if (startMarker->bSame("case"_Stz)) {
    i8 i_EO = eFindEO(sTok);

    if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
      vTrace("Fragment %s [%lld]\n"_Stz, sTok.szStz(), i_EO);

    gszaEoFragments[i_EO] = (Stz*)*aazCrs;
  }
  else {
    RASSERT(startMarker->bSame("macro"_Stz));
    i_AM = eFindAM(sTok);

    if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
      vTrace("Macro %s [%lld]\n"_Stz, sTok.szStz(), i_AM);

    gszaAmMacros[i_AM] = (Stz *) *aazCrs;
  }


  for (;;) {
    b = pFile->bNextLiteTok(sTok);
    if (!b || sTok.bSame(endMarker))
      break;

    StrLib::vAppendString(aazCrs,sTok.azChar());
  }

  StrLib::vAppendChar(aazCrs,'\n');
  StrLib::vAppendChar(aazCrs,'\0');
  if (AM::TRACE::bVerbose(C_VB_LOGICCOMPILER))
    if (i_AM != C_UNKNOWN)
    vTrace("-- %s\n"_Stz, gszaAmMacros[i_AM]);
}

/*----------------------------------------------------------------------------*/
/* find eAM by name                                                           */
/*----------------------------------------------------------------------------*/
eAM LogicCompiler::eFindAM(Stz* szEOname)
{
  _static_

  for (i8 i_AM = 1; i_AM < AM_ENUMS; i_AM++) {
    char* azMacro = szEOname->azChar() + 3;
    if (StrLib::bSame(aAM[i_AM]->azChar(), azMacro))
      return (eAM) i_AM;
  }

  return AM_NONE;
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
i8 LogicCompiler::iCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  
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
  
  m_pTinyC->vBegin();

  vAcquire();
  bool b = bCompile();
  UNUSED(b);
  vSymbols();
  vRelocate();

  //vRelease();

  m_pTinyC->vEnd();

  m_pLogicUnit->m_pLogicTimer = new LogicTimer(m_pLogicUnit->m_xRluCommon.m_iSql);

  return 1;
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
/* acquire compiler resources                                                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vAcquire(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  m_pTinyC->vAcquire(this, vTinyError);
}

/*----------------------------------------------------------------------------*/
/* call the dynamic function                                                  */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vCall(void* aDynArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  m_pLogicUnit->m_afDynFun(aDynArg,0,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* compile the synthesised code                                               */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  return m_pTinyC->bCompile(m_caSource);
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
  if ( ! m_pTinyC->bRelocate() )
    RASSERT(0);
  
  m_pLogicUnit->m_afDynFun =
    (i8 (*)(void*,i8,i8))m_pTinyC->pFindSymbol("aiDynFun");

  RASSERT(m_pLogicUnit->m_afDynFun);
}

/*----------------------------------------------------------------------------*/
/* add in any symbols                                                         */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSymbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
    
  for (i8 i_Symbol = 0; gxaDynamicSymbol[i_Symbol].m_afSymbol; i_Symbol++)
    m_pTinyC->vAddSymbol((char*)gxaDynamicSymbol[i_Symbol].m_szSymbolNm,
       (void*)gxaDynamicSymbol[i_Symbol].m_afSymbol);
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

  for (i8 i_Code = i_CodeFrom; i_Code <= i_CodeTill; i_Code = i_Code + 2) {

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
    if (AM::bRLUTimer()) {
      sPrint.vSprint("\n    vRlu_BEGINTMR(pLU);\n "_Stz);
      StrLib::vAppendString(&m_azSrcCrs,sPrint.azChar());
    }
    
    if (EO_CALL == eCode) {
      
      iArg = m_pLogicUnit->iCodeArg(i_Code+2);
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
        EA_SUBSCR == EoArgs[eCode].m_eArgument ||
        EA_TIME   == EoArgs[eCode].m_eArgument ||
        EA_TIMESTAMP   == EoArgs[eCode].m_eArgument) {
      i8 iArg = m_pLogicUnit->iCodeArg(i_Code+2);
      StrLib::bReplace(sReplaced.azChar(),azFrag,
                       "iArg",StrLib::wToString(iArg,caInt));
      StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
      i_Code += EoArgs[eCode].m_nArgBytes;
    }
    else
    if (EA_STRING == EoArgs[eCode].m_eArgument) {
      char* azStr = (char*)m_pLogicUnit->aCodeArg(i_Code+2);
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
    if (AM::bRLUTimer()) {
      sPrint.vSprint("    vRlu_ENDTMR(pLU,%s);\n\n "_Stz,aEO[eCode]->azChar());
      StrLib::vAppendString(&m_azSrcCrs,sPrint.azChar());
    }

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
/* add source code fragment for given rangeof eo codes                        */
/*----------------------------------------------------------------------------*/
Stz* LogicCompiler::szGetMacro(eAM eMacro, TagSet* pTagSet, s4K& sDst)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_LOGICCOMPILER, _FOOTER_);

  s32 sTag;
  s4K sReplaced;
  bool bReplaced = false;
  sDst = gszaAmMacros[eMacro];

  //vTrace("SRC: %s\n"_Stz, sDst.azChar());
  for (i8 i_Tag = 0; i_Tag < pTagSet->nEnames(); i_Tag++) {

    //vTrace("tag: %s\n"_Stz, pTagSet->szName(i_Tag));

    // expects only strings in the tagsSet
    switch (pTagSet->eToken(i_Tag)) {
      case TN_INTEGER: {
        i8 iValue = pTagSet->iValue(i_Tag);
        s32 sValue;
        StrLib::vToString(sValue.azChar(), iValue);
        bReplaced = StrLib::bReplace(sReplaced.azChar(), sDst.azChar(),
                         (char *) pTagSet->szName(i_Tag), sValue.azChar());
        break;
      }
      case TN_STRING:
        bReplaced = StrLib::bReplace(sReplaced.azChar(), sDst.azChar(),
                         (char *) pTagSet->szName(i_Tag), (char *) pTagSet->szValue(i_Tag));
        break;

      default:
        RASSERT(0);
    }
    sDst = sReplaced;
    sReplaced = ""_Stz;
  }

  return (Stz*)sDst;
}

/*----------------------------------------------------------------------------*/
/* add source code fragment for main routine                                  */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRtnSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  s4K sReplaced;

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
  "\n  /* Code for Qf%4lld                                                   */"
  "\n  /*====================================================================*/"
  "\n"
  "\n  #include \"tcclib.h\" // include libc header for TCC"
  "\n  int strcmp(const char *__s1, const char *__s2);"
  "\n"
  "\n  typedef char i1;"
  "\n  typedef unsigned short u2;"
  "\n  typedef long long i8;"
  "\n  typedef unsigned char byte;"
  "\n  typedef double d8;"
  "\n  typedef unsigned long long m8;"
  "\n  typedef unsigned long long u8;"
  "\n  typedef char Stz;"
  "\n"
  "\n  typedef unsigned char eEO;"
  "\n  typedef unsigned short tET;"
  "\n"
  "\n  static const i8 C_1MB = 1048576;"
  "\n"
  "\n  static const void* C_ADR_REGION = (void*)0x204800000000;"
  "\n  static const i8 C_DATUM_NULL = -9223372036854775807;"
  "\n  static const i8 C_CRW_HEADBYTES = 32;"
  "\n  static const i8 C_CRW_FIRSTHEAD = 640;"
  "\n  static const tET C_ENT_END_TICKET = 0xFFFF;"
  "\n"
  "\n  #define C_BITS_PER_WORD 64"
  "\n  #define C_MAX_LVL_LEVELS 6"
  "\n  #define C_LOGIC_LOCALS 32"
  "\n  #define C_UNKNOWN -1"
  "\n"
  "\n  enum eKC { KC_BEFORE=1, KC_SAME, KC_AFTER };"
  "\n  enum eSI { "
  "\n             SI_CELLARRAY_ADR=1, SI_CELLARRAY_ANT, SI_CELLARRAY_PST,"
  "\n             SI_KEY,"
  "\n             SI_STRAREA_ADR, SI_STRAREA_OFS"
  "\n           };"
  "\n"
  "\n  #include \"../src/base/EnumGroups.h\""
  "\n  #include \"../src/store/Ent_int.h\""
  "\n  #include \"../src/store/InflateSet.h\""
  "\n  #include \"../src/store/LIB_interface.h\""
  "\n  #include \"../src/store/DEV_interface.h\""
  "\n  #include \"../src/store/RLU_interface.h\""
  "\n  #include \"../src/store/RluCommon.h\""
  "\n  #include \"../src/base/MathLib.h\""
  "\n  #include \"../src/base/eEO.h\""
  "\n  #include \"../src/base/eDM.h\""
  "\n"
  "\n  #define ArgNil() "
  "\n  #define ArgGet(x) "
  "\n  #define dLU0(x) "
  "\n  #define dLU1(x,a) "
  "\n  #define dLU2(x,a,b) "
  "\n  #define dLU3(x,a,b,c) "
  "\n  #define nullptr 0"
  "\n"
  "\n  #define GetStr(_azStr) "
  "\n"
  "\n  #define C_BYTES_PER_WORD 8"
  "\n"
  "\n  #define nWordRoundedStrBytes(_Size) "
  "      ( ( (_Size / C_BYTES_PER_WORD) * C_BYTES_PER_WORD) + "
  "      (_Size %% C_BYTES_PER_WORD ? C_BYTES_PER_WORD : 0) ) "
  "\n"
  "\n  #define I_PushInt(__iValue) "
  "     *(++aiDstk) = __iValue; "
  "\n"
  "\n  #define I_PopInt()   *(aiDstk--) "
  "\n"
  "\n  #define P_PushPtr(__pPtr) "
  "     *(++aiDstk) = (i8)__pPtr; "
  "\n"
  "\n  #define P_PopPtr()  (void*)(*(aiDstk--)) "
  "\n"
  "\n  #define S_PopStr(__azDst)"
  "      if (*aiDstk == C_DATUM_NULL) { "
  "        *(i8*)__azDst = C_DATUM_NULL; "
  "      } "
  "      else {      "
  "        nSize = *(aiDstk--);"
  "        azSrc = (char*)(aiDstk - (nWordRoundedStrBytes(nSize)/8 - 1));"
  "        strcpy(__azDst,azSrc);"
  "        aiDstk -= nWordRoundedStrBytes(nSize)/8;"
  "      } "
  "\n"
  "\n  #define S_PushStr(__azStr)"
  "      if (((i8)(__azStr)) == C_DATUM_NULL) { "
  "        *(++aiDstk) = C_DATUM_NULL; "
  "      } "
  "      else { "
  "        nSize = strlen(__azStr) + 1;"
  "        azDst = (char*)(++aiDstk);"
  "        strcpy(azDst,__azStr);"
  "        aiDstk += nWordRoundedStrBytes(nSize)/8 - 1;"
  "        *(++aiDstk) = nSize;"
  "      } "
  "\n"
  "\n  #define S_StrAdr() ( (char*)(aiDstk) - nWordRoundedStrBytes(*aiDstk) )"
  "\n"
  "\n  #define S_StrDrp()"
  "      if (*aiDstk == C_DATUM_NULL) { "
  "        aiDstk--; "
  "      } "
  "      else {      "
  "        nSize = *(aiDstk--);"
  "        aiDstk -= nWordRoundedStrBytes(nSize)/8;"
  "      } "
  "\n"
  "\n"
  "\n  #define S_StrDup() "
  "      if (*aiDstk == C_DATUM_NULL) { "
  "        *(++aiDstk) = C_DATUM_NULL;  "
  "      } "
  "      else {      "
  "         nSize = *(aiDstk); "
  "         azSrc = (char*)(aiDstk - (nWordRoundedStrBytes(nSize)/8 - 1) - 1);  "
  "         azDst = (char*)(++aiDstk); "
  "         strcpy(azDst,(char*)azSrc); "
  "         aiDstk += nWordRoundedStrBytes(nSize)/8 - 1; "
  "         *(++aiDstk) = nSize; "
  "      } "
  "\n"
  "\n  #define EO_ENTER(eCode) "
  "\n  #define EO_EXIT(eCode) "
  "\n";

  s16KB sReplaced;

  sReplaced.vSprint((Stz*)azFrag,m_pLogicUnit->iQueryFragment());
  //vTrace("After : %s\n"_Stz, sReplaced.azChar());

  StrLib::vAppendString(&m_azSrcCrs,sReplaced.azChar());
  if (AM::bRLUTimer())
    StrLib::vAppendString(&m_azSrcCrs,
                          "\n  #define JumpTo(__iArg,__EO) { vRlu_ENDTMR(pLU,(i8)__EO); goto L_##__iArg; }"
                          "\n"
                          "\n  extern i8 giaVectorSet[];"
                          "\n"
                          ""
                          );
  else
    StrLib::vAppendString(&m_azSrcCrs,
                          "\n  #define JumpTo(__iArg,__EO) goto L_##__iArg;"
                          "\n"
                          "\n  extern i8 giaVectorSet[];"
                          "\n"
                          ""
                          );
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
  "\n    i8* aiLocals;"
  "\n    i8* aiPayload;"
  "\n"
  "\n    i8 iTopA;"
  "\n    i8 iTopB;"
  "\n    i8 iTopC;"
  "\n    i8 iCmp;"
  "\n    i8 iRes;"
  "\n    Stz* szTopA;"
  "\n    Stz* szTopB;"
  "\n    char* azTopA;"
  "\n    char* azTopB;"
  "\n    Stz* szRes;"
  "\n    i8 nSize;"
  "\n    char* azSrc;"
  "\n    char* azDst;"
  "\n    char* azArg;"
  "\n    i8 iBuffer;"
  "\n    i8 iItem;"
  "\n    tET tNext;"
  "\n    struct Ent_int* aEnt_int;"
  "\n    i8 iWrkA, iWrkB, iWrkC, iWrkD, iWrkE, iWrkF;"
  "\n"
  "\n    i8 iaColSet[64];"
  "\n    i8 i_ColSet = 0;"
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
  "\n    char* azTopA;"
  "\n    char* azTopB;"
  "\n    i8 iBuffer;"
  "\n    i8 iItem;"
  "\n    tET tNext;"
  "\n    struct Ent_int* aEnt_int;"
  "\n    i8* aiLocals;"
  "\n    i8* aiPayload;"
  "\n    i8 iWrkA, iWrkB, iWrkC, iWrkD, iWrkE, iWrkF;"
  "\n"
  "\n    i8 iaColSet[64];"
  "\n    i8 i_ColSet = 0;"
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
