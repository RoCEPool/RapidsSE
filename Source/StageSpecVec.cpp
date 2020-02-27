/*============================================================================*/
/* StageSpecVec.cpp -- Stage Specification Vector                             */
/*============================================================================*/

#include "PipelineSpecs.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StageSpecVec::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STAGESPECVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StageSpecVec::operator delete(void* pAddr)
{
  StageSpecVec* pStageSpecVec = reinterpret_cast<StageSpecVec*>(pAddr);
  
  HeapMem::vFree(pStageSpecVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSpecVec::StageSpecVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPECVEC,_FOOTER_);
  
   _checkInitialised_(CN_STAGESPECVEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGESPECVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSpecVec::~StageSpecVec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPECVEC,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find stage with given name                                                 */
/*----------------------------------------------------------------------------*/
StageSpec* StageSpecVec::pFindStage(Stz* szStageNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPECVEC,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    StageSpec* pStageSpec = pGet(i_Stage);
    
    if (pStageSpec->szStageNm()->bSame(szStageNm))
      return pStageSpec;
  }
  
  return nullptr;
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StageSpecVec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
