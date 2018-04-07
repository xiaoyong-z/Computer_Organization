#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
    fs_mkdir(".beargit");

    FILE *findex = fopen(".beargit/.index", "w");
    fclose(findex);

    write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");

    return 0;
}


/* beargit add <filename>
 * 
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char *filename) {
    FILE *findex = fopen(".beargit/.index", "r");
    FILE *fnewindex = fopen(".beargit/.newindex", "w");

    char line[FILENAME_SIZE];
    while (fgets(line, sizeof(line), findex)) {
        strtok(line, "\n");
        if (strcmp(line, filename) == 0) {
            fprintf(stderr, "ERROR: File %s already added\n", filename);
            fclose(findex);
            fclose(fnewindex);
            fs_rm(".beargit/.newindex");
            return 3;
        }

        fprintf(fnewindex, "%s\n", line);
    }

    fprintf(fnewindex, "%s\n", filename);
    fclose(findex);
    fclose(fnewindex);

    fs_mv(".beargit/.newindex", ".beargit/.index");

    return 0;
}


/* beargit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char *filename) {
    FILE *findex = fopen(".beargit/.index", "r");
    FILE *fnewindex = fopen(".beargit/.newindex", "w");
    int success = 0;

    char line[FILENAME_SIZE];
    while (fgets(line, sizeof(line), findex)) {
        strtok(line, "\n");
        if (strcmp(line, filename) == 0) {
            success = 1;
            continue;
        }
        fprintf(fnewindex, "%s\n", line);
    }
    if (!success){
        fprintf(stderr, "ERROR: File %s not tracked\n", filename);
        fclose(findex);
        fclose(fnewindex);
        fs_rm(".beargit/.newindex");
        return 1;
    }

    fclose(findex);
    fclose(fnewindex);
    fs_mv(".beargit/.newindex", ".beargit/.index");

    return 0;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char *go_bears = "GO BEARS!";

int is_commit_msg_ok(const char *msg) {
    if(strstr(msg, go_bears) != NULL) {
        return 1;
    }
    return 0;
}

void next_commit_id(char *commit_id) {
    int sum = 0;
    for (int i = 0; i < COMMIT_ID_SIZE - 1; i++){
        if (commit_id[i] == '1')
            sum = sum * 3 + 0;
        else if (commit_id[i] == '6')
            sum = sum * 3 + 1;
        else if (commit_id[i] == 'c')
            sum += sum * 3 + 2;
    }
    sum++;
    int temp;
    for (int i = COMMIT_ID_SIZE - 2; i >= 0; i--){
        temp = sum % 3;
        sum = sum / 3;
        if (temp == 0)
            commit_id[i] = '1';
        else if (temp == 1)
            commit_id[i] = '6';
        else if (temp == 2)
            commit_id[i] = 'c';
    }
}


int beargit_commit(const char *msg) {

    if (!is_commit_msg_ok(msg)) {
        fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
        return 1;
    }
    char commit_id[COMMIT_ID_SIZE];
    read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
    next_commit_id(commit_id);
    commit_id[COMMIT_ID_SIZE - 1] = '\0';

    char dirPath[COMMIT_ID_SIZE + 10];
    sprintf(dirPath, "%s%s", ".beargit/", commit_id);
    fs_mkdir(dirPath);

    char indexPath[COMMIT_ID_SIZE + 20];
    char prevPath[COMMIT_ID_SIZE + 20];
    char msgPath[COMMIT_ID_SIZE + 20];
    sprintf(indexPath, "%s%s", dirPath, "/.index");
    sprintf(prevPath, "%s%s", dirPath, "/.prev");
    sprintf(msgPath, "%s%s", dirPath, "/.msg");
    FILE* findex = fopen(".beargit/.index", "r");

    fs_cp(".beargit/.index", indexPath);
    fs_cp(".beargit/.prev", prevPath);

    char line[FILENAME_SIZE];
    while (fgets(line, sizeof(line), findex) != NULL) /* read a line from a file */ {
//        printf("%s\n", line);
        strtok(line, "\n");
        char dest[COMMIT_ID_SIZE + 20];
        sprintf(dest, "%s%s%s", dirPath, "/" , line);
        fs_cp(line, dest);
    }

    write_string_to_file(msgPath, msg);
    write_string_to_file(".beargit/.prev", commit_id);

    fclose(findex);
    return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status() {

    /* COMPLETE THE REST */
    fprintf(stdout, "Tracked files:\n\n");
    FILE *findex = fopen(".beargit/.index", "r");
    char line[FILENAME_SIZE];
    int numberOfFiles = 0;
    while (fgets(line, sizeof(line), findex) != NULL) /* read a line from a file */ {
        numberOfFiles++;
        fprintf(stdout, "  %s", line); //print the file contents on stdout.
    }
    fclose(findex);
    fprintf(stdout, "\n%d files total\n", numberOfFiles);
    return 0;
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */

int beargit_log() {
    int k = 0;
    char line[COMMIT_ID_SIZE];
    read_string_from_file(".beargit/.prev", line, COMMIT_ID_SIZE);
    while((strstr(line, "0") == NULL)){
        fprintf(stdout, "\ncommit %s\n    ", line);
        char msgfile[COMMIT_ID_SIZE + 20];
        char msg[MSG_SIZE];
        sprintf(msgfile, "%s%s%s", ".beargit/", line, "/.msg");
        read_string_from_file(msgfile, msg, MSG_SIZE);
        fprintf(stdout, "%s\n", msg);
        char filename[COMMIT_ID_SIZE + 20];
        sprintf(filename, "%s%s%s", ".beargit/", line, "/.prev");
        read_string_from_file(filename, line, COMMIT_ID_SIZE);
        k++;
    }
    if (!k){
        fprintf(stderr, "ERROR: There are no commits!\n");
        return 1;
    }
    else{
        fprintf(stdout, "\n");
    }
    return 0;
}
