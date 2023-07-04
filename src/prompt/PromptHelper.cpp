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
    PromptFun* pFunc = gxPromptCatalog.m_aaFun[i];
    switch(pFunc->m_ePromptFun)
    {
      case PF_ANTLR:           { pFunc->m_bEnabled = false; continue; }
      case PF_APIDEMO:         { pFunc->m_bEnabled = false; continue; }
      case PF_ARGS:            { pFunc->m_bEnabled = false; continue; }
      case PF_ASCII:           { pFunc->m_bEnabled = false; continue; }
      case PF_ATTACHMAP:       { pFunc->m_bEnabled = false; continue; }
      case PF_BG:              { pFunc->m_bEnabled = false; continue; }
      case PF_BP:              { pFunc->m_bEnabled = false; continue; }
      case PF_BPA:             { pFunc->m_bEnabled = false; continue; }
      case PF_BYE:             {                            continue; }
      case PF_CALC:            { pFunc->m_bEnabled = false; continue; }
      case PF_CANCEL:          { pFunc->m_bEnabled = false; continue; }
      case PF_CD:              { pFunc->m_bEnabled = false; continue; }
      case PF_CLASSES:         { pFunc->m_bEnabled = false; continue; }
      case PF_CMDS:            { pFunc->m_bEnabled = false; continue; }
      case PF_CONFIG:          { pFunc->m_bEnabled = false; continue; }
      case PF_CONSTANTS:       { pFunc->m_bEnabled = false; continue; }
      case PF_DDLLOCK:         { pFunc->m_bEnabled = false; continue; }
      case PF_DELTA:           { pFunc->m_bEnabled = false; continue; }
      case PF_DELTARESET:      { pFunc->m_bEnabled = false; continue; }
      case PF_DESCRIBE:        { pFunc->m_bEnabled = false; continue; }
      case PF_DICTREE:        { pFunc->m_bEnabled = false; continue; }
      case PF_DSV:             { pFunc->m_bEnabled = false; continue; }
      case PF_DUMPTOKENS:      { pFunc->m_bEnabled = false; continue; }
      case PF_EDIT:            { pFunc->m_bEnabled = false; continue; }
      case PF_ENUMS:           { pFunc->m_bEnabled = false; continue; }
      case PF_ERRNO:           { pFunc->m_bEnabled = false; continue; }
      case PF_EXECUTING:       { pFunc->m_bEnabled = false; continue; }
      case PF_FACT:            { pFunc->m_bEnabled = false; continue; }
      case PF_FILECAT:         { pFunc->m_bEnabled = false; continue; }
      case PF_FILEPROC:        { pFunc->m_bEnabled = false; continue; }
      case PF_FILERAND:        { pFunc->m_bEnabled = false; continue; }
      case PF_FILESTATS:       { pFunc->m_bEnabled = false; continue; }
      case PF_FLOATS:          { pFunc->m_bEnabled = false; continue; }
      case PF_FOLDERS:         { pFunc->m_bEnabled = false; continue; }
      case PF_FUNS:            { pFunc->m_bEnabled = false; continue; }
      case PF_GIVEMEM:         { pFunc->m_bEnabled = false; continue; }
      case PF_GLOBALS:         { pFunc->m_bEnabled = false; continue; }
      case PF_HASHDEFS:        { pFunc->m_bEnabled = false; continue; }
      case PF_HEAPSUMMARY:     { pFunc->m_bEnabled = false; continue; }
      case PF_HELP:            { pFunc->m_bEnabled = false; continue; }
      case PF_HEXDUMP:         { pFunc->m_bEnabled = false; continue; }
      case PF_HOW:             { pFunc->m_bEnabled = false; continue; }
      case PF_HW:              { pFunc->m_bEnabled = false; continue; }
      case PF_INDEXENTRIES:    { pFunc->m_bEnabled = false; continue; }
      case PF_JOB:             { pFunc->m_bEnabled = false; continue; }
      case PF_JOBCLASS:        { pFunc->m_bEnabled = false; continue; }
      case PF_JOBS:            { pFunc->m_bEnabled = false; continue; }
      case PF_LASTJOB:         { pFunc->m_bEnabled = false; continue; }
      case PF_LET:             { pFunc->m_bEnabled = false; continue; }
      case PF_LIMITS:          { pFunc->m_bEnabled = false; continue; }
      case PF_LOG:             { pFunc->m_bEnabled = false; continue; }
      case PF_LOGNOTE:         { pFunc->m_bEnabled = false; continue; }
      case PF_LOGSECTION:      { pFunc->m_bEnabled = false; continue; }
      case PF_LOGWRITE:        { pFunc->m_bEnabled = false; continue; }
      case PF_LS:              { pFunc->m_bEnabled = false; continue; }
      case PF_LVL:             { pFunc->m_bEnabled = false; continue; }
      case PF_MASKS:           { pFunc->m_bEnabled = false; continue; }
      case PF_MEMORY:          { pFunc->m_bEnabled = false; continue; }
      case PF_MEMORYLEAKS:     { pFunc->m_bEnabled = false; continue; }
      case PF_MEMTEST:         { pFunc->m_bEnabled = false; continue; }
      case PF_MKFIFO:          { pFunc->m_bEnabled = false; continue; }
      case PF_MON:             { pFunc->m_bEnabled = false; continue; }
      case PF_MRI:             { pFunc->m_bEnabled = false; continue; }
      case PF_NOW:             { pFunc->m_bEnabled = false; continue; }
      case PF_NUMBERS:         { pFunc->m_bEnabled = false; continue; }
      case PF_OBJECTLIST:      { pFunc->m_bEnabled = false; continue; }
      case PF_OBJECTVALIDATE:  { pFunc->m_bEnabled = false; continue; }
      case PF_OLIST:           { pFunc->m_bEnabled = false; continue; }
      case PF_OPTIMISERLVL:    { pFunc->m_bEnabled = false; continue; }
      case PF_OUTPUT:          { pFunc->m_bEnabled = false; continue; }
      case PF_OPCAT:           { pFunc->m_bEnabled = false; continue; }
      case PF_OPS:             { pFunc->m_bEnabled = false; continue; }
      case PF_OPSYS:           { pFunc->m_bEnabled = false; continue; }
      case PF_PDSTEST:         { pFunc->m_bEnabled = false; continue; }
      case PF_PLIST:           { pFunc->m_bEnabled = false; continue; }
      case PF_PMC:             { pFunc->m_bEnabled = false; continue; }
      case PF_POW2:            { pFunc->m_bEnabled = false; continue; }
      case PF_PREFIXES:        { pFunc->m_bEnabled = false; continue; }
      case PF_PRINT:           {                            continue; }
      case PF_PROCEDURE:       { pFunc->m_bEnabled = false; continue; }
      case PF_PROMPT:          { pFunc->m_bEnabled = false; continue; }
      case PF_PROMPTDEBUG:     { pFunc->m_bEnabled = false; continue; }
      case PF_PWD:             { pFunc->m_bEnabled = false; continue; }
      case PF_QUESTION:        { pFunc->m_bEnabled = false; continue; }
      case PF_RASSERT:         { pFunc->m_bEnabled = false; continue; }
      case PF_READY:           { pFunc->m_bEnabled = false; continue; }
      case PF_RESOURCES:       { pFunc->m_bEnabled = false; continue; }
      case PF_RETIRED:         { pFunc->m_bEnabled = false; continue; }
      case PF_RMFIFO:          { pFunc->m_bEnabled = false; continue; }
      case PF_RND:             { pFunc->m_bEnabled = false; continue; }
      case PF_SCAN:            {                            continue; }
      case PF_SE:              { pFunc->m_bEnabled = false; continue; }
      case PF_SELECT:          { pFunc->m_bEnabled = false; continue; }
      case PF_SELFIES:         { pFunc->m_bEnabled = false; continue; }
      case PF_SETDBG:          { pFunc->m_bEnabled = false; continue; }
      case PF_SHELL:           { pFunc->m_bEnabled = false; continue; }
      case PF_SHORTCUT:        {                            continue; }
      case PF_SHOW:            { pFunc->m_bEnabled = false; continue; }
      case PF_SINCE:           { pFunc->m_bEnabled = false; continue; }
      case PF_SIZES:           { pFunc->m_bEnabled = false; continue; }
      case PF_SLEEP:           {                            continue; }
      case PF_SLEEPERS:        { pFunc->m_bEnabled = false; continue; }
      case PF_SLEEPING:        { pFunc->m_bEnabled = false; continue; }
      case PF_SNAPSHOTDELTA:   { pFunc->m_bEnabled = false; continue; }
      case PF_SNAPSHOTINSPECT: { pFunc->m_bEnabled = false; continue; }
      case PF_SNAPSHOTLIST:    { pFunc->m_bEnabled = false; continue; }
      case PF_SNAPSHOTNEWER:   { pFunc->m_bEnabled = false; continue; }
      case PF_SNAPSHOTOBJECTS: { pFunc->m_bEnabled = false; continue; }
      case PF_SO:              { pFunc->m_bEnabled = false; continue; }
      case PF_SOCKET:          { pFunc->m_bEnabled = false; continue; }
      case PF_ST:              { pFunc->m_bEnabled = false; continue; }
      case PF_START:           { pFunc->m_bEnabled = false; continue; }
      case PF_STARTCOMPLETED:  { pFunc->m_bEnabled = false; continue; }
      case PF_STARTLINE:       { pFunc->m_bEnabled = false; continue; }
      case PF_STARTREADY:      { pFunc->m_bEnabled = false; continue; }
      case PF_STARTRESET:      { pFunc->m_bEnabled = false; continue; }
      case PF_STARTSTATUS:     { pFunc->m_bEnabled = false; continue; }
      case PF_STORE:           { pFunc->m_bEnabled = false; continue; }
      case PF_TAG:             { pFunc->m_bEnabled = false; continue; }
      case PF_TAGS:            { pFunc->m_bEnabled = false; continue; }
      case PF_TAKEMEM:         { pFunc->m_bEnabled = false; continue; }
      case PF_TASKQUEUES:      { pFunc->m_bEnabled = false; continue; }
      case PF_TEMPNAME:        { pFunc->m_bEnabled = false; continue; }
      case PF_THREADS:         { pFunc->m_bEnabled = false; continue; }
      case PF_TIMER:           { pFunc->m_bEnabled = false; continue; }
      case PF_TOOLS:           { pFunc->m_bEnabled = false; continue; }
      case PF_TPCH:            { pFunc->m_bEnabled = false; continue; }
      case PF_TRACKMEMORY:     { pFunc->m_bEnabled = false; continue; }
      case PF_UPTIME:          { pFunc->m_bEnabled = false; continue; }
      case PF_UT_SKIPLIST:     { pFunc->m_bEnabled = false; continue; }
      case PF_VERBOSE:         { pFunc->m_bEnabled = false; continue; }
      case PF_VERSION:         {                            continue; }
      case PF_WAIT_BG:         { pFunc->m_bEnabled = false; continue; }
      case PF_WAITING:         { pFunc->m_bEnabled = false; continue; }
      case PF_WITH:            { pFunc->m_bEnabled = false; continue; }
      case PF_WITHOUT:         { pFunc->m_bEnabled = false; continue; }
      default:                 { NEVER_REACH;                         }
    }
  }
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
