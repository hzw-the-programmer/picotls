CFLAGS += --c99 --gnu

MINCPATH  += DAPS/source/picotls/deps/cifra/src \
	DAPS/source/picotls/deps/cifra/src/ext \
	DAPS/source/picotls/deps/micro-ecc \
	DAPS/source/picotls/include

MSRCPATH += DAPS/source/picotls/deps/cifra/src \
	DAPS/source/picotls/deps/micro-ecc \
	DAPS/source/picotls/lib \
	DAPS/source/picotls/lib/cifra

SOURCES += uECC.c
SOURCES += aes.c
SOURCES += blockwise.c
SOURCES += chacha20.c
SOURCES += chash.c
SOURCES += curve25519.c
SOURCES += drbg.c
SOURCES += hmac.c
SOURCES += gcm.c
SOURCES += gf128.c
SOURCES += modes.c
SOURCES += poly1305.c
SOURCES += sha256.c
SOURCES += sha512.c

SOURCES += cifra.c
SOURCES += x25519.c
SOURCES += chacha20_wrapper.c
SOURCES += aes128.c
SOURCES += aes256.c
SOURCES += random.c
SOURCES += minicrypto-pem.c
SOURCES += uecc_wrapper.c
SOURCES += asn1.c
SOURCES += ffx.c

SOURCES += hpke.c
SOURCES += picotls.c
SOURCES += pembase64.c