/*============================================================================*/
/* Routine.hpp -- A Forth Routine                                             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_WSP_ROUTINE = C_SIZE_SMALL_OBJECT - 128;

  class Routine : public Object
  {
  private:
    Word* m_p_ExoWord;
  
    SingleWord m__PadToCacheLine[6]; // padding to cache line

    Wsp_t<C_WSP_ROUTINE> m_xWsp;
  
    // must be last
    Footer m_xFooter;

  public:
   void* operator new(size_t);
   void  operator delete(void* aAddress);
  
    Routine(Word* p_Word);
   ~Routine(void) noexcept(false);

    static Routine* pToRoutine(ForthCell* aInxPtr);
    static Word*    pToWord(ForthCell* aInxPtr);
  
    byte* ayAllot(i8 iBytes);

    void* aAppend(ForthCell* aCell);
    void  vAppend(ForthCell* aCell);
    void  vAppend(i8 iInt);
    void  vAppend(StepFp* afpStep);
    void  vAppend(TypedCell* aTypedCell);
  
    void  vReset(void);
  
    byte* ayWspArea(void);
    byte* ayWspLatter(void);
    i8    nWspUsed(void);

  private:
    Footer* aFooter(void);
  };

  Routine* _pRoutine(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
