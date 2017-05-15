//
// Created by fatih on 5/16/17.
//

#pragma once

#include <cstdint>
#include <string>
#include <malt/engine_impl.hpp>

namespace malt
{
namespace impl
{
    template <class AssetT>
    struct asset_adapter
    {
        static MALT_WEAK_SYMBOL AssetT load(const char* path);
    };
}

namespace asset
{
    template <class AssetT>
    AssetT load(const char* path)
    {
        return impl::asset_adapter<AssetT>::load(path);
    }

    template <class AssetT>
    AssetT load(const std::string& path)
    {
        return load<AssetT>(path.c_str());
    }
}
}
