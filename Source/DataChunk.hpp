/*============================================================================*/
/* DataChunk.hpp -- Data Chunk                                                 */
/*============================================================================*/
#pragma once

namespace Chunks
{
  static const i8 C_DATA_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class DataChunk : public Object
  {
  protected:
    i8   m_nDataBytes;      // current number of bytes of actual data
    Stz* m_aLastDataByte;   // last data byte of where data chunk ends
    i8   m_nRemainingBytes; // bytes remaining
  

    WspFxd<C_DATA_CHUNK> m_xWsp; // workspace
  
  public:
    void* operator new(size_t);
    
    DataChunk(eCN eClass);
    ~DataChunk(void) noexcept(false);
  
    void  vCopy(DataChunk* pSource);
    char* aDataArea(void);
    Stz*  aStz(void);
  };
  
  DataChunk* _pDataChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
