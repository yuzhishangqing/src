/*
 * Copyright (C) Internet Systems Consortium, Inc. ("ISC")
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/* $Id: key.c,v 1.7 2020/02/23 23:40:21 jsg Exp $ */



#include <stddef.h>
#include <stdint.h>

#include <isc/region.h>
#include <isc/util.h>

#include <dst/dst.h>

#include "dst_internal.h"

uint16_t
dst_region_computeid(const isc_region_t *source, unsigned int alg) {
	uint32_t ac;
	const unsigned char *p;
	int size;

	REQUIRE(source != NULL);
	REQUIRE(source->length >= 4);

	p = source->base;
	size = source->length;

	if (alg == DST_ALG_RSAMD5)
		return ((p[size - 3] << 8) + p[size - 2]);

	for (ac = 0; size > 1; size -= 2, p += 2)
		ac += ((*p) << 8) + *(p + 1);

	if (size > 0)
		ac += ((*p) << 8);
	ac += (ac >> 16) & 0xffff;

	return ((uint16_t)(ac & 0xffff));
}

unsigned int
dst_key_size(const dst_key_t *key) {
	return (key->key_size);
}

unsigned int
dst_key_alg(const dst_key_t *key) {
	return (key->key_alg);
}

void
dst_key_setbits(dst_key_t *key, uint16_t bits) {
	unsigned int maxbits;
	if (bits != 0) {
		RUNTIME_CHECK(dst_key_sigsize(key, &maxbits) == ISC_R_SUCCESS);
		maxbits *= 8;
		REQUIRE(bits <= maxbits);
	}
	key->key_bits = bits;
}

uint16_t
dst_key_getbits(const dst_key_t *key) {
	return (key->key_bits);
}

/*! \file */
