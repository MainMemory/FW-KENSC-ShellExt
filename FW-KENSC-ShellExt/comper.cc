#include "StdAfx.h"
#undef min
#undef max
/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Flamewing 2013-2015 <flamewing.sonic@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <istream>
#include <ostream>
#include <sstream>

#include "comper.h"
#include "bigendian_io.h"
#include "bitstream.h"
#include "lzss.h"

using namespace std;

// NOTE: This has to be changed for other LZSS-based compression schemes.
struct ComperAdaptor {
	typedef unsigned short stream_t;
	typedef unsigned short descriptor_t;
	typedef bigendian<descriptor_t> descriptor_endian_t;
	// Number of bits on descriptor bitfield.
	const static size_t NumDescBits = sizeof(descriptor_t) * 8;
	// Number of bits used in descriptor bitfield to signal the end-of-file
	// marker sequence.
	const static size_t NumTermBits = 1;
	// Flag that tells the compressor that new descriptor fields is needed
	// when a new bit is needed and all bits in the previous one have been
	// used up.
	const static size_t NeedEarlyDescriptor = 0;
	// Flag that marks the descriptor bits as being in big-endian bit
	// order (that is, highest bits come out first).
	const static size_t DescriptorLittleEndianBits = 0;
	// Size of the search buffer.
	const static size_t SearchBufSize = 256;
	// Size of the look-ahead buffer.
	const static size_t LookAheadBufSize = 256;
	// Total size of the sliding window.
	const static size_t SlidingWindowSize = SearchBufSize + LookAheadBufSize;
	// Computes the cost of a symbolwise encoding, that is, the cost of encoding
	// one single symbol..
	const static size_t symbolwise_weight() {
		// Symbolwise match: 1-bit descriptor, 16-bit length.
		return 1 + 16;
	}
	// Computes the cost of covering all of the "len" vertices starting from
	// "off" vertices ago, for matches with len > 1.
	// A return of "numeric_limits<size_t>::max()" means "infinite",
	// or "no edge".
	const static size_t dictionary_weight(size_t UNUSED(dist), size_t UNUSED(len)) {
		// Preconditions:
		// len > 1 && len <= LookAheadBufSize && dist != 0 && dist <= SearchBufSize
		// Dictionary match: 1-bit descriptor, 8-bit distance, 8-bit length.
		return 1 + 8 + 8;
	}
	// Given an edge, computes how many bits are used in the descriptor field.
	static size_t desc_bits(AdjListNode const &UNUSED(edge)) {
		// Comper always uses a single bit descriptor.
		return 1;
	}
	// Comper finds no additional matches over normal LZSS.
	static void extra_matches(stream_t const *UNUSED(data),
	                          size_t UNUSED(basenode),
	                          size_t UNUSED(ubound), size_t UNUSED(lbound),
	                          LZSSGraph<ComperAdaptor>::MatchVector &UNUSED(matches)) {
	}
	// Comper needs no additional padding at the end-of-file.
	static size_t get_padding(size_t UNUSED(totallen), size_t UNUSED(padmask)) {
		return 0;
	}
};

typedef LZSSGraph<ComperAdaptor> CompGraph;
typedef LZSSOStream<ComperAdaptor> CompOStream;
typedef LZSSIStream<ComperAdaptor> CompIStream;

void comper::decode_internal(istream &in, iostream &Dst) {
	CompIStream src(in);

	while (in.good()) {
		if (!src.descbit()) {
			// Symbolwise match.
			BigEndian::Write2(Dst, BigEndian::Read2(in));
		} else {
			// Dictionary match.
			// Distance and length of match.
			size_t distance = (0x100 - src.getbyte()) * 2,
			       length = src.getbyte();
			if (length == 0) {
				break;
			}

			for (size_t i = 0; i <= length; i++) {
				streampos Pointer = Dst.tellp();
				Dst.seekg(streamoff(Pointer) - distance);
				unsigned short Word = BigEndian::Read2(Dst);
				Dst.seekp(Pointer);
				BigEndian::Write2(Dst, Word);
			}
		}
	}
}

bool comper::decode(istream &Src, iostream &Dst, streampos Location) {
	Src.seekg(Location);
	stringstream in(ios::in | ios::out | ios::binary);
	in << Src.rdbuf();

	in.seekg(0);
	decode_internal(in, Dst);
	return true;
}

void comper::encode_internal(ostream &Dst, unsigned char const *&Buffer,
                             streamsize const BSize) {
	// Compute optimal Comper parsing of input file.
	CompGraph enc(Buffer, BSize, 1u);
	CompGraph::AdjList list = enc.find_optimal_parse();
	CompOStream out(Dst);

	streamoff pos = 0;
	// Go through each edge in the optimal path.
	for (CompGraph::AdjList::const_iterator it = list.begin();
	        it != list.end(); ++it) {
		AdjListNode const &edge = *it;
		size_t len = edge.get_length(), dist = edge.get_distance();
		// The weight of each edge uniquely identifies how it should be written.
		// NOTE: This needs to be changed for other LZSS schemes.
		if (len == 1) {
			// Symbolwise match.
			out.descbit(0);
			out.putbyte(Buffer[pos]);
			out.putbyte(Buffer[pos + 1]);
		} else {
			// Dictionary match.
			out.descbit(1);
			out.putbyte(-dist);
			out.putbyte(len - 1);
		}
		// Go to next position.
		pos = edge.get_dest() * 2;
	}

	// Push descriptor for end-of-file marker.
	out.descbit(1);

	out.putbyte(0x00);
	out.putbyte(0x00);
}

bool comper::encode(istream &Src, ostream &Dst) {
	Src.seekg(0, ios::end);
	streamsize ISize = Src.tellg();
	Src.seekg(0);
	// Pad to even size.
	streamsize BSize = ISize + ((ISize & 1) != 0 ? 1 : 0);
	char *const Buffer = new char[BSize];
	unsigned char const *ptr = reinterpret_cast<unsigned char *>(Buffer);
	Src.read(Buffer, ISize);
	// If we had to increase buffer size, we need to set the last byte in the
	// buffer manually. We will pad with a 0 byte.
	if (ISize != BSize) {
		Buffer[ISize] = 0;
	}

	encode_internal(Dst, ptr, BSize);

	delete [] Buffer;
	return true;
}
