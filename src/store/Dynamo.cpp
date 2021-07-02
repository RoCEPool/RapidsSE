/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Grid.hpp"
#include "OpSpec.hpp"
#include "PipelineSpec.hpp"
#include "Pipeline.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "StageSpec.hpp"

#include "TagConstants.h"

#include "SP_TknVecStr.hpp"

#ifdef WITH_NESTED_PIPELINES
#include "SP_PipelineSpec.hpp"
#endif

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Dynamo,CN_DYNAMO)
__aNewFunction(Dynamo,CN_DYNAMO)
__vDeleteFunction(Dynamo,CN_DYNAMO)
__aFooterFunction(Dynamo)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamo::Dynamo(Stz* szPipelineStmt, Job* p_PipelineJob)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_p_PipelineJob = p_PipelineJob;
  m_p_PipelineSpec_root = nullptr;
  
  m_pScanner = new Scanner(szPipelineStmt);
    
  _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  delete m_pScanner;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* translate token vector into a pipeline statement                           */
/*----------------------------------------------------------------------------*/
bool Dynamo::bToPipelineStmt(String* pAsPipelineStmt,
                             TokenVec* pFromTokenVec)
{
  _static_
  
  i8 nTkns = pFromTokenVec->nTokens();
  
  Stz* szOpNm;
  Stz* szOpArg;
  
  char cStageStart   = 'A';
  char cStageCurrent = 'A';

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pAsPipelineStmt->vCopy(
    "pipeline unnamed\n"
    "begin\n"
    ""_Stz);

  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on origin :\n"
    "    start\n"
    "    | periodic once\n"
    "    | stageB\n"
    ""_Stz,
  cStageStart);

  ++ cStageCurrent;
  
  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on all partitions :\n"
    ""_Stz,
    cStageCurrent);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Tkn = 0; i_Tkn < nTkns; /*none*/) {

    szOpNm = pFromTokenVec->pGet(i_Tkn++)->szChars();

    // -- -- -- -- -- -- -- -- -- --
    
    if (szOpNm->bSame("scan"_Stz)) {
      
      szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();

      pAsPipelineStmt->vCatSprint("    scan %s\n"_Stz,szOpArg);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
      }
      
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("filter"_Stz)) {
      
      pAsPipelineStmt->vCatSprint("    | filter 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint(" %s"_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint(" <>\"\n"_Stz);
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("project"_Stz)) {
      
      pAsPipelineStmt->vCatSprint("    | project 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint("%s "_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint("\"\n"_Stz);
            
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else
    if (szOpNm->bSame("aggreg"_Stz)) {
      
      pAsPipelineStmt->vCatSprint(
        "    | stage%c\n"
        ""_Stz,
        cStageCurrent + 1);

      ++ cStageCurrent;
      
      pAsPipelineStmt->vCatSprint(
        "  stage stage%c on origin :\n"
        ""_Stz,
        cStageCurrent);
      
      pAsPipelineStmt->vCatSprint("    aggreg 0"_Stz);
      pAsPipelineStmt->vCatSprint(" \""_Stz);
      
      while (i_Tkn < nTkns) {
        szOpArg = pFromTokenVec->pGet(i_Tkn++)->szChars();
        if (szOpArg->bSame("|"_Stz))
          break;
        pAsPipelineStmt->vCatSprint("%s "_Stz,szOpArg);
      }
      
      pAsPipelineStmt->vCatSprint("\"\n"_Stz);
            
    }
    
    // -- -- -- -- -- -- -- -- -- --
    
    else {
      
      return false;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pAsPipelineStmt->vCatSprint(
    "    | stage%c\n"
    ""_Stz,
    cStageCurrent + 1);
  
  ++ cStageCurrent;
  
  pAsPipelineStmt->vCatSprint(
    "  stage stage%c on origin :\n"
    "    output\n"
    "    | sink\n"
    "    | stage%c\n"
    ""_Stz,
    cStageCurrent, cStageStart);

  pAsPipelineStmt->vConcat(
    "end ;\n"
    ""_Stz);
  
  vTrace("bToPipelineStmt : \n%s\n"_Stz,pAsPipelineStmt->szStz());
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* recursively compile the tokenised pipeline statement                       */
/*----------------------------------------------------------------------------*/
bool Dynamo::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
        
  m_pScanner->vTokenBegin(0);
      
  if (!  PipelineSpec::bCompile(nullptr,
                                nullptr,
                                m_pScanner,
                                this,
                                
                                m_p_PipelineSpec_root))
    return false;
  
  
#ifdef WITH_NESTED_PIPELINES
  if (m_p_PipelineSpec_root->szPipelineNm()->bSame("nestTest"_Stz)) {
    SP_PipelineSpec spPipelineSpec(m_p_PipelineSpec_root);
    vTrace("\n\nPipelineSpec:\n%s\n"_Stz,spPipelineSpec.sz());
    vDebugPoint();
  }
#endif

  return true;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
PipelineSpec* Dynamo::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_p_PipelineSpec_root;
}

/*----------------------------------------------------------------------------*/
/* show the pipeline                                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  StageSpec* pStageSpec = m_p_PipelineSpec_root->pStageSpec(0);
  
  sSTMT sPipe("pipeline "_Stz);
  pStageSpec->vToPipeStr(sPipe.szStz());
    
  m_pScanner->pTknVec()->vTokenise(sPipe);
  
  SP_TknVecStr spTknVecStr;
  vTrace("TknVecStr ]]%s[[\n"_Stz,spTknVecStr.sz(m_pScanner->pTknVec()));
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
