malt.asset
===

This module implements asset loading for malt games.

After including the module in your project, you can basically load any asset using the following syntax:

```
auto asset = malt::asset::load<type_of_asset>("path/to/asset");
```

malt.asset will search for a proper asset loader for the type and the asset file with the path given, and will
return the loaded object. Loaders for assets defined by a module should be written in those modules for them to be
able to be loaded.

This module implements a very simple asset `text_asset` which basically represents a text file, and the appropriate
loader for it.

It can be used as:

```
using namespace malt;
auto txt_asset = asset::load<text_asset>("game_config.txt");
std::cout << txt_asset.c_str() << '\n';
```
