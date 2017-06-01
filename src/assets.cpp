//
// Created by fatih on 5/16/17.
//

#include <malt_asset/asset_loader.hpp>

namespace malt
{
namespace asset{
    asset_file::asset_file(const char* path)
            :
            m_path(path),
            m_stream(path)
    {
        Expects(m_stream.good());
        m_stream.seekg(0, std::ios_base::end);
        m_is_binary = false;
        m_size = m_stream.tellg();
    }
}
}