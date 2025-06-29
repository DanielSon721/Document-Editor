#include "document.h"
#include <stdio.h>
#include <string.h>

/*Daniel Son, 119710265, djson721*/

int init_document(Document *doc, const char *name) {

    /*checks for invalid parameters*/
    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);

    return SUCCESS;
}

int reset_document(Document *doc) {

    /*checks for invalid parameters*/
    if (doc == NULL) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;

    return SUCCESS;
}

int print_document(Document *doc) {

    int i, j;
    char *ptr;

    /*checks for invalid parameters*/
    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    /*prints contents of doc*/
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            ptr = doc->paragraphs[i].lines[j];
            while (*ptr != '\0') {
                printf("%c", *ptr);
                ptr++;
            }
            printf("\n");
        }
        if (doc->paragraphs[i].number_of_lines != 0 && i < doc->number_of_paragraphs - 1) {
            printf("\n");
        }
    }

    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {

    int i;
    Paragraph new_paragraph;
    new_paragraph.number_of_lines = 0;

    /*checks for invalid parameters*/
    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    /*shifts following paragraphs down by one*/
    doc->number_of_paragraphs++;
    for (i = doc->number_of_paragraphs - 1; i > paragraph_number; i--) {
        doc->paragraphs[i] = doc->paragraphs[i - 1];
    }

    /*adds new paragraph*/
    doc->paragraphs[paragraph_number] = new_paragraph;

    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {

    int i;

    /*checks for invalid parameters*/
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs|| paragraph_number < 1 || doc->paragraphs[paragraph_number - 1].number_of_lines == MAX_PARAGRAPH_LINES || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines || new_line == NULL) {
        return FAILURE;
    }

    /*shifts following lines down by one*/
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
    for (i = doc->paragraphs[paragraph_number - 1].number_of_lines - 1; i > line_number; i--) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], doc->paragraphs[paragraph_number - 1].lines[i - 1]);
    }

    /*adds new line*/
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);

    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {

    /*checks for invalid parameters*/
    if (doc == NULL || number_of_lines == NULL || paragraph_number > doc -> number_of_paragraphs) {
        return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    return SUCCESS;
}

int get_number_lines(Document *doc, int *number_of_lines) {

    int i, line_count = 0;

    /*checks for invalid parameters*/
    if (doc == NULL) {
        return FAILURE;
    }

    /*counts number of lines*/
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        line_count += doc->paragraphs[i].number_of_lines;
    }

    *number_of_lines = line_count;

    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    return add_line_after(&(*doc), paragraph_number, doc->paragraphs[paragraph_number - 1].number_of_lines, new_line);
}

int remove_line(Document *doc, int paragraph_number, int line_number) {

    int i;

    /*checks for invalid parameters*/
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
        return FAILURE;
    }

    /*removes line by shifting all lines after target line up by one line*/
    for (i = line_number - 1; i < doc->paragraphs[paragraph_number - 1].number_of_lines; i++) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], doc->paragraphs[paragraph_number - 1].lines[i + 1]);
    }

    doc -> paragraphs[paragraph_number - 1].number_of_lines--;

    return SUCCESS;
}

int is_blank(const char *str) {
    /*If the string is NULL or the first character is the null terminator, it's empty*/
    if (str == NULL || *str == '\0') {
        return 1; /*String is empty*/
    }

    /*Iterate through the string*/
    while (*str != '\0') {
        if (*str != ' ') {
            return 0; /*Found a non-space character*/
        }
        str++;
    }

    return 1; /*String contains only spaces*/
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {

    int i, j, k = 0, maximum_reached = 0;

    /*checks for invalid parameters*/
    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }

    /*creates new paragraph*/
    add_paragraph_after(doc, k);
    k++;

    /*loads contents of data into doc*/
    for (i = 0, j = 0; j < data_lines; i++, j++) {
        if (is_blank(&data[j][0])) {
            if (doc->number_of_paragraphs == MAX_PARAGRAPHS) {
                maximum_reached = 1;
                break;
            }
            add_paragraph_after(doc, k);
            i = -1;
            k++;
        }
        else {
            add_line_after(doc, k, i, data[j]);
        }
    }

    if (maximum_reached) {
        return FAILURE;
    }
    else {
        return SUCCESS;
    }
}

/*helper method to determine if target is found*/
int is_match(char *curr_pos, const char *target) {

    int i;
    char substring[MAX_STR_SIZE + 1];

    for (i = 0; i < strlen(target); i++) {
        substring[i] = curr_pos[i];
    }
    substring[i] = '\0';

    return strcmp(substring, target);
}

int replace_text(Document *doc, const char *target, const char *replacement) {

    int i, j, k, difference;
    char *ptr;

    /*checks for invalid parameters*/
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {

            ptr = doc->paragraphs[i].lines[j];

            while (*ptr != '\0') {
                /*if target is found*/
                if (!is_match(ptr, target)) {

                    /*preserves everything after target*/
                    char rest_of_string[MAX_STR_SIZE + 1];
                    char *ptr2 = ptr;
                    for (k = 0; k < strlen(target); k++) {
                        ptr2++;
                    }
                    strcpy(rest_of_string, ptr2);

                    /*replaces target with replacement*/
                    strncpy(ptr, replacement, strlen(replacement));

                    /*adds everything after target back*/
                    difference = (int) (strlen(replacement) - strlen(target));
                    if (difference >= 0) {
                        for (k = 0; k < difference; k++) {
                            ptr2++;
                        }
                    }
                    else {
                        for (k = 0; k < difference * -1; k++) {
                            ptr2--;
                        }   
                    }
                    strcpy(ptr2, rest_of_string);

                    ptr += strlen(replacement);
                }
                else {
                    ptr += 1;
                }
            }
        }
    }

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {

    char highlighted_target[MAX_STR_SIZE + 1], *ptr = highlighted_target;
    int i;

    /*checks for invalid parameters*/
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /*creates a string of highlighted target*/
    *ptr++ = '[';
    strcpy(ptr, target);
    for (i = 0; i < strlen(target); i++) {
        ptr++;
    }
    *ptr++ = ']';
    *ptr = '\0';

    /*replaces text with highlighted target*/
    return replace_text(&(*doc), target, highlighted_target);
}

int remove_text(Document *doc, const char *target) {

    /*checks for invalid parameters*/
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /*replaces target with empty string*/
    return replace_text(&(*doc), target, "");
}

int load_file(Document *doc, const char *filename) {

    int i = 0;
    char data[MAX_PARAGRAPH_LINES * MAX_PARAGRAPHS][MAX_STR_SIZE + 1];
    char buffer[MAX_STR_SIZE];
    FILE *filePointer;
    filePointer = fopen(filename, "r");

    if (doc == NULL || filename == NULL || filePointer == NULL) {
        return FAILURE;
    }

    while (fgets(buffer, sizeof(buffer), filePointer)) {
        buffer[strlen(buffer) - 1] = '\0';
        strcpy(data[i], buffer);
        i++;
    }

    /*Close the file after reading*/
    fclose(filePointer);

    return load_document(&(*doc), data, i);
}

int save_document(Document *doc, const char *filename) {
    
    FILE *file;
    int i, j;

    file = fopen(filename, "w");
    if (doc == NULL || filename == NULL || file == NULL) {
        return FAILURE;
    }

    /* Write the document's paragraphs and lines to the file */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            fprintf(file, "%s\n", doc->paragraphs[i].lines[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    return SUCCESS;
}