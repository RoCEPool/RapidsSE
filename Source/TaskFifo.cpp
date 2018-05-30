/*============================================================================*/
/* TaskFifo.cpp - task fifo                                                   */
/*============================================================================*/

#include "Tasks.h"


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskFifo* Tasks::_pTaskFifo(void* aAddr)
{
  TaskFifo* pTaskFifo = reinterpret_cast<TaskFifo*>(aAddr);
  
  RASSERT( !pTaskFifo || CN_TASKFIFO == pTaskFifo->eClass() );
  
  return pTaskFifo;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskFifo::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_TASKFIFO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskFifo::operator new(size_t iSize, TaskFifo* pTaskFifo)
{
  RASSERT(iSize);
  return pTaskFifo;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskFifo::operator delete(void* pTaskFifoAddress)
{
  TaskFifo* pTaskFifo = reinterpret_cast<TaskFifo*>(pTaskFifoAddress);
  
  Heap::vFree(pTaskFifo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskFifo::TaskFifo(void)
: Object(CN_TASKFIFO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKFIFO,_FOOTER_);
  
  _checkInitialised_(CN_TASKFIFO);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKFIFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskFifo::~TaskFifo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKFIFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskFifo::vContentDump(void)
{
  vDump(CN_TASKFIFO, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskFifo::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_TASKFIFO);
  RASSERT(pObject->eClass() == CN_TASKFIFO);
  
  TaskFifo* pTaskFifo = reinterpret_cast<TaskFifo*>(pObject);
  
  vTrace("\n"_Stz);
  //pTaskQueue->List_t<Task>::vContentDump("TaskQueue::"_Stz, aTab);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TaskFifo::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKFIFO);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
