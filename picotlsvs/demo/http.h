#if !defined(__HTTP_H__)
#define __HTTP_H__

#include <stdint.h>
#include "slice.h"

#define CONTENT_LENGTH "Content-Length"

typedef enum {
	PARSE_FIRSTLINE,
	PARSE_HEADERS,
	PARSE_BODY,
} http_parse_step_e;

typedef struct http_ctx {
    http_parse_step_e parse_step;
    int status;
    size_t len;
    void (*header_cb)(struct http_ctx *ctx, const slice_t *k, const slice_t *v);
    void *ctx;
} http_ctx_t;

void http_parse_response(http_ctx_t *ctx, const uint8_t *ptr, size_t *len);

#endif