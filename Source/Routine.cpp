/*============================================================================*/
/* Routine.hpp -- Forth Routine                                               */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(SenVM,Routine,CN_ROUTINE)
__aNewFunction(Routine,CN_ROUTINE)
__vDeleteFunction(Routine,CN_ROUTINE)
__aFooterFunction(Routine)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Routine::Routine(Word* p_ExoWord)
: Object(CN_ROUTINE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);

  m_p_ExoWord = p_ExoWord;
  
  m_xWsp.vZero(CN_ROUTINE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Routine::~Routine(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the outer routine of given instruction ptr                             */
/*----------------------------------------------------------------------------*/
Routine* Routine::pToRoutine(Cell* aInxPtr)
{
  _static_
    
  Routine* aRoutine = _pMaskDownToObject<Routine*>(aInxPtr,C_MASK_SMALL_OBJECT);

  Routine* pRoutine = _pRoutine(aRoutine);
    
  return pRoutine;
}

/*----------------------------------------------------------------------------*/
/* get the outer word of given instruction ptr                                */
/*----------------------------------------------------------------------------*/
Word* Routine::pToWord(Cell* aInxPtr)
{
  _static_
    
  Routine* pRoutine = pToRoutine(aInxPtr);

  Word* pWord = pRoutine->m_p_ExoWord;
  
  return pWord;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot data space                                                           */
/*----------------------------------------------------------------------------*/
byte* Routine::ayAllot(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  byte* ayAddress = m_xWsp.ayAllot(nBytes);
    
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  return ayAddress;
}


/*----------------------------------------------------------------------------*/
/* append a pointer to a cell                                                 */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  IGNORE_RETURN(aAppend(aCell));
}

/*----------------------------------------------------------------------------*/
/* append a pointer to a cell, return the address of the allotted cell        */
/*----------------------------------------------------------------------------*/
void* Routine::aAppend(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  Cell* aAllottedCell = (Cell*)ayAllot(sizeof(aCell));
  
  aAllottedCell->m_aJumpTo = aCell;
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  i8* aAllottedInt = (i8*)ayAllot(sizeof(i8));

  *aAllottedInt = iInt;
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to step function                                 */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(StepFp* afpStepFp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);

  StepFp** aafpStepFp = (StepFp**)ayAllot(sizeof(StepFp*));

  * aafpStepFp = afpStepFp;
}

/*----------------------------------------------------------------------------*/
/* append a value (the entire value, not a pointer to it)                     */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  i8 nBytes = aAnyVal->nStackUsage();
  
  char* aValueAddress = (char*)ayAllot(nBytes);
  
  MemLib::vCopy(aValueAddress,aAnyVal,nBytes,CN_ROUTINE);
}

/*----------------------------------------------------------------------------*/
/* append a value (the entire value, not a pointer to it)                     */
/*----------------------------------------------------------------------------*/
void Routine::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  m_xWsp.vReset();
}

/*----------------------------------------------------------------------------*/
/* get the start of the wsp area                                              */
/*----------------------------------------------------------------------------*/
byte* Routine::ayWspArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  return m_xWsp.ayArea();
}

/*----------------------------------------------------------------------------*/
/* get the latter portion of the wsp area                                     */
/*----------------------------------------------------------------------------*/
byte* Routine::ayWspLatter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  return m_xWsp.ayLatter();
}

/*----------------------------------------------------------------------------*/
/* get the latter portion of the wsp area                                     */
/*----------------------------------------------------------------------------*/
i8 Routine::nWspUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROUTINE,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
