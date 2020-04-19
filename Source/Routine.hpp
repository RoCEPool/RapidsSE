/*============================================================================*/
/* Routine.hpp -- A Forth Routine                                             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_WSP_ROUTINE = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Routine
  {
  public:
    Word* m_p_ExoWord;
  
    Wsp_t<C_WSP_ROUTINE> m_xWsp;
  
  public:
    Routine(Word* p_Word);
  
  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
