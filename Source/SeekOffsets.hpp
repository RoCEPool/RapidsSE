/*============================================================================*/
/* SeekOffsets.hpp -- Seek Offsets                                            */
/*============================================================================*/
#pragma once

namespace Load
{
  class SeekOffsets : public Operand
  {
  protected:
    i8 m_nOffsets;   // no of offsets
    
    struct {
      
      i8 n_iOffsets; // chunk of csv records
      i8 m_nBytes;   // bytes in teh offset
      
    } m_xSetOffsets[C_SET_OFFSETS];
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    SeekOffsets(void);
   ~SeekOffsets(void) noexcept(false);
  };

  SeekOffsets* _pSeekOffsets(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
