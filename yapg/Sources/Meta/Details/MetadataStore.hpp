#ifndef YAPG_GAME_META_DETAILS_METADATASTORE_H
#define YAPG_GAME_META_DETAILS_METADATASTORE_H

#include <map>
#include <memory>
#include <typeindex>
#include <string>

#include "Meta/Details/ClassMetadata.hpp"
#include "Meta/Details/Metadata.hpp"
#include "Meta/Details/TypeMetadata.hpp"

namespace yapg
{

namespace meta
{

class MetadataStore
{
public:
    template<class C>
    static ClassMetadata<C>& registerClass(const std::string& className = std::string())
    {
        metadatas.emplace(std::type_index(typeid(C)), std::unique_ptr<Metadata>(new ClassMetadata<C>()));
        Metadata& to_return = *metadatas[std::type_index(typeid(C))];

        if(!className.empty())
        {
            //Add it also to the metadatasByNames
            metadatasByNames[className] = &to_return;
        }

        return dynamic_cast<ClassMetadata<C>&>(to_return);
    }

    template<typename T>
    static TypeMetadata<T>& registerType(const std::string& typeName = std::string())
    {
        metadatas.emplace(std::type_index(typeid(T)), std::unique_ptr<Metadata>(new TypeMetadata<T>()));
        Metadata& to_return = *metadatas[std::type_index(typeid(T))];

        if(!typeName.empty())
        {
            //Add it also to the metadatasByNames
            metadatasByNames[typeName] = &to_return;
        }

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

    static Metadata& getMetadata(const std::string& name)
    {
        if(metadatasByNames.count(name) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + name + std::string(", which is not declared to MetadataStore or not named !"));
        }
        return *metadatasByNames[name];
    }

private:
    static std::map<std::type_index, std::unique_ptr<Metadata>> metadatas;
    static std::map<std::string, Metadata*> metadatasByNames; //< Stores not owning pointers to metadata to be able to get a metadata according to its class/type name
};

}

}

#endif
