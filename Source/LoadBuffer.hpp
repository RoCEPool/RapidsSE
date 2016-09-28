/*============================================================================*/
/* LoadBuffer.hpp -- File Load Buffer                                         */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace Structures
{
  static const i8 C_LOADBUFFER_SIZE = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  class LoadBuffer : public Buffer
  {
  private:
    
    char* m_pEndChunk;   // last byte of where chunk ends
    char* m_pNextRecord; // first byte of next record
    
    char  m_wWorkarea[C_LOADBUFFER_SIZE];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadBuffer(void);
    ~LoadBuffer(void);

    i8    iBytesWorkarea(void);
    i8    iCompleteBytes(i8 iBytesRead);
    i8    iNextRecord(void);
    i8    iEndChunk(void);
    char* pNextRecord(i8& iRecordBytes);
    char* pWorkarea(void);
    
  private:
    void  vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/