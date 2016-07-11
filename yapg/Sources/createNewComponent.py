#!/usr/bin/python3

headerContent = """#ifndef YAPG_GAME_COMPONENTS_{1}COMPONENT_H
#define YAPG_GAME_COMPONENTS_{1}COMPONENT_H

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace yapg
{{

class {0}Component : public Component
{{
public:
    {0}Component(entityx::Entity entity);
    virtual ~{0}Component();

    virtual std::string getName() const;

    static void registerComponent(LuaState& state);
}};

std::ostream& operator<<(std::ostream& stream, const {0}Component& component);

}}

#endif
"""

srcContent = """#include "Components/{0}Component.hpp"

#include "Lua/EntityHandle.hpp"

namespace yapg
{{

{0}Component::{0}Component(entityx::Entity entity) :
    Component(entity)
{{

}}

{0}Component::~{0}Component()
{{

}}

std::string {0}Component::getName() const
{{
    return "{0}";
}}

void {0}Component::registerComponent(LuaState& state)
{{
    meta::MetadataStore::registerClass<{0}Component>();

    EntityHandle::declareComponent<{0}Component>("{1}");

    state.getState().new_usertype<{0}Component>("{1}_component"
        //TODO: Register the properties here
    );
    state.declareComponentGetter<{0}Component>("{1}");
}}

std::ostream& operator<<(std::ostream& stream, const {0}Component& component)
{{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}}

}}
"""

import sys, getopt

def main(argv):
   componentName = ''
   try:
      opts, args = getopt.getopt(argv,"hc:",["component-name="])
   except getopt.GetoptError:
      print('createNewComponent.py -c <component_name>')
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print('Create an hpp and cpp file to declare a component')
         sys.exit()
      elif opt in ("-c", "--component-name"):
         componentName = arg
   generateHeaderFile(componentName)
   generateSrcFile(componentName)

def generateHeaderFile(name):
    headerPath = "./" + name + "Component.hpp"
    print("Creating " + headerPath + " header...")
    headerFile = open(headerPath, 'w')
    content = headerContent.format(name, name.upper())
    headerFile.write(content)
    headerFile.close()
    print(" [OK]")

def generateSrcFile(name):
    srcPath = "./" + name + "Component.cpp"
    print("Creating " + srcPath + " source...")
    srcFile = open(srcPath, 'w')
    content = srcContent.format(name, name.lower())
    srcFile.write(content)
    srcFile.close()
    print(" [OK]")

if __name__ == "__main__":
   main(sys.argv[1:])
