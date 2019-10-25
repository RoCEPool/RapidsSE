/*============================================================================*/
/* RegSet.hpp -- Task RegSet                                                  */
/*============================================================================*/
#pragma once

namespace Registers
{
  class RegSet : public Object
  {
  friend class Tasks::Task;
  
  public:
    Task* m_pTask;  // which task
  
    i8    m_i_NextRnd;    // which next repeatable random nummber?

    Node* m_pSendNode;    // node to send to
  
    REG<File> m_rpFile;   // file

    OperandQueue* m_pOpQueLeft;
    OperandQueue* m_pOpQueRight;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RegSet(Task* pTask);
   ~RegSet(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  RegSet* _pRegSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
