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
Routine* Routine::pToRoutine(void* aInxPtr)
{
  _static_
    
  Word* pWord = pToWord(aInxPtr);
  
  Routine* pRoutine = nullptr;
    
  return pRoutine;
}

/*----------------------------------------------------------------------------*/
/* get the outer word of given instruction ptr                                */
/*----------------------------------------------------------------------------*/
Word* Routine::pToWord(void* aInxPtr)
{
  _static_
    
  Word* pWord = _pMaskDownToObject<Word*>(aInxPtr,C_MASK_MEDIUM_OBJECT);
    
  return pWord;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
