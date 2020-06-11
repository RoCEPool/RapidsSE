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
    i8    m_nTicks;
    
    Timer m_oTimer;
  
    // must be last
    Footer m_xFooter;
    
  public:
    RU_Cpu(void);
   ~RU_Cpu(void) noexcept(false);
  
    void vEnd(void);
    void vReport(void);
    void vStart(void);
    void vWriteReport(File* pFile);
    
    void vBuffersInc(void);
    void vRowsAdd(i8 nRows);

    i8 nBuffers(void);
    i8 nRows(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
