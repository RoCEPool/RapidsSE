/*============================================================================*/
/* CodeCell.hpp -- cell in the routine area of a word                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  //
  // declare a function pointer to any member function of Object
  //   n.b. Object is non-virtual,
  //        so fpMEMBERFN can be cast to a sub-class of Object
  //
  typedef void (Object::*fpMEMBERFN)(void);

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
      // additionally cells can be an argument to a another cell
      //
      
      CodeCell* m_aJumpTo;  // a pointer to another cell within same routine
      
      _Header m_xTypedValue; // or header for embedded typed value (PushValue)
    };
  };
  
  CodeCell* _aCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
