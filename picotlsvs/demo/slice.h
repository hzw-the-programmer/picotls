#if !defined(__SLICE_H__)
#define __SLICE_H__

#include <stdint.h>

#define SLICE_INVALID_INDEX SIZE_MAX
#define SPACES "\t\n\v\f\r "
#define SPACES_LEN 6

typedef struct slice{
    uint8_t *ptr;
    size_t len;
} slice_t;

slice_t slice_new(uint8_t *ptr, size_t len);
size_t slice_search(const slice_t *s, uint8_t b);
void slice_ltrim(slice_t *s, const slice_t *cutset);
void slice_rtrim(slice_t *s, const slice_t *cutset);
void slice_trim(slice_t *s, const slice_t *cutset);
void slice_ltrim_space(slice_t *s);
void slice_rtrim_space(slice_t *s);
void slice_trim_space(slice_t *s);
uint64_t slice_to_uint64(const slice_t *s);
int32_t slice_read_until(slice_t *s, uint8_t b, slice_t *out);
int32_t slice_read_line(slice_t *s, slice_t *l);
int32_t slice_cmp(const slice_t *s1, const slice_t *s2);

#endif // __SLICE_H__