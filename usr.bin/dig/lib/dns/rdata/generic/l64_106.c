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

#ifndef RDATA_GENERIC_L64_106_C
#define RDATA_GENERIC_L64_106_C

#include <string.h>

#include <isc/net.h>

#define RRTYPE_L64_ATTRIBUTES (0)

static inline isc_result_t
totext_l64(ARGS_TOTEXT) {
	isc_region_t region;
	char buf[sizeof("xxxx:xxxx:xxxx:xxxx")];
	unsigned short num;

	REQUIRE(rdata->type == dns_rdatatype_l64);
	REQUIRE(rdata->length == 10);

	UNUSED(tctx);

	dns_rdata_toregion(rdata, &region);
	num = uint16_fromregion(&region);
	isc_region_consume(&region, 2);
	snprintf(buf, sizeof(buf), "%u", num);
	RETERR(str_totext(buf, target));

	RETERR(str_totext(" ", target));

	snprintf(buf, sizeof(buf), "%x:%x:%x:%x",
		 region.base[0]<<8 | region.base[1],
		 region.base[2]<<8 | region.base[3],
		 region.base[4]<<8 | region.base[5],
		 region.base[6]<<8 | region.base[7]);
	return (str_totext(buf, target));
}

static inline isc_result_t
fromwire_l64(ARGS_FROMWIRE) {
	isc_region_t sregion;

	REQUIRE(type == dns_rdatatype_l64);

	UNUSED(type);
	UNUSED(options);
	UNUSED(rdclass);
	UNUSED(dctx);

	isc_buffer_activeregion(source, &sregion);
	if (sregion.length != 10)
		return (DNS_R_FORMERR);
	isc_buffer_forward(source, sregion.length);
	return (mem_tobuffer(target, sregion.base, sregion.length));
}

static inline isc_result_t
towire_l64(ARGS_TOWIRE) {

	REQUIRE(rdata->type == dns_rdatatype_l64);
	REQUIRE(rdata->length == 10);

	UNUSED(cctx);

	return (mem_tobuffer(target, rdata->data, rdata->length));
}







#endif	/* RDATA_GENERIC_L64_106_C */
