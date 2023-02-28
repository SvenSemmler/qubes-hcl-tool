/******************************************************************************
 Module: table based options handling
 Author: Sven Semmler (Sven@SvenSemmler.org)
 ******************************************************************************/
#ifndef OPTS_H
#define OPTS_H

#include <stdint.h>
#include <stdbool.h>

#define OPT_ROWS(tbl) (sizeof tbl / sizeof(struct OPT))

typedef bool (*HANDLER_FPTR)(/*@null@*/ const char *str);

struct OPT {
  
             const char         *id_str;
             const char         *desc_str;
  /*@null@*/ const HANDLER_FPTR  handler_fptr;

};

bool parse_opts(      int         argc, 
                      char       *argv[],
                const struct OPT  opts[],
                const uint8_t     num_opts);

void print_usage(const char       *name_str,
                 const struct OPT  opts[],
                 const uint8_t     num_opts);

void print_desc(const char       *name_str,
                const char       *desc_str,
                const struct OPT  opts[],
                const uint8_t     num_opts);

#endif /* OPTS_H */
