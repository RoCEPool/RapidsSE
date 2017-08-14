/*============================================================================*/
/* Scheduler.hpp - schedule tasks to threads                                  */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread;
}

namespace Schedulers
{
  class Scheduler : public Object
  {
  public:
    Scheduler(eCN eClassNo);
    ~Scheduler(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
