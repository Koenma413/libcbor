/*
 * Copyright (c) 2014-2015 Pavel Kalvoda <me@pavelkalvoda.com>
 *
 * libcbor is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "encoders.h"

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

size_t _cbor_encode_uint8(uint8_t value, unsigned char *buffer, size_t buffer_size, uint8_t offset)
{
	if (value <= 23) {
		if (buffer_size >= 1) {
			buffer[0] = value + offset;
			return 1;
		}
	} else {
		if (buffer_size >= 2) {
			buffer[0] = 0x18 + offset;
			buffer[1] = value;
			return 2;
		}
	}
	return 0;
}

size_t _cbor_encode_uint16(uint16_t value, unsigned char *buffer, size_t buffer_size, uint8_t offset)
{
	if (buffer_size >= 3) {
		buffer[0] = 0x19 + offset;

#ifdef HAVE_ENDIAN_H
		*(uint16_t *) &buffer[1] = htobe16(value);
#else
	#if IS_BIG_ENDIAN
			*(uint16_t *) &buffer[1] = value;
	#else
			buffer[1] = value >> 8;
			buffer[2] = value;
	#endif
#endif

		return 3;
	} else
		return 0;
}

size_t _cbor_encode_uint32(uint32_t value, unsigned char *buffer, size_t buffer_size, uint8_t offset)
{
	if (buffer_size >= 5) {
		buffer[0] = 0x1A + offset;

#ifdef HAVE_ENDIAN_H
		*(uint32_t *) &buffer[1] = htobe32(value);
#else
	#if IS_BIG_ENDIAN
			*(uint32_t *) &buffer[1] = value;
	#else
			buffer[1] = value >> 24;
			buffer[2] = value >> 16;
			buffer[3] = value >> 8;
			buffer[4] = value;
	#endif
#endif

		return 5;
	} else
		return 0;
}

size_t _cbor_encode_uint64(uint64_t value, unsigned char *buffer, size_t buffer_size, uint8_t offset)
{
	if (buffer_size >= 9) {
		buffer[0] = 0x1B + offset;

#ifdef HAVE_ENDIAN_H
		*(uint64_t *) &buffer[1] = htobe64(value);
#else
		#if IS_BIG_ENDIAN
			*(uint64_t *) &buffer[1] = value;
		#else
			buffer[1] = value >> 56;
			buffer[2] = value >> 48;
			buffer[3] = value >> 40;
			buffer[4] = value >> 32;
			buffer[5] = value >> 24;
			buffer[6] = value >> 16;
			buffer[7] = value >> 8;
			buffer[8] = value;
		#endif
#endif

		return 9;
	} else
		return 0;
}

size_t _cbor_encode_uint(uint64_t value, unsigned char *buffer, size_t buffer_size, uint8_t offset)
{
	if (value <= UINT16_MAX) if (value <= UINT8_MAX)
		return _cbor_encode_uint8((uint8_t) value, buffer, buffer_size, offset);
	else
		return _cbor_encode_uint16((uint16_t) value, buffer, buffer_size, offset);
	else if (value <= UINT32_MAX)
		return _cbor_encode_uint32((uint32_t) value, buffer, buffer_size, offset);
	else
		return _cbor_encode_uint64((uint64_t) value, buffer, buffer_size, offset);
}
