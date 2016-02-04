#ifndef YAPG_GAME_META_DETAILS_METADATASTORE_H
#define YAPG_GAME_META_DETAILS_METADATASTORE_H

#include <map>
#include <memory>
#include <typeindex>
#include <string>

#include "Meta/Details/ClassMetadata.hpp"
#include "Meta/Details/Metadata.hpp"
#include "Meta/Details/TypeMetadata.hpp"

namespace meta
{

class MetadataStore
{
public:
    template<class C>
    static ClassMetadata<C>& registerClass()
    {
        metadatas.emplace(std::type_index(typeid(C)), std::unique_ptr<Metadata>(new ClassMetadata<C>()));
        Metadata& to_return = *metadatas[std::type_index(typeid(C))];
        return dynamic_cast<ClassMetadata<C>&>(to_return);
    }

    template<typename T>
    static TypeMetadata<T>& registerType()
    {
        metadatas.emplace(std::type_index(typeid(T)), std::unique_ptr<Metadata>(new TypeMetadata<T>()));
        Metadata& to_return = *metadatas[std::type_index(typeid(T))];
        return dynamic_cast<TypeMetadata<T>&>(to_return);
    }

    template<class C>
    static Metadata& getMetadata()
    {
        if(metadatas.count(std::type_index(typeid(C))) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + std::type_index(typeid(C)).name() + std::string(", which is not declared to MetadataStore !"));
        }
        return *metadatas[std::type_index(typeid(C))];
    }

    static Metadata& getMetadata(std::type_index typeindex)
    {
        if(metadatas.count(typeindex) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + typeindex.name() + std::string(", which is not declared to MetadataStore !"));
        }
        return *metadatas[typeindex];
    }

private:
    static std::map<std::type_index, std::unique_ptr<Metadata>> metadatas;
};

}

#endif
