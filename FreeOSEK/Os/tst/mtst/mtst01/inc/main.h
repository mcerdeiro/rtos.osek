/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of FreeOSEK.
 *
 * FreeOSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *             
 * Linking FreeOSEK statically or dynamically with other modules is making a
 * combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of FreeOSEK give
 * you permission to combine FreeOSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with FreeOSEK solely through the FreeOSEK defined interface. 
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for FreeOSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of FreeOSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 * 
 * FreeOSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _MAIN_H_
#define _MAIN_H_
/** \brief Module Test Header File
 **
 ** \file mtst01/inc/main.h
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_MT Module Tests
 ** @{ */
/** \addtogroup FreeOSEK_Os_MT_01 Module Test 1
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.1 MaCe add MAIN_MEMMAP check
 * 20081205 v0.1.0 MaCe	initial version
 */  

/*==================[inclusions]=============================================*/
#include "Types.h"

/*==================[macros]=================================================*/
#define MAIN_MEMMAP DISABLE

#define INVALID_TASK 0xFE

#define INVALID_RESOURCE 0xFE

#define INVALID_ALARM 0xFF

#define mt_GetMark() (mt_StateCounter)

#define mt_SetMark(mark)                                                   \
   {                                                                       \
      if ( (mark) == 0 )                                                   \
      {                                                                    \
         mt_StateCounter = 0;                                              \
      }                                                                    \
      else if ( (mt_StateCounter+1) == (mark) )                            \
      {                                                                    \
         mt_StateCounter++;                                                \
      }                                                                    \
      else                                                                 \
      {                                                                    \
      }                                                                    \
   }

#define mt_SetTaskAssert(state) (mt_TaskAssert = (state) )

#define mt_GetTaskAssert() (mt_TaskAssert)

#define TASKASSERT(cond)                                                                              \
   {                                                                                                  \
      if (cond)                                                                                       \
      {                                                                                               \
         ASSERT_ARCH();                                                                               \
         mt_TaskAssert = TS_FAILED;                                                                   \
      }                                                                                               \
   }

#define mt_SetTestCase(testcase) (mt_TestCase = (testcase))

#define mt_GetTestCase() (mt_TestCase)

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
#if (MAIN_MEMMAP == ENABLE)
#define TestSuite_START_SEC_CODE
#include "MemMap.h"
#endif

/** \brief main function
 **
 ** Project main function. This function is called after the c conformance
 ** initialisation. This function shall call the StartOS in the right
 ** Application Mode. The StartOS API shall never return.
 **
 **/
int main
(
   void
) ATTRIBUTES();

#if (MAIN_MEMMAP == ENABLE)
#define TestSuite_STOP_SEC_CODE
#include "MemMap.h"
#endif

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _MAIN_H_ */

