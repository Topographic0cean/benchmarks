/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */

#include "dhry.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

/* Global Variables: */

Rec_Pointer Ptr_Glob,
    Next_Ptr_Glob;
int Int_Glob;
Boolean Bool_Glob;
char Ch_1_Glob,
    Ch_2_Glob;
int Arr_1_Glob[50];
int Arr_2_Glob[50][50];

FILE *output;
void Proc_1(
    REG Rec_Pointer Ptr_Val_Par);
void Proc_2( One_Fifty *Int_Par_Ref);
void Proc_3( Rec_Pointer *Ptr_Ref_Par);
void Proc_4();
void Proc_5();
extern void Proc_6(Enumeration Enum_Val_Par, Enumeration* Enum_Ref_Par);
extern void Proc_7(
    One_Fifty Int_1_Par_Val,
    One_Fifty Int_2_Par_Val,
    One_Fifty *Int_Par_Ref);
extern void Proc_8(
    Arr_1_Dim Arr_1_Par_Ref,
    Arr_2_Dim Arr_2_Par_Ref,
    int Int_1_Par_Val,
    int Int_2_Par_Val);
Boolean Func_2(Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);
Boolean Func_3(Enumeration Enum_Par_Val);

extern Enumeration Func_1(
    Capital_Letter Ch_1_Par_Val,
    Capital_Letter Ch_2_Par_Val);
/* forward declaration necessary since Enumeration may not simply be int */

#ifndef REG
Boolean Reg = false;
#define REG
/* REG becomes defined as empty */
/* i.e. no register variables   */
#else
Boolean Reg = true;
#endif

/* variables for time measurement: */

#ifdef TIMES
struct tms time_info;
/* see library function "times" */
#define Too_Small_Time 120
/* Measurements should last at least about 2 seconds */
#endif
#ifdef TIME
/* see library function "time"  */
#define Too_Small_Time 2
/* Measurements should last at least 2 seconds */
#endif

long Begin_Time,
    End_Time,
    User_Time;
float Microseconds,
    Dhrystones_Per_Second;

/* end of variables for time measurement */

void run(int Number_Of_Runs, int proc)
{
  One_Fifty Int_1_Loc;
  REG One_Fifty Int_2_Loc;
  One_Fifty Int_3_Loc;
  REG char Ch_Index;
  Enumeration Enum_Loc;
  Str_30 Str_1_Loc;
  Str_30 Str_2_Loc;
  REG int Run_Index;
  char filename[32];

  /* Initializations */

  sprintf(filename, "dhry%d.log", proc);
  output = fopen(filename, "w");
  if (!output)
    exit(1);

  Next_Ptr_Glob = (Rec_Pointer)malloc(sizeof(Rec_Type));
  Ptr_Glob = (Rec_Pointer)malloc(sizeof(Rec_Type));

  Ptr_Glob->Ptr_Comp = Next_Ptr_Glob;
  Ptr_Glob->Discr = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp = 40;
  strcpy(Ptr_Glob->variant.var_1.Str_Comp,
         "DHRYSTONE PROGRAM, SOME STRING");
  strcpy(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob[8][7] = 10;
  /* Was missing in published program. Without this statement,    */
  /* Arr_2_Glob [8][7] would have an undefined value.             */
  /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
  /* overflow may occur for this array element.                   */

  fprintf(output, "\n");
  fprintf(output, "Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  fprintf(output, "\n");
  if (Reg)
  {
    fprintf(output, "Program compiled with 'register' attribute\n");
    fprintf(output, "\n");
  }
  else
  {
    fprintf(output, "Program compiled without 'register' attribute\n");
    fprintf(output, "\n");
  }

  fprintf(output, "Execution starts, %d runs through Dhrystone\n", Number_Of_Runs);

  /***************/
  /* Start timer */
  /***************/

#ifdef TIMES
  times(&time_info);
  Begin_Time = (long)time_info.tms_utime;
#endif
#ifdef TIME
  Begin_Time = time(0);
#endif

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {

    Proc_5();
    Proc_4();
    /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = !Func_2(Str_1_Loc, Str_2_Loc);
    /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc) /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
      /* Int_3_Loc == 7 */
      Proc_7(Int_1_Loc, Int_2_Loc, &Int_3_Loc);
      /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
    /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8(Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
    /* Int_Glob == 5 */
    Proc_1(Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
    /* loop body executed twice */
    {
      if (Enum_Loc == Func_1(Ch_Index, 'C'))
      /* then, not executed */
      {
        Proc_6(Ident_1, &Enum_Loc);
        strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
      }
    }
    /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
    /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2(&Int_1_Loc);
    /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/

#ifdef TIMES
  times(&time_info);
  End_Time = (long)time_info.tms_utime;
#endif
#ifdef TIME
  End_Time = time((long *)0);
#endif

  fprintf(output, "Execution ends\n");
  fprintf(output, "\n");
  fprintf(output, "Final values of the variables used in the benchmark:\n");
  fprintf(output, "\n");
  fprintf(output, "Int_Glob:            %d\n", Int_Glob);
  fprintf(output, "        should be:   %d\n", 5);
  fprintf(output, "Bool_Glob:           %d\n", Bool_Glob);
  fprintf(output, "        should be:   %d\n", 1);
  fprintf(output, "Ch_1_Glob:           %c\n", Ch_1_Glob);
  fprintf(output, "        should be:   %c\n", 'A');
  fprintf(output, "Ch_2_Glob:           %c\n", Ch_2_Glob);
  fprintf(output, "        should be:   %c\n", 'B');
  fprintf(output, "Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  fprintf(output, "        should be:   %d\n", 7);
  fprintf(output, "Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  fprintf(output, "        should be:   Number_Of_Runs + 10\n");
  fprintf(output, "Ptr_Glob->\n");
  fprintf(output, "  Ptr_Comp:          %p\n", Ptr_Glob->Ptr_Comp);
  fprintf(output, "        should be:   (implementation-dependent)\n");
  fprintf(output, "  Discr:             %d\n", Ptr_Glob->Discr);
  fprintf(output, "        should be:   %d\n", 0);
  fprintf(output, "  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  fprintf(output, "        should be:   %d\n", 2);
  fprintf(output, "  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  fprintf(output, "        should be:   %d\n", 17);
  fprintf(output, "  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  fprintf(output, "        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  fprintf(output, "Next_Ptr_Glob->\n");
  fprintf(output, "  Ptr_Comp:          %p\n", Next_Ptr_Glob->Ptr_Comp);
  fprintf(output, "        should be:   (implementation-dependent), same as above\n");
  fprintf(output, "  Discr:             %d\n", Next_Ptr_Glob->Discr);
  fprintf(output, "        should be:   %d\n", 0);
  fprintf(output, "  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  fprintf(output, "        should be:   %d\n", 1);
  fprintf(output, "  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  fprintf(output, "        should be:   %d\n", 18);
  fprintf(output, "  Str_Comp:          %s\n",
          Next_Ptr_Glob->variant.var_1.Str_Comp);
  fprintf(output, "        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  fprintf(output, "Int_1_Loc:           %d\n", Int_1_Loc);
  fprintf(output, "        should be:   %d\n", 5);
  fprintf(output, "Int_2_Loc:           %d\n", Int_2_Loc);
  fprintf(output, "        should be:   %d\n", 13);
  fprintf(output, "Int_3_Loc:           %d\n", Int_3_Loc);
  fprintf(output, "        should be:   %d\n", 7);
  fprintf(output, "Enum_Loc:            %d\n", Enum_Loc);
  fprintf(output, "        should be:   %d\n", 1);
  fprintf(output, "Str_1_Loc:           %s\n", Str_1_Loc);
  fprintf(output, "        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  fprintf(output, "Str_2_Loc:           %s\n", Str_2_Loc);
  fprintf(output, "        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  fprintf(output, "\n");

  User_Time = End_Time - Begin_Time;

  if (User_Time < Too_Small_Time)
  {
    fprintf(output, "Measured time too small to obtain meaningful results\n");
    fprintf(output, "Please increase number of runs\n");
    fprintf(output, "\n");
  }
  else
  {
#ifdef TIME
    Microseconds = (float)User_Time * Mic_secs_Per_Second / (float)Number_Of_Runs;
    Dhrystones_Per_Second = (float)Number_Of_Runs / (float)User_Time;
#else
    Microseconds = (float)User_Time * Mic_secs_Per_Second / ((float)HZ * ((float)Number_Of_Runs));
    Dhrystones_Per_Second = ((float)HZ * (float)Number_Of_Runs) / (float)User_Time;
#endif
    fprintf(output, "Microseconds for one run through Dhrystone: ");
    fprintf(output, "%6.1f \n", Microseconds);
    fprintf(output, "Dhrystones per Second:                      ");
    fprintf(output, "%6.1f \n", Dhrystones_Per_Second);
    fprintf(output, "\n");
  }

  fclose(output);
}

void Proc_1(
    REG Rec_Pointer Ptr_Val_Par)
    /******************/

/* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
  /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign(*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3(&Next_Record->Ptr_Comp);
  /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                      == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
  /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6(Ptr_Val_Par->variant.var_1.Enum_Comp,
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7(Next_Record->variant.var_1.Int_Comp, 10,
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign(*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */

void Proc_2( One_Fifty *Int_Par_Ref)
    /******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

{
  One_Fifty Int_Loc;
  Enumeration Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
    /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    }                          /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */

void Proc_3( Rec_Pointer *Ptr_Ref_Par)
    /******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */


{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7(10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */

void Proc_4() /* without parameters */
/*******/
/* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */

void Proc_5() /* without parameters */
/*******/
/* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */

/* Procedure for the assignment of structures,          */
/* if the C compiler doesn't support this feature       */
#ifdef NOSTRUCTASSIGN
memcpy(d, s, l) register char *d;
register char *s;
register int l;
{
  while (l--)
    *d++ = *s++;
}
#endif

int main(int argc, char *argv[])
{
  int processes = 1;
  int proc;
  int pids[9];

  if (argc < 2)
  {
    printf("Please specify the number of loops\n");
    exit(1);
  }
  if (argc > 2)
  {
    processes = atoi(argv[2]);
    if (processes > 8)
      processes = 8;
  }
  for (proc = 1; proc <= processes; proc++)
  {
    pids[proc] = fork();
    if (!pids[proc])
    {
      run(atoi(argv[1]), proc);
      exit(0);
    }
  }
  for (proc = 1; proc <= processes; proc++)
  {
    waitpid(pids[proc], 0, 0);
  }
  exit(0);
}
