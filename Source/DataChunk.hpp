/*============================================================================*/
/* DataChunk.hpp -- Data Chunk                                                 */
/*============================================================================*/
#pragma once

namespace Chunks
{
  static const i8 C_WSP_DATACHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class DataChunk : public Object
  {
  public:
    i8   m_nDataBytes;        // current number of bytes of actual data
    Stz* m_aLastDataByte;     // last data byte of where data chunk ends
    i8   m_nUnprocessedBytes; // bytes yet to be processed
  
    Wsp_t<C_WSP_DATACHUNK> m_xWsp; // workspace
  
  public:
    void* operator new(size_t);
    
    DataChunk(eCN eClass);
    ~DataChunk(void) noexcept(false);
  
    void  vCopy(DataChunk* pSource);
    char* aDataArea(void);
    i8    nQuantumBytes(void);
    void  vSetBytes(i8 nDataBytes);
  };
  
  DataChunk* _pDataChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
