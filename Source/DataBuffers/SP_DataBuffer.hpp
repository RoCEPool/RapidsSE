/*============================================================================*/
/* SP_DataBuffer.hpp - Data Buffer sprint helper                              */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class SP_DataBuffer : public Sprint
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_DataBuffer(void);
   ~SP_DataBuffer(void) noexcept(false);

    Stz* sz(DataBuffer* pDataBuffer);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
