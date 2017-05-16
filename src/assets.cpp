//
// Created by fatih on 5/16/17.
//

#include "../include/malt_asset/assets.hpp"

#include <malt_asset/asset_loader.hpp>

namespace malt
{
namespace asset{
    asset_file::asset_file(const char* path)
            :
            m_path(path),
            m_stream(path)
    {
        m_stream.seekg(0, std::ios::seekdir::end);
        m_size = m_stream.tellg();
    }
}
}