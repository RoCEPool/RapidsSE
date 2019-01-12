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
      
      StepFp* m_afpEntryStep; // address of a Word::m_pEntryStep that
                              // contains a function pointer to the step which
                              // enters a word.
                              // that step will be:
                              //   - Task::vEnterRoutine for normal words
                              //   - Task::vEnterObject for objects
                              //   - Task::vXyz for primitives
      
      Cell*   m_aJumpTo;      // pointer to another cell within same routine
      
    };
  };
  
  Cell* _aCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
