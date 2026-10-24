#ifndef LINC_TRIE_H
#define LINC_TRIE_H

#include <linc.h>

#include <stddef.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define linc_trie_put(tr, s) \
    trie_add(tr, s, NULL, 0)

#define LINC_TRIE_N 256

typedef enum {
    TRIE_MATCH,
    TRIE_AMBIG,
    TRIE_NONE,
    TRIE_WAIT,
} linc_trie_status;

typedef struct linc_trie linc_trie;

linc_trie *linc_trie_create(void);
void linc_trie_destroy(linc_trie *tr);
void linc_trie_add(linc_trie *tr, const char *s, void *data, size_t sz) ;
void *linc_trie_lookup(linc_trie *tr, const char *s, linc_trie_status *stat);

#endif
