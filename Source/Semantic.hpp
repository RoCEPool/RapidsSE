/*============================================================================*/
/* Semantic.hpp -- Semantic Context                                           */
/*============================================================================*/
#pragma once

namespace Sessions
{
  class Semantic : public Object
  {
  friend SePrompt;
  
  private:
  
    TagSet m_oEnameSet; // ename-value set

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* aAddr);
    
    Semantic(void);
   ~Semantic(void) noexcept(false);
  
    TagSet* pEnameSet(void);

  private:
    Footer* aFooter(void);
  };
  
  Semantic* _pSemantic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
