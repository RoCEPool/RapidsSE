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

    REG<File> m_xpFile0;    // file0[]
    REG<File> m_xpFile1;    // file1[]

    REG<Row>      m_xpRow;
    REG<Operand>  m_xpSubset;
    REG<Table>    m_xpTable;
    REG<TablePtn> m_xpTablePtn;
    
    // pipeline members
    Dynamo*   m_pDynamo;    // dynamo pipleine compiler
  
    Operand*  m_pOperand;   // operand
    
    REG<Object> m_xpObject0; // general purpose reg 0[]
    REG<Object> m_xpObject1; // general purpose reg 1[]
    REG<Object> m_xpObject2; // general purpose reg 2[]
    REG<Object> m_xpObject3; // general purpose reg 3[]

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
