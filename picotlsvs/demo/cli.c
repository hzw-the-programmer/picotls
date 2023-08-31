#include <winsock2.h>
#include <stdio.h>
#include "picotls.h"
#include "picotls/minicrypto.h"
#include "http.h"

#define DEFAULT_BUFLEN 1024

#define REQUEST "GET / HTTP/1.1\r\n" \
                "Host: localhost\r\n" \
                "\r\n"

void process(SOCKET soc);

int cli_test(int argc, char* argv[]) {
	int err;

	WSADATA wsa;
	err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (err != 0) {
		printf("WSAStartup failed: %d\n", err);
		return 1;
	}

	struct addrinfo hints = {0}, * result = NULL, * ptr = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	err = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (err != 0) {
		printf("getaddrinfo failed: %d\n", err);
		WSACleanup();
		return 1;
	}

	SOCKET soc = INVALID_SOCKET;

	ptr = result;
	soc = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (soc == INVALID_SOCKET) {
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	err = connect(soc, ptr->ai_addr, ptr->ai_addrlen);
	if (err == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(soc);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	process(soc);

	closesocket(soc);
	WSACleanup();

	return 0;
}

void process(SOCKET soc) {
	ptls_context_t ctx = { 0 };
	ctx.random_bytes = ptls_minicrypto_random_bytes;
	ctx.get_time = &ptls_get_time;
	ctx.key_exchanges = ptls_minicrypto_key_exchanges;
	ctx.cipher_suites = ptls_minicrypto_cipher_suites;

	ptls_t *tls = ptls_client_new(&ctx);

	ptls_buffer_t sendbuf;
	ptls_buffer_init(&sendbuf, "", 0);
	
	ptls_handshake(tls, &sendbuf, NULL, 0, NULL);

	ptls_buffer_t recvbuf;
	ptls_buffer_init(&recvbuf, "", 0);

	http_ctx_t http_ctx = {0};
	int req_count = 0;

	while (1) {
		int result;
		
		while (sendbuf.off) {
			result = send(soc, sendbuf.base, sendbuf.off, 0);
			if (result > 0) {
				ptls_buffer_shift(&sendbuf, result);
			}
			else {
				goto end;
			}
		}

		uint8_t buf[DEFAULT_BUFLEN];

		result = recv(soc, buf, DEFAULT_BUFLEN, 0);
		if (result > 0) {
			size_t len = result, left = 0, off = 0;
				
			while ((left = len - off) > 0) {
				if (!ptls_handshake_is_complete(tls)) {
					result = ptls_handshake(tls, &sendbuf, buf, &left, NULL);
					if (result == 0) {
						printf("req_count=%d\n", req_count++);
						ptls_send(tls, &sendbuf, REQUEST, sizeof(REQUEST) - 1);
					}
				}
				else {
					result = ptls_receive(tls, &recvbuf, buf + off, &left);
					if (result == 0) {
                        if (recvbuf.off) {
                            for (size_t i = 0; i < recvbuf.off; i++) {
                                printf("%c", recvbuf.base[i]);
							}
                            size_t consumed = recvbuf.off;
							if (http_parse_response(&http_ctx, recvbuf.base, &consumed)) {
                                memset(&http_ctx, 0, sizeof(http_ctx));
                                if (req_count < 3) {
                                    printf("req_count=%d\n", req_count++);
                                    ptls_send(tls, &sendbuf, REQUEST, sizeof(REQUEST) - 1);
								}
							}
                            ptls_buffer_shift(&recvbuf, consumed);
						}
					}
				}
				off += left;
			}
		}
		else {
			goto end;
		}
	}

end:
	ptls_buffer_dispose(&sendbuf);
	ptls_buffer_dispose(&recvbuf);
	ptls_free(tls);
}