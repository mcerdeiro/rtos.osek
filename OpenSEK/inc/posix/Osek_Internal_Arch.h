/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of OpenSEK.
 *
 * OpenSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *             
 * Linking OpenSEK statically or dynamically with other modules is making a
 * combined work based on OpenSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of OpenSEK give
 * you permission to combine OpenSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with OpenSEK solely through the OpenSEK defined interface. 
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for OpenSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of OpenSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 * 
 * OpenSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _OSEK_INTERNAL_ARCH_H_
#define _OSEK_INTERNAL_ARCH_H_
/** \brief OpenSEK Internal Architecture Dependent Header File
 **
 ** \file posix/Osek_Internal_Arch.h
 ** \arch posix
 **/

/** \addtogroup OpenSEK
 ** @{ */
/** \addtogroup OpenSEK_Internal
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.1 MaCe remove std type definitions, now in Types.h
 * 20080725 v0.1.0 MaCe	initial version
 */  

/*==================[inclusions]=============================================*/
#include "ucontext.h"	/* used to task changes */
#include "mqueue.h"		/* used to simulate interrupts */
#include "errno.h"		/* used to read errno */
#include "stdio.h"		/* used to print debug information */
#include "signal.h"		/* used to simulate interrupts */
#include "unistd.h"		/* used to create a fork to poll the interrupts */
#include "stdlib.h"		/* used to call exit to terminate the process */
#include "time.h"			/* used to simulate the hardware timer */

/*==================[macros]=================================================*/
/** \brief Interrupt Secure Start Macro
 **
 ** This macro shall be used to disable the interrupts
 **/
#define IntSecure_Start() SuspendAllInterrupts()

/** \brief Interrupt Secure End Macro
 **
 ** This macro shall be used to restore the interrupts
 **/
#define IntSecure_End() ResumeAllInterrupts()

/** \brief osekpause
 **
 **/
#define osekpause()			\
	{								\
		PreCallService();		\
	 	(void)usleep(1); 		\
		PostCallService();	\
	}

/** \brief Call to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#define CallTask(task)															\
	{																					\
		uint8 jmp = 1;																\
		(void)getcontext(TasksConst[GetRunningTask()].TaskContext);	\
		jmp--;																		\
		if (jmp == 0)																\
		{																				\
			/* set as running task */											\
			SetRunningTask(task);												\
			(void)setcontext(TasksConst[(task)].TaskContext);			\
		}																				\
	}

/** \brief Jmp to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#define JmpTask(task)			\
	{									\
		PreCallService();			\
		(void)setcontext(TasksConst[(task)].TaskContext);	\
	}

/** \brief Save context */
#define SaveContext(task) 		\
	{									\
		PreCallService();			\
		(void)getcontext(TasksConst[(task)].TaskContext);	\
		PostCallService();		\
	}

/** \brief Set the entry point for a task */
#define SetEntryPoint(task)	\
	{									\
		PreCallService();			\
		(void)makecontext(TasksConst[(task)].TaskContext, TasksConst[(task)].EntryPoint, 0);	\
		PostCallService();		\
	}

/** \brief */
#define ResetStack(task)																													\
	{																																				\
		TasksConst[loopi].TaskContext->uc_stack.ss_sp = TasksConst[loopi].StackPtr;      /* set stack pointer */	\
		TasksConst[loopi].TaskContext->uc_stack.ss_size = TasksConst[loopi].StackSize;   /* set stack size */		\
		TasksConst[loopi].TaskContext->uc_stack.ss_flags = 0;																		\
   }

#define ISR_NMI      0
#define ISR_CTR      1
#define ISR_CANRX    2
#define ISR_CANTX    3

#define EnableOSInterrupts()															\
	{																							\
		InterruptMask &= (InterruptFlagsType)~(OSEK_OS_INTERRUPT_MASK);	\
	}

#define EnableInterrupts()		\
	{									\
		InterruptState = 1;		\
	}

/** \brief Get Counter Actual Value
 **
 ** This macro returns the actual value of the counter
 **
 ** \param[in] CounterID id of the counter to be readed
 ** \return Actual value of the counter
 **/
#define GetCounter_Arch(CounterID) (CountersVar[CounterID].Time)

/** \brief Pre ISR Macro
 **
 ** This macro is called every time that an ISR Cat 2 is started
 **/
#define PreIsr2_Arch(isr)

/** \brief Post ISR Macro
 **
 ** This macro is called every time that an ISR Cat 2 is finished
 **/
#define PostIsr2_Arch(isr)

#define SavePosixStack() \
   {                                   \
      /* save actual win esp */        \
      __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (PosixStack) : : "eax"); \
   }

/** \brief Pre Call Service
 **
 ** This macro shall be called before calling any posix system service
 **/
#define PreCallService() 		\
	{									\
		/* save osek stack */	\
		__asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (OsekStack) : : "eax"); \
		/* get windows stack */	\
		__asm__ __volatile__ ("movl %0, %%esp;" : : "g" (PosixStack) ); \
	}

/** \brief Post Call Service
 **
 ** This macro shall be called after calling any posix system service
 **/
#define PostCallService()		\
	{									\
		/* get windows stack */ \
		__asm__ __volatile__ ("movl %0, %%esp;" : : "g" (OsekStack) ); \
	}


/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
/** \brief Interrupt Falg
 **
 ** This variable indicate the state of the posix interrupts. If bit 0 is set
 ** interrupt 0 has been activated, if bit 1 is set interrupt 1 has been
 ** activated, and so on.
 **/
extern InterruptFlagsType InterruptFlag;

/** \brief Message Queue
 **/
extern mqd_t MessageQueue;

/** \brief Message Queue Attributes
 **/
extern struct mq_attr MessageQueueAttr;

/** \brief Message Signal
 **/
extern struct sigaction MessageSignal;

/** \brief Kill Signal
 **/
extern struct sigaction KillSignal;

/** \brief Signal Event
 **/
extern struct sigevent SignalEvent;

/** \brief Osek Hardware Timer 0
 **/
extern uint32 OsekHWTimer0;

extern uint32 PosixStack;

extern uint32 OsekStack;

/*==================[external functions declaration]=========================*/
/** \brief Posix Interrupt Handler
 **
 ** This function is called every time when a interrupt message is received.
 **/
extern void PosixInterruptHandler(int status);

extern void HWTimerFork(uint8 timer);

extern void OsekKillSigHandler(int status);

extern void OSEK_ISR_HWTimer0(void);

extern void OSEK_ISR_HWTimer1(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OSEK_INTERNAL_ARCH_H_ */

