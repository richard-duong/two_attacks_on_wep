#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* exit_on_fail
 * ========================================================================
 * Objective:
 * A way to standardize debugging by exiting and printing
 * on failure of execution
 *
 * Input:
 * char* name     : name of client/server
 * char* issue    : which connection method to check
 * int validity   : value to compare for pass/fail
 *
 * Output:
 * None
 *
 * Result:
 * If validity < 0    :  Print failure and exit
 * If validity >= 0   :  Continue quietly
 */

void exit_on_fail(char* name, char* issue, int validity){
  if(validity < 0){
    printf("%s - %s has detected a failure\n", name, issue);
    exit(1);
  }
}



/* verbose_exit_on_fail
 * ========================================================================
 * Objective:
 * A more verbose version of exit_on_fail(char*, char*, int) that 
 * prints and exits on failure but also prints and continues on success
 *
 *
 * Input:
 * char* name     : name of client/server
 * char* issue    : which connection method to check
 * int validity   : value to compare for pass/fail
 *
 * Output:
 * None
 *
 * Result:
 * If validity < 0    :  Print failure and exit
 * If validity >= 0   :  Print success and continue
 */

void verbose_exit_on_fail(char* name, char* issue, int validity){
  if(validity < 0){
    printf("%s - %s has detected a failure\n", name, issue);
    exit(1);
  } 
  else{
    printf("%s - %s has ran successfully\n", name, issue);
  }
}


