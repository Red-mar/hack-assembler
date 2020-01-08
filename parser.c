#include "parser.h"

static int variables = 0;
static int absolute_file_lines = 0;

char* to_binary16(int num, char* return_string) {
    for (int bits = 0; bits < 16; bits++) {
        if(bits == 0) {
            num = num << 1;
            continue;
        }
        int bit = (num & 0x8000);
        num = num << 1;
        if (bit > 0) {
            strcat(return_string, "1");
        } else {
            strcat(return_string, "0");
        }
    }
    return return_string;
}

char* parse_file(char* buffer, char* code) {
    int pass = 0;
    variables = 0;

    while(1) {
        absolute_file_lines = 0;
        int total_chars = 0;
        int lines = 0;
        int i = 0;
        char c= ' ';
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

            if ( (cmd == A_COMMAND && pass != 0) ||
                 (cmd == L_COMMAND && pass != 1)) {
                char* symbol = (char*)malloc(100);
                if(parse_symbol(line, symbol, cmd) != 0) {
                    printf("Error parsing symbol: %s\n", line);
                } else if(cmd == A_COMMAND){
                    char* a_command = (char*)malloc(20);
                    strcat(code, parse_a_command(symbol_ht, symbol, a_command));
                    free(a_command);
                } else if(cmd == L_COMMAND) {
                    parse_l_command(symbol_ht, symbol, lines);
                    lines--;
                }
                free(symbol);
            }

            else if (cmd == C_COMMAND && pass != 0) {
                //get dest
                char* dest = (char*)malloc(100);
                char* comp = (char*)malloc(100);
                char* jump = (char*)malloc(100);
                if (parse_dest(line, dest) > 1) printf("Error parsing dest: %s\n", line);
                if (parse_comp(line, comp) > 1) printf("Error parsing comp: %s\n", line);
                if (parse_jump(line, jump) > 1) printf("Error parsing jump: %s\n", line);

                char* c_command = (char*)malloc(20);
                parse_c_command(dest, comp, jump, c_command);
                strcat(code, c_command);
                free(c_command);
                free(dest);
                free(comp);
                free(jump);
            } else if (cmd == NOT_SET) {
                lines--;
            }
            lines++;
            absolute_file_lines++;
            free(line);
            total_chars += i;
            i = 0;
        }
break2:;
        if (pass == 1) break;
        pass = 1;
    }
    return code;
}

char* parse_a_command(hash_table* ht, char* symbol, char* return_string) {
    // A start with 0
    strcat(return_string, "0");
    ht_item* label = get_ht_item(ht, symbol);
    if (symbol[0] >= '0' && symbol[0] <= '9') {
        char* binary_string = (char*)malloc(20);
        strcat(return_string, to_binary16(atoi(symbol), binary_string));
        free(binary_string);
    } else if (label) {
        strcat(return_string, label->value);
    } else {
        char* binary_string = (char*)malloc(20);
        strcat(return_string, to_binary16(VARIABLE_MEMORY_OFFSET + variables, binary_string));
        insert_ht_item(ht, symbol, binary_string);
        free(binary_string);
        variables++;
    }

    strcat(return_string, "\n");
    return return_string;
}

void parse_l_command(hash_table* ht, char* symbol, int lines) {
    char* binary_string = (char*)malloc(20);
    to_binary16(lines, binary_string);
    insert_ht_item(ht, symbol, binary_string);
    free(binary_string);
}

char* parse_c_command(const char* dest, const char* comp, const char* jump, char* return_string) {
    // C start with 1XX
    strcat(return_string, "111");
    char* dest_code = "000";
    ht_item* dest_item = get_ht_item(dest_ht, dest);
    if (dest_item != NULL) dest_code = dest_item->value;
    else printf("Invalid DST command: %s at %d\n", dest, absolute_file_lines);

    char* comp_code = "0000000";
    ht_item* comp_item = get_ht_item(comp_ht, comp);
    if (comp_item != NULL) comp_code = comp_item->value;
    else printf("Invalid CMP command: %s at %d\n", comp, absolute_file_lines);

    char* jump_code = "000";
    ht_item* jump_item = get_ht_item(jump_ht, jump);
    if (jump_item != NULL) jump_code = jump_item->value;
    else printf("Invalid JMP command: %s at %d\n", jump, absolute_file_lines);

    strcat(return_string, comp_code);
    strcat(return_string, dest_code);
    strcat(return_string, jump_code);
    strcat(return_string, "\n");
    return return_string;
}

int parse_comments(char* line) {
    int i = 0;
    int start = 0;
    char c = ' ';
    while(c != '\0') {
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
    int is_dest = 0;
    while(c != '\n') {
        c = line[i];
        if (c == ' ') {
            start = i + 1;
        }
        if (c == '=') {
            end = i;
            is_dest = 1;
            break;
        }
        i++;
    }
    if (is_dest == 0) {
        strcat(dest, "null");
        return 1;
    }
    if (end == 0) {
        return 1;
    }
    strncpy(dest, line + start, end - start);
    return 0;
}

int parse_comp(const char* line, char* comp) {
    int i = 0;
    int start = 0;
    int end = 0;
    char c = '\0';
    while(c != '\n') {
        c = line[i];
        if (c == '=' || c == ' ') {
            start = i + 1;
        }
        if (c == ';' || c == '\n' || c == '\r') {
            end = i;
            break;
        }
        i++;
    }
    if (end == 0) {
        return 1;
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
        strcat(jump, "null");
        return 1;
    }
    if (start == 0 || i == 0) {
        return 2;
    }
    // jump is always 3 characters
    strncpy(jump, line + start, 3);
    return 0;
}

int parse_symbol(const char* line, char* symbol, enum command cmd) {
    char identifier = '\0';
    char end_identifier = '\0';
    if(cmd == A_COMMAND) {
        identifier = '@';
        end_identifier = ' ';
    } else if (cmd == L_COMMAND) {
        identifier = '(';
        end_identifier = ')';
    }
    if(cmd == A_COMMAND || cmd == L_COMMAND) {
        int i = 0;
        int identifier_i = 0;

        char c = '\0';
        while (c != '\n') {
            c = line[i];
            if (c == identifier) {
                identifier_i = i;
            }
            i++;
        }
        i = identifier_i + 1;
        c = '\0';
        while (c != end_identifier && c != '\n' && c != ' '
               // WINDOWS???
               && c != '\r') {
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
