//
// Created by Mehmet Fatih BAKIR on 16/05/2017.
//

#pragma once

#include <malt/list.hpp>
#include <malt_asset/asset_loader.hpp>
#include <unordered_map>

namespace malt {
    class text_asset
    {
        const char* m_data;
    public:
        text_asset(const char* data)
                :m_data(data)
        { }

        const char* c_str() const
        { return m_data; }
    };

    class txt_loader
    {
    public:
        using types = meta::list<text_asset>;
        bool check(meta::type<text_asset>, const asset::asset_file&);
        text_asset load(meta::type<text_asset>, asset::asset_file& f);

    private:
        std::unordered_map<std::string, std::string> m_cache;
    };
}

