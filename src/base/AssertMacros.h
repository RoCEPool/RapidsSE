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
    BASE::vThrow(FK_ASSERT,\
                        __func__,__FILE__,__LINE__,\
                        (Stz*)#condition); \

/*----------------------------------------------------------------------------*/
/* ASSERT macro                                                               */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ASSERT_MACRO
  #define ASSERT(condition) \
    if (! (condition)) \
      BASE::vThrow(FK_ASSERT,\
                          __func__,__FILE__,__LINE__,\
                          (Stz*)#condition); \
EndDefine
#else
  #define ASSERT(condition) EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* RASSERT macro                                                              */
/*----------------------------------------------------------------------------*/
#define RASSERT(condition) \
  if (! (condition)) \
    BASE::vThrow(FK_RASSERT,\
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
/* THROW_EXCEPTION macro                                                      */
/*----------------------------------------------------------------------------*/
#define THROW_USER(szDescription) \
  BASE::vThrowUser(FK_THROW,\
                          __func__,__FILE__,__LINE__,\
                         szDescription); \
EndDefine

/*----------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached             */
/*----------------------------------------------------------------------------*/
#define NEVER_REACH \
  BASE::vThrow(FK_REACHED,\
                      __func__,__FILE__,__LINE__, \
                      (Stz*)"NEVER_REACH point reached"); \
EndDefine

/*----------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached             */
/*----------------------------------------------------------------------------*/
#define NOT_IMPLEMENTED_YET \
  BASE::vThrow(FK_REACHED,\
                      __func__,__FILE__,__LINE__, \
                      (Stz*)"NOT_IMPLEMENTED_YET point reached"); \
EndDefine


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
