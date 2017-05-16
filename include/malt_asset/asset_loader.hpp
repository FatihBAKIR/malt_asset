//
// Created by Mehmet Fatih BAKIR on 16/05/2017.
//
#pragma once

#include <array>
#include <fstream>
#include <gsl/gsl_assert>

namespace malt
{
    namespace asset
    {
        class asset_file
        {
            const char* m_path;
            mutable std::ifstream m_stream;
            bool m_is_binary;
            size_t m_size;

        public:
            asset_file(const char* path);

            const char* get_path() const { return m_path; }

            size_t get_size() const { return m_size; }

            template <class FunT>
            bool check_magic(int begin, int end, const FunT& test) const
            {
                std::array<char, 128> buffer;
                Expects(end - begin <= buffer.size());

                m_stream.seekg(begin, std::ios_base::beg);
                m_stream.read(buffer.data(), end - begin);

                return test((const uint8_t*)buffer.data());
            }

            std::istream& get_stream()
            {
                if (m_is_binary)
                {
                    m_stream.open(get_path(), std::ios_base::in);
                }
                else
                {
                    m_stream.seekg(0, std::ios_base::beg);
                }

                m_stream.seekg(0, std::ios_base::beg);
                return m_stream;
            }

            std::istream& get_binary_stream()
            {
                if (!m_is_binary)
                {
                    m_stream.open(get_path(), std::ios_base::in | std::ios_base::binary);
                }
                else
                {
                    m_stream.seekg(0, std::ios_base::beg);
                }

                return m_stream;
            }
        };

    }
}
