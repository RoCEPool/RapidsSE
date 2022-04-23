/*============================================================================*/
/* Prompt_ALLINONE.cpp - all prompt functions                                 */
/*============================================================================*/

#include "BASE.h"

#include "Prompt_ALLINONE.hpp"

#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "Api.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Session.hpp"
#include "Definition.hpp"
#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "StreamDef.hpp"
#include "VertexSpec.hpp"
#include "Stream.hpp"
#include "JobFilePath.hpp"
#include "Job.hpp"
#include "JobList.hpp"

#include "Pipeline.hpp"
#include "Procedure.hpp"
#include "Sessions.hpp"

#include "Dynamo.hpp"

#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"
#include "PromptHelper.hpp"

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"

#include "PlanNode.hpp"
#include "PlanDefinitionParser.hpp"
#include "SqlPlanner.hpp"

#include "CmdLineArgs.hpp"
#include "Locator.hpp"
#include "LogicUnit.hpp"
#include "SP_Job.hpp"

#include "UT_SkipList.hpp"
#include "AttachMap.hpp"
#include "Output.hpp"

extern CmdLineArgs gxCmdLineArgs;
extern AttachMap* gpAttachMap;

//using namespace Parser;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_args.cpp -- show command line arguments                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_args (
  PF_ARGS,           // prompt function
  "Prompt_args"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "args"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_args,      // prompt function
  true,              // enabled

  true,              // visible
  "args"_Stz,        // help
  ""_Stz             // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_args(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::vPrintArguments();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ascii.cpp -- Ascii Helper                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_ascii (
  PF_ASCII,           // prompt function
  "Prompt_ascii"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "- ascii"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_ascii,      // prompt function
  true,               // enabled

  true,               // visible
  "ascii"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ascii(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();

  if (1 == iNumTokens) {
    
    for (i4 i_Char = 0; i_Char < 128; i_Char++) {
      if (0 == i_Char % 8)
        vTrace("| "_Stz);
      vTrace("%02x %-3s | "_Stz,i_Char,StrLib::azAscii(i_Char));
      if (i_Char && 0 == (i_Char+1) % 8)
        vTrace("\n"_Stz);
    }

    return true;
  }
  
  Token* pTkn1 = pInpTknVec->pGet(1);

  char* aChar = (char*)pTkn1->szChars();
  vTrace("Ascii: "_Stz);
  while (*aChar)
    vTrace("%02x "_Stz,*aChar++);
  vTrace("\n"_Stz);

  aChar = (char*)pTkn1->szChars();
  vTrace("     : "_Stz);
  while (*aChar)
    vTrace(" %c "_Stz,*aChar++);
  vTrace("\n"_Stz);
    
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_attachMap.hpp -- Print AttachMap Information                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_attachMap (
  PF_ATTACHMAP,           // prompt function
  "Prompt_attachMap"_Stz, // name of the prompt function
  "comment"_Stz,          // comment about the function
  "-- attachMap"_Stz,        // the prompt command
  nullptr,                // the second word
  bPrompt_attachMap,      // prompt function
  true,                   // enabled

  true,                   // visible
  "attachMap"_Stz,        // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_attachMap(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  if (AM::bAttached())
    AM::vPrintAttachMap();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_bp.cpp -- buffer pool report                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_bp (
  PF_BP,                // prompt function
  "Prompt_bp"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "bp"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_bp,           // prompt function
  true,                 // enabled

  true,                 // visible
  "bp"_Stz,             // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_bp(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  
  BufferPool::vReport();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_bpa.cpp -- buffer pool analysis                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_bpa (
  PF_BPA,               // prompt function
  "Prompt_bpa"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "bpa"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_bpa,          // prompt function
  true,                 // enabled

  true,                 // visible
  "bpa"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_bpa(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  PoolHelper::vPoolAnalyze();
  
  vTrace("\nsuccess: 0 rows affected\n"_Stz);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_bye.cpp -- bye from prompt                                          */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_bye (
  PF_BYE,            // prompt function
  "Prompt_bye"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the function
  "bye"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_bye,       // prompt function
  true,              // enabled

  true,              // visible
  "bye"_Stz,         // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_bye(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  pPrompt->vContinue(false);
  
  vTrace("\nGood Bye!\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_calc.cpp -- calc                                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_calc (
  PF_BYE,            // prompt function
  "Prompt_calc"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "calc"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_calc,      // prompt function
  true,              // enabled

  true,              // visible
  "calc"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_calc(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

//  TokenVec* pTknVec = pPrompt->pInpTknVec();

//  Calculator* pCalculator = new Calculator;
// 
//  TokenVec* pCalcTknVec = TokenVec::pGetSubVec(pTknVec,1, pTknVec->nTokens() - 1);
//  
//  if (! pCalculator->bCompile(pCalcTknVec)) {
//    delete pCalculator;
//    return true;
//  }
///*
//  //                     a    b    c    d    e    f    g    h    i
//  i8 aiColValues[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900 };
//  
//  i8 iResult = 0;
//
//  //for (i8 i = 0; i < C_10M; i++)
//    iResult = pCalculator->iEvaluate(aiColValues);
//*/
//  
//  i8 iResult = pCalculator->iCalc();
//  
//  vTrace("  %lld\n"_Stz,iResult);
//  
//  delete pCalculator;
//  
//  delete pCalcTknVec;
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_cancel.cpp -- cancel a pipeline job                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_cancel (
  PF_CANCEL,            // prompt function
  "Prompt_cancel"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "cancel"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_cancel,      // prompt function
  true,                // enabled

  true,              // visible
  "cancel"_Stz,      // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_cancel(SePrompt* pPrompt)
{
  RASSERT(pPrompt);


  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("cancel"_Stz));
  
  JobId tJobId;
  tJobId = pInpTknVec->pGet(1)->iToI8();
  
  // find the pipeline job
  Job* pJob = AM::pJob_FindParticipating(tJobId);
  
  if ( ! pJob ) {
    vTrace("job %lld not found\n"_Stz, tJobId.iAsI8());
    AM::vJob_DumpParticipating();
    return false;
  }
  else
    vTrace("Cancelling job %lld\n"_Stz, tJobId.iAsI8());
  
  // get the pipeline from the job
  Pipeline* pPipeline = pJob->pPipeline();

  // cancel the pipeline
  if ( ! pPipeline->bCancel(nullptr) ) {
    vTrace("Cancel rejected. Job %lld almost done.\n"_Stz, tJobId.iAsI8());
    return false;
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_cd.cpp -- change working directory                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Dir.hpp" //?

PromptFun __Prompt_cd (
  PF_CD,             // prompt function
  "Prompt_cd"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the function
  "cd"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_cd,        // prompt function
  true,              // enabled

  true,              // visible
  "cd"_Stz,          // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_cd(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s256 sDir;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("cd"_Stz));
  
  sDir = pInpTknVec->pGet(1)->szChars();
  
  i8 i_Token;
  for (i_Token = 2; i_Token < C_MAX_TOKENS; i_Token++) {
    if (! pInpTknVec->pGet(i_Token)->bHasChars())
      break;
    sDir.vConcat(pInpTknVec->pGet(i_Token)->szChars());
  }
  
  if ( ! Dir::bChangeCurrent(sDir) ) {
    return false;
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_classes.cpp -- classes report                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_classes (
  PF_CLASSES,           // prompt function
  "Prompt_classes"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "classes"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_classes,      // prompt function
  true,                 // enabled

  true,                 // visible
  "classes"_Stz,        // help
  ""_Stz                // description
);

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_classes(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  char cGroup;
  s256 sName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // eCN enum
  
  cGroup = '~';
  
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {

//    char cFirst = *aCN[i_Class]->azChar();
//
//    if ('~' == cGroup) {
//      if ('A' == cFirst)
//        vTrace("\n"_Stz);
//    }
//    else  {
//      if (cGroup != cFirst)
//        vTrace("\n"_Stz);
//    }
    
    if (StrLib::bEnds(aCN[i_Class]->azChar(),(char*)"__"))
      vTrace("\n"_Stz);
    
    eHT eHeap = gxaSizeHeap[i_Class].m_eWhichHeap;
    
    char cHeap = '?';
         if (HT_INVALID          == eHeap) cHeap = 'I';
    else if (HT_T_LARGE  == eHeap) cHeap = 'L';
    else if (HT_T_MEDIUM == eHeap) cHeap = 'M';
    else if (HT_T_SMALL  == eHeap) cHeap = 'S';
    else if (HT_T_TINY   == eHeap) cHeap = 'T';

    vTrace("    /* %3lld 0x%03X %8lldB Heap:%c */ CN_%s,\n"_Stz,
           
           i_Class,
           i_Class,
           gxaSizeHeap[i_Class].m_nClassSize,
           cHeap,
           aCN[i_Class],
           
           C_VARGS_END
           );
    
//    if ('~' != cGroup)
//      cGroup = cFirst;
//    else if ('A' == cFirst)
//      cGroup = cFirst;
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // enum aCN
  
  vTrace("\n\n"_Stz);

  cGroup = '~';
  
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    
//    char cFirst = *aCN[i_Class]->azChar();
//
//    if ('~' == cGroup) {
//      if ('A' == cFirst)
//        vTrace("\n"_Stz);
//    }
//    else  {
//      if (cGroup != cFirst)
//        vTrace("\n"_Stz);
//    }
    
    
    if (StrLib::bEnds(aCN[i_Class]->azChar(),(char*)"__"))
      vTrace("\n"_Stz);

    sName = "\""_Stz;
    sName.vConcat(aCN[i_Class]);
    sName.vConcat("\"_Stz,"_Stz);
    
    vTrace("    /* %3lld 0x%03X */ %-26s // CN_%s\n"_Stz,
           i_Class,i_Class,sName.azChar(),aCN[i_Class]);
    
//    if ('~' != cGroup)
//      cGroup = cFirst;
//    else if ('A' == cFirst)
//      cGroup = cFirst;
  }

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_cmds.cpp -- prompt cmds function                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "PromptFun.hpp"
#include "PromptCatalog.hpp"

PromptFun __Prompt_cmds (
  PF_CMDS,               // prompt function
  "Prompt_cmds"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "cmds -DISABLED-"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_cmds,          // prompt function
  true,                  // enabled

  true,                  // visible
  "cmds"_Stz,            // help
  ""_Stz                 // description
);

extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_cmds(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  
  RASSERT(0);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_config.cpp -- simple configuration report                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


PromptFun __Prompt_config (
  PF_CONFIG,            // prompt function
  "Prompt_config"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the function
  "config"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_config,       // prompt function
  true,                 // enabled

  true,                 // visible
  "config"_Stz,         // help
  ""_Stz                // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_config(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s32 sShmSize;
  MagLib::vByteMag(AM::iRegionSize(), sShmSize);
  
  s32 sFreeSpace;
  MagLib::vByteMag(gpBufferPool->iFreeSpace(), sFreeSpace);
  
  vTrace("  Node %s ShmLib %s Free %s Nodes %lld PtnsPerNode %lld Expressions %lld\n"_Stz,
         gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
         sShmSize.azChar(),
         sFreeSpace.azChar(),
         gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes,
         gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode,
         gxCmdLineArgs.m_xSysArgs.m_nThreads
         );
    
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_constants.cpp -- important constants report                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_constants (
  PF_CONSTANTS,            // prompt function
  "Prompt_constants"_Stz,  // name of the prompt function
  "comment"_Stz,           // comment about the function
  "constants"_Stz,         // the prompt command
  nullptr,                 // the second word
  bPrompt_constants,       // prompt function
  true,                    // enabled

  true,                    // visible
  "constants"_Stz,         // help
  ""_Stz                   // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_constants(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("  C_LOW_BUFFERS       = %lld\n"_Stz,C_LOW_BUFFERS);
  vTrace("  C_MAX_BCD_PRECISION = %lld\n"_Stz,C_MAX_BCD_PRECISION);
  vTrace("  C_MAX_COLUMNS       = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_FLT_PRECISION = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  vTrace("  C_MAX_INT_PRECISION = %lld\n"_Stz,C_MAX_INT_PRECISION);
  vTrace("  C_MAX_NODES         = %lld\n"_Stz,C_MAX_NODES);
  vTrace("  C_MAX_LGLPTNS    = %lld\n"_Stz,C_MAX_LGLPTNS);
  vTrace("  C_MAX_RECORD        = %lld\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_SESSIONS      = %lld\n"_Stz,C_MAX_SESSIONS);
  vTrace("  C_MAX_SHM_PERCENT   = %lld\n"_Stz,C_MAX_SHM_PERCENT);
  vTrace("  C_MAX_PATH          = %lld\n"_Stz,C_MAX_PATH);
  vTrace("  C_MAX_PTNSPERNODE   = %lld\n"_Stz,C_MAX_PTNSPERNODE);
  vTrace("  C_MAX_THREADS       = %lld\n"_Stz,C_MAX_THREADS);
  vTrace("  C_MAX_TOKENS        = %lld\n"_Stz,C_MAX_TOKENS);
  vTrace("  C_MAX_VCH_PRECISION = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  vTrace("  C_MIN_SHM_REGION    = %lld\n"_Stz,C_MIN_SHM_REGION);
  vTrace("  C_WSP_DATACHUNK     = %lld\n"_Stz,C_WSP_DATACHUNK);
  vTrace("  C_WSP_STACK         = %lld\n"_Stz,C_WSP_STACK);
  vTrace("\n"_Stz);

  vTrace("  C_SIZE_SMALL_OBJECT  = %lld\n"_Stz,C_SIZE_SMALL_OBJECT);
  vTrace("  C_SIZE_MEDIUM_OBJECT = %lld\n"_Stz,C_SIZE_MEDIUM_OBJECT);
  vTrace("  C_SIZE_LARGE_OBJECT  = %lld\n"_Stz,C_SIZE_LARGE_OBJECT);
  vTrace("  C_SIZE_POOL_BUFFER   = %lld\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("\n"_Stz);
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ddlLock.cpp -- ddlLock inspection                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_ddlLock (
  PF_DDLLOCK,            // prompt function
  "Prompt_ddlLock"_Stz,  // name of the prompt function
  "comment"_Stz,         // comment about the function
  "ddlLock"_Stz,         // the prompt command
  nullptr,               // the second word
  bPrompt_ddlLock,       // prompt function
  true,                  // enabled

  true,                  // visible
  "ddlLock"_Stz,         // help
  ""_Stz                 // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ddlLock(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("  DdlLock : %016x\n"_Stz,AM::MASTERLOCK::uDdlLockWord());

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_describe.cpp -- Ascii Helper                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

extern SubSysCatalog gxSubSysCatalog;

PromptFun __Prompt_describe (
  PF_DESCRIBE,           // prompt function
  "Prompt_describe"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "- describe"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_describe,      // prompt function
  true,                  // enabled

  false,                 // visible
  "ascii"_Stz,           // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_describe(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  gxSubSysCatalog.vList();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_dlist.cpp -- demon list report                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_dlist (
  PF_OLIST,             // prompt function
  "Prompt_dlist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "dlist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_dlist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "dlist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_dlist(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  AM::pDemonJobs()->vListJobs();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_dsv.cpp -- buffer pool report                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_dsv (
  PF_BP,                // prompt function
  "Prompt_dsv"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "dsv"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_dsv,          // prompt function
  true,                 // enabled

  true,                 // visible
  "dsv"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_dsv(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pGrid()->vDsv();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_dumpTokens.cpp -- dump tokens on/off                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_dumpTokens (
  PF_DUMPTOKENS,           // prompt function
  "Prompt_dumpTokens"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "dumpTokens"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_dumpTokens,      // prompt function
  true,                    // enabled

  true,                 // visible
  "errno"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_dumpTokens(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    Token* pTkn1 = pInpTknVec->pGet(1);
    if (pTkn1->szChars()->bIsSomeTrue())
      pPrompt->vDumpTokens(true);
    else
      pPrompt->vDumpTokens(false);
  }
  else {
    if (pPrompt->bDumpTokens())
      vTrace(" on\n"_Stz);
    else
      vTrace(" off\n"_Stz);
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_edit -- Prompt edit function                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_edit (
    PF_EDIT,                // prompt function
    "Prompt_edit"_Stz,      // name of the prompt function
    "comment"_Stz,          // comment about the function
    "edit"_Stz,             // the prompt command
    nullptr,                // the second word
    bPrompt_edit,           // prompt function
    true,                   // enabled

    true,                   // visible
    "edit"_Stz,        // help
    ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_edit(SePrompt* pPrompt) {
  RASSERT(pPrompt);

  s16KB sSMT;
  s256 sFilePath;

  sSMT = "this is a pipeline"_Stz;
  sFilePath = "../tmp/pipeline.txt"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a file
  File* pFile = new File(pPrompt);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // open file for writing
  bool bOpened = pFile->bOpenWrite(sFilePath.szStz());
  if (! bOpened) {
    vTrace("Unable to open file: %s\n"_Stz,pFile->sFilePath().szStz());
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write sSMT to the file
  pFile->vPrint(sSMT);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // close the file
  pFile->vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // open file via vim
  SysCall oSysCall;
  i8 iExitStatus = Shell::iEdit(pPrompt, sFilePath.szStz(),oSysCall);
  if ( iExitStatus != 0 ) {
    vTrace("Unable to edit file: %s\n"_Stz,pFile->sFilePath().szStz());
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // open file for reading
  bOpened = pFile->bOpenRead(sFilePath);
  if (! bOpened) {
    vTrace("Unable to open file: %s\n"_Stz,pFile->sFilePath().szStz());
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read the file back in
  i8 nBytes = pFile->nReadFile(sSMT);
  UNUSED(nBytes);
  
  vTrace("%s\n"_Stz, sSMT.azChar());

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_errno.cpp -- errno print                                            */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_errno (
  PF_ERRNO,             // prompt function
  "Prompt_errno"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "errno"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_errno,        // prompt function
  true,                 // enabled

  true,                 // visible
  "errno"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_errno(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 iErrNo = 0;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    // get the no of indexEntries to set
    iErrNo = pInpTknVec->pGet(1)->iToI8();

    vTrace("  %s\n"_Stz,strerror((int)iErrNo));
    
    return true;
  }
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  for (iErrNo = 0; iErrNo < 128; iErrNo++) {
    if (StrLib::bContains(strerror((int)iErrNo),"Unknown error"))
      break;
    vTrace("  %lld %s\n"_Stz,iErrNo,strerror((int)iErrNo));
  }

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_executing.cpp -- executing queue report                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_executing (
  PF_EXECUTING,           // prompt function
  "Prompt_executing"_Stz, // name of the prompt function
  "comment"_Stz,          // comment about the function
  "executing"_Stz,        // the prompt command
  nullptr,                // the second word
  bPrompt_executing,      // prompt function
  true,                   // enabled

  true,                   // visible
  "executing"_Stz,        // help
  ""_Stz                  // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_executing(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pSchedule()->vDumpExecutingSet();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_fact.cpp -- fact prompt function                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_fact (
  PF_FACT,            // prompt function
  "Prompt_fact"_Stz,  // name of the prompt function
  "comment"_Stz,      // comment about the function
  "fact"_Stz,         // the prompt command
  nullptr,            // the second word
  bPrompt_fact,       // prompt function
  true,               // enabled

  true,               // visible
  "fact"_Stz,         // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_fact(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

//  TokenVec* pTknVec = pPrompt->pInpTknVec();
//  
//  vTrace("... Fact ...\n"_Stz);
//  
//  Kbase* pKb = new Kbase("Test1"_Stz);
//      
//  DicStack* pDicStack = & pPrompt->pSession()->m_oDicStack;
//  
//  Table* pTable;
//  pTable = Table::pFindTable(pDicStack,"customer"_Stz);
//  pKb->vImportTable(pTable);
//  pTable = Table::pFindTable(pDicStack,"orders"_Stz);
//  pKb->vImportTable(pTable);
//  pTable = Table::pFindTable(pDicStack,"lineitem"_Stz);
//  pKb->vImportTable(pTable);
//
//  pKb->vImportStatement("select * from lineitem"_Stz);
//
//  pKb->vTest();
//
//  delete pKb;
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_file.cpp -- file statistics                                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_file (
  PF_FILESTATS,      // prompt function
  "Prompt_file"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "file"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_file,      // prompt function
  true,              // enabled

  true,              // visible
  "file"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_file(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  File::vStatistics();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_fileCat.cpp -- file cat                                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_fileCat (
  PF_FILECAT,           // prompt function
  "Prompt_fileCat"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "fileCat"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_fileCat,      // prompt function
  true,                 // enabled

  true,              // visible
  "fileCat"_Stz,     // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_fileCat(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s256 sFile;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("fileCat"_Stz));
  
  sFile = pInpTknVec->pGet(1)->szChars();
  i8 nLines = pInpTknVec->pGet(2)->iToI8();

  File* pFile = new File(pPrompt);
  
  if (! pFile->bOpenRead(sFile)) {
    vTrace("No such file\n"_Stz);
    return true;
  }
  
  s16KB sLine;
  int i_Line = 0;
  while (pFile->bReadLine(sLine)) {
    if (++i_Line > nLines)
      break;
    vTrace("%s\n"_Stz,sLine.szStz());
  }
  
  pFile->vClose();
  
  delete pFile;

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_fileProc.cpp -- file proc                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_fileProc (
  PF_FILEPROC,           // prompt function
  "Prompt_fileProc"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "fileProc"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_fileProc,      // prompt function
  true,                  // enabled

  true,               // visible
  "fileProc"_Stz,     // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_fileProc(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s256 sFileInp;
  s256 sFileOut;

  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pTknVec->pGet(0)->bSame("fileProc"_Stz));
  
  sFileInp = pTknVec->pGet(1)->szChars();
  sFileOut = pTknVec->pGet(2)->szChars();
  i8 nLines = pTknVec->nItems() > 3 ? pTknVec->pGet(3)->iToI8() : 0;

  LocalFile oFileInp;
  LocalFile oFileOut;

  if (! oFileInp.bOpenRead(sFileInp)) {
    vTrace("No such input file\n"_Stz);
    return true;
  }
  if (! oFileOut.bOpenWrite(sFileOut)) {
    vTrace("Unable to open output file\n"_Stz);
    return true;
  }

  s16KB sLine_1;
  s16KB sLine_2;
  int i_Line = 0;
  while (oFileInp.bReadLine(sLine_1)) {
    
    if (! oFileInp.bReadLine(sLine_2))
      break;

    if (nLines && ++i_Line > nLines)
      break;
    
    vTrace("%lld: %s%s\n"_Stz,i_Line,sLine_1.szStz(),sLine_2.szStz());
    
    oFileOut.vPrint("%s%s\n"_Stz,sLine_1.szStz(),sLine_2.szStz());
  }
  
  oFileInp.vClose();
  oFileOut.vClose();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_fileRand.cpp -- file randomise                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "SP_Timer.hpp"


PromptFun __Prompt_fileRand (
  PF_FILERAND,           // prompt function
  "Prompt_fileRand"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "fileRand"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_fileRand,      // prompt function
  true,                  // enabled

  true,                  // visible
  "fileRand"_Stz,        // help
  ""_Stz                 // description
);

int PROMPT::iFileRandCompare(const void* aLeft, const void* aRight)
{
  _fileRand* aFileRandLeft  = (_fileRand*)aLeft;
  _fileRand* aFileRandRight = (_fileRand*)aRight;
  
  if (aFileRandLeft->m_iRandom < aFileRandRight->m_iRandom)
    return -1;
  else
  if (aFileRandLeft->m_iRandom > aFileRandRight->m_iRandom)
    return 1;
  else
    return 0;
}

// fileRand "/Users/Shared/tpch/SF1/nation.csv"   "/tmp/nation_RND.csv" ;
// fileRand "/Users/Shared/tpch/SF1/lineItem.csv" "/tmp/lineItem_RND.csv" ;
/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_fileRand(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s256 sPathIn;
  s256 sPathOut;

  i8 nLines = 0;
  
  Timer    xTimer;
  SP_Timer spTimer;

  
  _fileRand* aFileRand = (_fileRand*)malloc(sizeof(_fileRand) * C_10M);
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("fileRand"_Stz));
  
  sPathIn  = pInpTknVec->pGet(1)->szChars();
  sPathOut = pInpTknVec->pGet(2)->szChars();

  vTrace("fileRand %s into %s\n"_Stz,sPathIn.szStz(),sPathOut.szStz());
  
  File* pFileIn  = new File(pPrompt);
  File* pFileOut = new File(pPrompt);
  
  bool bOpened = pFileIn->bOpenRead(sPathIn);
  if (!bOpened) {
    vTrace("Unable to open input file %s\n"_Stz,sPathIn.szStz());
    goto L_Exit;
  }

  bOpened = pFileOut->bOpenWrite(sPathOut);
  if (!bOpened) {
    vTrace("Unable to open output file %s\n"_Stz,sPathOut.szStz());
    goto L_Exit;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  xTimer.vStart();
  while (pFileIn->bReadLine((Stz*)aFileRand[nLines].m_caLine))
    nLines++;
  xTimer.vUpdate();
  vTrace("Read %lld lines (%s)\n"_Stz,nLines,spTimer.sz(&xTimer));
  
  xTimer.vStart();
  for (i8 i_Line = 0; i_Line < nLines; i_Line++)
    aFileRand[i_Line].m_iRandom = RndLib::iInteger();
  xTimer.vUpdate();
  vTrace("Set random numbers (%s)\n"_Stz,spTimer.sz(&xTimer));

  xTimer.vStart();
  qsort(aFileRand,
        nLines,
        sizeof(_fileRand),
        PROMPT::iFileRandCompare
        );
  xTimer.vUpdate();
  vTrace("Sorted (%s)\n"_Stz,spTimer.sz(&xTimer));

  xTimer.vStart();
  for (i8 i_Line = 0; i_Line < nLines; i_Line++)
    pFileOut->vPrint("%s\n"_Stz,aFileRand[i_Line].m_caLine);
  xTimer.vUpdate();
  vTrace("Written (%s)\n"_Stz,spTimer.sz(&xTimer));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_Exit:
  pFileIn->vClose();
  pFileOut->vClose();

  delete pFileIn;
  delete pFileOut;

  free(aFileRand);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_floats.cpp -- prompt floats function                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <float.h>

PromptFun __Prompt_floats (
  PF_FLOATS,             // prompt function
  "Prompt_floats"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "floats"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_floats,        // prompt function
  true,                  // enabled

  true,                  // visible
  "floats"_Stz,          // help
  ""_Stz                 // description
);

extern PromptCatalog gxPromptCatalog;

extern PromptFun __Prompt_actionSequence;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
struct xConv {
  union {
    i8 _i;
    d8 _d;
    byte _y[8];
  };
  
} ;

void vHuh(d8 d)
{
  xConv x;
  
  x._d = d;
  
  printf("(%g : %02x%02x%02x%02x%02x%02x%02x%02x %lld) ",
         x._d,
         x._y[7], x._y[6], x._y[5], x._y[4],
         x._y[3], x._y[2], x._y[1], x._y[0],
         x._i);
}

bool PROMPT::bPrompt_floats(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  struct xConv x;
  
  vTrace("+dbl_min "_Stz); vHuh(DBL_MIN);   vTrace("\n"_Stz);
  vTrace("+dbl_max "_Stz); vHuh(DBL_MAX);   vTrace("\n"_Stz);
  vTrace("-dbl_min "_Stz); vHuh(-DBL_MIN);  vTrace("\n"_Stz);
  vTrace("-dbl_max "_Stz); vHuh(-DBL_MAX);  vTrace("\n"_Stz);

  x._i = C_DATUM_PLUS_INFINITY;
  vTrace("+dbl_inf "_Stz); vHuh(x._d);  vTrace("\n"_Stz);
  
  x._i = C_DATUM_MINUS_INFINITY;
  vTrace("-dbl_inf "_Stz); vHuh(x._d); vTrace("\n"_Stz);
  
  x._i = C_DATUM_NULL;
  vTrace("null     "_Stz); vHuh(x._d);           vTrace("\n"_Stz);

  x._d = DBL_MAX;
  vTrace("+dbl_inf "_Stz); vHuh(x._d);  vTrace("\n"_Stz);
  
  x._d = -DBL_MIN;
  vTrace("-dbl_inf "_Stz); vHuh(x._d); vTrace("\n"_Stz);
  
  x._d = -DBL_MIN;
  x._i += 1;
  vTrace("null     "_Stz); vHuh(x._d);           vTrace("\n"_Stz);
  
  x._i = C_DATUM_PLUS_INFINITY;
  vTrace("+dbl_inf "_Stz); vHuh(x._d);  vTrace("\n"_Stz);

  x._i = C_DATUM_MINUS_INFINITY;
  vTrace("-dbl_inf "_Stz); vHuh(x._d); vTrace("\n"_Stz);
  
  x._i = C_DATUM_NULL;
  vTrace("null     "_Stz); vHuh(x._d); vTrace("\n"_Stz);

  
  printf("%lld: ",(i8)0);
  vHuh((d8)0.0);
  vHuh((d8)0.0);
  vHuh((d8)0.0);
  vHuh((d8)0.0);
  printf("\n");

  d8 dMptv = 1.0;
  d8 dDptv = 1.0;
  d8 dMntv = -1.0;
  d8 dDntv = -1.0;
  
  for (i8 n = 1; n <= 312; n++) {
    
//    printf("%lld %llx: %llx %llx %llx %llx : %g %g %g %g\n",
//           n,n,
//           dMptv,dDptv, dMntv,dDntv,
//           dMptv,dDptv, dMntv,dDntv);

    printf("%lld: ",n);
    vHuh(dMptv);
    vHuh(dDptv);
    vHuh(dMntv);
    vHuh(dDntv);
    printf("\n");

    
    dMptv *= 10.0;
    dDptv /= 10.0;
    dMntv *= 10.0;
    dDntv /= 10.0;
  }
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_flist.cpp -- prompt folders function                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_flist (
  PF_FOLDERS,          // prompt function
  "Prompt_flist"_Stz,  // name of the prompt function
  "comment"_Stz,       // comment about the function
  "flist"_Stz,         // the prompt command
  nullptr,             // the second word
  bPrompt_flist,       // prompt function
  true,                // enabled

  true,                // visible
  "flist"_Stz,         // help
  ""_Stz               // description
);


/*----------------------------------------------------------------------------*/
/* prompt flist                                                               */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_flist(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
    
  Locator::vFlist();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_funs.cpp -- prompt funs function                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

extern PromptCatalog gxPromptCatalog;
extern PromptFun     __Prompt_actionSequence;

PromptFun __Prompt_funs (
  PF_FUNS,               // prompt function
  "Prompt_funs"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "funs"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_funs,          // prompt function
  true,                  // enabled

  true,                  // visible
  "funs"_Stz,            // help
  ""_Stz                 // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_funs(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  if (AM::bMetadata()) {
    vTrace("metadata: (function VARCHAR)\n"_Stz);
  }
  
  gxPromptCatalog.vFuns(PF_LOGSECTION);
  gxPromptCatalog.vFuns(PF_SSNSTATE);
  gxPromptCatalog.vFuns(PF_ARGS);
  gxPromptCatalog.vFuns(PF_ASCII);
  gxPromptCatalog.vFuns(PF_ATTACHMAP);
  gxPromptCatalog.vFuns(PF_BP);
  gxPromptCatalog.vFuns(PF_BYE);
  gxPromptCatalog.vFuns(PF_CD);
  gxPromptCatalog.vFuns(PF_CLASSES);
  gxPromptCatalog.vFuns(PF_CMDS);
  gxPromptCatalog.vFuns(PF_CONFIG);
  gxPromptCatalog.vFuns(PF_CONSTANTS);
  gxPromptCatalog.vFuns(PF_ENUMS);
  gxPromptCatalog.vFuns(PF_ERRNO);
  gxPromptCatalog.vFuns(PF_EXECUTING);
  gxPromptCatalog.vFuns(PF_FILESTATS);
  gxPromptCatalog.vFuns(PF_FLOATS);
  gxPromptCatalog.vFuns(PF_FUNS);
  gxPromptCatalog.vFuns(PF_GIVEMEM);
  gxPromptCatalog.vFuns(PF_HASHDEFS);
  gxPromptCatalog.vFuns(PF_HEAPSUMMARY);
  gxPromptCatalog.vFuns(PF_HELP);
  gxPromptCatalog.vFuns(PF_HEXDUMP);
  gxPromptCatalog.vFuns(PF_HW);
  gxPromptCatalog.vFuns(PF_INDEXENTRIES);
  gxPromptCatalog.vFuns(PF_JOB);
  gxPromptCatalog.vFuns(PF_JOBCLASS);
  gxPromptCatalog.vFuns(PF_LASTJOB);
  gxPromptCatalog.vFuns(PF_LOGNOTE);
  gxPromptCatalog.vFuns(PF_LOGSECTION);
  gxPromptCatalog.vFuns(PF_LOGWRITE);
  gxPromptCatalog.vFuns(PF_LS);
  gxPromptCatalog.vFuns(PF_LVL);
  gxPromptCatalog.vFuns(PF_MASKS);
  gxPromptCatalog.vFuns(PF_MEMORY);
  gxPromptCatalog.vFuns(PF_MEMORYLEAKS);
  gxPromptCatalog.vFuns(PF_MEMTEST);
  gxPromptCatalog.vFuns(PF_MKFIFO);
  gxPromptCatalog.vFuns(PF_MON);
  gxPromptCatalog.vFuns(PF_NOW);
  gxPromptCatalog.vFuns(PF_NUMBERS);
  gxPromptCatalog.vFuns(PF_OLIST);
  gxPromptCatalog.vFuns(PF_OPSYS);
  gxPromptCatalog.vFuns(PF_PLIST);
  gxPromptCatalog.vFuns(PF_PMC);
  gxPromptCatalog.vFuns(PF_POW2);
  gxPromptCatalog.vFuns(PF_PREFIXES);
  gxPromptCatalog.vFuns(PF_PRINT);
  gxPromptCatalog.vFuns(PF_PWD);
  gxPromptCatalog.vFuns(PF_QUESTION);
  gxPromptCatalog.vFuns(PF_RASSERT);
  gxPromptCatalog.vFuns(PF_READY);
  gxPromptCatalog.vFuns(PF_REAP);
  gxPromptCatalog.vFuns(PF_RESOURCES);
  gxPromptCatalog.vFuns(PF_RETIRED);
  gxPromptCatalog.vFuns(PF_RMFIFO);
  gxPromptCatalog.vFuns(PF_RND);
  gxPromptCatalog.vFuns(PF_SE);
  gxPromptCatalog.vFuns(PF_SHELL);
  gxPromptCatalog.vFuns(PF_SHORTCUT);
  gxPromptCatalog.vFuns(PF_SINCE);
  gxPromptCatalog.vFuns(PF_SIZES);
  gxPromptCatalog.vFuns(PF_SLEEP);
  gxPromptCatalog.vFuns(PF_SLEEPERS);
  gxPromptCatalog.vFuns(PF_SLEEPING);
  gxPromptCatalog.vFuns(PF_SOCKET);
  gxPromptCatalog.vFuns(PF_SSN);
  gxPromptCatalog.vFuns(PF_ST);
  gxPromptCatalog.vFuns(PF_START);
  gxPromptCatalog.vFuns(PF_STARTLINE);
  gxPromptCatalog.vFuns(PF_TAKEMEM);
  gxPromptCatalog.vFuns(PF_TASKQUEUES);
  gxPromptCatalog.vFuns(PF_TEMPNAME);
  gxPromptCatalog.vFuns(PF_THREADS);
  gxPromptCatalog.vFuns(PF_TIMER);
  gxPromptCatalog.vFuns(PF_TRACKMEMORY);
  gxPromptCatalog.vFuns(PF_UPTIME);
  gxPromptCatalog.vFuns(PF_VERSION);
  gxPromptCatalog.vFuns(PF_WAITING);

  vTrace("\nsuccess: %lld rows affected\n"_Stz, gxPromptCatalog.m_nFunctions);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_giveMem.cpp -- give heap memory                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_giveMem (
  PF_GIVEMEM,           // prompt function
  "Prompt_giveMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "giveMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_giveMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "giveMem"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_giveMem(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  gpBufferPool->vGiveBuffers();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_globals.cpp -- print globals                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_globals (
  PF_GIVEMEM,           // prompt function
  "Prompt_globals"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "globals"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_globals,      // prompt function
  true,                 // enabled

  true,                 // visible
  "globals"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_globals(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::vPrintGlobals();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_hashDefs.cpp -- hash define report                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_hashDefs (
  PF_HASHDEFS,          // prompt function
  "Prompt_numbers"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hashDefs"_Stz,       // the prompt command
  nullptr,              // the second word
  bPrompt_hashDefs,     // prompt function
  true,                 // enabled

  true,                 // visible
  "hashDefs"_Stz,       // help
  ""_Stz                // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_hashDefs(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  
  
  vTrace("\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_heapSummary.cpp -- heap summary report                              */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_heapSummary (
  PF_HEAPSUMMARY,           // prompt function
  "Prompt_heapSummary"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the function
  "heapSummary"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_heapSummary,      // prompt function
  true,                     // enabled

  true,                     // visible
  "heapSummary"_Stz,        // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_heapSummary(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  
  Heaps::vSummaryReport();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_help.cpp -- print prompt help                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

extern PromptCatalog gxPromptCatalog;

PromptFun __Prompt_help (
  PF_HELP,           // prompt function
  "Prompt_help"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "help"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_help,      // prompt function
  true,              // enabled

  true,              // visible
  "help"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_help(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("Help\n"_Stz);
  
//  SysCall oSysCall;
//
//  s256 sCommand;
//
//  sCommand.vSprint("man ./ManPages/HelpText"_Stz);
//
//  IGNORE_RETURN(Shell::iExecute(sCommand,oSysCall));
  
  
  
  gxPromptCatalog.vHelp();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_hexdump.cpp -- memory hex dump                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_hexdump (
  PF_HEXDUMP,           // prompt function
  "Prompt_hexdump"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hexdump"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_hexdump,      // prompt function
  true,                 // enabled

  true,                 // visible
  "hexdump"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_hexdump(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 iNumTokens = pInpTknVec->nItems();
  if (2 != iNumTokens)
    return false;
  
  //if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
  //  return false;
    
  void* aAddress = reinterpret_cast<void*>(pInpTknVec->pGet(1)->iToI8());
  
  MemLib::vHexDump(aAddress,C_1KB);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_how.cpp -- how was a request executed                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_how (
  PF_HOW,               // prompt function
  "Prompt_how"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "how"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_how,          // prompt function
  true,                 // enabled

  true,                 // visible
  "how"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_how(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 nTokens = pInpTknVec->nItems();

  //i8 iReqId = pPrompt->iRequestId();
  
  i8 iDetail = C_UNKNOWN;
  if (2 == nTokens)
    iDetail = pInpTknVec->pGet(1)->iToI8();
  
  vTrace("how: request\n"_Stz,
         
         //iReqId,
         
         C_VARGS_END);
    
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_hw.cpp -- print hardware related information                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_hw (
  PF_HW,                // prompt function
  "Prompt_hw"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hw"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_hw,           // prompt function
  true,                 // enabled

  true,                 // visible
  "hw"_Stz,             // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_hw(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s32 sMem;
  MagLib::vByteMag(Hardware::iPhysicalMemory(),sMem);

  s32 sCpuFreq;
  MagLib::vHzMag(Hardware::iCpuFrequency(),sCpuFreq);

  unsigned int x = 0x1;
  char *c = (char*) &x;
  
  vTrace("hw: %lld cores; %s memory; %s cpu frequency; %s\n"_Stz,
         Hardware::iCoreCount(),
         sMem.szStz(),
         sCpuFreq.szStz(),
         *c == 0x1 ? "little endian"_Stz : "big endian"_Stz
         );
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_indexEntries.cpp -- set index entries                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_indexEntries (
  PF_INDEXENTRIES,            // prompt function
  "Prompt_indexEntries"_Stz,  // name of the prompt function
  "comment"_Stz,              // comment about the function
  "indexEntries"_Stz,         // the prompt command
  nullptr,                    // the second word
  bPrompt_indexEntries,       // prompt function
  true,                       // enabled

  true,                       // visible
  "indexEntries"_Stz,         // help
  ""_Stz                      // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_indexEntries(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("indexEntries"_Stz));
  
  // get number of items in token vector
  i8 nTokens = pInpTknVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1/2 tokens
  if (nTokens != 1 && nTokens != 2)
    return false;
  
  if (nTokens == 1) {
    vTrace("IndexEntries: %lld\n"_Stz, AM::nIndexEntries());
    vTrace("\nsuccess: 1 rows affected\n"_Stz);
    return true;
  }
  
  if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
    return false;
  
  // get the no of indexEntries to set
  i8 iIndexEntries = pInpTknVec->pGet(1)->iToI8();
  
  AM::vIndexEntries(iIndexEntries);
  
  vTrace("\nsuccess: 0 rows affected\n"_Stz);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_job.hpp -- Print Job Information                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_job (
  PF_JOB,                 // prompt function
  "Prompt_job"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "job"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_job,            // prompt function
  true,                   // enabled

  true,                   // visible
  "job"_Stz,              // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_job(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Token* pArg0 = nullptr;
  Token* pArg1 = nullptr;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pInpTknVec->nItems();
  
  if (nTokens != 3)
    return false;
  
  pArg0 = pInpTknVec->pGet(1);
  pArg1 = pInpTknVec->pGet(2);
  
  s32 sNumber;
  sNumber.vConcat(pArg1->szChars());
  vTrace("Job: %s\n"_Stz,sNumber.szStz());
  
  i8 iNumber = sNumber.iToI8();
  
  Job* pJob = _pJob((Job*)iNumber);
  
  if (! pJob) {
    vTrace("Not a job\n"_Stz);
    return false;
  }
  
  pJob->vPrintJobContents();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_jobs.cpp -- jobs                                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_jobs (
  PF_JOBS,              // prompt function
  "Prompt_jobs"_Stz,    // name of the prompt function
  "comment"_Stz,        // comment about the function
  "jobs"_Stz,           // the prompt command
  nullptr,              // the second word
  bPrompt_jobs,         // prompt function
  true,                 // enabled

  true,                 // visible
  "jobs"_Stz,           // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_jobs(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  vTrace("Jobs : %lld (threshold %lld)\n"_Stz
         ,AM::JOBS::nJobsActive()
         ,AM::JOBS::nJobsThreshold()
         );
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_lastJob.cpp -- last job id                                          */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_lastJob (
  PF_LASTJOB,           // prompt function
  "Prompt_lastJob"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "lastJob"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_lastJob,      // prompt function
  true,                 // enabled

  true,                 // visible
  "lastJob"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_lastJob(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  SeDeputy* pDeputy = pPrompt->pCurrentDeputy();
  
  JobId tLastJobId = pDeputy->tLastJobId();
  
  vTrace("Last Job Id : %llu\n"_Stz
         ,tLastJobId.uJobNo()
         );
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_limits.cpp -- system limits                                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_limits (
  PF_LIMITS,            // prompt function
  "Prompt_limits"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the function
  "limits"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_limits,       // prompt function
  true,                 // enabled

  true,                 // visible
  "numbers"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_limits(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("\n"_Stz);
  
  vTrace("  C_MAX_COLUMNS  = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_RECORD   = %lldB\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_VCH      = %lld\n"_Stz,C_MAX_VCH);

  vTrace("\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_llist.cpp -- local list report                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_llist (
  PF_OLIST,             // prompt function
  "Prompt_llist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "llist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_llist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "llist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_llist(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  AM::pLocalJobs()->vListJobs();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_log.cpp -- log                                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_log (
  PF_LOG,             // prompt function
  "Prompt_log"_Stz,    // name of the prompt function
  "comment"_Stz,       // comment about the function
  "log"_Stz,           // the prompt command
  nullptr,             // the second word
  bPrompt_log,         // prompt function
  true,                // enabled

  true,                // visible
  "log"_Stz,           // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_log(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  //TokenVec* pInpTknVec = pPrompt->pInpTknVec();
    
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_logNote.cpp -- write a note into the log                            */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_logNote (
  PF_LOGNOTE,           // prompt function
  "Prompt_logNote"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "logNote"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_logNote,      // prompt function
  true,                 // enabled

  true,                 // visible
  "logNote"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_logNote(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  //TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  //ASSERT(pInpTknVec->pGet(0)->bSame("logNote"_Stz));
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_logSection.cpp -- write a section line into the log                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_logSection (
  PF_LOGSECTION,           // prompt function
  "Prompt_logSection"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "logSection"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_logSection,      // prompt function
  true,                    // enabled

  true,                    // visible
  "logSection"_Stz,        // help
  ""_Stz                   // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_logSection(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  if (pInpTknVec->nTokens() < 1)
    return true;
  
  Stz* szSectionNm = pInpTknVec->pGet(1)->szChars();
  i8 iSectNmLen = szSectionNm->nLength();

  char caSection[256] = { '\0' };
  
  StrLib::vPad(caSection,127-iSectNmLen-4,'#');
  
  StrLib::vConcat(caSection,' ');
  StrLib::vConcat(caSection,szSectionNm->azChar());
  StrLib::vConcat(caSection,' ');
  StrLib::vConcat(caSection,'#');
  StrLib::vConcat(caSection,'#');
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Dir.hpp" //?

PromptFun __Prompt_ls (
  PF_LS,             // prompt function
  "Prompt_ls"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the function
  "ls"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_ls,        // prompt function
  true,              // enabled

  true,              // visible
  "ls"_Stz,          // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ls(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  LocalDir oDir;
  s256 sDirPath;
  s256 sEntryNm;
  i8 iEntryType;
                   
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("ls"_Stz));
  UNUSED(pInpTknVec);
  
  // get the current directory
  if (!Dir::bGetCurrent(sDirPath))
    return false;
  
  // open the current directory
  if (!oDir.bOpen(sDirPath))
    return false;
  
  // read the current directory and list contents
  while (oDir.bRead()) {
    sEntryNm = oDir.szEntryNm();
    iEntryType = oDir.iEntryType();
    vTrace("%c %s\n"_Stz, iEntryType == DT_DIR ? 'd' : 'f', sEntryNm.azChar());
  }
  
  // close the current directory
  oDir.vClose();
                   
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_lvl.cpp -- random number helper                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_lvl (
  PF_LVL,           // prompt function
  "Prompt_lvl"_Stz, // name of the prompt function
  "comment"_Stz,    // comment about the function
  "lvl"_Stz,        // the prompt command
  nullptr,          // the second word
  bPrompt_lvl,      // prompt function
  true,             // enabled

  true,             // visible
  "lvl"_Stz,        // help
  ""_Stz            // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_lvl(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 i_Next = 0;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    Token* pTkn0 = pInpTknVec->pGet(1);
    if (pTkn0->szChars()->bIsSomeTrue())
      i_Next = C_UNKNOWN;
  }
  
  i8 nLvlLevels = AM::nLvlLevels();
  i8 iLvlRatio  = AM::iLvlRatio();

  for (i8 i = 0; i < 128; i++) {
    i8 iLevel = RndLib::iLogLevel(nLvlLevels,iLvlRatio,i_Next);
    vTrace("%lld "_Stz,iLevel);
    if (0 == (i+1)%8)
      vTrace("\n"_Stz);
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_masks.cpp -- masks report                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_masks (
  PF_MASKS,             // prompt function
  "Prompt_masks"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "masks"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_masks,        // prompt function
  true,                 // enabled

  true,                 // visible
  "masks"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_masks(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("\n"_Stz);

  vTrace("C_SIZE_POOL_BUFFER      0x%llx\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("C_OBJECT_TO_BUFFER_MASK 0x%llx\n"_Stz,C_OBJECT_TO_BUFFER_MASK);
  vTrace("\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_memTest.cpp -- memory pattern test                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_memTest (
  PF_MEMTEST,           // prompt function
  "Prompt_memTest"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "memTest"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_memTest,      // prompt function
  true,                 // enabled

  true,                 // visible
  "memTest"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_memTest(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  MemTest* pMemTest = new MemTest;
  
  MemLib::vDumpWords(pMemTest,sizeof(MemTest));
  
  i8 iCmp = MemLib::iCompareWords(pMemTest->m_caCharLowerFlipped,
                                  pMemTest->m_caCharUpperFlipped,16);
  
  vTrace("%lld\n"_Stz,iCmp);
  
  delete pMemTest;
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_memory.cpp -- memory statistics                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_memory (
  PF_MEMORY,           // prompt function
  "Prompt_memory"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "memory"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_memory,      // prompt function
  true,                // enabled

  true,                // visible
  "memory"_Stz,        // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_memory(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  bool bReset = false;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  if (3 == iNumTokens)
    bReset = pInpTknVec->pGet(2)->bSame("reset"_Stz);

  PromptHelper::vMemoryStatistics();
  
  if (bReset)
    MemLib::vReset();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_memoryLeaks.cpp -- memory leaks                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_memoryLeaks (
  PF_MEMORYLEAKS,              // prompt function
  "Prompt_memoryLeaks"_Stz,    // name of the prompt function
  "memory leaks"_Stz,          // comment about the function
  "memoryLeaks"_Stz,           // the prompt command
  nullptr,                     // the second word
  bPrompt_memoryLeaks,         // prompt function
  true,                        // enabled

  true,                        // visible
  "memoryLeaks"_Stz,           // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_memoryLeaks(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 nLeaksDetected = 0;
  
  // force memory leaks test
//  Text* pText_leak = new Text;
//  vTrace("Text_leak : %p\n"_Stz,pText_leak);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  JobList* pRetiredJobs = AM::pRetiredJobs();
  i8 nRetired = pRetiredJobs->nJobs();
  
  bool bFatal = false;
  if (2 == iNumTokens && pInpTknVec->pGet(1)->bSame("fatal"_Stz))
    bFatal = true;
  
  // are we tracking memory?
  if ( ! pPrompt->bTrackingMemory() ) {
    vTrace("error: Memory not being tracked; Run trackMemory;\n"_Stz);
    return true;
  }
  
  if (AM::bMetadata())
    vTrace("metadata: (objectName VARCHAR, count INTEGER)\n"_Stz);


  if (2 == iNumTokens && pInpTknVec->pGet(1)->bSame("dump"_Stz)) {
    bPrompt_memoryLeaks_dump(pPrompt);
  }

  
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ObjectSnapshot* pPoolSnapshot_then = pPrompt->pSnapshotAtTrackingMemory();
  ObjectSnapshot* pPoolSnapshot_curr = pPrompt->pSnapshotAtMemoryLeaks();
  

  
  if (gpBufferPool->bDontFree() || AM::pHeaps()->m_bNoFree) {
    vTrace("bPrompt_memoryLeaks: skipping ... (--dontFree supplied)\n"_Stz);
    goto L_END;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for all uset jobs in the system to complete

  // note: the tracking of memory works with an assumption that
  //       whatever was started after trackingMemory has completed

  for (i8 iAttempt = 0; iAttempt < C_ATTEMPTS; iAttempt++) {
    
    Api::vReapJobsOnce();
    
    i8 nJobsActive    = AM::JOBS::nJobsActive();
    i8 nJobsThreshold = AM::JOBS::nJobsThreshold();
    
    vTrace("bPrompt_memoryLeaks: %lld active %lld threshold\n"_Stz,
           nJobsActive,nJobsThreshold);
    
    if (nJobsActive <= nJobsThreshold)
      break;
    
    AM::vNapMsec(200,CN_SEPROMPT);
  }
  
  
  
  
  for (i8 iAttempt = 0; iAttempt < C_ATTEMPTS; iAttempt++) {

    Api::vReapJobsOnce();
    
    nLeaksDetected = 0;
    pPoolSnapshot_curr->vTakeSnapshot();
    
    for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {
      
      eCN eClass = (eCN)i_Class;
      
      if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
        continue;

      // check if this class to be ignored for memory leak detection
      if (gxaSizeHeap[eClass].m_bIgnoreMemoryLeak)
        continue;

      i8 nDelta = pPoolSnapshot_curr->m_nObjects[eClass] -
                    pPoolSnapshot_then->m_nObjects[eClass];
      
//      vTrace("LEAKTEST: %s %lld %lld %lld\n"_Stz,aCN[eClass],
//             pPoolSnapshot_curr->m_nObjects[eClass],
//             pPoolSnapshot_then->m_nObjects[eClass],
//             nDelta);
      
      if (0 == nDelta)
        continue;
      
      // we have a memory leak
      nLeaksDetected++;
      
      // lets retry
      break;
    }
    
    // no memory leaks ?
    if (0 == nLeaksDetected)
      goto L_END;
    
    // we have memory leaks; sleep and try again!
    AM::vNapMsec(200,CN_SEPROMPT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  nRetired = pRetiredJobs->nJobs();
  vTrace("Retired Jobs : %lld)\n"_Stz,nRetired);

  
  // attempted C_ATTEMPTS to see if memory leaks settle down, now time to report
  nLeaksDetected = 0;
  pPoolSnapshot_curr->vTakeSnapshot();

  for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
    
    if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
      continue;

    // check if this class to be ignored for memory leak detection
    if (gxaSizeHeap[eClass].m_bIgnoreMemoryLeak)
      continue;
    
    i8 nDelta = pPoolSnapshot_curr->m_nObjects[eClass] -
                  pPoolSnapshot_then->m_nObjects[eClass];
    
    if (0 == nDelta)
      continue;
    
    vTrace("row: ('%s',  %lld)\n"_Stz,aCN[eClass],nDelta);
    nLeaksDetected++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_END:
  
  vTrace("\nsuccess: %lld rows affected\n"_Stz, nLeaksDetected);
  
  //if (nLeaksDetected)
  //  bPrompt_memoryLeaks_dump(pPrompt);
  
  if (nLeaksDetected && bFatal)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "fatal: memory leaks detected"_Stz);
  
  if (! nLeaksDetected)
    vTrace("No memory leaks detected\n"_Stz);
  
  pPrompt->vTrackMemory(false);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_memoryLeaks_dump(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  PoolHelper::vObjectDump(0);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_mkfifo.cpp -- make fifo                                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Fifo.hpp" //?

PromptFun __Prompt_mkfifo (
  PF_MKFIFO,             // prompt function
  "Prompt_mkfifo"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "mkfifo"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_mkfifo,        // prompt function
  true,                  // enabled

  true,                  // visible
  "mkfifo"_Stz,          // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_mkfifo(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s256 sFifoPath;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("mkfifo"_Stz));
  
  sFifoPath = pInpTknVec->pGet(1)->szChars();
  
  if (! Fifo::bMake(sFifoPath)) {
    vTrace("Failed to make fifo: %s\n"_Stz,sFifoPath.azChar());
    return false;
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_now.cpp -- show current local date and time                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_now (
  PF_NOW,           // prompt function
  "Prompt_now"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the function
  "now"_Stz,         // the prompt command
  nullptr,           // the second word
  bPrompt_now,       // prompt function
  true,              // enabled

  true,              // visible
  "date"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_now(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Now spNow;

  vTrace("%s (local)\n"_Stz,spNow.sz());
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_numbers.cpp -- numbers report                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <float.h>
#include <limits.h>

PromptFun __Prompt_numbers (
  PF_NUMBERS,           // prompt function
  "Prompt_numbers"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "numbers"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_numbers,      // prompt function
  true,                 // enabled

  true,                 // visible
  "numbers"_Stz,        // help
  ""_Stz                // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_numbers(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("\n"_Stz);

  vTrace("  C_MAGIC  = 0x%llx\n"_Stz,C_MAGIC);
  vTrace("  C_FOOTER = 0x%llx\n"_Stz,C_FOOTER);
  vTrace("\n"_Stz);
  
  vTrace("  C_SCRIBBLE_PATTERN = 0x%llx\n"_Stz,C_SCRIBBLE_PATTERN);
  vTrace("\n"_Stz);
  
  vTrace("  C_DATUM_NULL = %lld\n"_Stz,C_DATUM_NULL);
  vTrace("  C_DATUM_MINUS_INFINITY = %lld\n"_Stz,C_DATUM_MINUS_INFINITY);
  vTrace("  C_DATUM_PLUS_INFINITY = %lld\n"_Stz,C_DATUM_PLUS_INFINITY);
  vTrace("\n"_Stz);
  
  vTrace("  C_I8_MIN %&lld = %lld\n"_Stz,C_I8_MIN);
  vTrace("  C_I8_MIN %%llu = %llu\n"_Stz,(u8)C_I8_MIN);
  vTrace("  C_I8_MAX %&lld = %lld\n"_Stz,C_I8_MAX);
  vTrace("  C_I8_MAX %%llu = %llu\n"_Stz,(u8)C_I8_MAX);
  vTrace("\n"_Stz);
  
  vTrace("  LLONG_MIN = %lld\n"_Stz,LLONG_MIN);
  vTrace("  LLONG_MIN = %p\n"_Stz,  LLONG_MIN);
  vTrace("  LLONG_MAX = %lld\n"_Stz,LLONG_MAX);
  vTrace("  LLONG_MAX = %p\n"_Stz,  LLONG_MAX);
  vTrace("\n"_Stz);
  
  vTrace("  DBL_MIN %%e = %e\n"_Stz,DBL_MIN);
  vTrace("  DBL_MIN %%g = %g\n"_Stz,DBL_MIN);
  vTrace("  DBL_MIN %%f = %f\n"_Stz,DBL_MIN);
  vTrace("  DBL_MAX %%e = %e\n"_Stz,DBL_MAX);
  vTrace("  DBL_MAX %%g = %g\n"_Stz,DBL_MAX);
  vTrace("  DBL_MAX %%f = %f\n"_Stz,DBL_MAX);
  vTrace("\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_objectList.cpp -- objects list report                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_objectList (
  PF_OBJECTLIST,            // prompt function
  "Prompt_objectList"_Stz,  // name of the prompt function
  "comment"_Stz,            // comment about the function
  "- objectList"_Stz,         // the prompt command
  nullptr,                  // the second word
  bPrompt_objectList,       // prompt function
  true,                     // enabled

  true,                     // visible
  "objectList"_Stz,         // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_objectList(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  
  ObjectSnapshot* pObjectSnapshot = new ObjectSnapshot;
  
  pObjectSnapshot->vTakeSnapshot();
  
  pObjectSnapshot->vPrintCounts();

  delete pObjectSnapshot;
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_objectValidate.cpp -- object validation                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_objectValidate (
  PF_OBJECTVALIDATE,            // prompt function
  "Prompt_objectValidate"_Stz,  // name of the prompt function
  "comment"_Stz,                // comment about the function
  "objectValidate"_Stz,         // the prompt command
  nullptr,                      // the second word
  bPrompt_objectValidate,       // prompt function
  true,                         // enabled

  true,                         // visible
  "objectValidate"_Stz,         // help
  ""_Stz                        // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_objectValidate(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  PoolHelper::vObjectValidate();
  
  return true;
}
/*----------------------------------------------------------------------------*/
//bool PROMPT::bPrompt_objectValidate(SePrompt* pPrompt)
//{
//  s48K sReport;
//  pPrompt->pBPA()->vObjectReport(sReport);
//
//  vTrace("------------------------------------\n"_Stz);
//  vTrace("Current Object Report\n"_Stz);
//  vTrace("------------------------------------\n"_Stz);
//
//  vTrace("%s"_Stz,sReport.szStz());
//  vTrace("====================================\n"_Stz);
//  return true;
//}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_olist.cpp -- originator list report                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_olist (
  PF_OLIST,             // prompt function
  "Prompt_olist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "olist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_olist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "olist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_olist(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  AM::pOriginatingJobs()->vListJobs();

  return true;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_optimiserLvl.cpp -- get/set optimisation level                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_optimiserLvl (
  PF_OPTIMISERLVL,           // prompt function
  "Prompt_optimiserLvl"_Stz, // name of the prompt function
  "comment"_Stz,             // comment about the function
  "optimiserLvl"_Stz,        // the prompt command
  nullptr,                   // the second word
  bPrompt_optimiserLvl,      // prompt function
  true,                      // enabled

  true,                      // visible
  "optimiserLvl"_Stz,        // help
  ""_Stz                     // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_optimiserLvl(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 iOptimiserLvl = 0;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    Token* pTkn0 = pInpTknVec->pGet(1);
    if (pTkn0->szChars()->bIsInteger()) {
      iOptimiserLvl = pTkn0->iToI8();
      AM::SYSARGS::vOptimiserLvl(iOptimiserLvl);
      return true;
    }
  }
  
  vTrace("optimiserLvl %lld\n"_Stz,AM::SYSARGS::iOptimiserLvl());
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_output.cpp -- enable/disable session output                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_output (
  PF_OUTPUT,                   // prompt function
  "Prompt_output"_Stz,         // name of the prompt function
  "session output"_Stz,        // comment about the function
  "output"_Stz,                // the prompt command
  nullptr,                     // the second word
  bPrompt_output,              // prompt function
  true,                        // enabled

  true,                        // visible
  "output"_Stz,                // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_output(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("disable"_Stz)) {
    
    vTrace("OUTPUT DISABLE\n"_Stz);
    pPrompt->vOutputEnabled(false);
  }
  else
  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("enable"_Stz)) {

    vTrace("OUTPUT ENABLE\n"_Stz);
    pPrompt->vOutputEnabled(true);
  }
  else {
    if (pPrompt->bOutputEnabled())
      vTrace("OUTPUT IS ENABLED\n"_Stz);
    else
      vTrace("OUTPUT IS DISABLED\n"_Stz);
  }
  
  vTrace("\nsuccess: 0 rows affected\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_opsys.cpp -- Print OS Information                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_opsys (
  PF_OPSYS,           // prompt function
  "Prompt_opsys"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "opsys"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_opsys,      // prompt function
  true,               // enabled

  true,               // visible
  "opsys"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_opsys(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  
  OpSys::vGetInfo();
  OpSys::vTraceInfo();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_plist.cpp -- participant list report                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_plist (
  PF_PLIST,             // prompt function
  "Prompt_plist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "plist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_plist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "plist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_plist(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pParticipatingJobs()->vListJobs();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_pmc.cpp -- perfromance monitor counters                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_pmc (
  PF_PMC,               // prompt function
  "Prompt_pmc"_Stz,     // name of the prompt function
  "comment"_Stz,        // comment about the function
  "pmc"_Stz,            // the prompt command
  nullptr,              // the second word
  bPrompt_pmc,          // prompt function
  true,                 // enabled

  true,                 // visible
  "pmc"_Stz,            // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_pmc(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 iPmc = 0; //iPmc_Info();
  
  vTrace("pmc: %lld\n"_Stz,iPmc);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_pow2.cpp -- power of 2                                              */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_pow2 (
  PF_POW2,           // prompt function
  "Prompt_pow2"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "- pow2"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_pow2,      // prompt function
  true,              // enabled

  true,              // visible
  "pow2"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_pow2(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  u8 uVal = 1;
  
  for (i8 i = 0; i < 64; i++) {
    vTrace("%2lld: %llu\n"_Stz,i,uVal);
    uVal *= 2;
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_prefixes.cpp -- list of available prompt prefixes                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_prefixes (
  PF_PREFIXES,            // prompt function
  "Prompt_prefixes"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "prefixes"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_prefixes,       // prompt function
  true,                   // enabled

  true,                   // visible
  "prefixes"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_prefixes(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("time\n"_Stz);
  vTrace("mark\n"_Stz);
  vTrace("nowait\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_print (
  PF_PRINT,          // prompt function
  "Prompt_print"_Stz,// name of the prompt function
  "comment"_Stz,     // comment about the function
  "print"_Stz,       // the prompt command
  nullptr,           // the second word
  bPrompt_print,     // prompt function
  true,              // enabled

  true,              // visible
  "print"_Stz,       // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_print(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Input* pInput = pPrompt->pInputGet();
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("print"_Stz));
  
  i8 nTokens = pInpTknVec->nTokens();
  
  if (pInpTknVec->nTokens() == 1) {
    vTrace("\n"_Stz);
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // section handling
  
  if (pInpTknVec->nTokens() >= 3) {
    
    if (pInpTknVec->pGet(1)->bSame("division"_Stz)) {
        
      Stz* szDivisionNm = pInpTknVec->pGet(2)->szChars();

      pPrompt->vDivision(szDivisionNm);
      
      vTrace("\n"_Stz);
      vTrace("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
            "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"
            "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/"
            "\n"_Stz);
      vTrace("§%s @line %lld Division %lld) "_Stz,
            pInput->szPath(),
            pInpTknVec->pGet(2)->iLineNo(),
            pPrompt->iNextDivisionNo()
            );
      
      for (i8 i_Tkn = 2; i_Tkn < nTokens; i_Tkn++)
        vTrace("%s "_Stz,pInpTknVec->pGet(i_Tkn)->szChars());

      vTrace(":\n"_Stz);
      vTrace("\n"_Stz);
      
      return true;
    }
    
    if (pInpTknVec->pGet(1)->bSame("section"_Stz)) {
        
      Stz* szSectionNm = pInpTknVec->pGet(2)->szChars();

      pPrompt->vSection(szSectionNm);
      
      vTrace("\n"_Stz);
      vTrace("-- - -- - -- - -- - -- - -- - -- - -- - "
            "-- - -- - -- - -- - -- - -- - -- - -- - "
            "-- - -- - -- - -- - -- - -- - -- - -- - "
            "--\n"_Stz);
      vTrace("§%s @line %lld Section %lld.%lld) "_Stz,
            pInput->szPath(),
            pInpTknVec->pGet(2)->iLineNo(),
            pPrompt->iDivisionNo(),
            pPrompt->iNextSectionNo()
            );
      
      for (i8 i_Tkn = 2; i_Tkn < nTokens; i_Tkn++)
        vTrace("%s "_Stz,pInpTknVec->pGet(i_Tkn)->szChars());

      vTrace(":\n"_Stz);
      vTrace("\n"_Stz);
      
      return true;
    }

  }
     
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  for (i8 i_Tkn = 1; i_Tkn < nTokens; i_Tkn++)
//    vTrace("%s "_Stz,pInpTknVec->pGet(i_Tkn)->szChars());
//  vTrace("\n"_Stz);

  s4K sOut;
  pInpTknVec->vVecToString(sOut,1,false);
//  vTrace("%s\n"_Stz,sOut.szStz());

  s4K sReplaced;
  TagSet* pTagSet = pPrompt->pUsrTagSet();
  AM::STR::vReplaceTags(sReplaced, sOut, pTagSet);
  vTrace("%s\n"_Stz,sReplaced.szStz());

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_procedure.cpp -- define a stored procedure                          */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_procedure (
  PF_PROCEDURE,           // prompt function
  "Prompt_procedure"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "procedure"_Stz,    // the prompt command
  nullptr,            // the second word
  bPrompt_procedure,  // prompt function
  true,               // enabled

  true,               // visible
  "procedure"_Stz,    // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_procedure(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  //pInpTknVec->vListTokens();
  
  pInpTknVec->vIntegrityCheck();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("procedure"_Stz));
    
  Procedure::vAddProcedure(pInpTknVec);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_prompt.cpp -- prompt settings managememnt                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_prompt (
  PF_PROMPT,              // prompt function
  "Prompt_prompt"_Stz,    // name of the prompt function
  "comment"_Stz,          // comment about the function
  "prompt"_Stz,           // the prompt command
  nullptr,                // the second word
  bPrompt_prompt,         // prompt function
  true,                   // enabled

  true,                   // visible
  "prompt"_Stz,           // help
  ""_Stz                  // description
);

#define TKNpf() { \
  if (i_Tkn >= nTokens) { \
    vTrace("Invalid tag command\n"_Stz); \
    return false; \
  } \
  pTkn = pTknVec->pGet(i_Tkn); \
  i_Tkn++; \
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_prompt(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec   = pPrompt->pInpTknVec();
  i8 nTokens = pTknVec->nTokens();

  TagSet* pSettings = pPrompt->pPmtTagSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (1 == nTokens) {
    pSettings->vDumpSet();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Token* pTkn;
  i8 i_Tkn = 0;

  s32  sName;
  s32  sTagNm;
  s256 sTagValue;

  TKNpf();
  TKNpf();

  bool bDelete = false;
  
  if (pTkn->bSame("delete"_Stz)) {
    bDelete = true;
    TKNpf();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTkn->bSame("<"_Stz)) {
    TKNpf();
    sName = pTkn->szChars();
    TKNpf();
  }
  else {
    sName = pTkn->szChars();
  }
  
  sTagNm.vSprint("<%s>"_Stz,sName.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDelete) {
    pSettings->vDelete(sTagNm);
    return true;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TKNpf();
  TKNpf();

  sTagValue = pTkn->szChars();

  pSettings->vInsert(sTagNm,sTagValue);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_promptDebug.cpp -- get / set prompt debug                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_promptDebug (
  PF_PROMPTDEBUG,           // prompt function
  "Prompt_promptDebug"_Stz, // name of the prompt function
  "comment"_Stz,            // comment about the function
  "promptDebug"_Stz,        // the prompt command
  nullptr,                  // the second word
  bPrompt_promptDebug,      // prompt function
  true,                     // enabled

  true,                     // visible
  "promptDebug"_Stz,        // help
  ""_Stz                    // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_promptDebug(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec = pPrompt->pInpTknVec();

  ASSERT(pTknVec->pGet(0)->bSame("promptDebug"_Stz));

  if (1 == pTknVec->nItems()) {
    
    vTrace("%lld\n"_Stz,iJotDebug());
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec->pGet(1)->eToken() != TN_INTEGER)
    return false;
  
  i8 iDebug = pTknVec->pGet(1)->iToI8();
  
  vJotDebug(iDebug);
  
  pPrompt->vDebugMode(iDebug);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_pwd.cpp -- print working directory                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Dir.hpp" //?

PromptFun __Prompt_pwd (
  PF_PWD,             // prompt function
  "Prompt_pwd"_Stz,   // name of the prompt function
  "comment"_Stz,      // comment about the function
  "pwd"_Stz,          // the prompt command
  nullptr,            // the second word
  bPrompt_pwd,        // prompt function
  true,               // enabled

  true,               // visible
  "pwd"_Stz,          // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_pwd(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("pwd"_Stz));
  UNUSED(pInpTknVec);

  s256 sDir;
  if (Dir::bGetCurrent(sDir))
    vTrace("%s\n"_Stz, sDir.azChar());
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_quesiton.cpp -- print prompt help                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_question (
  PF_QUESTION,           // prompt function
  "Prompt_question"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "?"_Stz,               // the prompt command
  nullptr,               // the second word
  bPrompt_question,      // prompt function
  true,                  // enabled

  true,                  // visible
  "?"_Stz,               // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_question(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  PROMPT::bPrompt_help(pPrompt);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_rassert.cpp -- execute an rassert(0)                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_rassert (
  PF_RASSERT,          // prompt function
  "Prompt_rassert"_Stz,// name of the prompt function
  "comment"_Stz,       // comment about the function
  "rassert"_Stz,       // the prompt command
  nullptr,             // the second word
  bPrompt_print,       // prompt function
  true,                // enabled

  true,                // visible
  "rassert"_Stz,       // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_rassert(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  RASSERT(0);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ready.cpp -- ready queue report                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_ready (
  PF_READY,               // prompt function
  "Prompt_ready"_Stz,     // name of the prompt function
  "comment"_Stz,          // comment about the function
  "ready"_Stz,            // the prompt command
  nullptr,                // the second word
  bPrompt_ready,          // prompt function
  true,                   // enabled

  true,                   // visible
  "ready"_Stz,            // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ready(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pSchedule()->vDumpReadyQueue();
  AM::pSchedule()->vDumpReadyQueueLP();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_reap.cpp -- explicilty reap the retired list                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_reap (
  PF_REAP,                     // prompt function
  "Prompt_reap"_Stz,           // name of the prompt function
  "reap retired job list"_Stz, // comment about the function
  "reap"_Stz,                  // the prompt command
  nullptr,                     // the second word
  bPrompt_reap,                // prompt function
  true,                        // enabled

  true,                        // visible
  "reap"_Stz,                  // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_reap(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

//  if (pPrompt->pInpTknVec()->nItems() == 1 &&
//      pPrompt->pInpTknVec()->pGet(1)->bSame("tableScan"_Stz))
    
  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("disable"_Stz)) {
    
    vTrace("REAP DISABLE\n"_Stz);
    AM::SYSTEM::vReapEnabled(false);
  }
  else
  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("enable"_Stz)) {

    vTrace("REAP ENABLE\n"_Stz);
    AM::SYSTEM::vReapEnabled(true);

  }
  else
  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("state"_Stz)) {

    if (AM::SYSTEM::bReapEnabled())
      vTrace("REAP IS ENABLED\n"_Stz);
    else
      vTrace("REAP IS DISABLED\n"_Stz);
  }
  else
  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("force"_Stz)) {

    vTrace("REAP FORCE\n"_Stz);
    
    Api::vReapAllJobs(true);
  }
  else
  if (pPrompt->pInpTknVec()->nItems() == 1) {
    
    vTrace("REAP LIST\n"_Stz);
    
    Api::vReapAllJobs();
  }
  
  vTrace("\nsuccess: 0 rows affected\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_resources.cpp -- print resource report                              */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_resources (
  PF_RESOURCES,          // prompt function
  "Prompt_resources"_Stz,// name of the prompt function
  "comment"_Stz,         // comment about the function
  "resources"_Stz,       // the prompt command
  nullptr,               // the second word
  bPrompt_resources,     // prompt function
  true,                  // enabled

  true,                  // visible
  "resources"_Stz,       // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_resources(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  if (AM::bAttached()) AM::pResources()->vReport();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_retired.cpp -- print retired list                                   */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_retired (
  PF_RETIRED,           // prompt function
  "Prompt_retired"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "retired"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_retired,      // prompt function
  true,                 // enabled

  true,                 // visible
  "retired"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_retired(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pRetiredJobs()->vListJobs();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_rmfifo.cpp -- remove fifo                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Fifo.hpp" //?

PromptFun __Prompt_rmfifo (
  PF_RMFIFO,             // prompt function
  "Prompt_rmfifo"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "rmfifo"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_rmfifo,        // prompt function
  true,                  // enabled

  true,                  // visible
  "rmfifo"_Stz,          // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_rmfifo(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s256 sFifoPath;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("rmfifo"_Stz));
  
  sFifoPath = pInpTknVec->pGet(1)->szChars();
  
  if (! Fifo::bRemove(sFifoPath)) {
    vTrace("Failed to remove fifo: %s\n"_Stz,sFifoPath.azChar());
    return false;
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_rnd.cpp -- random number helper                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Mersenne.hpp"

PromptFun __Prompt_rnd (
  PF_RND,           // prompt function
  "Prompt_rnd"_Stz, // name of the prompt function
  "comment"_Stz,    // comment about the function
  "rnd"_Stz,        // the prompt command
  nullptr,          // the second word
  bPrompt_rnd,      // prompt function
  true,             // enabled

  true,             // visible
  "rnd"_Stz,        // help
  ""_Stz            // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_rnd(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  Mersenne::init_genrand64(0);
  
//  for (i8 i = 0; i < 1024; i++) {
//    vTrace("/* %4lld */ %20lluull, "_Stz,i,Random::uInteger());
//    if (0 == (i+1) % 1)
//      vTrace("\n"_Stz);
//  }

  i8 i_Next = 0;
  for (i8 i = 0; i < 32; i++) {
    vTrace("/* %4lld */ %20lluull, "_Stz,i,RndLib::uRepeatable(i_Next));
    if (0 == (i+1) % 1)
      vTrace("\n"_Stz);
  }

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_scan.cpp -- scan to pipeline                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_scan (
  PF_SCAN,           // prompt function
  "Prompt_scan"_Stz,    // name of the prompt function
  "comment"_Stz,        // comment about the function
  "scan"_Stz,           // the prompt command
  nullptr,              // the second word
  bPrompt_scan,         // prompt function
  true,                 // enabled

  false,                // visible
  "scan"_Stz,           // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_scan(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  //vTrace("bPrompt_scan\n"_Stz);
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  s48K sPipelineStmt;
  
  if (AM::PROMPT::bToPipelineStmt(sPipelineStmt,pInpTknVec)) {
    
    pPrompt->pSession()->m_sToDo_ssn = sPipelineStmt;
    
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_select.cpp -- select & explain statements                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_select (
  PF_SCAN,           // prompt function
  "Prompt_select"_Stz,  // name of the prompt function
  "comment"_Stz,        // comment about the function
  "select"_Stz,         // the prompt command
  nullptr,              // the second word
  bPrompt_select,       // prompt function
  true,                 // enabled

  false,                // visible
  "select"_Stz,         // help
  ""_Stz                // description
);

PromptFun __Prompt_explain (
    PF_SCAN,           // prompt function
    "Prompt_explain"_Stz,  // name of the prompt function
    "comment"_Stz,        // comment about the function
    "explain"_Stz,        // the prompt command
    nullptr,              // the second word
    bPrompt_select,       // prompt function
    true,                 // enabled

    false,                // visible
    "select"_Stz,         // help
    ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_select(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  //vTrace("bPrompt_select\n"_Stz);
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
//  s16KB sSelectStmt;
//  pInpTknVec->vVecToString(sSelectStmt,0,true);
//
//  s16KB sSqlCompiler;
//  sSqlCompiler.vSprint("sqlCompiler \"%s\""_Stz,sSelectStmt.szStz());
  //vTrace(":: %s\n"_Stz,sSqlCompiler.szStz());
  
  Stz* szRaw = pPrompt->szRawInput();

  s16KB sSqlCompiler;
  sSqlCompiler.vSprint("sqlCompiler \"%s\""_Stz,szRaw);

  pPrompt->pSession()->m_sToDo_ssn = sSqlCompiler;
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_selfies.cpp -- Ascii Helper                                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

extern SubSysCatalog gxSubSysCatalog;
extern ToolCatalog   gxToolCatalog;
extern GizmoCatalog  gxGizmoCatalog;

PromptFun __Prompt_selfies (
  PF_SELFIES,           // prompt function
  "Prompt_selfies"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "__selfies"_Stz,      // the prompt command
  nullptr,              // the second word
  bPrompt_selfies,      // prompt function
  true,                 // enabled

  false,                // visible
  "ascii"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_selfies(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  Stz* szCmd = pInpTknVec->pGet(0)->szChars();
  
  if (gxSubSysCatalog.bCallFn(szCmd,pInpTknVec))
    return true;
  
  if (gxToolCatalog.bCallFn(szCmd,pInpTknVec))
    return true;
  
  if (gxGizmoCatalog.bCallFn(szCmd,pInpTknVec))
    return true;
  
  vTrace("error: %s : %s\n"_Stz,aEC[EC_INVALID_COMMAND],szCmd);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_setDbg.cpp -- set debug                                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_setDbg (
  PF_SETDBG,           // prompt function
  "Prompt_setDbg"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "setDbg"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_setDbg,      // prompt function
  true,                // enabled

  true,               // visible
  "setDbg"_Stz,       // help
  ""_Stz              // description
);

//union {
//
//  struct {
//    i8 m_i1;
//    i8 m_i2;
//  };
//
//  i8 m_iaArray[2];
//
//} u;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_setDbg(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("setDbg"_Stz));
  
  //vTrace(" sizof(u) %lld\n"_Stz,sizeof(u));
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_shell.cpp -- shell command                                          */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_shell (
  PF_SHELL,           // prompt function
  "Prompt_shell"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "shell"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_shell,      // prompt function
  true,               // enabled

  true,               // visible
  "shell"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_shell(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  s4K sCommand;
  
  i8 nTokens = pTknVec->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 1; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pTknVec->pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
      
      case TN_BOOLEAN:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_DECIMAL:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_FLOAT:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_INTEGER:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_NAME:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_OTHER:
        sCommand.vConcat(pToken->szChars());
        break;

      case TN_STRING:
        //sCommand.vConcat("\""_Stz);
        sCommand.vConcat(pToken->szChars());
        //sCommand.vConcat("\""_Stz);
        break;
        
      default:
        NEVER_REACH;
    }
    
    sCommand.vConcat(" "_Stz);
  }
  
  // concat a terminating semicolon (needed??)
  sCommand.vConcat(";"_Stz);

  vTrace("Shell:: %s\n"_Stz,sCommand.szStz());
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(sCommand.szStz(),oSysCall);
  
  return iExitStatus ? true : false;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_shortcut.cpp -- shortcut                                            */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_shortcut (
  PF_SHORTCUT,        // prompt function
  "Prompt_shortcut"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "shortcut"_Stz,     // the prompt command
  nullptr,            // the second word
  bPrompt_shortcut,   // prompt function
  true,               // enabled

  true,               // visible
  "shortcut"_Stz,     // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_shortcut(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec = pPrompt->pInpTknVec();

  ASSERT(pTknVec->pGet(0)->bSame("shortcut"_Stz));

  if (1 == pTknVec->nItems()) {
    ShortCutVec* pShortCutVec = pPrompt->pShortCutVec();
    i8 nShortCuts = pShortCutVec->nItems();
    for (i8 i_ShortCut = 0; i_ShortCut < nShortCuts; i_ShortCut++) {
      ShortCut* pShortCut = pShortCutVec->pGet(i_ShortCut);
      vTrace("  %s : %s \n"_Stz,
             pShortCut->m_sName.azChar(),
             pShortCut->m_sCmd.azChar());
    }
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sNm;
  s4K sCmd;
  
  sNm = pTknVec->pGet(1)->szChars();
  
  pTknVec->vStripSemicolon();
  
  pTknVec->vVecToString(sCmd.szStz(),2,false);
  
  ShortCut* pShortCut = new ShortCut(sNm,sCmd);
  
  pPrompt->pShortCutVec()->vAppend(pShortCut);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_show.cpp -- show                                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "SP_Grid.hpp"
#include "SP_Node.hpp"

PromptFun __Prompt_show (
  PF_SHOW,           // prompt function
  "Prompt_show"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "show"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_show,      // prompt function
  true,              // enabled

  true,              // visible
  "show"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_show(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTkVec = pPrompt->pInpTknVec();

  ASSERT(pTkVec->pGet(0)->bSame("show"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (pTkVec->pGet(1)->bSame("grid"_Stz)) {
    
    SP_Grid spGrid;
    Grid* pGrid = AM::pGrid();
    vTrace("  %s\n"_Stz,spGrid.sz(pGrid));
    
    vTrace("\n"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (pTkVec->pGet(1)->bSame("nodes"_Stz)) {
    
    SP_Node spNode;
    for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext())
      vTrace("  %s\n"_Stz,spNode.sz(oNode_I));
    
    vTrace("\n"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_sizes.cpp -- sizes report                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_sizes (
  PF_SIZES,             // prompt function
  "Prompt_sizes"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "sizes"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_sizes,        // prompt function
  true,                 // enabled

  true,                 // visible
  "sizes"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_sizes(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  vTrace("  sizeof(Header) = %lu; \n"_Stz, sizeof(Header));
  vTrace("  sizeof(Object) = %lu; \n"_Stz, sizeof(Object));
  vTrace("  sizeof(Footer) = %lu; \n"_Stz, sizeof(Footer));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(PoolBuffer) = %lu; \n"_Stz, sizeof(PoolBuffer));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(Task)   = %lu; \n"_Stz, sizeof(Task));
  vTrace("  sizeof(TypedStack)  = %lu; \n"_Stz, sizeof(TypedStack));
  vTrace("  Task+Stacks    = %lu; \n"_Stz, sizeof(TypedStack) + 3 * sizeof(TypedStack));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(s32)    = %lu; \n"_Stz, sizeof(s32));
  vTrace("  sizeof(s256)   = %lu; \n"_Stz, sizeof(s256));
  vTrace("  sizeof(s4K)    = %lu; \n"_Stz, sizeof(s4K));
  vTrace("  sizeof(s16KB)  = %lu; \n"_Stz, sizeof(s16KB));
  vTrace("  sizeof(s24KB)  = %lu; \n"_Stz, sizeof(s24KB));
  vTrace("  sizeof(s48K)   = %lu; \n"_Stz, sizeof(s48K));
  vTrace("  sizeof(s1M)    = %lu; \n"_Stz, sizeof(s1M));
  vTrace("\n"_Stz);
  
  vTrace("  sizeof(JobFilePath)   = %lu; \n"_Stz, sizeof(JobFilePath));
  vTrace("\n"_Stz);

  vTrace("  sizeof(FreeChunk)    = %lu; \n"_Stz, sizeof(FreeChunk));
  vTrace("  sizeof(Sessions) = %lu; \n"_Stz, sizeof(Sessions));
  vTrace("\n"_Stz);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_sleep.cpp -- sleep n seconds                                        */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_sleep (
  PF_SLEEP,           // prompt function
  "Prompt_sleep"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "sleep"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_sleep,      // prompt function
  true,               // enabled

  true,               // visible
  "sleep"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_sleep(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("sleep"_Stz));
  
  i8 nSeconds = pInpTknVec->pGet(1)->iToI8();
  
  AM::vNapMsec(nSeconds * C_1K,CN_SEPROMPT);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_sleepers.cpp -- sleepers statistics                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "PsxThd.hpp"  //?

PromptFun __Prompt_sleepers (
  PF_SLEEPERS,           // prompt function
  "Prompt_sleepers"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "sleepers"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_sleepers,      // prompt function
  true,                  // enabled

  true,                  // visible
  "sleepers"_Stz,        // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_sleepers(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  bool bReset = false;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  if (3 == iNumTokens)
    bReset = pInpTknVec->pGet(2)->bSame("reset"_Stz);

  PsxThd::vSleepers();
  
  if (bReset)
    MemLib::vReset();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_sleeping.cpp -- sleeping queue report                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_sleeping (
  PF_SLEEPING,            // prompt function
  "Prompt_sleeping"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "sleeping"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_sleeping,       // prompt function
  true,                   // enabled

  true,                   // visible
  "sleeping"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_sleeping(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pSchedule()->vDumpSleepingQueue();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_snapshotDelta.cpp -- snapshot delta                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_snapshotDelta (
  PF_SNAPSHOTDELTA,            // prompt function
  "Prompt_snapshotDelta"_Stz,  // name of the prompt function
  "comment"_Stz,               // comment about the function
  "snapshotDelta"_Stz,         // the prompt command
  nullptr,                     // the second word
  bPrompt_snapshotDelta,       // prompt function
  true,                        // enabled

  true,                        // visible
  "snapshotDelta"_Stz,         // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotDelta(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  ObjectSnapshot* pSnapshotObjects_then = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects_then)
    return true;
  
  ObjectSnapshot* pSnapshotObjects_curr = new ObjectSnapshot;
  pSnapshotObjects_curr->vTakeSnapshot();
  
  pSnapshotObjects_then->vDeltaList(pSnapshotObjects_curr);
  
  delete pSnapshotObjects_curr;
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_snapshotInspect.cpp -- snapshot inspect                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_snapshotInspect (
  PF_SNAPSHOTINSPECT,            // prompt function
  "Prompt_snapshotInspect"_Stz,  // name of the prompt function
  "comment"_Stz,                 // comment about the function
  "snapshotInspect"_Stz,         // the prompt command
  nullptr,                       // the second word
  bPrompt_snapshotInspect,       // prompt function
  true,                          // enabled

  true,                          // visible
  "snapshotInspect"_Stz,         // help
  ""_Stz                         // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotInspect(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  ObjectSnapshot* pSnapshotObjects_then = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects_then)
    return true;
    
  //pSnapshotObjects_then->vDeltaList(pSnapshotObjects_curr);
   
  i8 iAfter = pSnapshotObjects_then->m_iWhenSnapshot;
  
  PoolHelper::vInspectObjects(iAfter);
  
  vTrace("\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_snapshotList.cpp -- objects list report                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_snapshotList (
  PF_SNAPSHOTLIST,            // prompt function
  "Prompt_snapshotList"_Stz,  // name of the prompt function
  "comment"_Stz,              // comment about the function
  "snapshotList"_Stz,         // the prompt command
  nullptr,                    // the second word
  bPrompt_snapshotList,       // prompt function
  true,                       // enabled

  true,                       // visible
  "snapshotList"_Stz,         // help
  ""_Stz                      // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotList(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  ObjectSnapshot* pSnapshotObjects = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects)
    return true;
  
  pSnapshotObjects->vPrintCounts();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_snapshotNewer.cpp -- snapshot newer                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_snapshotNewer (
  PF_SNAPSHOTNEWER,            // prompt function
  "Prompt_snapshotNewer"_Stz,  // name of the prompt function
  "comment"_Stz,               // comment about the function
  "snapshotNewer"_Stz,         // the prompt command
  nullptr,                     // the second word
  bPrompt_snapshotNewer,       // prompt function
  true,                        // enabled

  true,                        // visible
  "snapshotNewer"_Stz,         // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotNewer(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  ObjectSnapshot* pSnapshotObjects_then = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects_then)
    return true;
    
  //pSnapshotObjects_then->vDeltaList(pSnapshotObjects_curr);
   
  i8 iAfter = pSnapshotObjects_then->m_iWhenSnapshot;
  
  PoolHelper::vObjectDump(iAfter);
  
  vTrace("\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_snapshotObjects.cpp -- snapshot objects                             */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_snapshotObjects (
  PF_SNAPSHOTOBJECTS,            // prompt function
  "Prompt_snapshotObjects"_Stz,  // name of the prompt function
  "comment"_Stz,                 // comment about the function
  "snapshotObjects"_Stz,         // the prompt command
  nullptr,                       // the second word
  bPrompt_snapshotObjects,       // prompt function
  true,                          // enabled

  true,                          // visible
  "snapshotObjects"_Stz,         // help
  ""_Stz                         // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_snapshotObjects(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  ObjectSnapshot* pSnapshotObjects = pPrompt->pSnapshotObjects();
  if (! pSnapshotObjects) {
    pSnapshotObjects = new ObjectSnapshot;
    pPrompt->vSnapshotObjects(pSnapshotObjects);
  }
  
  pSnapshotObjects->vTakeSnapshot();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_so.cpp -- print so information                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "SP_Gid.hpp"

PromptFun __Prompt_so (
  PF_SO,           // prompt function
  "Prompt_so"_Stz, // name of the prompt function
  "comment"_Stz,   // comment about the function
  "so"_Stz,        // the prompt command
  nullptr,         // the second word
  bPrompt_so,      // prompt function
  true,            // enabled

  true,                  // visible
  "so"_Stz,              // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_so(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  SP_Gid spId;
  
  //vTrace(" Request Id : %s\n"_Stz,spId.sz(IK_PROMPT,pPrompt->iRequestId()));
         
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_socket.cpp -- socket statistics                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Socket.hpp" //?

PromptFun __Prompt_socket (
  PF_SOCKET,           // prompt function
  "Prompt_socket"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "socket"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_socket,      // prompt function
  true,                // enabled

  true,                // visible
  "socket"_Stz,        // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_socket(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  Socket::vStatistics();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ssn.hpp -- Print Session Information                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_ssn (
  PF_SSN,                 // prompt function
  "Prompt_ssn"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "ssn"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_ssn,            // prompt function
  true,                   // enabled

  true,                   // visible
  "ssn"_Stz,              // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ssn(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Token* pArg0 = nullptr;
  Token* pArg1 = nullptr;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pInpTknVec->nItems();

  if (nTokens != 3)
    return false;

  pArg0 = pInpTknVec->pGet(1);
  pArg1 = pInpTknVec->pGet(2);

  s32 sNumber;
  sNumber.vConcat(pArg1->szChars());
  vTrace("Session: %s\n"_Stz,sNumber.szStz());

  i8 iNumber = sNumber.iToI8();
  
  Session* pSession = _pSession(iNumber);
  
  if (! pSession) {
    vTrace("Not a session\n"_Stz);
    return false;
  }
    
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_ssnState.hpp -- Print Session State Information                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_ssnState (
  PF_SSNSTATE,            // prompt function
  "Prompt_ssnState"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "ssnState"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_ssnState,       // prompt function
  true,                   // enabled

  true,                   // visible
  "ssnState"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ssnState(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Token* pArg = nullptr;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pInpTknVec->nItems();

  if (nTokens == 2)
    pArg = pInpTknVec->pGet(1);
  
  AM::pSessions()->vDump();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_start -- Prompt Start                                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_start (
  PF_START,               // prompt function
  "Prompt_start"_Stz,     // name of the prompt function
  "comment"_Stz,          // comment about the function
  "start"_Stz,            // the prompt command
  nullptr,                // the second word
  bPrompt_start,          // prompt function
  true,                   // enabled

  true,                   // visible
  "start"_Stz,            // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_start(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pTknVec->nItems();

  if (nTokens < 2) {
    return bPrompt_startStatus(pPrompt);
  }
  if (nTokens < 3) {
    Stz* szWhat = pTknVec->pGet(1)->szChars();

    if (szWhat->bSame("now"_Stz))
      return bPrompt_startNow(pPrompt);
    else
    if (szWhat->bSame("reset"_Stz))
      return bPrompt_startReset(pPrompt);
    else
      return bPrompt_startStatus(pPrompt);
  }

  i8 nStart = pTknVec->pGet(1)->szChars()->iToI8();
  Stz* szCommand = pTknVec->pGet(2)->szChars();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (!File::bExists(szCommand)) {
    vTrace("Command file %s does not exit\n"_Stz,szCommand);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Starter* pStarter = AM::STARTER::pStarter();
  
  vTrace("BEFORE grab write\n"_Stz);
  if (0 == pStarter->nGetStarted()) {
    pStarter->vGrabWriteLock();
  }
  vTrace("AFTER grab write\n"_Stz);
  
  sSTMT sCommand;

  sCommand.vSprint("../bin/rapidsse"
                   " --name %s"
                   " --prompt"
                   " --metadata"
                   " --command %s"
                   " &"_Stz

                   ,MY::szNodeNm()
                   ,szCommand
                   );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("start: %s\n"_Stz,sCommand.szStz());
  
  SysCall oSysCall;
  
  while (nStart--) {
    i8 iExitStatus = Shell::iExecute(sCommand.szStz(),oSysCall);
    if (! iExitStatus)
      pStarter->vIncStarted();
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_startComplete -- started has has completed                          */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_startCompleted (
  PF_STARTCOMPLETED,       // prompt function
  "Prompt_startCompleted"_Stz,// name of the prompt function
  "comment"_Stz,          // comment about the function
  "startCompleted"_Stz,   // the prompt command
  nullptr,                // the second word
  bPrompt_startCompleted, // prompt function
  true,                   // enabled

  true,                   // visible
  "startCompleted"_Stz,    // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_startCompleted(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
    
  Starter* pStarter = AM::STARTER::pStarter();

  pStarter->vDecReady();

  pStarter->vDropReadLock();
      
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_startLine -- Prompt Starter start line                              */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_startNow (
  PF_STARTLINE,           // prompt function
  "Prompt_startNow"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "startNow"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_startNow,       // prompt function
  true,                   // enabled

  true,                   // visible
  "startNow"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_startNow(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
    
  Starter* pStarter = AM::STARTER::pStarter();

  vTrace("BEFORE %lld ready ; %lld started \n"_Stz
         ,pStarter->nGetReady()
         ,pStarter->nGetStarted());

  while (pStarter->nGetReady() != pStarter->nGetStarted())
    AM::vNapMsec(10,CN_SEPROMPT);

  vTrace("AFTER %lld ready ; %lld started \n"_Stz
         ,pStarter->nGetReady()
         ,pStarter->nGetStarted());

  Timer* pTimer = new Timer;
  pTimer->vStart();
  
  pStarter->vDropWriteLock();
  
  
  vTrace("%lld passed start line\n"_Stz, pStarter->nGetStarted());
  
  
  while (pStarter->nGetReady())
    AM::vNapMsec(10,CN_SEPROMPT);
  
  pTimer->vUpdate();

  SP_Timer spTimer;
  vTrace("Elaped Time: %s (%lld sessions)\n"_Stz
         ,spTimer.sz(pTimer)
         ,pStarter->nGetStarted()
         );

  pStarter->vResetStarter();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_startReady -- read to start                                         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_startReady (
  PF_STARTREADY,               // prompt function
  "Prompt_startReady"_Stz,// name of the prompt function
  "comment"_Stz,          // comment about the function
  "startReady"_Stz,       // the prompt command
  nullptr,                // the second word
  bPrompt_startReady,     // prompt function
  true,                   // enabled

  true,                   // visible
  "startReady"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_startReady(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
    
  Starter* pStarter = AM::STARTER::pStarter();

  pStarter->vIncReady();

  pStarter->vGrabReadLock();
      
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_startReset -- reset start                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_startReset (
  PF_STARTRESET,          // prompt function
  "Prompt_startReset"_Stz,// name of the prompt function
  "comment"_Stz,          // comment about the function
  "startReset"_Stz,       // the prompt command
  nullptr,                // the second word
  bPrompt_startReset,     // prompt function
  true,                   // enabled

  true,                   // visible
  "startReset"_Stz,       // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_startReset(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
    
  Starter* pStarter = AM::STARTER::pStarter();
      
  pStarter->vResetStarter();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_startStatus -- status of starter                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_startStatus (
  PF_STARTSTATUS,          // prompt function
  "Prompt_startStatus"_Stz,// name of the prompt function
  "comment"_Stz,          // comment about the function
  "startStatus"_Stz,      // the prompt command
  nullptr,                // the second word
  bPrompt_startStatus,    // prompt function
  true,                   // enabled

  true,                   // visible
  "startStatus"_Stz,      // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_startStatus(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
    
  Starter* pStarter = AM::STARTER::pStarter();

  pStarter->vStatus();

      
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_stepTiming.cpp -- step timing                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "StepAccum.hpp"
#include "Steps.hpp"

PromptFun __Prompt_st (
  PF_ST,                // prompt function
  "Prompt_st"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "stepTiming"_Stz,     // the prompt command
  nullptr,              // the second word
  bPrompt_stepTiming,   // prompt function
  true,                 // enabled

  true,                 // visible
  "stepTiming"_Stz,     // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_stepTiming(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 nTokens = pInpTknVec->nTokens();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("stepTiming"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (1 == nTokens) {
    if (AM::pSteps())
      vTrace("Step timing started\n"_Stz);
    else
      vTrace("Step timing not started\n"_Stz);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // start step timing
  if ( pInpTknVec->pGet(1)->bSame("start"_Stz) ) {
    if (AM::pSteps()) {
      vTrace("Step timing already started\n"_Stz);
      return true;
    }
    AM::vSteps(new Steps);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset step timing
  if ( pInpTknVec->pGet(1)->bSame("reset"_Stz) ) {
    if (! AM::pSteps()) {
      vTrace("Step timing not started\n"_Stz);
      return true;
    }
    AM::pSteps()->vReset();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // show a step timing
  if ( pInpTknVec->pGet(1)->bSame("show"_Stz) && nTokens == 3) {
    
    s32 sStepNm = pInpTknVec->pGet(2)->szChars();

    if (! AM::pSteps()) {
      vTrace("Step timing not started\n"_Stz);
      return true;
    }
    AM::pSteps()->vShow(sStepNm);
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write step timings to given file
  if ( pInpTknVec->pGet(1)->bSame("write"_Stz) && nTokens == 3) {
    
    s256 sPath = pInpTknVec->pGet(2)->szChars();

    if (! AM::pSteps()) {
      vTrace("Step timing not started\n"_Stz);
      return true;
    }
    AM::pSteps()->vWrite(sPath);
    return true;
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_store.cpp -- store command                                          */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_store (
  PF_STORE,           // prompt function
  "Prompt_store"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "store"_Stz,        // the prompt command
  nullptr,            // the second word
  bPrompt_store,      // prompt function
  true,               // enabled

  true,               // visible
  "store"_Stz,        // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_store(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec = pPrompt->pInpTknVec();
  
  s4K sCommand;
  
  i8 nTokens = pTknVec->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 1; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pTknVec->pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
      
      case TN_BOOLEAN:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_DECIMAL:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_FLOAT:
        sCommand.vConcat(pToken->szChars());
        break;
        
       case TN_INTEGER:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_NAME:
        sCommand.vConcat(pToken->szChars());
        break;
        
      case TN_OTHER:
        sCommand.vConcat(pToken->szChars());
        break;

      case TN_STRING:
        //sCommand.vConcat("\""_Stz);
        sCommand.vConcat(pToken->szChars());
        //sCommand.vConcat("\""_Stz);
        break;
        
      default:
        NEVER_REACH;
    }
    
    sCommand.vConcat(" "_Stz);
  }
  
  // concat a terminating semicolon (needed??)
  sCommand.vConcat(";"_Stz);

  vTrace("store:: %s\n"_Stz,sCommand.szStz());
    
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_tag.cpp -- session tag managememnt                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_tag (
  PF_TAG,                 // prompt function
  "Prompt_tag"_Stz,       // name of the prompt function
  "comment"_Stz,          // comment about the function
  "tag"_Stz,              // the prompt command
  nullptr,                // the second word
  bPrompt_tag,            // prompt function
  true,                   // enabled

  true,                   // visible
  "tag"_Stz,              // help
  ""_Stz                  // description
);

#define TKNpf() { \
  if (i_Tkn >= nTokens) { \
    vTrace("Invalid tag command\n"_Stz); \
    return false; \
  } \
  pTkn = pTknVec->pGet(i_Tkn); \
  i_Tkn++; \
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_tag(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pTknVec   = pPrompt->pInpTknVec();
  i8 nTokens = pTknVec->nTokens();
  
  TagSet* pPmtTagSet = pPrompt->pPmtTagSet();
  TagSet* pUsrTagSet = pPrompt->pUsrTagSet();

  Token* pTkn;
  i8 i_Tkn = 0;

  s32  sName;
  s32  sTagNm;
  s256 sTagValue;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // tag <promptTag> = "Boo!" ;
  
  TKNpf(); // pTkn(0)
  TKNpf(); // pTkn(1)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bDelete = false;
  bool bDefault = false;

  if (pTkn->bSame("delete"_Stz)) {
    bDelete = true;
    TKNpf();
  }
  if (pTkn->bSame("default"_Stz)) {
    bDefault = true;
    TKNpf();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bPmtTagSet = false;

  if (pTkn->bSame("prompt"_Stz)) {
    bPmtTagSet = true;
    TKNpf();
  }
  else
  if (pTkn->bSame("user"_Stz)) {
    bPmtTagSet = false;
    TKNpf();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTkn->bSame("<"_Stz)) {
    TKNpf();
    sName = pTkn->szChars();
    TKNpf();
  }
  else {
    sName = pTkn->szChars();
  }
  
  sTagNm.vSprint("<%s>"_Stz,sName.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDelete) {
    
    if (bPmtTagSet)
      pPmtTagSet->vDelete(sTagNm);
    else
      pUsrTagSet->vDelete(sTagNm);

    //vTrace("\nsuccess: 0 rows affected\n"_Stz);
    return true;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TKNpf();
  TKNpf();

  sTagValue = pTkn->szChars();

  if (bDefault) {

    if (bPmtTagSet) {
      if (! pPmtTagSet->bHasName(sTagNm))
        pPmtTagSet->vReplace(sTagNm,sTagValue);
    }
    else {
      if (! pUsrTagSet->bHasName(sTagNm))
        pUsrTagSet->vReplace(sTagNm,sTagValue);
    }

  }
  else {

    if (bPmtTagSet) {
      pPmtTagSet->vReplace(sTagNm,sTagValue);
    }
    else {
      pUsrTagSet->vReplace(sTagNm,sTagValue);
    }

  }
  
  //vTrace("\nsuccess: 0 rows affected\n"_Stz);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_tags.cpp -- session tags report                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_tags (
  PF_TAGS,                // prompt function
  "Prompt_tags"_Stz,      // name of the prompt function
  "comment"_Stz,          // comment about the function
  "tags"_Stz,             // the prompt command
  nullptr,                // the second word
  bPrompt_tags,           // prompt function
  true,                   // enabled

  true,                   // visible
  "tags"_Stz,             // help
  ""_Stz                  // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_tags(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TagSet* pPmtTagSet = pPrompt->pPmtTagSet();
  TagSet* pUsrTagSet = pPrompt->pUsrTagSet();

  pPmtTagSet->vDumpSet(true);
  pUsrTagSet->vDumpSet(true);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_takeMem.cpp -- take heap memory                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_takeMem (
  PF_TAKEMEM,           // prompt function
  "Prompt_takeMem"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "takeMem"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_takeMem,      // prompt function
  true,                 // enabled

  true,                 // visible
  "takeMem"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_takeMem(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 nSpare = 0;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 iNumTokens = pInpTknVec->nItems();
  if (2 == iNumTokens) {
    if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
      return false;
    
    // get the no of indexEntries to set
    nSpare = pInpTknVec->pGet(1)->iToI8();
  }
    
  gpBufferPool->vTakeBuffers(nSpare);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* bPrompt_taskQueues.cpp -- taskQueues report                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_taskQueues (
  PF_TASKQUEUES,            // prompt function
  "bPrompt_taskQueues"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "taskQueues"_Stz,     // the prompt command
  nullptr,              // the second word
  bPrompt_taskQueues,   // prompt function
  true,                 // enabled

  true,                 // visible
  "taskQueues"_Stz,     // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_taskQueues(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->vDumpExecutingSet();
  pSchedule->vDumpReadyQueue();
  pSchedule->vDumpReadyQueueLP();
  pSchedule->vDumpSleepingQueue();
  pSchedule->vDumpWaitingSet();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_tempName.cpp -- show tempName                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_tempName (
  PF_TEMPNAME,           // prompt function
  "Prompt_tempName"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "tempName"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_tempName,      // prompt function
  true,                  // enabled

  true,                  // visible
  "tempName"_Stz,        // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_tempName(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  s32 sName;
  
  for (i8 n = 0; n < 1000; n++) {
    AM::vNextTempName(sName);
    vTrace("%s\n"_Stz,sName.szStz());
  }
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_threads.cpp -- threads report                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_threads (
  PF_THREADS,           // prompt function
  "Prompt_threads"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "threads"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_threads,      // prompt function
  true,                 // enabled

  true,                 // visible
  "threads"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_threads(SePrompt* pPrompt)
{
  RASSERT(pPrompt);
  
  TokenVec* pTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pTknVec->nItems();

  if (nTokens < 2) {
    Schedule::vThreadsReport();
    return true;
  }
  
  if (nTokens == 3) {
    Stz* szWhat = pTknVec->pGet(1)->szChars();

    if (szWhat->bSame("add"_Stz)) {
      i8 nStart = pTknVec->pGet(2)->szChars()->iToI8();
      vTrace("Adding %lld threads\n"_Stz,nStart);
      
      for (i8 i_NewThread = 0; i_NewThread < nStart; i_NewThread++) {
        Thread* pThread = AM::THREAD::pNewThread();
        pThread->vSpawnPsxThd_general();
      }
      
      AM::SYSARGS::vThreadsInc(nStart);
      return true;
    }
    
  }
  
  vTrace("Unknown threads command\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_timer.cpp -- timer                                                  */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_timer (
  PF_TIMER,                // prompt function
  "Prompt_timer"_Stz,      // name of the prompt function
  "comment"_Stz,           // comment about the function
  "timer"_Stz,             // the prompt command
  nullptr,                 // the second word
  bPrompt_timer,           // prompt function
  true,                    // enabled

  true,                    // visible
  "timerStart"_Stz,        // help
  ""_Stz                   // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_timer(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("start"_Stz)) {
    
    pPrompt->pTimer()->vStart();
    vTrace("TIMER STARTED\n"_Stz);
    return true;
  }

  pPrompt->pTimer()->vUpdate();

  vTrace("timer %lldms\n"_Stz,pPrompt->pTimer()->iMilliseconds());

  return true;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_tools.cpp -- System Tools                                           */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_tools (
  PF_TOOLS,              // prompt function
  "Prompt_tools"_Stz,    // name of the prompt function
  "comment"_Stz,         // comment about the function
  "- tools"_Stz,           // the prompt command
  nullptr,               // the second word
  bPrompt_tools,         // prompt function
  true,                  // enabled

  false,                 // visible
  "tools"_Stz,           // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_tools(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 iNumTokens = pInpTknVec->nItems();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_tpch.cpp -- show tpch                                               */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "TpchRatios.h"

PromptFun __Prompt_tpch (
  PF_TPCH,           // prompt function
  "Prompt_tpch"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "tpch"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_tpch,      // prompt function
  true,              // enabled

  true,              // visible
  "tpch"_Stz,        // help
  ""_Stz             // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_tpch(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  printf("C_1_BYTE  %llu\n",C_1_BYTE);
  printf("C_2_BYTES %llu\n",C_2_BYTES);
  printf("C_3_BYTES %llu\n",C_3_BYTES);
  printf("C_4_BYTES %llu\n",C_4_BYTES);
  printf("C_5_BYTES %llu\n",C_5_BYTES);
  printf("C_6_BYTES %llu\n",C_6_BYTES);
  printf("C_7_BYTES %llu\n",C_7_BYTES);
  printf("C_8_BYTES %llu\n",C_8_BYTES);
  
  s32 sMagSF;
  
  s32 sMagCustomer;
  s32 sMagLineItem;
  s32 sMagOrders;
  s32 sMagPart;
  s32 sMagPartSupp;
  s32 sMagSupplier;

  s32 sByteMagCustomer;
  s32 sByteMagLineItem;
  s32 sByteMagOrders;
  s32 sByteMagPart;
  s32 sByteMagPartSupp;
  s32 sByteMagSupplier;

  vTrace("        Customer    LineItem    Orders      Part        PartSupp     Supplier\n"_Stz);
  
  for (i8 iSF = 1; iSF <= C_1M; iSF *= 10) {
    MagLib::vUnitMag(iSF,sMagSF);
    vTrace("SF%-4s: "_Stz,sMagSF.azChar());
    
    MagLib::vUnitMag(C_TPCH_CUSTOMER * iSF,sMagCustomer);
    MagLib::vUnitMag(C_TPCH_LINEITEM * iSF,sMagLineItem);
    MagLib::vUnitMag(C_TPCH_ORDERS   * iSF,sMagOrders);
    MagLib::vUnitMag(C_TPCH_PART     * iSF,sMagPart);
    MagLib::vUnitMag(C_TPCH_PARTSUPP * iSF,sMagPartSupp);
    MagLib::vUnitMag(C_TPCH_SUPPLIER * iSF,sMagSupplier);

    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_CUSTOMER * iSF),sByteMagCustomer);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_LINEITEM * iSF),sByteMagLineItem);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_ORDERS   * iSF),sByteMagOrders);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_PART     * iSF),sByteMagPart);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_PARTSUPP * iSF),sByteMagPartSupp);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_SUPPLIER * iSF),sByteMagSupplier);

    vTrace("%4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s)\n"_Stz,
           sMagCustomer.azChar(),sByteMagCustomer.azChar(),
           sMagLineItem.azChar(),sByteMagLineItem.azChar(),
           sMagOrders.azChar(),  sByteMagOrders.azChar(),
           sMagPart.azChar(),    sByteMagPart.azChar(),
           sMagPartSupp.azChar(),sByteMagPartSupp.azChar(),
           sMagSupplier.azChar(),sByteMagSupplier.azChar());
  }
  
//  s32 sMag;
//  MagLib::vByteMag(IntLib::iByteSpace(C_1_BYTE), 3,sMag); vTrace("1B: %4s %lld\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_2_BYTES),3,sMag); vTrace("2B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_3_BYTES),3,sMag); vTrace("3B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_4_BYTES),3,sMag); vTrace("4B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_5_BYTES),3,sMag); vTrace("5B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_6_BYTES),3,sMag); vTrace("6B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_7_BYTES),3,sMag); vTrace("7B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_8_BYTES),3,sMag); vTrace("8B: %4s\n"_Stz,sMag.azChar());

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_trackMemory.cpp -- track memory                                     */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_trackMemory (
  PF_TRACKMEMORY,             // prompt function
  "Prompt_trackMemory"_Stz,   // name of the prompt function
  "track memory"_Stz,         // comment about the function
  "trackMemory"_Stz,          // the prompt command
  nullptr,                    // the second word
  bPrompt_trackMemory,        // prompt function
  true,                       // enabled

  true,                       // visible
  "trackMemory"_Stz,          // help
  ""_Stz                      // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_trackMemory(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  Api::vReapJobsOnce();

  pPrompt->vTrackMemory(true);

  i8 nJobs = AM::JOBS::nJobsActive();
  AM::JOBS::vJobsThreshold(nJobs);
  
  vTrace("\nsuccess: 0 rows affected\n"_Stz);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_uptime.cpp -- uptime prompt function                                */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_uptime (
  PF_UPTIME,             // prompt function
  "Prompt_uptime"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "uptime"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_uptime,        // prompt function
  true,                  // enabled

  true,                  // visible
  "uptime"_Stz,          // help
  ""_Stz                 // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_uptime(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  i8 nSeconds = AM::SYSTEM::nUptimeInSeconds();

  vTrace("%llds\n"_Stz,nSeconds);
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_timerStart.cpp -- timer start                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_ut_skipList (
  PF_UT_SKIPLIST,          // prompt function
  "Prompt_ut_skipList"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "ut_skipList"_Stz,       // the prompt command
  nullptr,                 // the second word
  bPrompt_ut_skipList,     // prompt function
  true,                    // enabled

  true,                    // visible
  "ut_skipList"_Stz,       // help
  ""_Stz                   // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_ut_skipList(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  bool bPassed = UT_SkipList::bTest();
 
  if (bPassed)
    vTrace("\nsuccess: 0 rows affected\n"_Stz);
  else
    vTrace("\nUT_SkipList failed\n"_Stz);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_verbose -- enable / disable verbose mode                            */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_verbose (
    PF_VERBOSE,                  // prompt function
    "Prompt_verbose"_Stz,        // name of the prompt function
    "enable / disable verbose mode"_Stz, // comment about the function
    "verbose"_Stz,               // the prompt command
    nullptr,                     // the second word
    bPrompt_verbose,                // prompt function
    true,                        // enabled

    true,                        // visible
    "reap"_Stz,                  // help
    ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_verbose(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("disable"_Stz)) {

    vTrace("VERBOSE DISABLED\n"_Stz);
    AM::SYSARGS::vVerbose(false);
    vTrace("\nsuccess: 0 rows affected\n"_Stz);
  }
  else
  if (pPrompt->pInpTknVec()->nItems() == 2 &&
      pPrompt->pInpTknVec()->pGet(1)->bSame("enable"_Stz)) {

    vTrace("VERBOSE ENABLED\n"_Stz);
    AM::SYSARGS::vVerbose(true);
    vTrace("\nsuccess: 0 rows affected\n"_Stz);
  }
  else {
    Stz* szCmd = pPrompt->pInpTknVec()->pGet(0)->szChars();
    vTrace("error: %s : %s\n"_Stz,aEC[EC_INVALID_COMMAND],szCmd);
  }

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_version.cpp -- get SE version                                       */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_version (
  PF_VERSION,           // prompt function
  "Prompt_version"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "version"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_version,      // prompt function
  true,                 // enabled

  true,                 // visible
  "version"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_version(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  /*
  [args].metadata if
    "(version VARCHAR)" metadata!
    "')" version "row: ('" "%v%v%v\n" str.format
    1 task::this task.session@ session.rowsProcessed
    else
      version
      then
   */
  ASSERT(pInpTknVec->pGet(0)->bSame("version"_Stz));
  
  if (AM::bMetadata()) {
    vTrace("\nmetadata: (version VARCHAR)\n\n"_Stz);
    vTrace("row: ('%s')\n"_Stz, AM::szVersionString());
    vTrace("\nsuccess: 1 rows affected\n"_Stz);
  }
  else
    vTrace("%s\n"_Stz, AM::szVersionString());
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_wait_bg.cpp -- wait on bg jobs                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_wait_bg (
  PF_WAIT_BG,        // prompt function
  "Prompt_wait_bg"_Stz,  // name of the prompt function
  "comment"_Stz,     // comment about the function
  "wait_bg"_Stz,     // the prompt command
  nullptr,           // the second word
  bPrompt_wait_bg,   // prompt function
  true,              // enabled

  true,              // visible
  "wait_bg"_Stz,     // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_wait_bg(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  pPrompt->vNapTillBgJobsCompleted();
  
  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_waiting.cpp -- waiting queue report                                 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_waiting (
  PF_WAITING,             // prompt function
  "Prompt_waiting"_Stz,   // name of the prompt function
  "comment"_Stz,          // comment about the function
  "waiting"_Stz,          // the prompt command
  nullptr,                // the second word
  bPrompt_waiting,        // prompt function
  true,                   // enabled

  true,                   // visible
  "waiting"_Stz,          // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_waiting(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  AM::pSchedule()->vDumpWaitingSet();

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_with.cpp -- compiled with report                                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_with (
  PF_WITH,             // prompt function
  "Prompt_with"_Stz,   // name of the prompt function
  "comment"_Stz,       // comment about the function
  "with"_Stz,          // the prompt command
  nullptr,             // the second word
  bPrompt_with,        // prompt function
  true,                // enabled

  true,                // visible
  "with"_Stz,       // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_with(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  i8 nTokens = pInpTknVec->nTokens();
  
  if (1 == nTokens) {
    BASE::vTraceWith();
    return true;
  }
  
  Stz* szWith = pInpTknVec->pGet(1)->szChars();
  
  if (! BASE::bTraceWith(szWith))
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "With: Code was not compiled with: %s"_Stz,szWith);

  return true;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Prompt_without.cpp -- compiled without report                              */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

PromptFun __Prompt_without (
  PF_WITH,              // prompt function
  "Prompt_without"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "without"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_without,      // prompt function
  true,                 // enabled

  true,                 // visible
  "with"_Stz,           // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/


bool PROMPT::bPrompt_without(SePrompt* pPrompt)
{
  RASSERT(pPrompt);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  i8 nTokens = pInpTknVec->nTokens();
  
  if (1 == nTokens) {
    BASE::vTraceWithout();
    return true;
  }
  
  Stz* szWith = pInpTknVec->pGet(1)->szChars();
  
  if (! BASE::bTraceWithout(szWith))
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Without: Code was compiled with: %s"_Stz,szWith);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
