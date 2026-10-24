#include <linc/trie.h>
#include <linc/xsys.h>

#include <stddef.h>

#define TRIE_CHILD_MAX 256

struct linc_trie {
    linc_trie *children[TRIE_CHILD_MAX];
    size_t child_n;
    bool term;
    void *data; /* valid if term is true */
};

linc_trie *linc_trie_create(void) {
    linc_trie *tr = xcalloc(1, sizeof(linc_trie));

    return tr;
}

void linc_trie_destroy(linc_trie *tr) {
    for (size_t i = 0; i < LINC_TRIE_N; ++i)
        if (tr->children[i])
            linc_trie_destroy(tr->children[i]);

    if (tr->data)
        free(tr->data);

    free(tr);
    tr = nullptr;
}

void linc_trie_add(linc_trie *tr, const char *s, void *data, size_t sz) {
    assert(tr);
    assert(s);

    if (s[0] == '\0') {
        tr->term = true;

        if (sz > 0) {
            tr->data = xmalloc(sz);
            memcpy(tr->data, data, sz);
        }

        return;
    }

    const uint8_t ch = s[0];

    if (!tr->children[ch]) {
        tr->children[ch] = linc_trie_create();
        ++tr->child_n;
    }

    linc_trie_add(tr->children[ch], &s[1], data, sz);
}

void *linc_trie_lookup(linc_trie *tr, const char *s, linc_trie_status *stat) {
    assert(tr);
    assert(s);
    assert(stat);

    const uint8_t ch = s[0];

    if (ch == '\0') {
        if (tr->term && tr->child_n == 0) {
            *stat = TRIE_MATCH;
        } else if (tr->term && tr->child_n > 0) {
            *stat = TRIE_AMBIG;
        } else if (!tr->term && tr->child_n > 0) {
            *stat = TRIE_WAIT;
        } else if (!tr->term && tr->child_n == 0)
            *stat = TRIE_NONE;

        return tr->data;
    }

    if (!tr->children[ch]) {
        *stat = TRIE_NONE;
        return NULL;
    }

    return linc_trie_lookup(tr->children[ch], &s[1], stat);
}
