/*============================================================================*/
/* PromptHelper.cpp -- a set of prompt helper functions                       */
/*============================================================================*/

#include "BASE.h"
#include "PromptFun.hpp"
#include "PromptHelper.hpp"
#include "PromptCatalog.hpp"
#include "ePF.h"

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* process command flags                                                      */
/*----------------------------------------------------------------------------*/
void PromptHelper::vClassFlags(ClassFlags* pFlags,
                              Stz* szGroupNm,
                              Stz* szaEnumNm[],
                              i8   iEnumLimit,
                              TokenVec* pTknVec)
{
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;
  
  i8 nTokens = pTknVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group>
  
  if (nTokens == 1) {
    
    for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
      if (pFlags->bEnum((eCN)i_Enum))
        vTrace("%s : %s\n"_Stz,
               szaEnumNm[i_Enum],
               "on"_Stz);
    
    vTrace("Mark%sAll is currently : %s\n"_Stz,
           szGroupNm,
           pFlags->bAll() ? "on"_Stz : "off"_Stz);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> ?
  // mark<Group> all
  // mark<Group> <name>

  if (nTokens == 2) {
    aTkn1 = pTknVec->pGet(1)->szChars();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
          vTrace("  %s\n"_Stz,szaEnumNm[i_Enum]);
      return;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("Mark%sAll is currently : %s\n"_Stz,
             szGroupNm,
             pFlags->bAll() ? "on"_Stz : "off"_Stz);
      return;
    }
    
    for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
      if (aTkn1->bSame(szaEnumNm[i_Enum]))
        vTrace("%s : %s\n"_Stz,
               szaEnumNm[i_Enum],
               pFlags->bEnum(i_Enum) ? "on"_Stz : "off"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> all on
  // mark<Group> all off
  // mark<Group> <name> on
  // mark<Group> <name> off

  if (nTokens == 3) {
    aTkn1 = pTknVec->pGet(1)->szChars();
    aTkn2 = pTknVec->pGet(2)->szChars();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        pFlags->vAll(true);
      else
        if (aTkn2->bIsSomeFalse())
          pFlags->vAll(false);
      return;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumNm[i_Enum]))
          pFlags->vEnum(i_Enum,true);
      return;
    }
    
    if (aTkn2->bIsSomeFalse()) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumNm[i_Enum]))
          pFlags->vEnum(i_Enum,false);
      return;
    }
    
    return;
  }

}

/*----------------------------------------------------------------------------*/
/* disable prompt functions                                                   */
/*----------------------------------------------------------------------------*/
void PromptHelper::vDisableFuncs(void)
{
  _static_

  for(i8 i = 0; i < gxPromptCatalog.m_nFunctions; i++)
  {
    switch(gxPromptCatalog.m_aaFun[i]->m_ePromptFun)
    {
      case PF_ANTLR: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_APIDEMO: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_ARGS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_ASCII: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_ATTACHMAP: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_BG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_BP: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_BPA: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_BYE: {
//        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CALC: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CANCEL: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CD: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CLASSES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CMDS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CONFIG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_CONSTANTS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_DDLLOCK: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_DELTA: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_DELTARESET: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_DESCRIBE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_DSV: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_DUMPTOKENS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_EDIT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_ENUMS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_ERRNO: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_EXECUTING: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FACT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FILECAT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FILEPROC: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FILERAND: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FILESTATS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FLOATS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FOLDERS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_FUNS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_GIVEMEM: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_GLOBALS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_HASHDEFS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_HEAPSUMMARY: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_HELP: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_HEXDUMP: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_HOW: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_HW: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_INDEXENTRIES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_JOB: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_JOBCLASS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_JOBS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LASTJOB: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LET: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LIMITS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LOG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LOGNOTE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LOGSECTION: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LOGWRITE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_LVL: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MASKS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MEMORY: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MEMORYLEAKS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MEMTEST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MKFIFO: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MON: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_MRI: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_NOW: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_NUMBERS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OBJECTLIST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OBJECTVALIDATE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OLIST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OPTIMISERLVL: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OUTPUT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OPCAT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OPS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_OPSYS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PLIST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PMC: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_POW2: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PREFIXES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PRINT: {
//        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PROCEDURE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PROMPT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PROMPTDEBUG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_PWD: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_QUESTION: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_RASSERT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_READY: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_REAP: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_RESOURCES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_RETIRED: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_RMFIFO: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_RND: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SCAN: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SELECT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SELFIES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SETDBG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SHELL: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SHORTCUT: {
//        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SHOW: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SINCE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SIZES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SLEEP: {
//        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SLEEPERS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SLEEPING: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SNAPSHOTDELTA: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SNAPSHOTINSPECT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SNAPSHOTLIST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SNAPSHOTNEWER: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SNAPSHOTOBJECTS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SO: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_SOCKET: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_ST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_START: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_STARTCOMPLETED: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_STARTLINE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_STARTREADY: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_STARTRESET: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_STARTSTATUS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_STORE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TAG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TAGS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TAKEMEM: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TASKQUEUES: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TEMPNAME: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_THREADS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TIMER: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TOOLS: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TPCH: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_TRACKMEMORY: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_UPTIME: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_UT_SKIPLIST: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_VERBOSE: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_VERSION: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_WAIT_BG: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_WAITING: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_WITH: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      case PF_WITHOUT: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }

      default: {
        gxPromptCatalog.m_aaFun[i]->m_bEnabled = false;
        continue;
      }
    }
  }
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
