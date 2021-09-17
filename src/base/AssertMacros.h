/*============================================================================*/
/* AssertMacros.hpp -- Assert Macro Definitions                               */
/*============================================================================*/
#pragma once

/*----------------------------------------------------------------------------*/
/* explicitly ignore the return value                                         */
/*----------------------------------------------------------------------------*/
#define IGNORE_RETURN(call) call EndDefine

/*----------------------------------------------------------------------------*/
/* check the object is within the pool                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_CORRECTNESS_ALGEBRA
  #define _poolAddress_(pArg) BASE::vCheckAddress(pArg,__func__) EndDefine
#else
  #define _poolAddress_(pArg) (void)pArg EndDefine
#endif


/*----------------------------------------------------------------------------*/
/* ALWAYS macro          ** USE CAREFULLY **    mainly in vCheckAssumptions() */
/*----------------------------------------------------------------------------*/
#define ALWAYS(condition) \
  if (! (condition)) \
    BASE::vThrowAlways(FK_ALWAYS,\
                       __func__,__FILE__,__LINE__,\
                      (Stz*)#condition); \

/*----------------------------------------------------------------------------*/
/* ASSERT macro                                                               */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ASSERT_MACRO
  #define ASSERT(condition) \
    if (! (condition)) \
      BASE::vThrowAssert(FK_ASSERT,\
                         __func__,__FILE__,__LINE__,\
                         (Stz*)#condition); \
EndDefine
#else
  #define ASSERT(condition) EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached             */
/*----------------------------------------------------------------------------*/
#define NEVER_REACH \
  BASE::vThrowReached(FK_REACHED,\
                     __func__,__FILE__,__LINE__, \
                     "NEVER_REACH point in code was reached"_Stz); \
EndDefine

/*----------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached             */
/*----------------------------------------------------------------------------*/
#define NOT_IMPLEMENTED_YET \
  BASE::vThrowUnimplemented(FK_UNIMPLEMENTED,\
                            __func__,__FILE__,__LINE__, \
                            "NOT_IMPLEMENTED_YET block of code hit"_Stz); \
EndDefine

/*----------------------------------------------------------------------------*/
/* RASSERT macro                                                              */
/*----------------------------------------------------------------------------*/
#define RASSERT(condition) \
  if (! (condition)) \
    BASE::vThrowAssert(FK_RASSERT,\
                      __func__,__FILE__,__LINE__,\
                      (Stz*)#condition); \
EndDefine

/*----------------------------------------------------------------------------*/
/* RASSERT macro                                                              */
/*----------------------------------------------------------------------------*/
#define RASSERT_FATAL(condition) \
  if (! (condition)) \
    BASE::vThrowAssert(FK_RASSERT,\
                      __func__,__FILE__,__LINE__,\
                      (Stz*)#condition); \
EndDefine

/*----------------------------------------------------------------------------*/
/* RASSERT_TYPE macro                                                         */
/*----------------------------------------------------------------------------*/
#define RASSERT_TYPE(pOBJECT,eCLASS) \
  if (pOBJECT && eCLASS != pOBJECT->eGetClassNo()) \
    BASE::vThrowType(FK_TYPE,\
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,eCLASS); \
EndDefine

/*----------------------------------------------------------------------------*/
/* THROW macro                                                                */
/*----------------------------------------------------------------------------*/
#define THROW(szDescription) \
  BASE::vThrow(FK_THROW,\
               __func__,__FILE__,__LINE__,\
               szDescription); \
EndDefine

/*----------------------------------------------------------------------------*/
/* THROW_LOWMEMORY macro                                                      */
/*----------------------------------------------------------------------------*/
#define THROW_LOWMEMORY(void) \
  BASE::vThrowLowMemory(FK_LOWMEMORY,\
                        __func__,__FILE__,__LINE__,\
                        "RUNNING LOW ON MEMORY"_Stz); \
EndDefine

/*----------------------------------------------------------------------------*/
/* THROW_EXCEPTION macro                                                      */
/*----------------------------------------------------------------------------*/
#define THROW_USER(szDescription) \
  BASE::vThrowUser(FK_THROW,\
                   __func__,__FILE__,__LINE__,\
                   szDescription); \
EndDefine

/*----------------------------------------------------------------------------*/
/* TO_BE_REMOVED macro (code is being removed; should never be reached)       */
/*----------------------------------------------------------------------------*/
#define TO_BE_REMOVED \
  BASE::vThrowReached(FK_REACHED,\
                     __func__,__FILE__,__LINE__, \
                     "TO_BE_REMOVED point in code was reached"_Stz); \
EndDefine


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
