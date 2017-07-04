//
// Created by Mehmet Fatih BAKIR on 16/05/2017.
//

#include <malt_asset/text_asset.hpp>
#include <malt/meta.hpp>
#include <malt_asset/asset_file.hpp>
#include <yaml-cpp/node/node.h>
#include <malt_asset/assets.hpp>

namespace malt
{
    bool txt_loader::check(meta::type <text_asset>, const asset::asset_file&)
    {
        return true;
    }

    text_asset txt_loader::load(meta::type <text_asset>, asset::asset_file& f)
    {
        auto it = m_cache.find(f.get_path());

        if (it == m_cache.end())
        {
            std::string x;
            x.assign(std::istreambuf_iterator<char>(f.get_stream()),
                std::istreambuf_iterator<char>());
            auto res = m_cache.emplace(f.get_path(), x);
            it = res.first;
        }

        return { it->second.c_str() };
    }

    bool txt_loader::check(meta::type<YAML::Node>, const asset::asset_file &) {
        return true;
    }

    YAML::Node txt_loader::load(meta::type<YAML::Node>, asset::asset_file &f) {
        auto ta = malt::asset::load<text_asset>(f.get_path());
        return YAML::Load(ta->c_str());
    }
}

