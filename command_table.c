#include "command_table.h"

hash_table* dest_ht;
hash_table* jump_ht;
hash_table* comp_ht;
hash_table* symbol_ht;

static void init_dest_ht() {
    hash_table* ht = new_hash_table(128);
    insert_ht_item(ht, "null", "000");
    insert_ht_item(ht, "M", "001");
    insert_ht_item(ht, "D", "010");
    insert_ht_item(ht, "MD", "011");
    insert_ht_item(ht, "A", "100");
    insert_ht_item(ht, "AM", "101");
    insert_ht_item(ht, "AD", "110");
    insert_ht_item(ht, "AMD", "111");
    dest_ht = ht;
}

static void init_jump_ht() {
    hash_table* ht = new_hash_table(128);
    insert_ht_item(ht, "null", "000");
    insert_ht_item(ht, "JGT", "001");
    insert_ht_item(ht, "JEQ", "010");
    insert_ht_item(ht, "JGE", "011");
    insert_ht_item(ht, "JLT", "100");
    insert_ht_item(ht, "JNE", "101");
    insert_ht_item(ht, "JLE", "110");
    insert_ht_item(ht, "JMP", "111");
    jump_ht = ht;
}

static void init_comp_ht() {
    hash_table* ht = new_hash_table(128);
    insert_ht_item(ht, "0", "0101010");
    insert_ht_item(ht, "1", "0111111");
    insert_ht_item(ht, "-1", "0111010");
    insert_ht_item(ht, "D", "0001100");
    insert_ht_item(ht, "A", "0110000");
    insert_ht_item(ht, "!D", "0001101");
    insert_ht_item(ht, "!A", "0110001");
    insert_ht_item(ht, "-D", "0001111");
    insert_ht_item(ht, "-A", "0110011");
    insert_ht_item(ht, "D+1", "0011111");
    insert_ht_item(ht, "A+1", "0110111");
    insert_ht_item(ht, "D-1", "0001110");
    insert_ht_item(ht, "A-1", "0110010");
    insert_ht_item(ht, "D+A", "0000010");
    insert_ht_item(ht, "D-A", "0010011");
    insert_ht_item(ht, "A-D", "0000111");
    insert_ht_item(ht, "D&A", "0000000");
    insert_ht_item(ht, "D|A", "0010101");

    insert_ht_item(ht, "M", "1110000");
    insert_ht_item(ht, "!M", "1110001");
    insert_ht_item(ht, "-M", "1110011");
    insert_ht_item(ht, "M+1", "1110111");
    insert_ht_item(ht, "M-1", "1110010");
    insert_ht_item(ht, "D+M", "1000010");
    insert_ht_item(ht, "D-M", "1010011");
    insert_ht_item(ht, "M-D", "1000111");
    insert_ht_item(ht, "D&M", "1000000");
    insert_ht_item(ht, "D|M", "1010101");
    comp_ht = ht;
}

static void init_symbol_ht() {
    hash_table* ht = new_hash_table(4096);
    insert_ht_item(ht, "SP", "000000000000000");
    insert_ht_item(ht, "LCL", "000000000000001");
    insert_ht_item(ht, "ARG", "000000000000010");
    insert_ht_item(ht, "THIS", "000000000000011");
    insert_ht_item(ht, "THAT", "000000000000100");
    insert_ht_item(ht, "R0", "000000000000000");
    insert_ht_item(ht, "R1", "000000000000001");
    insert_ht_item(ht, "R2", "000000000000010");
    insert_ht_item(ht, "R3", "000000000000011");
    insert_ht_item(ht, "R4", "000000000000100");
    insert_ht_item(ht, "R5", "000000000000101");
    insert_ht_item(ht, "R6", "000000000000110");
    insert_ht_item(ht, "R7", "000000000000111");
    insert_ht_item(ht, "R8", "000000000001000");
    insert_ht_item(ht, "R9", "000000000001001");
    insert_ht_item(ht, "R10", "000000000001010");
    insert_ht_item(ht, "R11", "000000000001011");
    insert_ht_item(ht, "R12", "000000000001100");
    insert_ht_item(ht, "R13", "000000000001101");
    insert_ht_item(ht, "R14", "000000000001110");
    insert_ht_item(ht, "R15", "000000000001111");
    insert_ht_item(ht, "SCREEN", "100000000000000");
    insert_ht_item(ht, "KBD", "110000000000000");
    symbol_ht = ht;
}

void init_command_ht() {
    init_comp_ht();
    init_jump_ht();
    init_dest_ht();
    init_symbol_ht();
}

void free_command_ht() {
    free_ht(comp_ht);
    free_ht(jump_ht);
    free_ht(dest_ht);
    free_ht(symbol_ht);
}
