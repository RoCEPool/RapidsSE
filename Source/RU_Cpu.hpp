/*============================================================================*/
/* RU_Cpu.hpp - CPU Resource Ussage                                           */
/*============================================================================*/
#pragma once

namespace ResourceUsage
{
  class RU_Cpu : public RU
  {
  private:
  
    i8    m_nBuffers;
    i8    m_nRows;
    Timer m_oTimer;
  
    // must be last
    Footer m_xFooter;
    
  public:
    RU_Cpu(void);
   ~RU_Cpu(void) noexcept(false);
  
    void vInit(void);
    void vReport(void);
    void vWriteReport(File* pFile);
    
    void vBuffersInc(void);
    void vRowsInc(void);

    i8 nBuffers(void);
    i8 nRows(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
