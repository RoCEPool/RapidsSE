/*============================================================================*/
/* RU_Memory.hpp - Memory Resource Ussage                                     */
/*============================================================================*/
#pragma once

namespace Resources
{
  class RU_Memory : public RU
  {
  private:
  
    struct {
      
      i8 m_nAlloc;
      i8 m_nFree;
      i8 m_nMax;

    } xClass[CN_ENUMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    RU_Memory(void);
   ~RU_Memory(void) noexcept(false);
  
    void vInit(void);
    void vReport(void);
    void vWriteReport(File* pFile);
    
    void vAllocInc(eCN eClassNo);
    void vFreeInc(eCN eClassNo);

    i8 nAlloc(eCN eClassNo);
    i8 nFree(eCN eClassNo);
    i8 nMax(eCN eClassNo);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
