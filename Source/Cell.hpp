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
      
      TaskStep* m_ppEntryStep; // pointer to the Word::m_pEntryStep that
                               // contains the address of the task step which
                               // enters a word.
                               // that step will be:
                               //   - Task::vEnterRoutine for normal words
                               //   - Task::vEnterObject for objects
                               //   - Task::vXyz for primitives
      
      Cell*     m_pJumpTo;     // pointer to another cell within same routine
      
    };
  };
  
  Cell* _aCell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
