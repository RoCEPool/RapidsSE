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
    
      //
      // Cells primarily contain the *indirect-address* of a function.
      //
      // This indirect-address is the speific address of the member m_pEntryStep
      // of a Word object instance (ie: &pWord->m_pEntryStep)
      //
      // That member of Word is a pointer to a function which executes the word.
      //
      // The function will be:
      //   i) Step::vDoColon for normal : words; or
      //  ii) Step::vXyz for primitives
      //
      
      fpMEMBERFN* m_afpStepFn;  // indirect address of a member function
      
      // In the case of i), the enter-routine function sets the current
      // tasks m_aInxPter
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      //
      // Additionally cells can be an argument for the previous cell.
      //
      // There are two cases:
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
