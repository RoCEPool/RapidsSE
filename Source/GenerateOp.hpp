/*============================================================================*/
/* GenerateOp.hpp -- Generate                                                 */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class GenerateOp : public Op
  {
  private:
    TablePtn*  m_pTablePtn;
    CrwBuffer* m_pCrwBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    GenerateOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~GenerateOp(void) noexcept(false);

    static eOR eCallLoop(void* aThis);
    eOR eFetchLoop(void);

    void vTablePtn(TablePtn* pTablePtn);
  
  private:
    Footer* aFooter(void);
  };

  GenerateOp* _pGenerateOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
