#include "document.h"
#include <stdio.h>
#include <string.h>

/*Daniel Son, 119710265, djson721*/

char* get_string_asterisk(const char* str) {

    /*Use strchr to find the first occurrence of the character*/
    char* pos = strchr(str, '*');
    char* next_line;
    
    /*If the character is not found, return NULL*/
    if (pos == NULL) {
        return NULL;
    }

    next_line = strchr(str, '\n');
    *next_line = '\0';
    
    
    /*Return the substring starting from the character after 'ch'*/
    return pos + 1;
}

void get_substring_quotes(const char* str, char* result, int target) {
    /*Find the first occurrence of a quote character*/
    char* start;
    char* end;
    int length;

    start = strchr(str, '"');
    /*if no starting quote is found, set result to an empty string and return*/
    if (start == NULL) {
        result[0] = '\0';
        return;
    }
    if (target == 1) {
        start = strchr(start + 1, '"');
        start = strchr(start + 1, '"');
    }
    /*if no starting quote is found, set result to an empty string and return*/
    if (start == NULL) {
        result[0] = '\0';
        return;
    }

    /*Find the next quote after the starting quote*/
    end = strchr(start + 1, '"');

    /*If no ending quote is found, set result to an empty string and return*/
    if (end == NULL) {
        result[0] = '\0';
        return;
    }
    
    /*Calculate the length of the substring between the quotes*/
    length = end - start - 1;
    
    /*Copy the substring between the quotes to the result buffer*/
    strncpy(result, start + 1, length);
    
    /*Add the null terminator*/
    result[length] = '\0';
}

int main(int argc, char *argv[]) {

    Document doc;
    char curr_line[MAX_STR_SIZE];
    char command[MAX_STR_SIZE];
    char invalid[MAX_STR_SIZE];
    int valid;

    if (argc == 1) { /*user input*/

        init_document(&doc, "main_document");
        
        while (fgets(curr_line, MAX_STR_SIZE, stdin)) {

            printf("> ");

            strcpy(command, "");
            strcpy(invalid, "");
            sscanf(curr_line, "%s", command);

            if (strcmp(command, "add_paragraph_after") == 0) {
                int paragraph_number;
                valid = sscanf(curr_line, "%s %d %s", command, &paragraph_number, invalid);
                if (valid != 2 || paragraph_number < 0 || !is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    if (add_paragraph_after(&doc, paragraph_number) == FAILURE) {
                        printf("add_paragraph_after failed\n");
                    }
                }
            }
            else if (strcmp(command, "add_line_after") == 0) {
                int paragraph_number;
                int line_number;
                char *new_line;
                valid = sscanf(curr_line, "%s %d %d", command, &paragraph_number, &line_number);
                new_line = get_string_asterisk(curr_line);
                if (valid != 3 || paragraph_number <= 0 || line_number < 0 || new_line == NULL) {
                    printf("Invalid Command\n");
                }
                else {
                    if (add_line_after(&doc, paragraph_number, line_number, new_line) == FAILURE) {
                        printf("add_line_after failed\n");
                    }
                }
            }
            else if (strcmp(command, "print_document") == 0) {
                sscanf(curr_line, "%s %s", command, invalid);
                if (!is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    if (print_document(&doc) == FAILURE) {
                        printf("print_document failed\n");
                    }
                }
            }
            else if (strcmp(command, "quit") == 0) {
                sscanf(curr_line, "%s %s", command, invalid);
                if (!is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    break;
                }
            }
            else if (strcmp(command, "exit") == 0) {
                sscanf(curr_line, "%s %s", command, invalid);
                if (!is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    break;
                }
            }
            else if (strcmp(command, "append_line") == 0) {
                int paragraph_number;
                char *new_line;
                valid = sscanf(curr_line, "%s %d", command, &paragraph_number);
                new_line = get_string_asterisk(curr_line);
                if (valid != 2 || paragraph_number <= 0 || new_line == NULL) {
                    printf("Invalid Command\n");
                }
                else {
                    if (append_line(&doc, paragraph_number, new_line) == FAILURE) {
                        printf("append_line failed\n");
                    }
                }
            }
            else if (strcmp(command, "remove_line") == 0) {
                int paragraph_number;
                int line_number;
                valid = sscanf(curr_line, "%s %d %d %s", command, &paragraph_number, &line_number, invalid);
                if (valid != 3 || paragraph_number <= 0 || line_number <= 0 || !is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    if (remove_line(&doc, paragraph_number, line_number) == FAILURE) {
                        printf("remove_line failed\n");
                    }
                }
            }
            else if (strcmp(command, "load_file") == 0) {
                char file_name[MAX_STR_SIZE];
                sscanf(curr_line, "%s %s %s", command, file_name, invalid);
                if (is_blank(&file_name) || !is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    if (load_file(&doc, file_name) == FAILURE) {
                        printf("load_file failed\n");
                    }
                }
            }
            else if (strcmp(command, "replace_text") == 0) {
                char target[MAX_STR_SIZE];
                char replacement[MAX_STR_SIZE];
                get_substring_quotes(curr_line, target, 0);
                get_substring_quotes(curr_line, replacement, 1);
                if ((is_blank(&target) && is_blank(&replacement)) || (!is_blank(target) && is_blank(&replacement))) {
                    printf("Invalid Command\n");
                }
                else {
                    if (replace_text(&doc, target, replacement) == FAILURE) {
                        printf("replace_text failed\n");
                    }
                }
            }
            else if (strcmp(command, "highlight_text") == 0) {
                char target[MAX_STR_SIZE];
                get_substring_quotes(curr_line, target, 0);
                if (is_blank(&target)) {
                    printf("Invalid Command\n");
                }
                else {
                    highlight_text(&doc, target);
                }
            }
            else if (strcmp(command, "remove_text") == 0) {
                char target[MAX_STR_SIZE];
                get_substring_quotes(curr_line, target, 0);
                if (is_blank(&target)) {
                    printf("Invalid Command\n");
                }
                else {
                    remove_text(&doc, target);
                }
            }
            else if (strcmp(command, "save_document") == 0) {
                char file_name[MAX_STR_SIZE];
                valid = sscanf(curr_line, "%s %s %s", command, file_name, invalid);
                if (valid != 2 || !is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    if (save_document(&doc, file_name) == FAILURE) {
                        printf("save_document failed\n");
                    }
                }
            }
            else if (strcmp(command, "reset_document") == 0) {
                sscanf(curr_line, "%s %s", command, invalid);
                if (!is_blank(&invalid)) {
                    printf("Invalid Command\n");
                }
                else {
                    reset_document(&doc);
                }
            }
        }
    }
    
    else if (argc == 2) { /*reading from file*/
        FILE *filePointer;
        filePointer = fopen(argv[1], "r");
        init_document(&doc, "main_document");
        if (filePointer == NULL) {
            printf("%s cannot be opened.\n", argv[1]);
        }
        else if (filePointer != NULL) {
            while (fgets(curr_line, MAX_STR_SIZE, filePointer) != NULL) {

                strcpy(command, "");
                strcpy(invalid, "");
                sscanf(curr_line, "%s", command);

                if (strcmp(command, "add_paragraph_after") == 0) {
                    int paragraph_number;
                    valid = sscanf(curr_line, "%s %d %s", command, &paragraph_number, invalid);
                    if (valid != 2 || paragraph_number < 0 || !is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (add_paragraph_after(&doc, paragraph_number) == FAILURE) {
                        printf("add_paragraph_after failed\n");
                        }
                    }
                }
                else if (strcmp(command, "add_line_after") == 0) {
                    int paragraph_number;
                    int line_number;
                    char *new_line;
                    valid = sscanf(curr_line, "%s %d %d", command, &paragraph_number, &line_number);
                    new_line = get_string_asterisk(curr_line);
                    if (valid != 3 || paragraph_number <= 0 || line_number < 0 || new_line == NULL) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (add_line_after(&doc, paragraph_number, line_number, new_line) == FAILURE) {
                            printf("add_line_after failed\n");
                        }
                    }
                }
                else if (strcmp(command, "print_document") == 0) {
                    sscanf(curr_line, "%s %s", command, invalid);
                    if (!is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (print_document(&doc) == FAILURE) {
                            printf("print_document failed\n");
                        }
                    }
                }
                else if (strcmp(command, "quit") == 0) {
                    sscanf(curr_line, "%s %s", command, invalid);
                    if (!is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        break;
                    }
                }
                else if (strcmp(command, "exit") == 0) {
                    sscanf(curr_line, "%s %s", command, invalid);
                    if (!is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        break;
                    }
                }
                else if (strcmp(command, "append_line") == 0) {
                    int paragraph_number;
                    char *new_line;
                    valid = sscanf(curr_line, "%s %d", command, &paragraph_number);
                    new_line = get_string_asterisk(curr_line);
                    if (valid != 2 || paragraph_number <= 0 || new_line == NULL) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (append_line(&doc, paragraph_number, new_line) == FAILURE) {
                            printf("append_line failed\n");
                        }
                    }
                }
                else if (strcmp(command, "remove_line") == 0) {
                    int paragraph_number;
                    int line_number;
                    valid = sscanf(curr_line, "%s %d %d %s", command, &paragraph_number, &line_number, invalid);
                    if (valid != 3 || paragraph_number <= 0 || line_number <= 0 || !is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (remove_line(&doc, paragraph_number, line_number) == FAILURE) {
                            printf("remove_line failed\n");
                        }
                    }
                }
                else if (strcmp(command, "load_file") == 0) {
                    char file_name[MAX_STR_SIZE];
                    sscanf(curr_line, "%s %s %s", command, file_name, invalid);
                    if (is_blank(&file_name) || !is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (load_file(&doc, file_name) == FAILURE) {
                            printf("load_file failed\n");
                        }
                    }
                }
                else if (strcmp(command, "replace_text") == 0) {
                    char target[MAX_STR_SIZE];
                    char replacement[MAX_STR_SIZE];
                    get_substring_quotes(curr_line, target, 0);
                    get_substring_quotes(curr_line, replacement, 1);
                    if ((is_blank(&target) && is_blank(&replacement)) || (!is_blank(target) && is_blank(&replacement))) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (replace_text(&doc, target, replacement) == FAILURE) {
                            printf("replace_text failed\n");
                        }
                    }
                }
                else if (strcmp(command, "highlight_text") == 0) {
                    char target[MAX_STR_SIZE];
                    get_substring_quotes(curr_line, target, 0);
                    if (is_blank(&target)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        highlight_text(&doc, target);
                    }
                }
                else if (strcmp(command, "remove_text") == 0) {
                    char target[MAX_STR_SIZE];
                    get_substring_quotes(curr_line, target, 0);
                    if (is_blank(&target)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        remove_text(&doc, target);
                    }
                }
                else if (strcmp(command, "save_document") == 0) {
                    char file_name[MAX_STR_SIZE];
                    valid = sscanf(curr_line, "%s %s %s", command, file_name, invalid);
                    if (valid != 2 || !is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        if (save_document(&doc, file_name) == FAILURE) {
                            printf("save_document failed\n");
                        }
                    }
                }
                else if (strcmp(command, "reset_document") == 0) {
                    sscanf(curr_line, "%s %s", command, invalid);
                    if (!is_blank(&invalid)) {
                        printf("Invalid Command\n");
                    }
                    else {
                        reset_document(&doc);
                    }
                }
            }
            /*Close the file after reading*/
            fclose(filePointer);
        }
    }
    else {
        printf("Usage: user_interface\nUsage: user_interface <filename>\n");
    }

    return SUCCESS;
}