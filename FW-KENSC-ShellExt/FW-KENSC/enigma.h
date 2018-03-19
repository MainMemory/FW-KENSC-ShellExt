/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Copyright (C) Flamewing 2011-2016 <flamewing.sonic@gmail.com>
 * Copyright (C) 2002-2004 The KENS Project Development Team
 * Copyright (C) 2002-2003 Roger Sanders (AKA Nemesis)
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

#ifndef __LIB_ENIGMA_H
#define __LIB_ENIGMA_H

#include <iosfwd>
#include "basic_decoder.h"
#include "moduled_adaptor.h"

class enigma;
using basic_enigma = BasicDecoder<enigma, false>;
using moduled_enigma = ModuledAdaptor<enigma, 4096u, 1u>;

class enigma : public basic_enigma, public moduled_enigma {
	friend basic_enigma;
	friend moduled_enigma;
	static bool encode(std::ostream &Dst, unsigned char const *data, size_t const Size);
public:
	static bool encode(std::istream &Src, std::ostream &Dst);
	static bool decode(std::istream &Src, std::ostream &Dst);
};

#endif // __LIB_ENIGMA_H
