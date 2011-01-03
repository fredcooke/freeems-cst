/**
 * @file       cst-test1.cpp
 * @headerfile
 * @author     sean
 * @brief      experiment with some packet encode ideas
 *
 * freeems-cst: freeems 'comms smoke test'
 *
 * Copyright (C) 2010 Sean Stasiak sstasiak at gmail
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* boost::test */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE feEncoder
#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>

/* std:: */
#include <vector>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <algorithm>

/* everything below will be extracted into a lib once I figure out
   if I like it */

//---------------------------------------------------------------//
/**
 * @public
 * @brief freeems packet encoder
 */
class feEncoder
{
  public:
    static std::vector<uint8_t> *
      encode( std::vector<uint8_t> const &p );

    enum
    {
        STX = 0xAA,
        ESC = 0xBB,
        ETX = 0xCC
    };

  private:
    feEncoder();    /**< static class */
    ~feEncoder();
};

/**
 * @public
 * @brief encode a flattened packet
 * @param[in] p reference to flattened packet
 * @retval vector<uint8_t> packet encoded in accordance with the freeems
 *                         core protocol spec. caller is responsible for
 *                         destruction when use is complete.
 */
std::vector<uint8_t> *
  feEncoder::encode( std::vector<uint8_t> const &p )
{
    std::vector<uint8_t> *const v = new std::vector<uint8_t>();
    v->reserve( 1024 * 4 );     /**< reserve initial worst case capacity */

    std::vector<uint8_t>::const_iterator pkt_iter = p.begin();
    while( pkt_iter != p.end() )
    {
        uint8_t b = *pkt_iter;

        /* ESC any reserved bytes in the stream */
        if( (b == feEncoder::STX ) ||
            (b == feEncoder::ETX ) ||
            (b == feEncoder::ESC ) )
        {
            v->push_back( feEncoder::ESC );
            b ^= 0xFF;
        }

        v->push_back( b );
        pkt_iter++;
    }

    return v;
}
//---------------------------------------------------------------//

using namespace std;

BOOST_AUTO_TEST_CASE( encode_nothing )
{
    vector<uint8_t> const pkt( 0 );
    auto_ptr<vector<uint8_t> const> const v( feEncoder::encode( pkt ) ) ;

    BOOST_REQUIRE( v.get() != 0 );          /**< exists .. */
    BOOST_CHECK( v.get()->empty() );        /**< but empty */
}

BOOST_AUTO_TEST_CASE( encode_something )
{
    uint8_t const data[] = { 0x00, 0x01, 0x02 };
    vector<uint8_t> const pkt( data, data+sizeof(data) );
    auto_ptr<vector<uint8_t> const> const v( feEncoder::encode( pkt ) ) ;

    BOOST_REQUIRE( v.get() != 0 );

    /* nothing was encoded, ends up being a copy straight across */
    BOOST_CHECK_EQUAL_COLLECTIONS( v.get()->begin(), v.get()->end(),
                                   pkt.begin(), pkt.end() );
}

BOOST_AUTO_TEST_CASE( encode_STX_only )
{
    uint8_t const data[] = { feEncoder::STX };
    vector<uint8_t> const pkt( data, data+sizeof(data) );
    auto_ptr<vector<uint8_t> const> const v( feEncoder::encode( pkt ) ) ;

    BOOST_REQUIRE( v.get() != 0 );

    uint8_t const expected[] = { feEncoder::ESC, (feEncoder::STX ^ 0xFF) };
    BOOST_CHECK_EQUAL_COLLECTIONS( v.get()->begin(), v.get()->end(),
                                   expected, expected+sizeof(expected) );
}

BOOST_AUTO_TEST_CASE( encode_ETX_only )
{
    uint8_t const data[] = { feEncoder::ETX };
    vector<uint8_t> const pkt( data, data+sizeof(data) );
    auto_ptr<vector<uint8_t> const> const v( feEncoder::encode( pkt ) ) ;

    BOOST_REQUIRE( v.get() != 0 );

    uint8_t const expected[] = { feEncoder::ESC, (feEncoder::ETX ^ 0xFF) };
    BOOST_CHECK_EQUAL_COLLECTIONS( v.get()->begin(), v.get()->end(),
                                   expected, expected+sizeof(expected) );
}

BOOST_AUTO_TEST_CASE( encode_ESC_only )
{
    uint8_t const data[] = { feEncoder::ESC };
    vector<uint8_t> const pkt( data, data+sizeof(data) );
    auto_ptr<vector<uint8_t> const> const v( feEncoder::encode( pkt ) ) ;

    BOOST_REQUIRE( v.get() != 0 );

    uint8_t const expected[] = { feEncoder::ESC, (feEncoder::ESC ^ 0xFF) };
    BOOST_CHECK_EQUAL_COLLECTIONS( v.get()->begin(), v.get()->end(),
                                   expected, expected+sizeof(expected) );
}

BOOST_AUTO_TEST_CASE( encode_all_framed_by_junk )
{
    uint8_t const data[] = { 0x11,
                             feEncoder::ESC,
                             feEncoder::ETX,
                             feEncoder::STX,
                             0x22, 0x33 };
    vector<uint8_t> const pkt( data, data+sizeof(data) );
    auto_ptr<vector<uint8_t> const> const v( feEncoder::encode( pkt ) ) ;

    BOOST_REQUIRE( v.get() != 0 );

    uint8_t const expected[] = { 0x11,
                                 feEncoder::ESC, (feEncoder::ESC ^ 0xFF),
                                 feEncoder::ETX, (feEncoder::ETX ^ 0xFF),
                                 feEncoder::STX, (feEncoder::STX ^ 0xFF),
                                 0x22, 0x33 };
    BOOST_CHECK_EQUAL_COLLECTIONS( v.get()->begin(), v.get()->end(),
                                   expected, expected+sizeof(expected) );
}
