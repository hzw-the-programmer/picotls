#include <assert.h>
#include "http.h"

static void test_1()
{
    char buf[] = "HTTP/1.1 ";
    size_t len = sizeof(buf) - 1;
    size_t consumed = 0;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == consumed);
}

static void test_2()
{
    char buf[] = "HTTP/1.1 200 OK\r";
    size_t len = sizeof(buf) - 1;
    size_t consumed = 0;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == consumed);
}

static void test_3()
{
    char buf[] = "HTTP/1.1 200 OK\r\n";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = len;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
}

static void test_4()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH;
    size_t len = sizeof(buf) - 1;
    size_t cosumed = 17;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
}

static void test_5()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = 17;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
}

static void test_6()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = len;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 3);
}

static void test_7()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n\r";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = 36;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 3);
}

static void test_8()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n\r\n";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = len;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 3);
}

static void test_9()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n\r\nh";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = len;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 2);
}

static void test_10()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n\r\nhz";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = len;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 1);
}

static void test_11()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n\r\nhzw";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = len;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 0);
}

static void test_12()
{
    char buf[] = "HTTP/1.1 200 OK\r\n" CONTENT_LENGTH ": 3\r\n\r\nhzwHTTP/1.1 ";
    size_t len = sizeof(buf) - 1;
    size_t cosumed = 41;
    http_ctx_t ctx = {0};

    http_parse_response(&ctx, buf, &len);
    assert(len == cosumed);
    assert(ctx.status == 200);
    assert(ctx.len == 0);
}

int http_test(int argc, char *argv[])
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    test_11();
    test_12();
}