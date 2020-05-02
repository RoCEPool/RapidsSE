/*============================================================================*/
/* Routine.hpp -- A Forth Routine                                             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_WSP_ROUTINE = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Routine : public Object
  {
  public:
    Word* m_p_ExoWord;
  
    Wsp_t<C_WSP_ROUTINE> m_xWsp;
  
  public:
    Routine(Word* p_Word);
  
    static Routine* pToRoutine(Cell* aInxPtr);
    static Word*    pToWord(Cell* aInxPtr);
  
    byte* ayAllot(i8 iBytes);

    void* aAppend(Cell* aCell);
    void  vAppend(Cell* aCell);
    void  vAppend(i8 iInt);
    void  vAppend(StepFp* afpStep);
    void  vAppend(AnyVal* aAnyVal);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
