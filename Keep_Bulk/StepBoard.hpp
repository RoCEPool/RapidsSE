/*============================================================================*/
/* StepBoard.hpp -- Step Board                                                */
/*============================================================================*/
#pragma once

namespace Tasks
{
  static const i8 C_STEPBOARD_THREADS = 64;
  static const i8 C_STEPBOARD_NAME    = 64;
  
  class StepBoard
  {
  public:
    i8   m_iThreads; // how many threads
    s256 m_sStepName[C_STEPBOARD_THREADS];  // last step name
    i8   m_iStepPoint[C_STEPBOARD_THREADS]; // last step point
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StepBoard(i8 iThreads);
    ~StepBoard(void);
    
    void vContentDump(void);
    void vName(i8 iThread, Str* aName);
    void vPoint(i8 iThread, i8 iPoint);
    void vReset(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
