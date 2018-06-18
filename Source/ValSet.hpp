/*============================================================================*/
/* ValSet.hpp - set of values                                                 */
/*============================================================================*/
#pragma once

namespace DBobjects {
  class EntryBuffer;
  class WorkingBuffer;
}

namespace JobModel {
  class Job;
}

namespace Values
{
  class ValSet : public Value
  {
    static const i8 C_VALSET_DATAAREA = C_1K * 63; // has to fit inside ValWrk
    
  private:
    i8  m_iBytes; // bytes used in work space
    DBobjects::EntryBuffer*   m_pRowBuffer;
    i8  m_iRowBufferBytes;
    
    JobModel::Job* m_pJob;
    
    DBobjects::WorkingBuffer* m_pWorkingBuffer;
    i8  m_iWorkingBufferBytes;
    
    char m_wWorkspace[C_VALSET_DATAAREA];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    ValSet(void);
    ~ValSet(void);

    void vAppend(Stz*& pCursor, Value* pValue);
    bool bExtractValue(Stz*& pCursor, Value*& pValue);
    bool bExtractValue(Stz*& pCursor, eCN& eClassNo, Value*& pValue);
    JobModel::Job* pGetJob();
    DBobjects::EntryBuffer* pGetRowBuffer();
    DBobjects::WorkingBuffer* pGetWorkingBuffer();
    bool bHasJob(void);
    bool bHasRowBuffer(void);
    bool bHasWorkingBuffer(void);
    DBobjects::EntryBuffer* pRowBuffer(void);
    void vRowBuffer(DBobjects::EntryBuffer* pRowBuffer);
    bool bSetJob(JobModel::Job* pJob);
    bool bSetRowBuffer(DBobjects::EntryBuffer* pRowBuffer);
    bool bSetWorkingBuffer(DBobjects::WorkingBuffer* pWorkingBuffer);
    i8   nSize(void);
    void vToString(Stz* aWorkspace);
    DBobjects::WorkingBuffer* pWorkingBuffer(void);
    void vWorkingBuffer(DBobjects::WorkingBuffer* pWorkingBuffer);

  private:
    Stz* aFirstByte(void);
    Stz* aLastByte(void);
    ValWrk* pToValWrk(Stz* pCursor);
  };
  
  ValSet* _pValSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
