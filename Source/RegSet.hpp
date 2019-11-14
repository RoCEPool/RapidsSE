/*============================================================================*/
/* RegSet.hpp -- Task Register Set                                            */
/*============================================================================*/
#pragma once

namespace Registers
{
  class RegSet : public Object
  {
  friend class Tasks::Task;
  
  public:
    Task* m_pTask; // which task
  
    eLH       m_eDdlLocked; // this task has the ddl lock?
    i8        m_i_NextRnd;  // which next repeatable random nummber?
    Node*     m_pSendNode;  // node to send to
  
    REF<Job>  m_xpJobRef;   // job reference
    REF<Node> m_xpNodeRef;  // node reference

    REG<Dir>  m_xpDir0;     // dir0[]
    REG<Dir>  m_xpDir1;     // dir1[]
    REG<Dir>  m_xpDir2;     // dir2[]
    REG<File> m_xpFile0;    // file0[]
    REG<File> m_xpFile1;    // file1[]
    REG<File> m_xpFile2;    // file2[]

    // pipeline members
    Pipeline*   m_pPipeline;      // pipeline[]
    Dynamo*     m_pDynamo;        // dynamo[]
    StreamDef*  m_pStreamDef;     // streamDef[]
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RegSet(Task* pTask);
   ~RegSet(void) noexcept(false);
  
    void vContentDump(void);

  private:
    Footer* aFooter(void);
  };
  
  RegSet* _pRegSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
