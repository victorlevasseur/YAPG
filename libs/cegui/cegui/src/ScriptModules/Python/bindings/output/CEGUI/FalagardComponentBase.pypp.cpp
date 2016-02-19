// This file has been generated by Py++.

#include "boost/python.hpp"
#include "generators/include/python_CEGUI.h"
#include "FalagardComponentBase.pypp.hpp"

namespace bp = boost::python;

struct FalagardComponentBase_wrapper : CEGUI::FalagardComponentBase, bp::wrapper< CEGUI::FalagardComponentBase > {

    FalagardComponentBase_wrapper( )
    : CEGUI::FalagardComponentBase( )
      , bp::wrapper< CEGUI::FalagardComponentBase >(){
        // null constructor
    
    }

    virtual bool handleFontRenderSizeChange( ::CEGUI::Window & window, ::CEGUI::Font const * font ) const  {
        if( bp::override func_handleFontRenderSizeChange = this->get_override( "handleFontRenderSizeChange" ) )
            return func_handleFontRenderSizeChange( boost::ref(window), boost::python::ptr(font) );
        else{
            return this->CEGUI::FalagardComponentBase::handleFontRenderSizeChange( boost::ref(window), boost::python::ptr(font) );
        }
    }
    
    bool default_handleFontRenderSizeChange( ::CEGUI::Window & window, ::CEGUI::Font const * font ) const  {
        return CEGUI::FalagardComponentBase::handleFontRenderSizeChange( boost::ref(window), boost::python::ptr(font) );
    }

    void initColoursRect( ::CEGUI::Window const & wnd, ::CEGUI::ColourRect const * modCols, ::CEGUI::ColourRect & cr ) const {
        CEGUI::FalagardComponentBase::initColoursRect( boost::ref(wnd), boost::python::ptr(modCols), boost::ref(cr) );
    }

    virtual void render_impl( ::CEGUI::Window & srcWindow, ::CEGUI::Rectf & destRect, ::CEGUI::ColourRect const * modColours, ::CEGUI::Rectf const * clipper, bool clipToDisplay ) const {
        bp::override func_render_impl = this->get_override( "render_impl" );
        func_render_impl( boost::ref(srcWindow), boost::ref(destRect), boost::python::ptr(modColours), boost::python::ptr(clipper), clipToDisplay );
    }

    bool writeColoursXML( ::CEGUI::XMLSerializer & xml_stream ) const {
        return CEGUI::FalagardComponentBase::writeColoursXML( boost::ref(xml_stream) );
    }

};

void register_FalagardComponentBase_class(){

    { //::CEGUI::FalagardComponentBase
        typedef bp::class_< FalagardComponentBase_wrapper, boost::noncopyable > FalagardComponentBase_exposer_t;
        FalagardComponentBase_exposer_t FalagardComponentBase_exposer = FalagardComponentBase_exposer_t( "FalagardComponentBase", bp::init< >() );
        bp::scope FalagardComponentBase_scope( FalagardComponentBase_exposer );
        { //::CEGUI::FalagardComponentBase::getColours
        
            typedef ::CEGUI::ColourRect const & ( ::CEGUI::FalagardComponentBase::*getColours_function_type )(  ) const;
            
            FalagardComponentBase_exposer.def( 
                "getColours"
                , getColours_function_type( &::CEGUI::FalagardComponentBase::getColours )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "*!\n\
                \n\
                    Return the ColourRect used by this component.\n\
            \n\
                @return\n\
                    ColourRect object holding the colours currently in use by this\n\
                    component.\n\
                *\n" );
        
        }
        { //::CEGUI::FalagardComponentBase::getComponentArea
        
            typedef ::CEGUI::ComponentArea const & ( ::CEGUI::FalagardComponentBase::*getComponentArea_function_type )(  ) const;
            
            FalagardComponentBase_exposer.def( 
                "getComponentArea"
                , getComponentArea_function_type( &::CEGUI::FalagardComponentBase::getComponentArea )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "*!\n\
                \n\
                    Return the ComponentArea of this component.\n\
            \n\
                @return\n\
                    ComponentArea object describing the component's current target area.\n\
                *\n" );
        
        }
        { //::CEGUI::FalagardComponentBase::handleFontRenderSizeChange
        
            typedef bool ( ::CEGUI::FalagardComponentBase::*handleFontRenderSizeChange_function_type )( ::CEGUI::Window &,::CEGUI::Font const * ) const;
            typedef bool ( FalagardComponentBase_wrapper::*default_handleFontRenderSizeChange_function_type )( ::CEGUI::Window &,::CEGUI::Font const * ) const;
            
            FalagardComponentBase_exposer.def( 
                "handleFontRenderSizeChange"
                , handleFontRenderSizeChange_function_type(&::CEGUI::FalagardComponentBase::handleFontRenderSizeChange)
                , default_handleFontRenderSizeChange_function_type(&FalagardComponentBase_wrapper::default_handleFontRenderSizeChange)
                , ( bp::arg("window"), bp::arg("font") ) );
        
        }
        { //::CEGUI::FalagardComponentBase::initColoursRect
        
            typedef void ( FalagardComponentBase_wrapper::*initColoursRect_function_type )( ::CEGUI::Window const &,::CEGUI::ColourRect const *,::CEGUI::ColourRect & ) const;
            
            FalagardComponentBase_exposer.def( 
                "initColoursRect"
                , initColoursRect_function_type( &FalagardComponentBase_wrapper::initColoursRect )
                , ( bp::arg("wnd"), bp::arg("modCols"), bp::arg("cr") ) );
        
        }
        { //::CEGUI::FalagardComponentBase::render
        
            typedef void ( ::CEGUI::FalagardComponentBase::*render_function_type )( ::CEGUI::Window &,::CEGUI::ColourRect const *,::CEGUI::Rectf const *,bool ) const;
            
            FalagardComponentBase_exposer.def( 
                "render"
                , render_function_type( &::CEGUI::FalagardComponentBase::render )
                , ( bp::arg("srcWindow"), bp::arg("modColours")=bp::object(), bp::arg("clipper")=bp::object(), bp::arg("clipToDisplay")=(bool)(false) )
                , "*!\n\
                \n\
                    Render this component.  More correctly, the component is cached for\n\
                    rendering.\n\
            \n\
                @param srcWindow\n\
                    Window to use as the base for translating the component's ComponentArea\n\
                    into pixel values.\n\
            \n\
                @param modColours\n\
                    ColourRect describing colours that are to be modulated with the\n\
                    component's stored colour values to calculate a set of 'final' colour\n\
                    values to be used.  May be 0.\n\
                *\n" );
        
        }
        { //::CEGUI::FalagardComponentBase::render
        
            typedef void ( ::CEGUI::FalagardComponentBase::*render_function_type )( ::CEGUI::Window &,::CEGUI::Rectf const &,::CEGUI::ColourRect const *,::CEGUI::Rectf const *,bool ) const;
            
            FalagardComponentBase_exposer.def( 
                "render"
                , render_function_type( &::CEGUI::FalagardComponentBase::render )
                , ( bp::arg("srcWindow"), bp::arg("baseRect"), bp::arg("modColours")=bp::object(), bp::arg("clipper")=bp::object(), bp::arg("clipToDisplay")=(bool)(false) ) );
        
        }
        { //::CEGUI::FalagardComponentBase::render_impl
        
            typedef void ( FalagardComponentBase_wrapper::*render_impl_function_type )( ::CEGUI::Window &,::CEGUI::Rectf &,::CEGUI::ColourRect const *,::CEGUI::Rectf const *,bool ) const;
            
            FalagardComponentBase_exposer.def( 
                "render_impl"
                , render_impl_function_type( &FalagardComponentBase_wrapper::render_impl )
                , ( bp::arg("srcWindow"), bp::arg("destRect"), bp::arg("modColours"), bp::arg("clipper"), bp::arg("clipToDisplay") ) );
        
        }
        { //::CEGUI::FalagardComponentBase::setColours
        
            typedef void ( ::CEGUI::FalagardComponentBase::*setColours_function_type )( ::CEGUI::ColourRect const & ) ;
            
            FalagardComponentBase_exposer.def( 
                "setColours"
                , setColours_function_type( &::CEGUI::FalagardComponentBase::setColours )
                , ( bp::arg("cols") )
                , "*!\n\
                \n\
                    Set the colours to be used by this component.\n\
            \n\
                @param cols\n\
                    ColourRect object describing the colours to be used by this component.\n\
                *\n" );
        
        }
        { //::CEGUI::FalagardComponentBase::setColoursPropertySource
        
            typedef void ( ::CEGUI::FalagardComponentBase::*setColoursPropertySource_function_type )( ::CEGUI::String const & ) ;
            
            FalagardComponentBase_exposer.def( 
                "setColoursPropertySource"
                , setColoursPropertySource_function_type( &::CEGUI::FalagardComponentBase::setColoursPropertySource )
                , ( bp::arg("property") )
                , "*!\n\
                \n\
                    Set the name of the property where colour values can be obtained.\n\
            \n\
                @param property\n\
                    String containing the name of the property.\n\
                *\n" );
        
        }
        { //::CEGUI::FalagardComponentBase::setComponentArea
        
            typedef void ( ::CEGUI::FalagardComponentBase::*setComponentArea_function_type )( ::CEGUI::ComponentArea const & ) ;
            
            FalagardComponentBase_exposer.def( 
                "setComponentArea"
                , setComponentArea_function_type( &::CEGUI::FalagardComponentBase::setComponentArea )
                , ( bp::arg("area") )
                , "*!\n\
                \n\
                    Set the conponent's ComponentArea.\n\
            \n\
                @param area\n\
                    ComponentArea object describing a new target area for the component.\n\
                *\n" );
        
        }
        { //::CEGUI::FalagardComponentBase::writeColoursXML
        
            typedef bool ( FalagardComponentBase_wrapper::*writeColoursXML_function_type )( ::CEGUI::XMLSerializer & ) const;
            
            FalagardComponentBase_exposer.def( 
                "writeColoursXML"
                , writeColoursXML_function_type( &FalagardComponentBase_wrapper::writeColoursXML )
                , ( bp::arg("xml_stream") )
                , "*!\n\
                \n\
                    Writes xml for the colours to a OutStream.\n\
                    Will prefer property colours before explicit.\n\
            \n\
                \note\n\
                    This is intended as a helper function for sub-classes when outputting\n\
                    xml to a stream.\n\
            \n\
                @return\n\
                    - true if xml element was written.\n\
                    - false if nothing was output due to the formatting not being set\n\
                      (sub-class may then choose to do something else.)\n\
                *\n" );
        
        }
    }

}
