/*============================================================================*/
/* LoadChunk.hpp -- File Load Buffer                                         */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace Structures
{
  static const i8 C_LOADCHUNK_LENGTH = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  class LoadChunk : public Buffer,
                    public WspFxd<C_LOADCHUNK_LENGTH>
  {
  private:
    File  m_xFile;       // file being loaded
    char* m_aEndChunk;   // last byte of where chunk ends
    char* m_aNextRecord; // first byte of next record
        
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadChunk(void);
    ~LoadChunk(void);

    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void  vCloseFile(void);
    i8    iNextRecord(void);
    char* pNextRecord(i8& iRecordBytes);
    void  vOpenFile(s256& sPath);
    void  vReadChunk(i8 iSeek, i8 nBytes);
    
  private:
    void  vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
