//
// Created by fatih on 5/16/17.
//

#include <malt/list.hpp>
#include <tuple>
#include <iostream>
#include <malt/utilities.hpp>

namespace malt
{
    namespace assets
    {
        namespace detail
        {
            template <class AssetT>
            struct loadable_filter
            {
                template <class T>
                static constexpr bool value()
                {
                    constexpr auto res = meta::index_of_t<AssetT, typename T::types>() >= 0;
                    static_assert(res, "");
                    return res;
                }
            };

            template <class LoaderTs>
            class asset_mgr
            {
            public:
                template <class AssetT>
                AssetT load(const char* path)
                {
                    // determine the loaders that may be able to load AssetT
                    // run their predicates
                    // use the first loader to load the asset
                    // return

                    uint8_t buf[sizeof(AssetT)];
                    AssetT* obj = reinterpret_cast<AssetT*>(buf);

                    using candidates = meta::filter_t<loadable_filter<AssetT>, loader_ts>;
                    meta::for_each(candidates{}, [&](auto* c)
                    {
                        using type = std::remove_pointer_t<decltype(c)>;
                        constexpr auto index = meta::index_of_t<type, loader_ts>();
                        auto& loader = std::get<index>(loaders);
                        if (loader.check(meta::type<AssetT>{}))
                        {
                            new (obj) AssetT(loader.load(path));
                        }
                    });

                    auto x = malt::at_exit([=]{
                        obj->~AssetT();
                    });

                    return std::move(*obj);
                }

            private:
                using loader_ts = meta::list<LoaderTs>;
                using loaders_tuple = meta::convert_t<std::tuple, loader_ts>;

                loaders_tuple loaders;
            };
        }
    }
}
