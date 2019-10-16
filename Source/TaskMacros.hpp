/*============================================================================*/
/* TaskMacros.hpp - task macros                                               */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// variable argument throws

#define GET_THROW(_1,_2,_3,_4,NAME,...) NAME

#define THROW(...) \
  GET_THROW(__VA_ARGS__, THROW4, THROW3, THROW2, THROW1)(__VA_ARGS__)

#define THROW2(p1, p2) \
{ \
  s256 _sException; \
  _sException.vSprint(p2); \
  Task* pTask = Task::pThisTLSTask(); \
  pTask->vException(_sException,(Stz*)__FILE__,__LINE__); \
  Correctness::vThrowTrap(); \
  throw p1; \
}

#define THROW3(p1, p2, p3) \
{ \
  s256 _sException; \
  _sException.vSprint(p2,p3); \
  Task* pTask = Task::pThisTLSTask(); \
  pTask->vException(_sException,(Stz*)__FILE__,__LINE__); \
  Correctness::vThrowTrap(); \
  throw p1; \
}

#define THROW4(p1, p2, p3, p4) \
{ \
  s256 _sException; \
  _sException.vSprint(p2,p3,p4); \
  Task* pTask = Task::pThisTLSTask(); \
  pTask->vException(_sException,(Stz*)__FILE__,__LINE__); \
  Correctness::vThrowTrap(); \
  throw p1; \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// throw invalid auxiliary stack

#define THROW_INVALID_ASTACK(primitive) \
{ \
  THROW(FK_ASSERT,"Invalid aux stack for: %s"_Stz,primitive); \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// throw invalid data stack

#define THROW_INVALID_DSTACK(primitive) \
{ \
  THROW(FK_ASSERT,"Invalid data stack in for: %s"_Stz,primitive); \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// throw invalid return stack

#define THROW_INVALID_RSTACK(primitive) \
{ \
  THROW(FK_ASSERT,"Invalid return stack for: %s"_Stz,primitive); \
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
