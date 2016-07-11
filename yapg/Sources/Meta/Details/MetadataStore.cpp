#include "Meta/Metadata.hpp"

namespace yapg
{

namespace meta
{

std::map<std::type_index, std::unique_ptr<Metadata>> MetadataStore::metadatas =
    std::map<std::type_index, std::unique_ptr<Metadata>>();

std::map<std::string, Metadata*> MetadataStore::metadatasByNames =
    std::map<std::string, Metadata*>();

}

}
