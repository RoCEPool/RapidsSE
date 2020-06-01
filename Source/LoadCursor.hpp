/*============================================================================*/
/* LoadCursor.hpp -- Load Cursor                                              */
/*============================================================================*/
#pragma once

namespace UpperLoad
{
  class LoadCursor : public Object
  {
  public:
    Format* m_pFmt;              // using these formatters
    Stz*    m_aLastDataByte;     // last data byte of where data chunk ends
    i8      m_nUnprocessedBytes; // bytes yet to be processed
    Stz*    m_szNextRecord;   // first byte of next record
    
    Stz*    m_aDataArea;        // data area
    i8      m_nDataBytes;        // current number of bytes of actual data
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadCursor(Format* pFmt);
   ~LoadCursor(void) noexcept(false);
  
    void  vInit(Stz* aDataArea, i8 nDataBytes);
    bool  bNextRecord(Stz** aszRecord, i8& nRecordBytes);
  
  protected:
    bool  bFindRecordEnd(char** aazRecordEnd);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
