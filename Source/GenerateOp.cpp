/*============================================================================*/
/* GenerateOp.cpp -- Generate data                                            */
/*============================================================================*/

#include "PushDown.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenerateOp* PushDown::_pGenerateOp(void* aAddr)
{
  GenerateOp* pGenerateOp = reinterpret_cast<GenerateOp*>(aAddr);
  
  return pGenerateOp;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* GenerateOp::operator new(size_t)
{
  GenerateOp* pGenerateOp =
    static_cast<GenerateOp*>(HeapMem::pAllocate(CN_GENERATEOP));
  
  return pGenerateOp;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void GenerateOp::operator delete(void* aAddress)
{
  GenerateOp* pGenerateOp = static_cast<GenerateOp*>(aAddress);
  
  HeapMem::vFree(pGenerateOp);
}


/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
GenerateOp::GenerateOp(Stz* szOpLabel, Jobs::Job* pJob)
: Op(CN_GENERATEOP,szOpLabel,pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
  
  m_pTablePtn = nullptr;
  m_pCrwBuffer = nullptr;

  _checkInitialised_(CN_GENERATEOP);

  m_fpOpFun = GenerateOp::eCallLoop;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
GenerateOp::~GenerateOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the fetch loop function                                               */
/*----------------------------------------------------------------------------*/
eOR GenerateOp::eCallLoop(void* aThis)
{  
  eOR eCallAgain = ((GenerateOp*)aThis)->eFetchLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* the fetch operator loop function                                           */
/*----------------------------------------------------------------------------*/
eOR GenerateOp::eFetchLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
  
  for (m_pCrwBuffer = m_pTablePtn->pRootCrwBuffer();
       m_pCrwBuffer;
       m_pCrwBuffer = m_pCrwBuffer->pNextBuffer()) {
    
    vTrace("%s::bFetch %p\n"_Stz,m_sOpLabel.aStz(),m_pCrwBuffer);
    
    vPipeOut(m_pCrwBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* set table partition                                                        */
/*----------------------------------------------------------------------------*/
void GenerateOp::vTablePtn(TablePtn* pTablePtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);

  m_pTablePtn = pTablePtn;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(GenerateOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
