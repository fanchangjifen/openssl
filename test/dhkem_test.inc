/*
 * Copyright 2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

typedef struct {
    const char *curvename;
    /* seed */
    const unsigned char *ikm;
    size_t ikmlen;
    /* expected public key */
    const unsigned char *pub;
    size_t publen;
    /* expected private key */
    const unsigned char *priv;
    size_t privlen;
} TEST_DERIVEKEY_DATA;

typedef struct {
    const char *curve;
    /* The seed for the senders ephemeral key */
    const unsigned char *ikmE;
    size_t ikmElen;
    /* Recipient key */
    const unsigned char *rpub;
    size_t rpublen;
    const unsigned char *rpriv;
    size_t rprivlen;
    /* The senders generated ephemeral public key */
    const unsigned char *expected_enc;
    size_t expected_enclen;
    /* The generated shared secret */
    const unsigned char *expected_secret;
    size_t expected_secretlen;
    /* Senders Auth key */
    const unsigned char *spub;
    size_t spublen;
    const unsigned char *spriv;
    size_t sprivlen;
} TEST_ENCAPDATA;

static const char *dhkem_supported_curves[] = {
    "P-256",
    "P-384",
    "P-521",
    "X25519",
    "X448"
};

/* TEST vectors extracted from RFC 9180 */

/* Base test values */
static const unsigned char x25519_ikme[] = {
    0x72, 0x68, 0x60, 0x0d, 0x40, 0x3f, 0xce, 0x43,
    0x15, 0x61, 0xae, 0xf5, 0x83, 0xee, 0x16, 0x13,
    0x52, 0x7c, 0xff, 0x65, 0x5c, 0x13, 0x43, 0xf2,
    0x98, 0x12, 0xe6, 0x67, 0x06, 0xdf, 0x32, 0x34
};
static const unsigned char x25519_ikme_priv[] = {
    0x52, 0xc4, 0xa7, 0x58, 0xa8, 0x02, 0xcd, 0x8b,
    0x93, 0x6e, 0xce, 0xea, 0x31, 0x44, 0x32, 0x79,
    0x8d, 0x5b, 0xaf, 0x2d, 0x7e, 0x92, 0x35, 0xdc,
    0x08, 0x4a, 0xb1, 0xb9, 0xcf, 0xa2, 0xf7, 0x36
};
static const unsigned char x25519_ikme_pub[] = {
    0x37, 0xfd, 0xa3, 0x56, 0x7b, 0xdb, 0xd6, 0x28,
    0xe8, 0x86, 0x68, 0xc3, 0xc8, 0xd7, 0xe9, 0x7d,
    0x1d, 0x12, 0x53, 0xb6, 0xd4, 0xea, 0x6d, 0x44,
    0xc1, 0x50, 0xf7, 0x41, 0xf1, 0xbf, 0x44, 0x31
};
static const unsigned char x25519_rpub[] = {
    0x39, 0x48, 0xcf, 0xe0, 0xad, 0x1d, 0xdb, 0x69,
    0x5d, 0x78, 0x0e, 0x59, 0x07, 0x71, 0x95, 0xda,
    0x6c, 0x56, 0x50, 0x6b, 0x02, 0x73, 0x29, 0x79,
    0x4a, 0xb0, 0x2b, 0xca, 0x80, 0x81, 0x5c, 0x4d
};
static const unsigned char x25519_rpriv[] = {
    0x46, 0x12, 0xc5, 0x50, 0x26, 0x3f, 0xc8, 0xad,
    0x58, 0x37, 0x5d, 0xf3, 0xf5, 0x57, 0xaa, 0xc5,
    0x31, 0xd2, 0x68, 0x50, 0x90, 0x3e, 0x55, 0xa9,
    0xf2, 0x3f, 0x21, 0xd8, 0x53, 0x4e, 0x8a, 0xc8
};
static const unsigned char x25519_expected_enc[] = {
    0x37, 0xfd, 0xa3, 0x56, 0x7b, 0xdb, 0xd6, 0x28,
    0xe8, 0x86, 0x68, 0xc3, 0xc8, 0xd7, 0xe9, 0x7d,
    0x1d, 0x12, 0x53, 0xb6, 0xd4, 0xea, 0x6d, 0x44,
    0xc1, 0x50, 0xf7, 0x41, 0xf1, 0xbf, 0x44, 0x31
};
static const unsigned char x25519_expected_secret[] = {
    0xfe, 0x0e, 0x18, 0xc9, 0xf0, 0x24, 0xce, 0x43,
    0x79, 0x9a, 0xe3, 0x93, 0xc7, 0xe8, 0xfe, 0x8f,
    0xce, 0x9d, 0x21, 0x88, 0x75, 0xe8, 0x22, 0x7b,
    0x01, 0x87, 0xc0, 0x4e, 0x7d, 0x2e, 0xa1, 0xfc
};

static const unsigned char x25519_auth_ikme[] = {
    0x6e, 0x6d, 0x8f, 0x20, 0x0e, 0xa2, 0xfb, 0x20,
    0xc3, 0x0b, 0x00, 0x3a, 0x8b, 0x4f, 0x43, 0x3d,
    0x2f, 0x4e, 0xd4, 0xc2, 0x65, 0x8d, 0x5b, 0xc8,
    0xce, 0x2f, 0xef, 0x71, 0x80, 0x59, 0xc9, 0xf7
};
static const unsigned char x25519_auth_rpub[] = {
    0x16, 0x32, 0xd5, 0xc2, 0xf7, 0x1c, 0x2b, 0x38,
    0xd0, 0xa8, 0xfc, 0xc3, 0x59, 0x35, 0x52, 0x00,
    0xca, 0xa8, 0xb1, 0xff, 0xdf, 0x28, 0x61, 0x80,
    0x80, 0x46, 0x6c, 0x90, 0x9c, 0xb6, 0x9b, 0x2e
};
static const unsigned char x25519_auth_rpriv[] = {
    0xfd, 0xea, 0x67, 0xcf, 0x83, 0x1f, 0x1c, 0xa9,
    0x8d, 0x8e, 0x27, 0xb1, 0xf6, 0xab, 0xeb, 0x5b,
    0x77, 0x45, 0xe9, 0xd3, 0x53, 0x48, 0xb8, 0x0f,
    0xa4, 0x07, 0xff, 0x69, 0x58, 0xf9, 0x13, 0x7e
};
static const unsigned char x25519_auth_spub[] = {
    0x8b, 0x0c, 0x70, 0x87, 0x3d, 0xc5, 0xae, 0xcb,
    0x7f, 0x9e, 0xe4, 0xe6, 0x24, 0x06, 0xa3, 0x97,
    0xb3, 0x50, 0xe5, 0x70, 0x12, 0xbe, 0x45, 0xcf,
    0x53, 0xb7, 0x10, 0x5a, 0xe7, 0x31, 0x79, 0x0b
};
static const unsigned char x25519_auth_spriv[] = {
    0xdc, 0x4a, 0x14, 0x63, 0x13, 0xcc, 0xe6, 0x0a,
    0x27, 0x8a, 0x53, 0x23, 0xd3, 0x21, 0xf0, 0x51,
    0xc5, 0x70, 0x7e, 0x9c, 0x45, 0xba, 0x21, 0xa3,
    0x47, 0x9f, 0xec, 0xdf, 0x76, 0xfc, 0x69, 0xdd
};
static const unsigned char x25519_auth_expected_enc[] = {
    0x23, 0xfb, 0x95, 0x25, 0x71, 0xa1, 0x4a, 0x25,
    0xe3, 0xd6, 0x78, 0x14, 0x0c, 0xd0, 0xe5, 0xeb,
    0x47, 0xa0, 0x96, 0x1b, 0xb1, 0x8a, 0xfc, 0xf8,
    0x58, 0x96, 0xe5, 0x45, 0x3c, 0x31, 0x2e, 0x76
};
static const unsigned char x25519_auth_expected_secret[] = {
    0x2d, 0x6d, 0xb4, 0xcf, 0x71, 0x9d, 0xc7, 0x29,
    0x3f, 0xcb, 0xf3, 0xfa, 0x64, 0x69, 0x07, 0x08,
    0xe4, 0x4e, 0x2b, 0xeb, 0xc8, 0x1f, 0x84, 0x60,
    0x86, 0x77, 0x95, 0x8c, 0x0d, 0x44, 0x48, 0xa7
};

static const unsigned char p256_ikme[] = {
    0x42, 0x70, 0xe5, 0x4f, 0xfd, 0x08, 0xd7, 0x9d,
    0x59, 0x28, 0x02, 0x0a, 0xf4, 0x68, 0x6d, 0x8f,
    0x6b, 0x7d, 0x35, 0xdb, 0xe4, 0x70, 0x26, 0x5f,
    0x1f, 0x5a, 0xa2, 0x28, 0x16, 0xce, 0x86, 0x0e
};

static const unsigned char p256_ikme_pub[] = {
    0x04, 0xa9, 0x27, 0x19, 0xc6, 0x19, 0x5d, 0x50,
    0x85, 0x10, 0x4f, 0x46, 0x9a, 0x8b, 0x98, 0x14,
    0xd5, 0x83, 0x8f, 0xf7, 0x2b, 0x60, 0x50, 0x1e,
    0x2c, 0x44, 0x66, 0xe5, 0xe6, 0x7b, 0x32, 0x5a,
    0xc9, 0x85, 0x36, 0xd7, 0xb6, 0x1a, 0x1a, 0xf4,
    0xb7, 0x8e, 0x5b, 0x7f, 0x95, 0x1c, 0x09, 0x00,
    0xbe, 0x86, 0x3c, 0x40, 0x3c, 0xe6, 0x5c, 0x9b,
    0xfc, 0xb9, 0x38, 0x26, 0x57, 0x22, 0x2d, 0x18,
    0xc4
};
static const unsigned char p256_ikme_priv[] = {
    0x49, 0x95, 0x78, 0x8e, 0xf4, 0xb9, 0xd6, 0x13,
    0x2b, 0x24, 0x9c, 0xe5, 0x9a, 0x77, 0x28, 0x14,
    0x93, 0xeb, 0x39, 0xaf, 0x37, 0x3d, 0x23, 0x6a,
    0x1f, 0xe4, 0x15, 0xcb, 0x0c, 0x2d, 0x7b, 0xeb
};

static const unsigned char p256_ikmr[] = {
    0x66, 0x8b, 0x37, 0x17, 0x1f, 0x10, 0x72, 0xf3,
    0xcf, 0x12, 0xea, 0x8a, 0x23, 0x6a, 0x45, 0xdf,
    0x23, 0xfc, 0x13, 0xb8, 0x2a, 0xf3, 0x60, 0x9a,
    0xd1, 0xe3, 0x54, 0xf6, 0xef, 0x81, 0x75, 0x50
};

static const unsigned char p256_ikmr_pub[] = {
    0x04, 0xfe, 0x8c, 0x19, 0xce, 0x09, 0x05, 0x19,
    0x1e, 0xbc, 0x29, 0x8a, 0x92, 0x45, 0x79, 0x25,
    0x31, 0xf2, 0x6f, 0x0c, 0xec, 0xe2, 0x46, 0x06,
    0x39, 0xe8, 0xbc, 0x39, 0xcb, 0x7f, 0x70, 0x6a,
    0x82, 0x6a, 0x77, 0x9b, 0x4c, 0xf9, 0x69, 0xb8,
    0xa0, 0xe5, 0x39, 0xc7, 0xf6, 0x2f, 0xb3, 0xd3,
    0x0a, 0xd6, 0xaa, 0x8f, 0x80, 0xe3, 0x0f, 0x1d,
    0x12, 0x8a, 0xaf, 0xd6, 0x8a, 0x2c, 0xe7, 0x2e,
    0xa0
};

static const unsigned char p256_ikmr_priv[] = {
    0xf3, 0xce, 0x7f, 0xda, 0xe5, 0x7e, 0x1a, 0x31,
    0x0d, 0x87, 0xf1, 0xeb, 0xbd, 0xe6, 0xf3, 0x28,
    0xbe, 0x0a, 0x99, 0xcd, 0xbc, 0xad, 0xf4, 0xd6,
    0x58, 0x9c, 0xf2, 0x9d, 0xe4, 0xb8, 0xff, 0xd2
};

static const unsigned char p256_expected_enc[] = {
    0x04, 0xa9, 0x27, 0x19, 0xc6, 0x19, 0x5d, 0x50,
    0x85, 0x10, 0x4f, 0x46, 0x9a, 0x8b, 0x98, 0x14,
    0xd5, 0x83, 0x8f, 0xf7, 0x2b, 0x60, 0x50, 0x1e,
    0x2c, 0x44, 0x66, 0xe5, 0xe6, 0x7b, 0x32, 0x5a,
    0xc9, 0x85, 0x36, 0xd7, 0xb6, 0x1a, 0x1a, 0xf4,
    0xb7, 0x8e, 0x5b, 0x7f, 0x95, 0x1c, 0x09, 0x00,
    0xbe, 0x86, 0x3c, 0x40, 0x3c, 0xe6, 0x5c, 0x9b,
    0xfc, 0xb9, 0x38, 0x26, 0x57, 0x22, 0x2d, 0x18,
    0xc4
};
static const unsigned char p256_expected_secret[] = {
    0xc0, 0xd2, 0x6a, 0xea, 0xb5, 0x36, 0x60, 0x9a,
    0x57, 0x2b, 0x07, 0x69, 0x5d, 0x93, 0x3b, 0x58,
    0x9d, 0xcf, 0x36, 0x3f, 0xf9, 0xd9, 0x3c, 0x93,
    0xad, 0xea, 0x53, 0x7a, 0xea, 0xbb, 0x8c, 0xb8
};

static const unsigned char p521_ikme[] = {
    0x7f, 0x06, 0xab, 0x82, 0x15, 0x10, 0x5f, 0xc4,
    0x6a, 0xce, 0xeb, 0x2e, 0x3d, 0xc5, 0x02, 0x8b,
    0x44, 0x36, 0x4f, 0x96, 0x04, 0x26, 0xeb, 0x0d,
    0x8e, 0x40, 0x26, 0xc2, 0xf8, 0xb5, 0xd7, 0xe7,
    0xa9, 0x86, 0x68, 0x8f, 0x15, 0x91, 0xab, 0xf5,
    0xab, 0x75, 0x3c, 0x35, 0x7a, 0x5d, 0x6f, 0x04,
    0x40, 0x41, 0x4b, 0x4e, 0xd4, 0xed, 0xe7, 0x13,
    0x17, 0x77, 0x2a, 0xc9, 0x8d, 0x92, 0x39, 0xf7,
    0x09, 0x04
};

static const unsigned char p521_ikme_pub[] = {
    0x04, 0x01, 0x38, 0xb3, 0x85, 0xca, 0x16, 0xbb,
    0x0d, 0x5f, 0xa0, 0xc0, 0x66, 0x5f, 0xbb, 0xd7,
    0xe6, 0x9e, 0x3e, 0xe2, 0x9f, 0x63, 0x99, 0x1d,
    0x3e, 0x9b, 0x5f, 0xa7, 0x40, 0xaa, 0xb8, 0x90,
    0x0a, 0xae, 0xed, 0x46, 0xed, 0x73, 0xa4, 0x90,
    0x55, 0x75, 0x84, 0x25, 0xa0, 0xce, 0x36, 0x50,
    0x7c, 0x54, 0xb2, 0x9c, 0xc5, 0xb8, 0x5a, 0x5c,
    0xee, 0x6b, 0xae, 0x0c, 0xf1, 0xc2, 0x1f, 0x27,
    0x31, 0xec, 0xe2, 0x01, 0x3d, 0xc3, 0xfb, 0x7c,
    0x8d, 0x21, 0x65, 0x4b, 0xb1, 0x61, 0xb4, 0x63,
    0x96, 0x2c, 0xa1, 0x9e, 0x8c, 0x65, 0x4f, 0xf2,
    0x4c, 0x94, 0xdd, 0x28, 0x98, 0xde, 0x12, 0x05,
    0x1f, 0x1e, 0xd0, 0x69, 0x22, 0x37, 0xfb, 0x02,
    0xb2, 0xf8, 0xd1, 0xdc, 0x1c, 0x73, 0xe9, 0xb3,
    0x66, 0xb5, 0x29, 0xeb, 0x43, 0x6e, 0x98, 0xa9,
    0x96, 0xee, 0x52, 0x2a, 0xef, 0x86, 0x3d, 0xd5,
    0x73, 0x9d, 0x2f, 0x29, 0xb0
};

static const unsigned char p521_ikme_priv[] = {
    0x01, 0x47, 0x84, 0xc6, 0x92, 0xda, 0x35, 0xdf,
    0x6e, 0xcd, 0xe9, 0x8e, 0xe4, 0x3a, 0xc4, 0x25,
    0xdb, 0xdd, 0x09, 0x69, 0xc0, 0xc7, 0x2b, 0x42,
    0xf2, 0xe7, 0x08, 0xab, 0x9d, 0x53, 0x54, 0x15,
    0xa8, 0x56, 0x9b, 0xda, 0xcf, 0xcc, 0x0a, 0x11,
    0x4c, 0x85, 0xb8, 0xe3, 0xf2, 0x6a, 0xcf, 0x4d,
    0x68, 0x11, 0x5f, 0x8c, 0x91, 0xa6, 0x61, 0x78,
    0xcd, 0xbd, 0x03, 0xb7, 0xbc, 0xc5, 0x29, 0x1e,
    0x37, 0x4b
};

static const unsigned char p521_ikmr_pub[] = {
    0x04, 0x01, 0xb4, 0x54, 0x98, 0xc1, 0x71, 0x4e,
    0x2d, 0xce, 0x16, 0x7d, 0x3c, 0xaf, 0x16, 0x2e,
    0x45, 0xe0, 0x64, 0x2a, 0xfc, 0x7e, 0xd4, 0x35,
    0xdf, 0x79, 0x02, 0xcc, 0xae, 0x0e, 0x84, 0xba,
    0x0f, 0x7d, 0x37, 0x3f, 0x64, 0x6b, 0x77, 0x38,
    0xbb, 0xbd, 0xca, 0x11, 0xed, 0x91, 0xbd, 0xea,
    0xe3, 0xcd, 0xcb, 0xa3, 0x30, 0x1f, 0x24, 0x57,
    0xbe, 0x45, 0x2f, 0x27, 0x1f, 0xa6, 0x83, 0x75,
    0x80, 0xe6, 0x61, 0x01, 0x2a, 0xf4, 0x95, 0x83,
    0xa6, 0x2e, 0x48, 0xd4, 0x4b, 0xed, 0x35, 0x0c,
    0x71, 0x18, 0xc0, 0xd8, 0xdc, 0x86, 0x1c, 0x23,
    0x8c, 0x72, 0xa2, 0xbd, 0xa1, 0x7f, 0x64, 0x70,
    0x4f, 0x46, 0x4b, 0x57, 0x33, 0x8e, 0x7f, 0x40,
    0xb6, 0x09, 0x59, 0x48, 0x0c, 0x0e, 0x58, 0xe6,
    0x55, 0x9b, 0x19, 0x0d, 0x81, 0x66, 0x3e, 0xd8,
    0x16, 0xe5, 0x23, 0xb6, 0xb6, 0xa4, 0x18, 0xf6,
    0x6d, 0x24, 0x51, 0xec, 0x64
};
static const unsigned char p521_ikmr_priv[] = {
    0x01, 0x46, 0x26, 0x80, 0x36, 0x9a, 0xe3, 0x75,
    0xe4, 0xb3, 0x79, 0x10, 0x70, 0xa7, 0x45, 0x8e,
    0xd5, 0x27, 0x84, 0x2f, 0x6a, 0x98, 0xa7, 0x9f,
    0xf5, 0xe0, 0xd4, 0xcb, 0xde, 0x83, 0xc2, 0x71,
    0x96, 0xa3, 0x91, 0x69, 0x56, 0x65, 0x55, 0x23,
    0xa6, 0xa2, 0x55, 0x6a, 0x7a, 0xf6, 0x2c, 0x5c,
    0xad, 0xab, 0xe2, 0xef, 0x9d, 0xa3, 0x76, 0x0b,
    0xb2, 0x1e, 0x00, 0x52, 0x02, 0xf7, 0xb2, 0x46,
    0x28, 0x47
};

static const unsigned char p521_expected_enc[] = {
    0x04, 0x01, 0x38, 0xb3, 0x85, 0xca, 0x16, 0xbb,
    0x0d, 0x5f, 0xa0, 0xc0, 0x66, 0x5f, 0xbb, 0xd7,
    0xe6, 0x9e, 0x3e, 0xe2, 0x9f, 0x63, 0x99, 0x1d,
    0x3e, 0x9b, 0x5f, 0xa7, 0x40, 0xaa, 0xb8, 0x90,
    0x0a, 0xae, 0xed, 0x46, 0xed, 0x73, 0xa4, 0x90,
    0x55, 0x75, 0x84, 0x25, 0xa0, 0xce, 0x36, 0x50,
    0x7c, 0x54, 0xb2, 0x9c, 0xc5, 0xb8, 0x5a, 0x5c,
    0xee, 0x6b, 0xae, 0x0c, 0xf1, 0xc2, 0x1f, 0x27,
    0x31, 0xec, 0xe2, 0x01, 0x3d, 0xc3, 0xfb, 0x7c,
    0x8d, 0x21, 0x65, 0x4b, 0xb1, 0x61, 0xb4, 0x63,
    0x96, 0x2c, 0xa1, 0x9e, 0x8c, 0x65, 0x4f, 0xf2,
    0x4c, 0x94, 0xdd, 0x28, 0x98, 0xde, 0x12, 0x05,
    0x1f, 0x1e, 0xd0, 0x69, 0x22, 0x37, 0xfb, 0x02,
    0xb2, 0xf8, 0xd1, 0xdc, 0x1c, 0x73, 0xe9, 0xb3,
    0x66, 0xb5, 0x29, 0xeb, 0x43, 0x6e, 0x98, 0xa9,
    0x96, 0xee, 0x52, 0x2a, 0xef, 0x86, 0x3d, 0xd5,
    0x73, 0x9d, 0x2f, 0x29, 0xb0
};
static const unsigned char p521_expected_secret[] = {
    0x77, 0x6a, 0xb4, 0x21, 0x30, 0x2f, 0x6e, 0xff,
    0x7d, 0x7c, 0xb5, 0xcb, 0x1a, 0xda, 0xea, 0x0c,
    0xd5, 0x08, 0x72, 0xc7, 0x1c, 0x2d, 0x63, 0xc3,
    0x0c, 0x4f, 0x1d, 0x5e, 0x43, 0x65, 0x33, 0x36,
    0xfe, 0xf3, 0x3b, 0x10, 0x3c, 0x67, 0xe7, 0xa9,
    0x8a, 0xdd, 0x2d, 0x3b, 0x66, 0xe2, 0xfd, 0xa9,
    0x5b, 0x5b, 0x2a, 0x66, 0x7a, 0xa9, 0xda, 0xc7,
    0xe5, 0x9c, 0xc1, 0xd4, 0x6d, 0x30, 0xe8, 0x18
};

static const unsigned char p521_auth_ikme[] = {
    0xfe, 0x1c, 0x58, 0x9c, 0x2a, 0x05, 0x89, 0x38,
    0x95, 0xa5, 0x37, 0xf3, 0x8c, 0x7c, 0xb4, 0x30,
    0x0b, 0x5a, 0x7e, 0x8f, 0xef, 0x3d, 0x6c, 0xcb,
    0x8f, 0x07, 0xa4, 0x98, 0x02, 0x9c, 0x61, 0xe9,
    0x02, 0x62, 0xe0, 0x09, 0xdc, 0x25, 0x4c, 0x7f,
    0x62, 0x35, 0xf9, 0xc6, 0xb2, 0xfd, 0x6a, 0xef,
    0xf0, 0xa7, 0x14, 0xdb, 0x13, 0x1b, 0x09, 0x25,
    0x8c, 0x16, 0xe2, 0x17, 0xb7, 0xbd, 0x2a, 0xa6,
    0x19, 0xb0
};

static const unsigned char p521_auth_ikmr_pub[] = {
    0x04, 0x00, 0x7d, 0x41, 0x9b, 0x88, 0x34, 0xe7,
    0x51, 0x3d, 0x0e, 0x7c, 0xc6, 0x64, 0x24, 0xa1,
    0x36, 0xec, 0x5e, 0x11, 0x39, 0x5a, 0xb3, 0x53,
    0xda, 0x32, 0x4e, 0x35, 0x86, 0x67, 0x3e, 0xe7,
    0x3d, 0x53, 0xab, 0x34, 0xf3, 0x0a, 0x0b, 0x42,
    0xa9, 0x2d, 0x05, 0x4d, 0x0d, 0xb3, 0x21, 0xb8,
    0x0f, 0x62, 0x17, 0xe6, 0x55, 0xe3, 0x04, 0xf7,
    0x27, 0x93, 0x76, 0x7c, 0x42, 0x31, 0x78, 0x5c,
    0x4a, 0x4a, 0x6e, 0x00, 0x8f, 0x31, 0xb9, 0x3b,
    0x7a, 0x4f, 0x2b, 0x8c, 0xd1, 0x2e, 0x5f, 0xe5,
    0xa0, 0x52, 0x3d, 0xc7, 0x13, 0x53, 0xc6, 0x6c,
    0xbd, 0xad, 0x51, 0xc8, 0x6b, 0x9e, 0x0b, 0xdf,
    0xcd, 0x9a, 0x45, 0x69, 0x8f, 0x2d, 0xab, 0x18,
    0x09, 0xab, 0x1b, 0x0f, 0x88, 0xf5, 0x42, 0x27,
    0x23, 0x2c, 0x85, 0x8a, 0xcc, 0xc4, 0x4d, 0x9a,
    0x8d, 0x41, 0x77, 0x5a, 0xc0, 0x26, 0x34, 0x15,
    0x64, 0xa2, 0xd7, 0x49, 0xf4
};

static const unsigned char p521_auth_ikmr_priv[] = {
    0x01, 0x3e, 0xf3, 0x26, 0x94, 0x09, 0x98, 0x54,
    0x4a, 0x89, 0x9e, 0x15, 0xe1, 0x72, 0x65, 0x48,
    0xff, 0x43, 0xbb, 0xdb, 0x23, 0xa8, 0x58, 0x7a,
    0xa3, 0xbe, 0xf9, 0xd1, 0xb8, 0x57, 0x33, 0x8d,
    0x87, 0x28, 0x7d, 0xf5, 0x66, 0x70, 0x37, 0xb5,
    0x19, 0xd6, 0xa1, 0x46, 0x61, 0xe9, 0x50, 0x3c,
    0xfc, 0x95, 0xa1, 0x54, 0xd9, 0x35, 0x66, 0xd8,
    0xc8, 0x4e, 0x95, 0xce, 0x93, 0xad, 0x05, 0x29,
    0x3a, 0x0b
};

static const unsigned char p521_auth_ikms_pub[] = {
    0x04, 0x01, 0x5c, 0xc3, 0x63, 0x66, 0x32, 0xea,
    0x9a, 0x38, 0x79, 0xe4, 0x32, 0x40, 0xbe, 0xae,
    0x5d, 0x15, 0xa4, 0x4f, 0xba, 0x81, 0x92, 0x82,
    0xfa, 0xc2, 0x6a, 0x19, 0xc9, 0x89, 0xfa, 0xfd,
    0xd0, 0xf3, 0x30, 0xb8, 0x52, 0x1d, 0xff, 0x7d,
    0xc3, 0x93, 0x10, 0x1b, 0x01, 0x8c, 0x1e, 0x65,
    0xb0, 0x7b, 0xe9, 0xf5, 0xfc, 0x9a, 0x28, 0xa1,
    0xf4, 0x50, 0xd6, 0xa5, 0x41, 0xee, 0x0d, 0x76,
    0x22, 0x11, 0x33, 0x00, 0x1e, 0x8f, 0x0f, 0x6a,
    0x05, 0xab, 0x79, 0xf9, 0xb9, 0xbb, 0x9c, 0xcc,
    0xe1, 0x42, 0xa4, 0x53, 0xd5, 0x9c, 0x5a, 0xbe,
    0xbb, 0x56, 0x74, 0x83, 0x9d, 0x93, 0x5a, 0x3c,
    0xa1, 0xa3, 0xfb, 0xc3, 0x28, 0x53, 0x9a, 0x60,
    0xb3, 0xbc, 0x3c, 0x05, 0xfe, 0xd2, 0x28, 0x38,
    0x58, 0x4a, 0x72, 0x6b, 0x9c, 0x17, 0x67, 0x96,
    0xca, 0xd0, 0x16, 0x9b, 0xa4, 0x09, 0x33, 0x32,
    0xcb, 0xd2, 0xdc, 0x3a, 0x9f
};

static const unsigned char p521_auth_ikms_priv[] = {
    0x00, 0x10, 0x18, 0x58, 0x45, 0x99, 0x62, 0x5f,
    0xf9, 0x95, 0x3b, 0x93, 0x05, 0x84, 0x98, 0x50,
    0xd5, 0xe3, 0x4b, 0xd7, 0x89, 0xd4, 0xb8, 0x11,
    0x01, 0x13, 0x96, 0x62, 0xfb, 0xea, 0x8b, 0x65,
    0x08, 0xdd, 0xb9, 0xd0, 0x19, 0xb0, 0xd6, 0x92,
    0xe7, 0x37, 0xf6, 0x6b, 0xea, 0xe3, 0xf1, 0xf7,
    0x83, 0xe7, 0x44, 0x20, 0x2a, 0xaf, 0x6f, 0xea,
    0x01, 0x50, 0x6c, 0x27, 0x28, 0x7e, 0x35, 0x9f,
    0xe7, 0x76
};

static const unsigned char p521_auth_expected_enc[] = {
    0x04, 0x01, 0x7d, 0xe1, 0x2e, 0xde, 0x7f, 0x72,
    0xcb, 0x10, 0x1d, 0xab, 0x36, 0xa1, 0x11, 0x26,
    0x5c, 0x97, 0xb3, 0x65, 0x48, 0x16, 0xdc, 0xd6,
    0x18, 0x3f, 0x80, 0x9d, 0x4b, 0x3d, 0x11, 0x1f,
    0xe7, 0x59, 0x49, 0x7f, 0x8a, 0xef, 0xdc, 0x5d,
    0xbb, 0x40, 0xd3, 0xe6, 0xd2, 0x1d, 0xb1, 0x5b,
    0xdc, 0x60, 0xf1, 0x5f, 0x2a, 0x42, 0x07, 0x61,
    0xbc, 0xae, 0xef, 0x73, 0xb8, 0x91, 0xc2, 0xb1,
    0x17, 0xe9, 0xcf, 0x01, 0xe2, 0x93, 0x20, 0xb7,
    0x99, 0xbb, 0xc8, 0x6a, 0xfd, 0xc5, 0xea, 0x97,
    0xd9, 0x41, 0xea, 0x1c, 0x5b, 0xd5, 0xeb, 0xee,
    0xac, 0x7a, 0x78, 0x4b, 0x3b, 0xab, 0x52, 0x47,
    0x46, 0xf3, 0xe6, 0x40, 0xec, 0x26, 0xee, 0x1b,
    0xd9, 0x12, 0x55, 0xf9, 0x33, 0x0d, 0x97, 0x4f,
    0x84, 0x50, 0x84, 0x63, 0x7e, 0xe0, 0xe6, 0xfe,
    0x9f, 0x50, 0x5c, 0x5b, 0x87, 0xc8, 0x6a, 0x4e,
    0x1a, 0x6c, 0x30, 0x96, 0xdd
};

static const unsigned char p521_auth_expected_secret[] = {
    0x26, 0x64, 0x8f, 0xa2, 0xa2, 0xde, 0xb0, 0xbf,
    0xc5, 0x63, 0x49, 0xa5, 0x90, 0xfd, 0x4c, 0xb7,
    0x10, 0x8a, 0x51, 0x79, 0x7b, 0x63, 0x46, 0x94,
    0xfc, 0x02, 0x06, 0x1e, 0x8d, 0x91, 0xb3, 0x57,
    0x6a, 0xc7, 0x36, 0xa6, 0x8b, 0xf8, 0x48, 0xfe,
    0x2a, 0x58, 0xdf, 0xb1, 0x95, 0x6d, 0x26, 0x6e,
    0x68, 0x20, 0x9a, 0x4d, 0x63, 0x1e, 0x51, 0x3b,
    0xad, 0xf8, 0xf4, 0xdc, 0xfc, 0x00, 0xf3, 0x0a
};

static const TEST_DERIVEKEY_DATA ec_derivekey_data[] = {
    {
      "P-256",
      p256_ikme, sizeof(p256_ikme),
      p256_ikme_pub, sizeof(p256_ikme_pub),
      p256_ikme_priv, sizeof(p256_ikme_priv)
    },
    {
      "P-256",
      p256_ikmr, sizeof(p256_ikmr),
      p256_ikmr_pub, sizeof(p256_ikmr_pub),
      p256_ikmr_priv, sizeof(p256_ikmr_priv)
    },
    {
      "P-521",
      p521_ikme, sizeof(p521_ikme),
      p521_ikme_pub, sizeof(p521_ikme_pub),
      p521_ikme_priv, sizeof(p521_ikme_priv)
    }
};

static const TEST_ENCAPDATA ec_encapdata[] = {
    {
        "P-256",
        p256_ikme, sizeof(p256_ikme),
        p256_ikmr_pub, sizeof(p256_ikmr_pub),
        p256_ikmr_priv, sizeof(p256_ikmr_priv),
        p256_expected_enc, sizeof(p256_expected_enc),
        p256_expected_secret, sizeof(p256_expected_secret),
    },
    {
        "X25519",
        x25519_ikme, sizeof(x25519_ikme),
        x25519_rpub, sizeof(x25519_rpub),
        x25519_rpriv, sizeof(x25519_rpriv),
        x25519_expected_enc, sizeof(x25519_expected_enc),
        x25519_expected_secret, sizeof(x25519_expected_secret),
    },
    {
        "P-521",
        p521_ikme, sizeof(p521_ikme),
        p521_ikmr_pub, sizeof(p521_ikmr_pub),
        p521_ikmr_priv, sizeof(p521_ikmr_priv),
        p521_expected_enc, sizeof(p521_expected_enc),
        p521_expected_secret, sizeof(p521_expected_secret),
    },
    {
        "P-521",
        p521_auth_ikme, sizeof(p521_auth_ikme),
        p521_auth_ikmr_pub, sizeof(p521_auth_ikmr_pub),
        p521_auth_ikmr_priv, sizeof(p521_auth_ikmr_priv),
        p521_auth_expected_enc, sizeof(p521_auth_expected_enc),
        p521_auth_expected_secret, sizeof(p521_auth_expected_secret),
        p521_auth_ikms_pub, sizeof(p521_auth_ikms_pub),
        p521_auth_ikms_priv, sizeof(p521_auth_ikms_priv)
    },
    {
        "X25519",
        x25519_auth_ikme, sizeof(x25519_auth_ikme),
        x25519_auth_rpub, sizeof(x25519_auth_rpub),
        x25519_auth_rpriv, sizeof(x25519_auth_rpriv),
        x25519_auth_expected_enc, sizeof(x25519_auth_expected_enc),
        x25519_auth_expected_secret, sizeof(x25519_auth_expected_secret),
        x25519_auth_spub, sizeof(x25519_auth_spub),
        x25519_auth_spriv, sizeof(x25519_auth_spriv)
    }
};

/* Test vector from https://github.com/cfrg/draft-irtf-cfrg-hpke */
static const unsigned char x448_ikmr[] = {
    0xd4, 0x5d, 0x16, 0x52, 0xdf, 0x74, 0x92, 0x0a,
    0xbf, 0x94, 0xa2, 0x88, 0x3c, 0x83, 0x05, 0x0f,
    0x50, 0x2f, 0xf5, 0x12, 0xff, 0xb5, 0x6f, 0x07,
    0xb6, 0xd8, 0x33, 0xec, 0x8d, 0xda, 0x74, 0xb6,
    0xa1, 0xc1, 0xcc, 0x4d, 0x42, 0xa2, 0x26, 0x41,
    0xc0, 0x96, 0x3d, 0x3c, 0x21, 0xed, 0x82, 0x61,
    0xf3, 0x44, 0xdc, 0x9e, 0x05, 0x01, 0xa8, 0x1c
};
static const unsigned char x448_ikmr_priv[] = {
    0x27, 0xa4, 0x35, 0x46, 0x08, 0xf3, 0xbd, 0xd3,
    0x8f, 0x1f, 0x5a, 0xf3, 0x05, 0xf3, 0xe0, 0x68,
    0x2e, 0xfe, 0x4e, 0x25, 0x80, 0x82, 0x49, 0xd8,
    0xfc, 0xb5, 0x59, 0x27, 0xf6, 0xa9, 0xf4, 0x46,
    0xb8, 0xdc, 0x1d, 0x0a, 0x2c, 0x3b, 0x8c, 0xb1,
    0x33, 0xa5, 0x67, 0x3b, 0x59, 0xa6, 0xd5, 0x5c,
    0xe7, 0x54, 0xec, 0x0c, 0x9a, 0x55, 0x54, 0x01
};
static const unsigned char x448_ikmr_pub[] = {
    0x14, 0x5d, 0x08, 0x3e, 0xa7, 0xa6, 0x37, 0x9d,
    0xbb, 0x32, 0xdc, 0xbd, 0x8a, 0xff, 0x4c, 0x20,
    0x6e, 0xa5, 0xd0, 0x69, 0xb7, 0x5e, 0x96, 0xc6,
    0xdd, 0x2a, 0x3e, 0x38, 0xf4, 0x41, 0x47, 0x1a,
    0xc9, 0x7a, 0xdc, 0xa6, 0x41, 0xfd, 0xad, 0x66,
    0x68, 0x5a, 0x96, 0xf3, 0x2b, 0x7c, 0x3e, 0x06,
    0x46, 0x35, 0xfa, 0xb3, 0xcc, 0x89, 0x23, 0x4e
};

static const TEST_DERIVEKEY_DATA ecx_derivekey_data[] = {
    {
      "X25519",
      x25519_ikme, sizeof(x25519_ikme),
      x25519_ikme_pub, sizeof(x25519_ikme_pub),
      x25519_ikme_priv, sizeof(x25519_ikme_priv)
    },
    {
      "X448",
      x448_ikmr, sizeof(x448_ikmr),
      x448_ikmr_pub, sizeof(x448_ikmr_pub),
      x448_ikmr_priv, sizeof(x448_ikmr_priv)
    },
};

/*
 * Helper function to create a EC or ECX private key from bytes.
 * The public key can optionally be NULL.
 */
static EVP_PKEY *new_raw_private_key(const char *curvename,
                                     const unsigned char *priv, size_t privlen,
                                     const unsigned char *pub, size_t publen)
{
    int ok = 0;
    EVP_PKEY_CTX *ctx;
    EVP_PKEY *key = NULL;
    OSSL_PARAM *params = NULL;
    BIGNUM *privbn = NULL;
    OSSL_PARAM_BLD *bld = NULL;
    int ecx = (curvename[0] == 'X');

    if (ecx)
        ctx = EVP_PKEY_CTX_new_from_name(libctx, curvename, NULL);
    else
        ctx = EVP_PKEY_CTX_new_from_name(libctx, "EC", NULL);
    if (ctx == NULL)
        return 0;

    bld = OSSL_PARAM_BLD_new();
    if (bld == NULL)
        goto err;

    if (ecx) {
        if (!OSSL_PARAM_BLD_push_octet_string(bld, OSSL_PKEY_PARAM_PRIV_KEY,
                                             (char *)priv, privlen))
            goto err;
    } else {
        privbn = BN_bin2bn(priv, privlen, NULL);
        if (privbn == NULL)
            goto err;
        if (!OSSL_PARAM_BLD_push_utf8_string(bld, OSSL_PKEY_PARAM_GROUP_NAME,
                                             curvename, 0))
            goto err;
        if (!OSSL_PARAM_BLD_push_BN(bld, OSSL_PKEY_PARAM_PRIV_KEY, privbn))
            goto err;
    }

    if (pub != NULL) {
        if (!OSSL_PARAM_BLD_push_octet_string(bld, OSSL_PKEY_PARAM_PUB_KEY,
                                             (char *)pub, publen))
            goto err;
    }
    params = OSSL_PARAM_BLD_to_param(bld);
    if (params == NULL)
        goto err;

    if (EVP_PKEY_fromdata_init(ctx) <= 0)
          goto err;
    if (EVP_PKEY_fromdata(ctx, &key, EVP_PKEY_KEYPAIR, params) <= 0)
          goto err;
    ok = 1;
err:
    if (!ok) {
        EVP_PKEY_free(key);
        key = NULL;
    }
    BN_free(privbn);
    OSSL_PARAM_free(params);
    OSSL_PARAM_BLD_free(bld);
    EVP_PKEY_CTX_free(ctx);
    return key;
}

static EVP_PKEY *new_raw_public_key(const char *curvename,
                                    const unsigned char *pub, size_t publen)
{
    int ok = 0;
    EVP_PKEY_CTX *ctx;
    EVP_PKEY *key = NULL;
    OSSL_PARAM params[3], *p = params;
    int ecx = (curvename[0] == 'X');

    if (ecx)
        ctx = EVP_PKEY_CTX_new_from_name(libctx, curvename, NULL);
    else
        ctx = EVP_PKEY_CTX_new_from_name(libctx, "EC", NULL);
    if (ctx == NULL)
        return 0;

    if (!ecx)
        *p++ = OSSL_PARAM_construct_utf8_string(OSSL_PKEY_PARAM_GROUP_NAME,
                                                (char *)curvename, 0);
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_PKEY_PARAM_PUB_KEY,
                                             (char *)pub, publen);
    *p = OSSL_PARAM_construct_end();
    if (EVP_PKEY_fromdata_init(ctx) <= 0)
          goto err;
    if (EVP_PKEY_fromdata(ctx, &key, EVP_PKEY_PUBLIC_KEY, params) <= 0)
          goto err;
    ok = 1;
err:
    if (!ok) {
        EVP_PKEY_free(key);
        key = NULL;
    }
    EVP_PKEY_CTX_free(ctx);
    return key;
}

/* Helper function to perform encapsulation */
static int do_encap(const TEST_ENCAPDATA *t, EVP_PKEY *rpub, EVP_PKEY *spriv)
{
    int ret = 0;
    unsigned char secret[256] = { 0, };
    unsigned char enc[256] = { 0, };
    size_t secretlen = 0, enclen = 0;
    EVP_PKEY_CTX *sctx = NULL;
    OSSL_PARAM params[3], *p = params;

    *p++ = OSSL_PARAM_construct_utf8_string(OSSL_KEM_PARAM_OPERATION,
                                            (char *)OSSL_KEM_PARAM_OPERATION_DHKEM,
                                            0);
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KEM_PARAM_IKME,
                                             (char *)t->ikmE, t->ikmElen);
    *p = OSSL_PARAM_construct_end();

    if (!TEST_ptr(sctx = EVP_PKEY_CTX_new_from_pkey(libctx, rpub, NULL)))
        goto err;
    if (t->spriv == NULL) {
        if (!TEST_int_eq(EVP_PKEY_encapsulate_init(sctx, params), 1))
            goto err;
    } else {
        if (!TEST_int_eq(EVP_PKEY_auth_encapsulate_init(sctx, spriv, params), 1))
        goto err;
    }
    ret = TEST_int_eq(EVP_PKEY_encapsulate(sctx, NULL, &enclen, NULL,
                                              &secretlen), 1)
          && TEST_int_eq(EVP_PKEY_encapsulate(sctx, enc, &enclen, secret,
                                              &secretlen), 1)
          && TEST_mem_eq(enc, enclen, t->expected_enc, t->expected_enclen)
          && TEST_mem_eq(secret, secretlen,
                         t->expected_secret, t->expected_secretlen);
err:
    EVP_PKEY_CTX_free(sctx);
    return ret;
}

/* Helper function to perform decapsulation */
static int do_decap(const TEST_ENCAPDATA *t, EVP_PKEY *rpriv, EVP_PKEY *spub)
{
    int ret = 0;
    EVP_PKEY_CTX *recipctx = NULL;
    unsigned char secret[256] = { 0, };
    size_t secretlen = 0;

    if (!TEST_ptr(recipctx = EVP_PKEY_CTX_new_from_pkey(libctx, rpriv, NULL)))
        goto err;
    if (t->spub == NULL) {
        if (!TEST_int_eq(EVP_PKEY_decapsulate_init(recipctx, opparam), 1))
            goto err;
    } else {
        if (!TEST_int_eq(EVP_PKEY_auth_decapsulate_init(recipctx, spub,
                                                        opparam), 1))
            goto err;
    }
    ret = TEST_int_eq(EVP_PKEY_decapsulate(recipctx, NULL, &secretlen,
                                              t->expected_enc,
                                              t->expected_enclen), 1)
          && TEST_int_eq(EVP_PKEY_decapsulate(recipctx, secret, &secretlen,
                                              t->expected_enc,
                                              t->expected_enclen), 1)
          && TEST_mem_eq(secret, secretlen,
                         t->expected_secret, t->expected_secretlen);
err:
    EVP_PKEY_CTX_free(recipctx);
    return ret;
}