#ifndef REPL_H
#define REPL_H

void welcome_msg(void);
void goodbye_msg(void);

int is_EOF(char *input);
int is_input_length_valid(char *input);
void trim_input(char *input);
int is_exit_command(char *input);

int repl(char *in_stream, int max_input_len);

#endif