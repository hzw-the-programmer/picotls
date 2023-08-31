#include "http.h"

void http_parse_response(http_ctx_t* ctx, const uint8_t* ptr, size_t* len)
{
    slice_t s, l, k, t;

    s = slice_new(ptr, *len);
    *len = 0;
    
    if (ctx->parse_step == PARSE_BODY) {
    parse_body:
        if (s.len > ctx->len) {
            s.len = ctx->len;
        }
        ctx->len -= s.len;
        *len += s.len;
        return;
    }

    while (slice_read_line(&s, &l) == 0) {
        *len += s.ptr - l.ptr;
        
        if (l.len == 0) {
            ctx->parse_step = PARSE_BODY;
            goto parse_body;
        }

        if (ctx->parse_step == PARSE_FIRSTLINE) {
            slice_read_until(&l, ' ', &t);
            slice_ltrim_space(&l);
            ctx->status = slice_to_uint64(&l);
            ctx->parse_step = PARSE_HEADERS;
            continue;
        }

        slice_read_until(&l, ':', &k);
        slice_ltrim_space(&l);

        t = slice_new(CONTENT_LENGTH, sizeof(CONTENT_LENGTH) - 1);
        if (slice_cmp(&t, &k) == 0) {
            ctx->len = slice_to_uint64(&l);
        }
        
        if (ctx->header_cb) {
            ctx->header_cb(ctx, &l, &t);
        }
    }
}