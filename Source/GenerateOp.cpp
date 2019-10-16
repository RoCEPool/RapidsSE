/*============================================================================*/
/* GenerateOp.cpp -- Generate data                                            */
/*============================================================================*/

#include "DataOperators.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenerateOp* DataOperators::_pGenerateOp(void* aAddr)
{
  GenerateOp* pGenerateOp = reinterpret_cast<GenerateOp*>(aAddr);
  
  return pGenerateOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* GenerateOp::operator new(size_t)
{
  GenerateOp* pGenerateOp =
    static_cast<GenerateOp*>(HeapMem::pAllocate(CN_GENERATEOP));
  
  return pGenerateOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void GenerateOp::operator delete(void* aAddress)
{
  GenerateOp* pGenerateOp = static_cast<GenerateOp*>(aAddress);
  
  HeapMem::vFree(pGenerateOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenerateOp::GenerateOp(Stz* szOpLabel, Pipeline* pPipeline, i8 iBuffers)
: Op(CN_GENERATEOP,nullptr,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
  
  m_pRootBuffer = nullptr;
  m_iBuffers = iBuffers;
  
  _checkInitialised_(CN_GENERATEOP);

  m_fpCall = GenerateOp::eCallLoop;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
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
  eOR eCallAgain = ((GenerateOp*)aThis)->eGenerateLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call the fetch loop function                                               */
/*----------------------------------------------------------------------------*/
eOR GenerateOp::eGenerateLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GENERATEOP,_FOOTER_);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an IntBuffer
  m_pRootBuffer = new IntBuffer();
  IntBuffer* pIntBuffer = m_pRootBuffer;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Add random integer values to the buffer till the buffer is full
  i8 iTotalValues = ELEMENTS * m_iBuffers;
  
  while (iTotalValues--) {
    i8 iRandom = Random::iInteger(0,1000);
    if (pIntBuffer->bHasSpace())
      pIntBuffer->vAppendValue(iRandom);
    else {
      
      // pipeout buffer
      vTrace("%s::Generate %p\n"_Stz,m_sName.szStz(),pIntBuffer);
      vPipeOut(pIntBuffer);
      
      // create a new buffer
      IntBuffer* pNextBuffer = new IntBuffer();
      pIntBuffer->vNextBuffer(pNextBuffer);
      pNextBuffer->vPrevBuffer(pIntBuffer);
      pIntBuffer = pNextBuffer;
      pIntBuffer->vAppendValue(iRandom);
    }
  }
  
  // pipeout buffer
  vTrace("%s::Generate %p\n"_Stz,m_sName.szStz(),pIntBuffer);
  vPipeOut(pIntBuffer);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the buffer
  //vTrace("\n\nSorting buffer:\n"_Stz);
  //m_pRootBuffer->vDump();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(GenerateOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
