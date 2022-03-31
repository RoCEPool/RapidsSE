
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
