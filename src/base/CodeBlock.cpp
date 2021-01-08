/*============================================================================*/
/* CodeBlock.cpp -- Code Block                                                */
/*============================================================================*/

#include "BASE.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,CodeBlock,CN_CODEBLOCK)
__aNewFunction(CodeBlock,CN_CODEBLOCK)
__vDeleteFunction(CodeBlock,CN_CODEBLOCK)
__aFooterFunction(CodeBlock)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CodeBlock::CodeBlock(Word* p_ExoWord)
: Object(CN_CODEBLOCK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);

  m_p_ExoWord = p_ExoWord;
  
  m_xWsp.vZero(CN_CODEBLOCK);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CodeBlock::~CodeBlock(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the outer routine of given instruction ptr                             */
/*----------------------------------------------------------------------------*/
CodeBlock* CodeBlock::pToCodeBlock(CodeCell* aInxPtr)
{
  _static_
    
  CodeBlock* aCodeBlock = _pMaskDownToObject<CodeBlock*>(aInxPtr,C_MASK_SMALL_OBJECT);

  CodeBlock* pCodeBlock = _pCodeBlock(aCodeBlock);
    
  return pCodeBlock;
}

/*----------------------------------------------------------------------------*/
/* get the outer word of given instruction ptr                                */
/*----------------------------------------------------------------------------*/
Word* CodeBlock::pToWord(CodeCell* aInxPtr)
{
  _static_
    
  CodeBlock* pCodeBlock = pToCodeBlock(aInxPtr);

  Word* pWord = pCodeBlock->m_p_ExoWord;
  
  return pWord;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot data space                                                           */
/*----------------------------------------------------------------------------*/
byte* CodeBlock::ayAllot(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  byte* ayAddress = m_xWsp.ayAllot(nBytes);
    
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  return ayAddress;
}


/*----------------------------------------------------------------------------*/
/* append a jump to address, return the address of the allotted cell          */
/*----------------------------------------------------------------------------*/
void* CodeBlock::aAppend(CodeCell* aJumpTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  CodeCell* aAllottedCell = (CodeCell*)ayAllot(sizeof(aJumpTo));
  
  aAllottedCell->m_aJumpTo = aJumpTo;
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append a jump to address                                                   */
/*----------------------------------------------------------------------------*/
void CodeBlock::vAppend(CodeCell* aJumpTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  IGNORE_RETURN(aAppend(aJumpTo));
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to step function                                 */
/*----------------------------------------------------------------------------*/
void CodeBlock::vAppend(fpSTEPFN* afpStepFp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);

  fpSTEPFN** aafpStepFp = (fpSTEPFN**)ayAllot(sizeof(fpSTEPFN*));

  * aafpStepFp = afpStepFp;
}

/*----------------------------------------------------------------------------*/
/* append a value (the entire value, not a pointer to it)                     */
/*----------------------------------------------------------------------------*/
void CodeBlock::vAppend(TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  i8 nBytes = aTypedCell->nStackUsage();
  
  char* aValueAddress = (char*)ayAllot(nBytes);
  
  MemLib::vCopy(aValueAddress,aTypedCell,nBytes,CN_CODEBLOCK);
}

/*----------------------------------------------------------------------------*/
/* append a value (the entire value, not a pointer to it)                     */
/*----------------------------------------------------------------------------*/
void CodeBlock::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  m_xWsp.vReset();
}

/*----------------------------------------------------------------------------*/
/* get the start of the wsp area                                              */
/*----------------------------------------------------------------------------*/
byte* CodeBlock::ayWspArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  return m_xWsp.ayArea();
}

/*----------------------------------------------------------------------------*/
/* get the latter portion of the wsp area                                     */
/*----------------------------------------------------------------------------*/
byte* CodeBlock::ayWspLatter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  return m_xWsp.ayLatter();
}

/*----------------------------------------------------------------------------*/
/* get the latter portion of the wsp area                                     */
/*----------------------------------------------------------------------------*/
i8 CodeBlock::nWspUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODEBLOCK,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
