/*============================================================================*/
/* RecordBuffer.hpp -- Working Buffer                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  const int C_SIZEOF_RECORDSIZE = 4;
  
  class RecordBuffer : public Object
  {
  protected:
    i8   m_nRecords;                            // number of records in this buffer
    i8   m_nFreeBytes;                          // free space
    char m_cArea[C_SIZE_LARGE_OBJECT - C_1KB];  // record area

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    RecordBuffer(void);
   ~RecordBuffer(void) noexcept(false);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  public:
    bool  bAppendRecord(char* aSrcRecord, i8 nRecordSize);
    char* aFirstRecord(void);
    void  vInit(void);
    char* aNextRecord(char* aCurrentRecord);
    i8    nRecords(void);
    void  vShow(void);
    i8    nUsedBytes(void);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  private:
    char* aAllotSpace(i8 nBytes);
    i8    nFreeBytes(void);
  
    Footer* aFooter(void);
  };
  
  RecordBuffer* _pRecordBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
