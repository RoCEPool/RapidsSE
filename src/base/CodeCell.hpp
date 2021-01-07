/*============================================================================*/
/* CodeCell.hpp -- cell in code block of routine                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class CodeCell
  {
  public:
    union {
    
      //  ray to rewrite this comment
      
      //
      // cells are primarily a pointer to the address of a Word::m_pEntryStep
      // which contains a pointer the step function which executes the word
      //
      // that step will be:
      //   - Step::vEnterRoutine for normal words
      //   - Step::vXyz for primitives
      //
      
      fpMEMBERFN* m_afpStepFn;  // indirect address of a member function

      //
      // additionally cells can be an argument to previous cell :
      //
      
      CodeCell* m_aJumpTo;   // a) pointer to another cell within same routine
      
      _Header m_xTypedValue; // b) header for embedded typed value (PushValue)
    };
  };
  
  CodeCell* _aCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
