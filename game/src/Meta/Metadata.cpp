#include "Meta/Metadata.hpp"

namespace game
{
namespace meta
{

std::map<std::type_index, std::unique_ptr<Metadata>> MetadataStore::metadatas =
    std::map<std::type_index, std::unique_ptr<Metadata>>();

}
}