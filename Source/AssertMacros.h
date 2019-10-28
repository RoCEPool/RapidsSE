/*============================================================================*/
/* AssertMacros.hpp -- Assert Macro Definitions                               */
/*============================================================================*/
#pragma once

#include "eVL.h"

/*----------------------------------------------------------------------------*/
/* explicitly ignore the return value                                         */
/*----------------------------------------------------------------------------*/
#define IGNORE_RETURN(call) call

/*----------------------------------------------------------------------------*/
/* check the object is within the pool                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ADDRESS_CHECKING
  #define _poolAddress_(pArg) Correctness::vCheckAddress(pArg)
#else
  #define _poolAddress_(pArg)
#endif

/*----------------------------------------------------------------------------*/
/* ASSERT macro                                                               */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ASSERT_MACRO
  #define ASSERT(condition) \
    if (! (condition)) \
      Correctness::vThrow(FK_ASSERT,VL_FATAL,\
                          __func__,__FILE__,__LINE__,\
                          (Stz*)#condition);
#else
  #define ASSERT(condition)
#endif

/*----------------------------------------------------------------------------*/
/* RASSERT macro                                                              */
/*----------------------------------------------------------------------------*/
#define RASSERT(condition) \
  if (! (condition)) \
    Correctness::vThrow(FK_RASSERT,VL_FATAL,\
                        __func__,__FILE__,__LINE__,\
                        (Stz*)#condition);

/*----------------------------------------------------------------------------*/
/* RASSERT_TYPE macro                                                         */
/*----------------------------------------------------------------------------*/
#define RASSERT_TYPE(pOBJECT,eCLASS) \
  if (pOBJECT && eCLASS != pOBJECT->eClass()) \
    Correctness::vThrowType(FK_TYPE,VL_FATAL,\
                            __func__,__FILE__,__LINE__,\
                            pOBJECT,eCLASS);

/*----------------------------------------------------------------------------*/
/* THROW macro                                                                */
/*----------------------------------------------------------------------------*/
#define THROW(szDescription) \
  Correctness::vThrow(FK_THROW,VL_FATAL,\
                      __func__,__FILE__,__LINE__,\
                      szDescription);

/*----------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached             */
/*----------------------------------------------------------------------------*/
#define NEVER_REACH \
  Correctness::vThrow(FK_REACHED,VL_FATAL,\
                      __func__,__FILE__,__LINE__, \
                      (Stz*)"NEVER_REACH point reached");


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
