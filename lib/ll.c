#include "libflipper.h"

size_t lf_ll_count(struct _lf_ll *ll) {
    size_t count = 0;
    while (ll) {
        ll = ll->next;
        count++;
    }
    return count;
}

int lf_ll_append(struct _lf_ll **_ll, void *item, void (*deconstructor)(void *)) {
    if (!_ll) {
        lf_debug("[lf_ll_append] Error: _ll is NULL");
        goto fail;
    }

    struct _lf_ll *new = malloc(sizeof(struct _lf_ll));
    if (!new) {
        lf_debug("[lf_ll_append] Error: malloc failed");
        goto fail;
    }

    memset(new, 0, sizeof(struct _lf_ll));
    new->item = item;
    new->deconstructor = deconstructor;

    struct _lf_ll *head = *_ll;
    if (head) {
        while (head->next) head = head->next;
        head->next = new;
    } else {
        *_ll = new;
    }

    lf_debug("[lf_ll_append] Appended item %p to list at %p", item, *_ll);
    return lf_success;

fail:
    lf_debug("[lf_ll_append] Failed to append item %p", item);
    return lf_error;
}

int lf_ll_concat(struct _lf_ll **_lla, struct _lf_ll *_llb) {
    lf_assert(_lla, E_NULL, "invalid list reference");

    struct _lf_ll *head = *_lla;
    if (head) {
        while (head->next) head = head->next;
        head->next = _llb;
    } else {
        *_lla = _llb;
    }

    return lf_success;
fail:
    return lf_error;
}

void *lf_ll_item(struct _lf_ll *ll, size_t index) {
    while (index-- && ll) ll = ll->next;
    lf_assert(ll, E_NULL, "End of list reached before item.");

    return ll->item;
fail:
    return NULL;
}

// Helper function to free a single linked list node without modifying external pointers
static void lf_ll_free_node(struct _lf_ll *ll) {
    if (!ll) return;
    if (ll->deconstructor) ll->deconstructor(ll->item);
    free(ll);
}

void lf_ll_free(struct _lf_ll **_ll) {
    struct _lf_ll *ll = *_ll;
    if (!ll) return;
    *_ll = ll->next;
    lf_ll_free_node(ll);
}

void lf_ll_remove(struct _lf_ll **_ll, void *item) {
    lf_assert(_ll, E_NULL, "invalid list reference");

    struct _lf_ll *prev = NULL;
    struct _lf_ll *current = *_ll;

    while (current) {
        if (current->item == item) {
            if (prev) {
                prev->next = current->next;
            } else {
                *_ll = current->next;
            }
            lf_ll_free_node(current);
            current = prev ? prev->next : *_ll;
        } else {
            prev = current;
            current = current->next;
        }
    }

fail:
    return;
}

int lf_ll_apply_func(struct _lf_ll *ll, lf_ll_applier_func func, void *_ctx) {
    lf_assert(ll, E_NULL, "invalid list");
    lf_assert(func, E_NULL, "invalid applier function");

    int e = lf_success;
    do {
        e = func(ll->item, _ctx);
        if (e != lf_success) break;
    } while ((ll = ll->next));

    return e;
fail:
    return lf_error;
}

int lf_ll_release(struct _lf_ll **_ll) {
    lf_assert(_ll, E_NULL, "invalid list reference");
    while (*_ll) {
        struct _lf_ll *next = (*_ll)->next;
        lf_ll_free_node(*_ll);
        *_ll = next;
    }

    return lf_success;
fail:
    return lf_error;
}

struct _lf_ll *lf_ll_create(void) {
    return NULL;  // Empty list
}
