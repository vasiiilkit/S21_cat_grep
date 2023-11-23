#include "s21_cat.h"

int main(int argc, char **argv) {
  int ret = 0;
  flag fl = {0};
  if ((set_flag(argc, argv, &fl)) == 1) {
    ret = 1;
  } else {
    init_file(argc - optind, argv + optind, &fl);
  }
  return ret;
}

static int set_flag(int argc, char **argv, flag *fl_ag) {
  int ret = 0;
  char *short_par = "beEnstTv";
  if ((par_long(argc, argv, short_par, fl_ag)) == 1) {
    ret = 1;
  }
  return ret;
}

static int par_long(int argc, char **argv, char *short_par, flag *fl) {
  int res = 0;
  int index = 0;
  int flag;
  int error = 0;
  while ((flag = getopt_long(argc, argv, short_par, long_par, &index)) != -1) {
    switch (flag) {
      case 'b':
        fl->b = 1;
        fl->n = 0;
        break;
      case 'e':
        fl->e = 1;
        fl->v = 1;
        break;
      case 'E':
        fl->e = 1;
        break;
      case 'n':
        if ((fl->b) != 1) {
          fl->n = 1;
        }
        break;
      case 's':
        fl->s = 1;
        break;
      case 't':
        fl->t = 1;
        fl->v = 1;
        break;
      case 'T':
        fl->t = 1;
        break;
      case 'v':
        fl->v = 1;
        break;
      case '?':
        error = 1;
        break;
      default:
        error = 1;
        break;
    }
  }
  if (error) {
    res = 1;
  }
  return res;
}

static void init_file(int count_f, char **file_path, flag *fl) {
  FILE *fil = NULL;
  while (count_f > 0) {
    fil = fopen(*file_path, "r");
    if (fil == NULL) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", *file_path);
    } else {
      print_file(fil, fl);
    }
    count_f--;
    file_path++;
  }
  fclose(fil);
}

static void print_file(FILE *file, flag *fl_ag) {
  static char prev_char = '\n';
  static unsigned int line_count = 1;
  char current_symbol = fgetc(file);
  while (current_symbol != EOF) {
    if ((fl_ag->s) == 1) {
      count_s_flag(current_symbol, &line_count);
    }
    if (current_symbol != '\n' || line_count <= 2) {
      begin_line(prev_char, current_symbol, fl_ag);
      end_e(current_symbol, fl_ag);
      base(current_symbol, fl_ag);
    }
    prev_char = current_symbol;
    current_symbol = fgetc(file);
  }
}

static void count_s_flag(const char current_symbol,
                         unsigned *const line_count) {
  if (current_symbol == '\n') {
    *line_count += 1;
  } else {
    *line_count = 0;
  }
}

static void begin_line(char prev_char, char current_symbol, flag *fl_ag) {
  static unsigned line_number = 0;
  if ((fl_ag->b) == 1 && prev_char == '\n' && current_symbol != '\n') {
    printf("%6u\t", ++line_number);
  } else if ((fl_ag->n) == 1 && prev_char == '\n') {
    printf("%6u\t", ++line_number);
  }
}

static void end_e(char current_symbol, flag *fl_ag) {
  if ((fl_ag->e) == 1 && current_symbol == '\n') {
    printf("%c", '$');
  }
}

static void base(char current_symbol, flag *fl_ag) {
  if (isprint(current_symbol)) {
    printf("%c", current_symbol);
  } else if (current_symbol == '\t') {
    if ((fl_ag->t) == 1) {
      printf("^I");
    } else {
      printf("%c", current_symbol);
    }
  } else if (current_symbol == '\n') {
    printf("%c", current_symbol);
  } else {
    flag_v(current_symbol, fl_ag);
  }
}

static void flag_v(char current_char, flag *fl_ag) {
  if (iscntrl(current_char)) {
    if ((fl_ag->v) == 1) {
      flag_v_uprav(current_char);
    } else {
      printf("%c", current_char);
    }
  } else {
    if ((fl_ag->v) == 1) {
      char meta = current_char + 127;
      printf("%s", "M-");
      if (isprint(meta)) {
        printf("%c", meta);
      } else {
        flag_v_uprav(meta);
      }
    } else {
      printf("%c", current_char);
    }
  }
}

static void flag_v_uprav(char current_char) {
  if (current_char == 127) {
    current_char -= 64;
    printf("^");
    printf("%c", current_char);
  } else {
    current_char += 64;
    printf("^");
    printf("%c", current_char);
  }
}
