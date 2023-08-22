#include "slice.h"

slice_t slice_new(uint8_t *ptr, size_t len)
{
    slice_t s;

    s.ptr = ptr;
    s.len = len;

    return s;
}

size_t slice_search(const slice_t *s, uint8_t b)
{
    size_t i;

    for (i = 0; i < s->len; i++) {
        if (s->ptr[i] == b) {
            return i;
        }
    }

    return SLICE_INVALID_INDEX;
}

void slice_ltrim(slice_t *s, const slice_t *cutset)
{
    while (s->len > 0) {
        if (slice_search(cutset, s->ptr[0]) == SLICE_INVALID_INDEX) {
            break;
        }

        s->ptr++;
        s->len--;
    }
}

void slice_rtrim(slice_t *s, const slice_t *cutset)
{
    while (s->len > 0) {
        if (slice_search(cutset, s->ptr[s->len - 1]) == SLICE_INVALID_INDEX) {
            break;
        }

        s->len--;
    }
}

void slice_trim(slice_t *s, const slice_t *cutset)
{
    slice_ltrim(s, cutset);
    slice_rtrim(s, cutset);
}

void slice_ltrim_space(slice_t *s)
{
    slice_t cutset;

    cutset = slice_new(SPACES, SPACES_LEN);
    slice_ltrim(s, &cutset);
}

void slice_rtrim_space(slice_t *s)
{
    slice_t cutset;

    cutset = slice_new(SPACES, SPACES_LEN);
    slice_rtrim(s, &cutset);
}

void slice_trim_space(slice_t *s)
{
    slice_t cutset;

    cutset = slice_new(SPACES, SPACES_LEN);
    slice_trim(s, &cutset);
}

uint64_t slice_to_uint64(const slice_t *s)
{
    uint64_t n = 0;
    size_t i;

    for (i = 0; i < s->len; i++) {
        if (s->ptr[i] < '0' || s->ptr[i] > '9') {
            break;
        }

        n *= 10;
        n += s->ptr[i] - '0';
    }

    return n;
}

int32_t slice_read_until(slice_t *s, uint8_t b, slice_t *out)
{
    size_t i;

    i = slice_search(s, b);

    if (i == SLICE_INVALID_INDEX) {
        *out = *s;
        s->ptr += s->len;
        s->len = 0;
        return -1;
    }

    out->ptr = s->ptr;
    out->len = i;

    s->ptr += i + 1;
    s->len -= i + 1;

    return 0;
}

int32_t slice_read_line(slice_t *s, slice_t *l)
{
    int32_t ret;

    ret = slice_read_until(s, '\n', l);
    if (l->len > 0 && l->ptr[l->len - 1] == '\r') {
        l->len--;
    }

    return ret;
}

int32_t slice_cmp(const slice_t *s1, const slice_t *s2)
{
    if (s1->len == s2->len) {
        return memcmp(s1->ptr, s2->ptr, s1->len);
    }

    return (s1->len - s2->len) > 0 ? 1 : -1;
}
