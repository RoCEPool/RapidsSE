/*============================================================================*/
/* Routine.hpp -- Forth Routine                                               */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Routine::Routine(Word* p_ExoWord)
: Object(CN_ROUTINE)
{
  m_p_ExoWord = p_ExoWord;
}

/*----------------------------------------------------------------------------*/
/* get the outer routine of given instruction ptr                             */
/*----------------------------------------------------------------------------*/
Routine* Routine::pToRoutine(Cell* aInxPtr)
{
  _static_
    
  Word* pWord = pToWord(aInxPtr);
  
  Routine* pRoutine = pWord->pRoutine();
    
  return pRoutine;
}

/*----------------------------------------------------------------------------*/
/* get the outer word of given instruction ptr                                */
/*----------------------------------------------------------------------------*/
Word* Routine::pToWord(Cell* aInxPtr)
{
  _static_
    
  Word* pWord = _pMaskDownToObject<Word*>(aInxPtr,C_MASK_MEDIUM_OBJECT);
    
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
  //_precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  byte* ayAddress = m_xWsp.ayAllot(nBytes);
    
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return ayAddress;
}


/*----------------------------------------------------------------------------*/
/* append a pointer to a cell                                                 */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(Cell* aCell)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  IGNORE_RETURN(aAppend(aCell));
}

/*----------------------------------------------------------------------------*/
/* append a pointer to a cell, return the address of the allotted cell        */
/*----------------------------------------------------------------------------*/
void* Routine::aAppend(Cell* aCell)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aAllottedCell = (Cell*)ayAllot(sizeof(aCell));
  
  aAllottedCell->m_aJumpTo = aCell;
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(i8 iInt)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  i8* aAllottedInt = (i8*)ayAllot(sizeof(i8));

  *aAllottedInt = iInt;
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to step function                                 */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(StepFp* afpStepFp)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  StepFp** aafpStepFp = (StepFp**)ayAllot(sizeof(StepFp*));

  * aafpStepFp = afpStepFp;
}

/*----------------------------------------------------------------------------*/
/* append a value (the entire value, not a pointer to it)                     */
/*----------------------------------------------------------------------------*/
void Routine::vAppend(AnyVal* aAnyVal)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  i8 nBytes = aAnyVal->nStackUsage();
  
  char* aValueAddress = (char*)ayAllot(nBytes);
  
  MemLib::vCopy(aValueAddress,aAnyVal,nBytes,CN_WORD);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
