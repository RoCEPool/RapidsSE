/*============================================================================*/
/* CodeBlock.hpp -- Code Block                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_WSP_ROUTINE = C_SIZE_SMALL_OBJECT - 128;

  class CodeBlock : public Object
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
  
    CodeBlock(Word* p_Word);
   ~CodeBlock(void) noexcept(false);

    static CodeBlock* pToRoutine(CodeCell* aInxPtr);
    static Word*    pToWord(CodeCell* aInxPtr);
  
    byte* ayAllot(i8 iBytes);

    void* aAppend(CodeCell* aCell);
    void  vAppend(CodeCell* aCell);
    void  vAppend(i8 iInt);
    void  vAppend(fpSTEPFN* afpStep);
    void  vAppend(TypedCell* aTypedCell);
  
    void  vReset(void);
  
    byte* ayWspArea(void);
    byte* ayWspLatter(void);
    i8    nWspUsed(void);

  private:
    Footer* aFooter(void);
  };

  CodeBlock* _pCodeBlock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
