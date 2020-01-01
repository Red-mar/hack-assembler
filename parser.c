#include "parser.h"

void parse_file(char* buffer) {
    char c= ' ';
    int total_chars = 0;

    int i = 0;
    while(c != '\0') {
        c = ' ';
        while(c != '\n') {
            c = buffer[i + total_chars];
            i++;
            if (c == '\0') goto break2;
        }
        // Probably faster to just allocate a lot at the start
        char* line = (char *)malloc(i+1);
        strncpy(line, buffer + total_chars, i);
        parse_comments(line);
        enum command cmd = parse_command(line);
        if (cmd == A_COMMAND || cmd == L_COMMAND) {
            printf("LC: ");
            char* symbol = (char*)malloc(100);
            if(parse_symbol(line, symbol, cmd) != 0) {
                printf("Error parsing symbol: %s\n", line);
            } else {
                printf("SYMBOL:%s\n", symbol);
            }
            free(symbol);
        }
        if (cmd == C_COMMAND) {
            printf("C: ");
            //get dest
            char* dest = (char*)malloc(100);
            if(parse_dest(line, dest) != 0) {
                printf("Error parsing dest: %s\n", line);
            } else {
                printf("DEST:%s ", dest);
            }
            //get comp
            char* comp = (char*)malloc(100);
            int ret = 0;
            ret = parse_comp(line, comp);
            if(ret == 0) {
                printf("COMP:%s ", comp);

            } else if (ret == 1) {
            } else {
                printf("Error parsing comp: %s\n", line);
            }
            //get jump
            char* jump = (char*)malloc(100);
            ret = parse_jump(line, jump);
            if(ret == 0) {
                printf("JUMP:%s", jump);
            } else if (ret == 1) {
            } else {
                printf("Error parsing jump: %s\n", line);
            }

            char* command = parse_c_command(dest, comp, jump);

            free(dest);
            free(comp);
            free(jump);
            printf("\n");
        }
        free(line);
        total_chars += i;
        i = 0;
    }
    break2:;
}

char* parse_c_command(const char* dest, const char* comp, const char* jump) {
    char* dest_code = "000";
    char* comp_code = "0000000";
    char* jump_code = "000";
    // hash table??

    return "0000";
}

int parse_comments(char* line) {
    int i = 0;
    int start = 0;
    char c = '\0';
    while(c != '\n') {
        c = line[i];
        if ((c == '/' && line[i+1] == '/') && start == 0) {
            start = i;
        }
        i++;
    }
    if (start == 0) return 0;
    strncpy(line + start, "\0", i - start);
    strcat(line, "\n");
    return 0;
}

int parse_dest(const char* line, char* dest) {
    int i = 0;
    int start = 0;
    int end = 0;
    char c = '\0';
    while(c != '\n') {
        c = line[i];
        if (c == ' ') {
            start = i + 1;
        }
        if (c == '=') {
            end = i;
            break;
        }
        if (c == ';') {
            end = i;
            break;
        }
        i++;
    }
    if (end == 0 || start == 0) {
        return 1;
    }
    strncpy(dest, line + start, end - start);
    return 0;
}

int parse_comp(const char* line, char* comp) {
    int i = 0;
    int start = 0;
    int end = 0;
    int  is_comp = 0;
    char c = '\0';
    while(c != '\n') {
        c = line[i];
        if (c == '=') {
            start = i + 1;
            is_comp = 1;
        }
        if (c == ';' || c == '\n') {
            end = i;
            break;
        }
        i++;
    }
    if (is_comp == 0) {
        return 1;
    }
    if (end == 0 || start == 0) {
        return 2;
    }
    strncpy(comp, line + start, end - start);
    return 0;
}

int parse_jump(const char* line, char* jump) {
    int i = 0;
    int start = 0;
    char c = '\0';
    int is_jump = 0;
    while(c != '\n') {
        c = line[i];
        if (c == ';') {
            start = i + 1;
            is_jump = 1;
        }
        i++;
    }
    if (is_jump == 0) {
        return 1;
    }
    if (start == 0 || i == 0) {
        return 2;
    }
    strncpy(jump, line + start, i - start - 1);
    return 0;
}

int parse_symbol(const char* line, char* symbol, enum command cmd) {
    char identifier = '\0';
    char end_identifier = '\0';
    if(cmd == A_COMMAND) {
        identifier = '@';
        end_identifier = '\n';
    } else if (cmd == L_COMMAND) {
        identifier = '(';
        end_identifier = ')';
    }
    if(cmd == A_COMMAND || cmd == L_COMMAND) {
        int i = 0;
        int identifier_i = 0;

        char c = ' ';
        while (c != '\n') {
            c = line[i];
            if (c == identifier) {
                identifier_i = i;
            }
            i++;
        }
        i = identifier_i + 1;
        c = ' ';
        while (c != end_identifier) {
            c = line[i];
            i++;
        }
        i-=2;

        strncpy(symbol, line + identifier_i + 1, i - identifier_i);
        return 0;
    }
    return 1;
}

enum command parse_command(const char* line) {

    enum command cmd = NOT_SET;
    if (strchr(line, '@')) {
        cmd = A_COMMAND;
    }
    if (strchr(line, '(')) {
        if (cmd == NOT_SET) {
            cmd = L_COMMAND;
        } else {
            printf("Invalid command line:\n\t %s\n", line);
            return NOT_SET;
        }
    }
    if (strchr(line, ';') || strchr(line, '=')) {
        if (cmd == NOT_SET) {
            cmd = C_COMMAND;
        } else {
            printf("Invalid command line:\n\t %s\n", line);
            return NOT_SET;
        }
    }
    return cmd;
}
