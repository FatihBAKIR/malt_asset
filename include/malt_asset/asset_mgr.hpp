//
// Created by fatih on 5/16/17.
//
#pragma once

#include <malt/meta.hpp>
#include <tuple>
#include <iostream>
#include <malt/utilities.hpp>
#include <malt_asset/asset_file.hpp>
#include <malt_asset/assets_def.hpp>
#include <malt_asset/assets.hpp>
#include <map>

namespace malt
{
    namespace asset
    {
        class loader_error : public std::runtime_error
        {
        public:
            using std::runtime_error::runtime_error;
        };

        class load_fail : public std::runtime_error
        {
        public:
            using std::runtime_error::runtime_error;
        };

        namespace detail
        {
            template <class AssetT>
            struct loadable_filter
            {
                template <class T>
                static constexpr bool value()
                {
                    constexpr auto res = meta::index_of_t<AssetT, typename T::types>() >= 0;
                    return res;
                }
            };

            template <class... AssetTs>
            class asset_cache
            {
            private:
                using asset_ts = meta::list<AssetTs...>;
                template <class T>
                using map_t = std::map<path_type, T>;
                using maps_ts = meta::map_t<meta::mapper<map_t>, asset_ts>;
            };

            template <class... LoaderTs>
            class asset_mgr
            {
                template <class AssetT>
                AssetT load_impl(path_type path);
            public:

                template <class AssetT>
                asset_ptr<AssetT> load(path_type path)
                {
                    return std::make_shared<AssetT>(load_impl<AssetT>(path));
                }

            private:
                using loader_ts = meta::list<LoaderTs...>;
                using loaders_tuple = meta::convert_t<std::tuple, loader_ts>;

                loaders_tuple loaders;
            };

            template <class... lTs>
            template<class AssetT>
            AssetT asset_mgr<lTs...>::load_impl(path_type path) {
                // determine the loaders that may be able to load AssetT
                // run their predicates
                // use the first loader to load the asset
                // return

                uint8_t buf[sizeof(AssetT)];
                AssetT* obj = reinterpret_cast<AssetT*>(buf);
                bool is_valid = false;

                asset_file file(path);

                using candidates = meta::filter_t<loadable_filter<AssetT>, loader_ts>;
                meta::for_each(candidates{}, [&](auto* c)
                {
                    if (is_valid) return;
                    using type = std::remove_pointer_t<decltype(c)>;
                    constexpr auto index = meta::index_of_t<type, loader_ts>();
                    auto& loader = std::get<index>(loaders);
                    if (loader.check(meta::type<AssetT>{}, (const asset_file&)file))
                    {
                        try
                        {
                            new (obj) AssetT(loader.load(meta::type<AssetT>{}, file));
                            is_valid = true;
                        }
                        catch (loader_error& err)
                        {
                            std::cerr << "fail: " << path << "\nloader failed, even though it said it could load it:\n  " << err.what() << '\n';
                        }
                    }
                });

                auto x = malt::at_exit([=]{
                    if (is_valid)
                    {
                        obj->~AssetT();
                    }
                });

                if (!is_valid)
                {
                    throw load_fail("Asset couldn't be loaded!");
                }

                return std::move(*obj);
            }
        }
    }
}
