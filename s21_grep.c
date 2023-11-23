#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER 10000



typedef struct {
    bool e;
    bool i;
    bool v;
    bool c;
    bool l;
    bool n;
    bool f;
    bool h;
    bool s;
    bool o;
    char comp_string[BUFFER];
    char line_from_file[BUFFER];
    char filename[BUFFER];
} Flag;

void parser(int argc, char **argv, Flag *flag);
void output(char **argv, Flag *flag, int *flag_printable_name);
void print_match_only(Flag *flag, regmatch_t *start, regex_t *reg);


int main(int argc, char **argv) {
  Flag flag = {0};
  int flag_printable_name = 0;
  parser(argc, argv, &flag);
  while (optind < argc) {
    if (argc - optind > 1) {
        flag_printable_name = 1;
        }
    print_file(argv, &flag, &flag_printable_name);
    optind++;
  }

  return 0;
}

void parser(int argc, char **argv, Flag *flag) {
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "e:civlnhsf:o", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        flag->e = true;
        strcat(flag->comp_string, optarg);
        strcat(flag->comp_string, "|");
        break;
      case 'c':
        flag->c = true;
        break;
      case 'i':
        flag->i = true;
        break;
      case 'v':
        flag->v = true;
        break;
      case 'l':
        flag->l = true;
        break;
      case 'n':
        flag->n = true;
        break;
      case 'h':
        flag->h = true;
        break;
      case 's':
        flag->s = true;
        break;
      case 'o':
        if (flag->v != true) {
          flag->o = true;
        }
        break;
      default:
        break;
    }
  }
  if (flag->e == false) {
    if (argc > optind) {
      strcat(flag->comp_string, argv[optind]);
    }
    optind++;
  }
  if (flag->e == true) {
    flag->comp_string[strlen(flag->comp_string) - 1] = '\0';
  }
}

void print_file(char **argv, Flag *flag, int *flag_printable_name) {
    FILE *file;
    regmatch_t start;
    int count_lines = 0;
    int match = 0;
    file = fopen(argv[optind],"r");
    if(file == NULL){
        if (flag -> s == false) {
            printf("grep: %s: No such file or directory\n", argv[optind]);
        }
        return;
    }else{
        regex_t reg;
        int valve_flag = REG_EXTENDED;
        if (flag -> i == true) {
            valve_flag |= REG_ICASE;
        }

        regcomp(&reg,flag -> comp_string,valve_flag);
        while ((fgets(flag->line_from_file,BUFFER,file)) != NULL) {
            count_lines++;
            int compare = regexec(&reg,flag->line_from_file,1,&start,0);
            if (compare == 0){
                match++;
            }
            if(flag -> v){
                compare = !compare;
            }
        }
        
        

    }
}
