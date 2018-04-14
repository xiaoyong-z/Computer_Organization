#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <CUnit/Basic.h>
#include "beargit.h"
#include "util.h"

/* printf/fprintf calls in this tester will NOT go to file. */

#undef printf
#undef fprintf

/* The suite initialization function.
 * You'll probably want to delete any leftover files in .beargit from previous
 * tests, along with the .beargit directory itself.
 *
 * You'll most likely be able to share this across suites.
 */
int init_suite(void) {
    // preps to run tests by deleting the .beargit directory if it exists
    fs_force_rm_beargit_dir();
    unlink("TEST_STDOUT");
    unlink("TEST_STDERR");
    return 0;
}

/* You can also delete leftover files after a test suite runs, but there's
 * no need to duplicate code between this and init_suite
 */
int clean_suite(void) {
    return 0;
}

/* Simple test of fread().
 * Reads the data previously written by testFPRINTF()
 * and checks whether the expected characters are present.
 * Must be run after testFPRINTF().
 */
void simple_sample_test(void) {
    // This is a very basic test. Your tests should likely do more than this.
    // We suggest checking the outputs of printfs/fprintfs to both stdout
    // and stderr. To make this convenient for you, the tester replaces
    // printf and fprintf with copies that write data to a file for you
    // to access. To access all output written to stdout, you can read
    // from the "TEST_STDOUT" file. To access all output written to stderr,
    // you can read from the "TEST_STDERR" file.
    int retval;
    retval = beargit_init();
    CU_ASSERT(0 == retval);
    retval = beargit_add("asdf.txt");
    CU_ASSERT(0 == retval);
}

struct commit {
    char msg[MSG_SIZE];
    struct commit *next;
};


void free_commit_list(struct commit **commit_list) {
    if (*commit_list) {
        free_commit_list(&((*commit_list)->next));
        free(*commit_list);
    }

    *commit_list = NULL;
}

void run_commit(struct commit **commit_list, const char *msg) {
    int retval = beargit_commit(msg);
    CU_ASSERT(0 == retval);

    struct commit *new_commit = (struct commit *) malloc(sizeof(struct commit));
    new_commit->next = *commit_list;
    strcpy(new_commit->msg, msg);
    *commit_list = new_commit;
}

void Simple_branch_Test(void) {
    struct commit *commit_list = NULL;
    int retval;
    retval = beargit_init();
    CU_ASSERT(0 == retval);
    FILE *asdf = fopen("asdf.txt", "w");
    fclose(asdf);
    retval = beargit_add("asdf.txt");
    CU_ASSERT(0 == retval);
    run_commit(&commit_list, "GO BEARS!1");

    retval = beargit_branch();
    FILE *fstdout = fopen("TEST_STDOUT", "r");
    char branch[BRANCHNAME_SIZE];
    fgets(branch, BRANCHNAME_SIZE, fstdout);
    fclose(fstdout);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "* master", BRANCHNAME_SIZE));
    CU_ASSERT_FATAL(0 == retval);
}

void Complex_branch_Test(void) {
    struct commit *commit_list = NULL;
    int retval;
    retval = beargit_init();
    CU_ASSERT(0 == retval);
    FILE *asdf = fopen("asdf.txt", "w");
    fclose(asdf);
    retval = beargit_add("asdf.txt");
    CU_ASSERT(0 == retval);
    run_commit(&commit_list, "GO BEARS!1");

    beargit_checkout("fs",1);
    beargit_checkout("fwww", 1);
    beargit_checkout("fs", 0);
    beargit_branch();

    FILE *fstdout = fopen("TEST_STDOUT", "r");
    char branch[BRANCHNAME_SIZE];
    fgets(branch, BRANCHNAME_SIZE, fstdout);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "  master", BRANCHNAME_SIZE));

    fgets(branch, BRANCHNAME_SIZE, fstdout);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "* fs", BRANCHNAME_SIZE));
    fclose(fstdout);
}

void Simple_checkout_test(void){
    struct commit *commit_list = NULL;
    int retval;
    retval = beargit_init();
    CU_ASSERT(0 == retval);
    FILE *master = fopen("master.txt", "w");
    fclose(master);
    retval = beargit_add("master.txt");
    CU_ASSERT(0 == retval);
    run_commit(&commit_list, "GO BEARS!master");

    beargit_checkout("branch1",1);
    FILE *branch1 = fopen("branch1.txt", "w");
    fclose(branch1);
    retval = beargit_add("branch1.txt");
    CU_ASSERT(0 == retval);
    run_commit(&commit_list, "GO BEARS!branch1");
    CU_ASSERT(0 == retval);

    beargit_checkout("master", 0);
    beargit_checkout("branch2", 1);
    FILE *branch2 = fopen("branch2.txt", "w");
    fclose(branch2);
    retval = beargit_add("branch2.txt");
    CU_ASSERT(0 == retval);
    run_commit(&commit_list, "GO BEARS!branch2");
    CU_ASSERT(0 == retval);

    beargit_checkout("branch2", 0);
    beargit_branch();

    FILE *fstdout = fopen("TEST_STDOUT", "r");
    char branch[BRANCHNAME_SIZE];
    fgets(branch, BRANCHNAME_SIZE, fstdout);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "  master", BRANCHNAME_SIZE));

    fgets(branch, BRANCHNAME_SIZE, fstdout);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "  branch1", BRANCHNAME_SIZE));

    fgets(branch, BRANCHNAME_SIZE, fstdout);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "* branch2", BRANCHNAME_SIZE));

    FILE *findex = fopen(".beargit/.index", "r");
    fgets(branch, FILENAME_SIZE, findex);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "master.txt", FILENAME_SIZE));

    fgets(branch, FILENAME_SIZE, findex);
    strtok(branch, "\n");
    CU_ASSERT(!strncmp(branch, "branch2.txt", FILENAME_SIZE));

    fclose(fstdout);
}


void simple_log_test(void) {
    struct commit *commit_list = NULL;
    int retval;
    retval = beargit_init();
    CU_ASSERT(0 == retval);
    FILE *asdf = fopen("asdf.txt", "w");
    fclose(asdf);
    retval = beargit_add("asdf.txt");
    CU_ASSERT(0 == retval);
    run_commit(&commit_list, "GO BEARS!1");
    run_commit(&commit_list, "GO BEARS!2");
    run_commit(&commit_list, "GO BEARS!3");

    retval = beargit_log();
    CU_ASSERT(0 == retval);

    struct commit *cur_commit = commit_list;

    const int LINE_SIZE = 512;
    char line[LINE_SIZE];

    FILE *fstdout = fopen("TEST_STDOUT", "r");
    CU_ASSERT_PTR_NOT_NULL(fstdout);

    while (cur_commit != NULL) {
        char refline[LINE_SIZE];

        // First line is empty
        CU_ASSERT_PTR_NOT_NULL(fgets(line, LINE_SIZE, fstdout));
        CU_ASSERT(!strcmp(line, "\n"));

        // Second line is commit -- don't check the ID.
        CU_ASSERT_PTR_NOT_NULL(fgets(line, LINE_SIZE, fstdout));
        CU_ASSERT(!strncmp(line, "commit", strlen("commit")));

        // Third line is msg
        sprintf(refline, "    %s\n", cur_commit->msg);
        CU_ASSERT_PTR_NOT_NULL(fgets(line, LINE_SIZE, fstdout));
        CU_ASSERT_STRING_EQUAL(line, refline);

        cur_commit = cur_commit->next;
    }

    // Last line is empty
    CU_ASSERT_PTR_NOT_NULL(fgets(line, LINE_SIZE, fstdout));
    CU_ASSERT(!strcmp(line, "\n"));

    CU_ASSERT_PTR_NULL(fgets(line, LINE_SIZE, fstdout));

    // It's the end of output
    CU_ASSERT(feof(fstdout));
    fclose(fstdout);

    free_commit_list(&commit_list);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int cunittester() {
    CU_pSuite pSuite = NULL;
    CU_pSuite pSuite2 = NULL;

    CU_pSuite mySuite1 = NULL;
    CU_pSuite mySuite2 = NULL;
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to the Suite #1 */
    if (NULL == CU_add_test(pSuite, "Simple Test #1", simple_sample_test)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    pSuite2 = CU_add_suite("Suite_2", init_suite, clean_suite);
    if (NULL == pSuite2) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to the Suite #2 */
    if (NULL == CU_add_test(pSuite2, "Log output test", simple_log_test)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    mySuite1 = CU_add_suite("Suite_3", init_suite, clean_suite);
    if (NULL == mySuite1){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to the Suite #3 */
    if (NULL == CU_add_test(mySuite1, "Branch", Complex_branch_Test)){
        CU_cleanup_registry();
        return CU_get_error();
    }

    mySuite2 = CU_add_suite("Suite_4", init_suite, clean_suite);
    if (NULL == mySuite2){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(mySuite2, "Checkout", Simple_checkout_test)){
        CU_cleanup_registry();
        return CU_get_error();
    }
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

