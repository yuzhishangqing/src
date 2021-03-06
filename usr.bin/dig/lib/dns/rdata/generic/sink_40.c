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

#ifndef RDATA_GENERIC_SINK_40_C
#define RDATA_GENERIC_SINK_40_C

#include <dst/dst.h>

#define RRTYPE_SINK_ATTRIBUTES (0)

static inline isc_result_t
totext_sink(ARGS_TOTEXT) {
	isc_region_t sr;
	char buf[sizeof("255 255 255")];
	uint8_t meaning, coding, subcoding;

	REQUIRE(rdata->type == dns_rdatatype_sink);
	REQUIRE(rdata->length >= 3);

	dns_rdata_toregion(rdata, &sr);

	/* Meaning, Coding and Subcoding */
	meaning = uint8_fromregion(&sr);
	isc_region_consume(&sr, 1);
	coding = uint8_fromregion(&sr);
	isc_region_consume(&sr, 1);
	subcoding = uint8_fromregion(&sr);
	isc_region_consume(&sr, 1);
	snprintf(buf, sizeof(buf), "%u %u %u", meaning, coding, subcoding);
	RETERR(str_totext(buf, target));

	if (sr.length == 0U)
		return (ISC_R_SUCCESS);

	/* data */
	if ((tctx->flags & DNS_STYLEFLAG_MULTILINE) != 0)
		RETERR(str_totext(" (", target));

	RETERR(str_totext(tctx->linebreak, target));

	if (tctx->width == 0)   /* No splitting */
		RETERR(isc_base64_totext(&sr, 60, "", target));
	else
		RETERR(isc_base64_totext(&sr, tctx->width - 2,
					 tctx->linebreak, target));

	if ((tctx->flags & DNS_STYLEFLAG_MULTILINE) != 0)
		RETERR(str_totext(" )", target));

	return (ISC_R_SUCCESS);
}

static inline isc_result_t
fromwire_sink(ARGS_FROMWIRE) {
	isc_region_t sr;

	REQUIRE(type == dns_rdatatype_sink);

	UNUSED(type);
	UNUSED(rdclass);
	UNUSED(dctx);
	UNUSED(options);

	isc_buffer_activeregion(source, &sr);
	if (sr.length < 3)
		return (ISC_R_UNEXPECTEDEND);

	RETERR(mem_tobuffer(target, sr.base, sr.length));
	isc_buffer_forward(source, sr.length);
	return (ISC_R_SUCCESS);
}

static inline isc_result_t
towire_sink(ARGS_TOWIRE) {

	REQUIRE(rdata->type == dns_rdatatype_sink);
	REQUIRE(rdata->length >= 3);

	UNUSED(cctx);

	return (mem_tobuffer(target, rdata->data, rdata->length));
}






#endif	/* RDATA_GENERIC_SINK_40_C */
