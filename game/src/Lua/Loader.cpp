#include "Lua/Loader.hpp"

namespace game
{
namespace lua
{

std::map<std::type_index, std::unique_ptr<LoaderMetadata>> MetadataStore::metadatas =
    std::map<std::type_index, std::unique_ptr<LoaderMetadata>>();

}
}
