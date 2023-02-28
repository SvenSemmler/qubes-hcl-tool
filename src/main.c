/******************************************************************************
 Qubes OS Hardware Compatibility List Tool
 Reads HCL reports and exports CSV.

 Module: main() function and options handling
 Author: Sven Semmler (Sven@SvenSemmler.org)
 ******************************************************************************/

#include "opts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const char *PROG_NAME_STR = "qubes-hcl-tool";
static const char *PROG_DESC_STR = "Reads HCL reports and exports CSV.\n";
static const char *PROG_VER_STR  = "v0.0.1";

static bool assign_input(/*@null@*/ const char *filename_str);
static bool assign_output(/*@null@*/ const char *filename_str);
static bool print_help(/*@null@*/ const char *param_str);
static bool print_version(/*@null@*/ const char *param_str);

static struct OPT OPTS[] = {

  { "[-i inputfile]",  
    "input file to be used (default: stdin)",
    assign_input },

  { "[-o outputfile]", 
    "output file to be used (default: stdout)",
    assign_output },

  { "[-h]",            
    "show help",
    print_help },
  
  {"[-v]",            
   "show version",
   print_version },

};

static struct IN {

  /*@dependent@*/ FILE *file_ptr;

} in;

static struct OUT {

  /*@dependent@*/ FILE *file_ptr;

} out;

int main(int   argc, 
         char *argv[]) {
  
  in.file_ptr  = stdin;
  out.file_ptr = stdout;
  
  if(!parse_opts(argc, argv, OPTS, OPT_ROWS(OPTS))) {

    print_usage(PROG_NAME_STR, OPTS, OPT_ROWS(OPTS));
    exit(EXIT_FAILURE);
  }

  (void) fclose(in.file_ptr);
  (void) fclose(out.file_ptr);

  return EXIT_SUCCESS;
}

static bool assign_input(/*@null@*/ const char *filename_str) {

  if(NULL != filename_str) {

    FILE *file_ptr = fopen(filename_str, "rb+");

    if(NULL != file_ptr) {

      in.file_ptr = file_ptr;
      return true;
    }
  }
  
  return false;
}

static bool assign_output(/*@null@*/ const char *filename_str) {

  if (NULL != filename_str) {

    FILE *file_ptr = fopen(filename_str, "w");

    if(NULL != file_ptr) {

      out.file_ptr = file_ptr;
      return true;
    }
  }
  
  return false;
}

static bool print_help(/*@null@*/ const char *param_str) {

  assert(NULL == param_str);
  print_desc(PROG_NAME_STR, PROG_DESC_STR, OPTS, OPT_ROWS(OPTS));
  exit(EXIT_SUCCESS);
}

static bool print_version(/*@null@*/ const char *param_str) {

  assert(NULL == param_str);
/* https://sourceforge.net/p/mingw-w64/bugs/868/ */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
  (void) printf("%s\n", PROG_VER_STR);
#pragma clang diagnostic pop  
  exit(EXIT_SUCCESS);
}
