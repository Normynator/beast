//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_DETAIL_BUFFERS_RANGE_HPP
#define BOOST_BEAST_DETAIL_BUFFERS_RANGE_HPP

#include <boost/beast/core/detail/type_traits.hpp>
#include <boost/asio/buffer.hpp>
#include <iterator>
#include <type_traits>

namespace boost {
namespace beast {
namespace detail {

template<class BufferSequence>
class buffers_range_adaptor
{
    BufferSequence b_;

public:
#if BOOST_BEAST_DOXYGEN
    using value_type = __see_below__;
#else
    using value_type = typename std::conditional<
        boost::is_convertible<
            typename std::iterator_traits<
                typename detail::buffer_sequence_iterator<
                    BufferSequence>::type>::value_type,
                boost::asio::mutable_buffer>::value,
            boost::asio::mutable_buffer,
            boost::asio::const_buffer>::type;
#endif

    class const_iterator
    {
        friend class buffers_range_adaptor;

        using iter_type = typename
            buffer_sequence_iterator<BufferSequence>::type;

        iter_type it_;

        const_iterator(iter_type const& it)
            : it_(it)
        {
        }

    public:
        using value_type = typename
            buffers_range_adaptor::value_type;
        using pointer = value_type const*;
        using reference = value_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category =
            std::bidirectional_iterator_tag;
        
        bool
        operator==(const_iterator const& other) const
        {
            return it_ == other.it_;
        }

        bool
        operator!=(const_iterator const& other) const
        {
            return ! (*this == other);
        }

        reference
        operator*() const
        {
            return *it_;
        }

        pointer
        operator->() const = delete;

        const_iterator&
        operator++()
        {
            ++it_;
            return *this;
        }

        const_iterator
        operator++(int)
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        // deprecated
        const_iterator&
        operator--()
        {
            --it_;
            return *this;
        }

        // deprecated
        const_iterator
        operator--(int)
        {
            auto temp = *this;
            --(*this);
            return temp;
        }
    };

    explicit
    buffers_range_adaptor(BufferSequence const& b)
        : b_(b)
    {
    }

    const_iterator
    begin() const noexcept
    {
        return boost::asio::buffer_sequence_begin(b_);
    }

    const_iterator
    end() const noexcept
    {
        return boost::asio::buffer_sequence_end(b_);
    }
};

} // detail
} // beast
} // boost

#endif
