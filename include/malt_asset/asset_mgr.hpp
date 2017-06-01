//
// Created by fatih on 5/16/17.
//
#pragma once

#include <malt/list.hpp>
#include <tuple>
#include <iostream>
#include <malt/utilities.hpp>
#include <malt_asset/asset_loader.hpp>
#include <malt_asset/assets_def.hpp>

namespace malt
{
    namespace asset
    {
        class loader_error : public std::runtime_error
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
                    //static_assert(res, "no loader was able to load this type");
                    return res;
                }
            };

            template <class... LoaderTs>
            class asset_mgr
            {
            public:
                template <class AssetT>
                AssetT load(path_type path)
                {
                    // determine the loaders that may be able to load AssetT
                    // run their predicates
                    // use the first loader to load the asset
                    // return

                    uint8_t buf[sizeof(AssetT)];
                    AssetT* obj = reinterpret_cast<AssetT*>(buf);

                    asset_file file(path);

                    using candidates = meta::filter_t<loadable_filter<AssetT>, loader_ts>;
                    meta::for_each(candidates{}, [&](auto* c)
                    {
                        using type = std::remove_pointer_t<decltype(c)>;
                        constexpr auto index = meta::index_of_t<type, loader_ts>();
                        auto& loader = std::get<index>(loaders);
                        if (loader.check(meta::type<AssetT>{}, (const asset_file&)file))
                        {
                            try
                            {
                                new (obj) AssetT(loader.load(meta::type<AssetT>{}, file));
                            }
                            catch (loader_error& err)
                            {
                                std::cerr << "fail: " << path << "\nloader failed, even though it said it could load it:\n  " << err.what() << '\n';
                            }
                        }
                    });

                    auto x = malt::at_exit([=]{
                        obj->~AssetT();
                    });

                    return std::move(*obj);
                }

            private:
                using loader_ts = meta::list<LoaderTs...>;
                using loaders_tuple = meta::convert_t<std::tuple, loader_ts>;

                loaders_tuple loaders;
            };
        }
    }
}
