#include "http.h"

int http_parse_response(http_ctx_t* ctx, const uint8_t* ptr, size_t* len)
{
    slice_t s, line;

    s = slice_new(ptr, *len);
    *len = 0;
    
    if (ctx->parse_step == PARSE_BODY) {
    parse_body:
        if (s.len > ctx->len) {
            s.len = ctx->len;
        }
        ctx->len -= s.len;
        *len += s.len;
        
        if (ctx->body_cb && s.len) {
            ctx->body_cb(ctx, &s);
        }
        
        return ctx->len == 0;
    }

    while (slice_read_line(&s, &line) == 0) {
        *len += s.ptr - line.ptr;

        if (ctx->parse_step == PARSE_FIRSTLINE) {
            slice_t version, status, reason;

            reason = line;
            slice_read_until(&reason, ' ', &version);
            slice_read_until(&reason, ' ', &status);
            ctx->status = slice_to_uint64(&status);
            ctx->parse_step = PARSE_HEADERS;
            
            if (ctx->firstline_cb) {
                ctx->firstline_cb(ctx, &line, &version, &status, &reason);
            }
        } else {
            slice_t key, value, tmp;

            value = line;
            slice_read_until(&value, ':', &key);
            slice_ltrim_space(&value);

            tmp = slice_new(CONTENT_LENGTH, sizeof(CONTENT_LENGTH) - 1);
            if (slice_cmp(&tmp, &key) == 0) {
                ctx->len = slice_to_uint64(&value);
            }

            if (ctx->header_cb) {
                ctx->header_cb(ctx, &line, &key, &value);
            }

            if (line.len == 0) {
                ctx->parse_step = PARSE_BODY;
                goto parse_body;
            }
        }
    }

    return 0;
}