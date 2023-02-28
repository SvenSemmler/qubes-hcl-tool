/******************************************************************************
 Module: table based options handling
 Author: Sven Semmler (Sven@SvenSemmler.org)
 ******************************************************************************/

#include "opts.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static inline size_t trim(const char **str_ptr);

bool parse_opts(      int         argc, 
                      char       *argv[],
                const struct OPT  opts[],
                const uint8_t     num_opts) {

  int arg;

  assert(   2 <= argc     &&
         NULL != argv     && 
         NULL != opts     &&
            1 <= num_opts );

  for(arg=1; arg < argc; arg++) {

    const char *curr_str = argv[arg];
    size_t      curr_len = trim(&curr_str);

    assert(0 != curr_len);
    if(1 < curr_len && '-' == curr_str[0]) {

      uint16_t opt;

      curr_str++;
      curr_len--;

      for(opt = 0; opt < num_opts; opt++) {

        uint16_t pos;
        size_t   id_len;

        assert(NULL != opts[opt].id_str       && 
               NULL != opts[opt].desc_str     &&
               NULL != opts[opt].handler_fptr );
        
        pos    = 1;
        id_len = strlen(opts[opt].id_str);
        while(  pos < id_len-1               && 
              ('[' == opts[opt].id_str[pos]  || 
               '-' == opts[opt].id_str[pos]) )
          pos++;

        if(opts[opt].id_str[pos] == curr_str[0]) {

          if(id_len > pos+1 && 
             ']' == opts[opt].id_str[pos+1])
            pos++;

          if(pos+1 != id_len) {

            if(1 < curr_len)
              curr_str++;
            else if(arg < argc-1)
              curr_str = argv[++arg];
            else
              return false;
            curr_len = trim(&curr_str);

          } else
            curr_str = NULL;

          if(false == opts[opt].handler_fptr(curr_str))
            return false;
          else
            break;
        }
      }
      if(opt == num_opts)
        return false;
    } else
      return false;
  }
  return true;
}

static inline size_t trim(const char **str_ptr) {

  size_t len = 0;

  assert(NULL != str_ptr  &&
         NULL != *str_ptr );

  len = strlen(*str_ptr);
  while(0 != len && ' ' == *str_ptr[0]) {

    (*str_ptr)++;
    len--;
  }   
  return len;
}

void print_usage(const char       *name_str,
                 const struct OPT  opts[],
                 const uint8_t     num_opts) {

  uint16_t opt;

  assert(NULL != name_str &&
         NULL != opts     &&
            1 <= num_opts );

/* https://sourceforge.net/p/mingw-w64/bugs/868/ */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
  (void) printf("Usage: %s", name_str);
  for(opt = 0; opt < num_opts; opt++)
    (void) printf(" %s", opts[opt].id_str);
  (void) printf("\n");
#pragma clang diagnostic pop
}

void print_desc(const char       *name_str,
                const char       *desc_str,
                const struct OPT  opts[],
                const uint8_t     num_opts) {
  
  uint16_t opt = 0;
  size_t   len, usage_len = 0;

  assert(NULL != name_str &&
         NULL != desc_str &&
         NULL != opts     &&
            1 <= num_opts );

  print_usage(name_str, opts, num_opts);
/* https://sourceforge.net/p/mingw-w64/bugs/868/ */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
  (void) printf("%s\n", desc_str);
#pragma clang diagnostic pop
  for(opt = 0; opt < num_opts; opt++) {

    len = strlen(opts[opt].id_str);
    if(len > usage_len)
      usage_len = len;
  }

/* https://sourceforge.net/p/mingw-w64/bugs/868/ */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
  for(opt = 0; opt < num_opts; opt++) {
  
    uint16_t spaces = 0;

    (void) printf("  %s", opts[opt].id_str);
    len = strlen(opts[opt].id_str);
    for(; spaces < (usage_len - len + 2); spaces++)
      (void) printf(" ");
    (void) printf("%s\n", opts[opt].desc_str); 
  }  
  (void) printf("\n");
#pragma clang diagnostic pop
}
