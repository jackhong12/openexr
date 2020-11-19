///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
//	class TileDescriptionAttribute
//
//-----------------------------------------------------------------------------

#include <ImfTileDescriptionAttribute.h>


OPENEXR_IMF_INTERNAL_NAMESPACE_SOURCE_ENTER

using namespace OPENEXR_IMF_INTERNAL_NAMESPACE;

template <>
const char *
TileDescriptionAttribute::staticTypeName ()
{
    return "tiledesc";
}


template <>
void
TileDescriptionAttribute::writeValueTo (OPENEXR_IMF_INTERNAL_NAMESPACE::OStream &os, int version) const
{
    Xdr::write <StreamIO> (os, _value.xSize);
    Xdr::write <StreamIO> (os, _value.ySize);

    unsigned char tmp = _value.mode | (_value.roundingMode << 4);
    Xdr::write <StreamIO> (os, tmp);
}


template <>
void
TileDescriptionAttribute::readValueFrom (OPENEXR_IMF_INTERNAL_NAMESPACE::IStream &is,
					 int size,
					 int version)
{
    Xdr::read <StreamIO> (is, _value.xSize);
    Xdr::read <StreamIO> (is, _value.ySize);

    unsigned char tmp;
    Xdr::read <StreamIO> (is, tmp);

    //
    // four bits are allocated for 'mode' for future use (16 possible values)
    // but only values 0,1,2 are currently valid. '3' is a special valid enum value
    // that indicates bad values have been used
    //
    // roundingMode can only be 0 or 1, and 2 is a special enum value for 'bad enum'
    //
    unsigned char levelMode = tmp & 0x0f;
    if(levelMode > 3)
    {
        levelMode = 3;
    }

    _value.mode = LevelMode(levelMode);

    unsigned char levelRoundingMode = (tmp >> 4) & 0x0f;
    if(levelRoundingMode > 2)
    {
        levelRoundingMode = 2;
    }

    _value.roundingMode = LevelRoundingMode (levelRoundingMode);
    
}


OPENEXR_IMF_INTERNAL_NAMESPACE_SOURCE_EXIT 