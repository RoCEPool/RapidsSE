/*============================================================================*/
/* Cell.hpp -- cell in the routine area of a word                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Cell
  {
  public:
    union {
      
      //
      // cells are primarily a pointer to the address of a Word::m_pEntryStep
      // which contains a pointer the step function which executes the word
      //
      // that step will be:
      //   - Step::vEnterRoutine for normal words
      //   - Step::vEnterObject for objects
      //   - Step::vXyz for primitives
      //
      
      StepFp* m_afpEntryStep; // address of the Word::m_pEntryStep
      
      //
      // additionally cells can be a argument to the previous cell/stepfun
      //
      
      Cell*   m_aJumpTo;      // a pointer to another cell within same routine
      
      Value   m_xEmbValue;    // or an embedded value (for PushValue)
    };
  };
  
  Cell* _aCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
