#include "simplgui/TextBox.h"

#include <iostream>
#include "simplgui/Renderer.h"

namespace simplgui
{

std::shared_ptr<TextBox> TextBox::create(std::shared_ptr<ResourcesGetter> resGetter)
{
    return std::shared_ptr<TextBox>(new TextBox(resGetter));
}

TextBox::TextBox(std::shared_ptr<ResourcesGetter> resGetter) :
    Widget(resGetter),
    onTextChanged(),
    m_string(),
    m_displayedStr(),
    m_font(nullptr),
    m_firstDisplayedCharIndex(0),
    m_lastDisplayedCharIndex(0),
    m_selectionStart(0),
    m_selectionLen(0),
    m_hideChar(0),
    m_leftClicking(false)
{
    setSize(sf::Vector2f(150.f, 40.f));

    doThemeUpdate(); //Update the textbox appearance
}

int TextBox::getSelectionStart() const
{
    return m_selectionStart;
}

int TextBox::getSelectionLen() const
{
    return m_selectionLen;
}

void TextBox::setSelection(int start, int len)
{
    m_selectionStart = start;
    m_selectionLen = len;

    if(m_selectionStart + m_selectionLen == static_cast<int>(m_string.size()) && !m_string.empty())
        ensureCharacterIsVisible(m_selectionStart + m_selectionLen - 1);
    else
        ensureCharacterIsVisible(m_selectionStart + m_selectionLen);
}

void TextBox::setText(const std::u32string &text)
{
    m_string = text;

    updateText();
    setSelection(0);
    needAutoSizeUpdate();
}

void TextBox::setHideCharacter(char32_t hideChar)
{
    m_hideChar = hideChar;

    updateText();
    needAutoSizeUpdate();
}

void TextBox::doProcessEvent(simplgui::Event event)
{
    if(isFocused())
    {
        //Get the bounding box of the widget
        sf::Transform globalTr = getGlobalTransform();
        sf::FloatRect widgetRect(sf::Vector2f(0.f, 0.f), getEffectiveSize());
        widgetRect = globalTr.transformRect(widgetRect);

        if(event.type == simplgui::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
                {
                    sf::Vector2f relativeMousePosition(globalTr.getInverse().transformPoint(event.mouseButton.x, event.mouseButton.y));
                    setSelection(getCharacterIndexAt(relativeMousePosition.x, relativeMousePosition.y));
                    m_leftClicking = true;
                }
            }

            needAutoSizeUpdate();
            updateText();
        }
        else if(event.type == simplgui::Event::MouseMoved)
        {
            if(m_leftClicking)
            {
                if(widgetRect.contains(event.mouseMove.x, event.mouseMove.y))
                {
                    sf::Vector2f relativeMousePosition(globalTr.getInverse().transformPoint(event.mouseMove.x, event.mouseMove.y));
                    setSelection(m_selectionStart, getCharacterIndexAt(relativeMousePosition.x, relativeMousePosition.y) - m_selectionStart);
                }
            }

            needAutoSizeUpdate();
            updateText();
        }
        else if(event.type == simplgui::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Left && m_leftClicking)
            {
                if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
                {
                    sf::Vector2f relativeMousePosition(globalTr.getInverse().transformPoint(event.mouseButton.x, event.mouseButton.y));
                    setSelection(m_selectionStart, getCharacterIndexAt(relativeMousePosition.x, relativeMousePosition.y) - m_selectionStart);
                }
                m_leftClicking = false;
            }

            needAutoSizeUpdate();
            updateText();
        }
        else if(event.type == simplgui::Event::TextEntered)
        {
            sf::Uint32 character = event.text.unicode;
            if(character > 30 && (character < 127 || character > 159))
            {
                m_string.replace(
                    std::min(m_selectionStart, m_selectionStart + m_selectionLen),
                    std::max(m_selectionLen, -m_selectionLen),
                    std::u32string(1, static_cast<char32_t>(character))
                    );

                setSelection(std::min(m_selectionStart + 1, m_selectionStart + m_selectionLen + 1));
            }
            else if(character == 8)
            {
                if(!m_string.empty())
                {
                    if(!hasMultipleCharSelected() && m_selectionStart > 0)
                    {
                        m_string.replace(m_selectionStart-1, 1, U"");
                        setSelection(m_selectionStart-1);
                    }
                    else if(hasMultipleCharSelected())
                    {
                        eraseSelection();
                    }
                }
            }
            else //Avoid useless update (below) if the text is not processed
                return;

            needAutoSizeUpdate();
            updateText();

            onTextChanged.call(shared_from_this(), m_string);
        }
        else if(event.type == simplgui::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Delete)
            {
                if(!m_string.empty())
                {
                    if(!hasMultipleCharSelected() && m_selectionStart < static_cast<int>(m_string.size()))
                    {
                        m_string.replace(m_selectionStart, 1, U"");
                        setSelection(m_selectionStart);

                        needAutoSizeUpdate();
                        updateText();
                    }
                    else if(hasMultipleCharSelected())
                    {
                        eraseSelection();

                        needAutoSizeUpdate();
                        updateText();
                    }
                }
            }
            else if(event.key.code == sf::Keyboard::Left)
            {
                if(event.key.shift)
                {
                    if(m_selectionStart + m_selectionLen > 0)
                        setSelection(m_selectionStart, m_selectionLen-1);
                }
                else
                {
                    if(m_selectionStart > 0)
                    {
                        if(!hasMultipleCharSelected())
                            setSelection(m_selectionStart - 1);
                        else
                            setSelection(std::min(m_selectionStart, m_selectionStart + m_selectionLen));
                    }
                }
            }
            else if(event.key.code == sf::Keyboard::Right)
            {
                if(event.key.shift)
                {
                    if(m_selectionStart + m_selectionLen < static_cast<int>(m_string.size()))
                        setSelection(m_selectionStart, m_selectionLen+1);
                }
                else
                {
                    if(m_selectionStart < static_cast<int>(m_string.size()))
                    {
                        if(!hasMultipleCharSelected())
                            setSelection(m_selectionStart + m_selectionLen + 1);
                        else
                            setSelection(std::max(m_selectionStart, m_selectionStart + m_selectionLen));
                    }
                }
            }
        }
    }
}

void TextBox::doUpdate(sf::Time)
{

}

void TextBox::onSizeUpdated()
{
    updateText();
}

sf::Vector2f TextBox::doCalculateAutoSize() const
{
    if(!m_font)
        return sf::Vector2f(0.f, 0.f);

    sf::Vector2f currentTextSize = Renderer::getTextSize(m_string, *m_font, getTheme().getProperty<unsigned int>("font_size", 30));
    //Calculate the height using a custom text (so as the height is always correct even if the text is empty)
    sf::Vector2f textSize = Renderer::getTextSize(U"abfgjl", *m_font, getTheme().getProperty<unsigned int>("font_size", 30));

    return sf::Vector2f(currentTextSize.x, textSize.y) + sf::Vector2f(6.f, 6.f);
}

void TextBox::doThemeUpdate()
{
    if(getResourcesGetter().expired())
    {
        std::cout << "[WARNING] simplgui::TextBox: No resources getter affected, can't load the font." << std::endl;
        return;
    }
    std::shared_ptr<ResourcesGetter> resGetter(getResourcesGetter());

    m_font = resGetter->loadFont(getTheme().getProperty<std::string>("font", "Liberation.ttf"));

    updateText();
    needAutoSizeUpdate();
}

void TextBox::draw(sf::RenderTarget &target, sf::RenderStates) const
{
    //Draw the textbox background
    Renderer::drawBackgroundRectangle(target, shared_from_this(), sf::FloatRect({0, 0}, getEffectiveSize()));

    //Draw the cursor or the selection
    if(!hasMultipleCharSelected())
    {
        Renderer::drawRectangle(
            target,
            sf::FloatRect({getCharacterPosition(m_selectionStart).x, 3.f}, {2.f, getEffectiveSize().y - 6.f}),
            0.f,
            isFocused() ? getTheme().getProperty<StateColor>("text_color").normal : sf::Color::Transparent,
            sf::Color::Transparent,
            getGlobalTransform()
        );
    }
    else
    {
        Renderer::drawSelectionRectangle(
            target,
            shared_from_this(),
            sf::FloatRect(
                {getCharacterPosition(m_selectionStart).x, 3.f},
                {getCharacterPosition(m_selectionLen + m_selectionStart).x - getCharacterPosition(m_selectionStart).x, getEffectiveSize().y - 6.f}
            )
        );
    }

    //Draw the text
    Renderer::drawText(target, shared_from_this(), m_displayedStr, *m_font, sf::Vector2f(3.f, 3.f));
}

void TextBox::updateText()
{
    std::u32string strToDisplay;
    if(m_hideChar == 0)
        strToDisplay = m_string;
    else
        strToDisplay = std::u32string(m_string.size(), m_hideChar);

    m_displayedStr.clear();
    unsigned int textSize = getTheme().getProperty<unsigned int>("font_size", 30);

    for(auto it = strToDisplay.begin() + m_firstDisplayedCharIndex;
        it != strToDisplay.end() && (3.f + Renderer::getTextSize(m_displayedStr, *m_font, textSize).x < (getSize().x == AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f);
        ++it)
    {
        m_displayedStr.push_back(*it);
    }

    if(3.f + Renderer::getTextSize(m_displayedStr, *m_font, textSize).x >= (getSize().x == AUTO_SIZE ? getMaxSize().x : getSize().x) - 6.f)
        m_displayedStr.erase(m_displayedStr.size()-1);

    m_lastDisplayedCharIndex = m_displayedStr.size() > 0 ? m_firstDisplayedCharIndex + m_displayedStr.size()-1 : 0;
}

void TextBox::ensureCharacterIsVisible(unsigned int pos)
{
    updateText();

    //Move the display margin until the character is visible
    while(pos < m_firstDisplayedCharIndex)
    {
        --m_firstDisplayedCharIndex;
        needAutoSizeUpdate();
        updateText();
    }
    while(pos > m_lastDisplayedCharIndex)
    {
        ++m_firstDisplayedCharIndex;
        needAutoSizeUpdate();
        updateText();
    }

    //Try to avoid a blank space on the right of the TextBox if the text is too much offsetted
    while(m_lastDisplayedCharIndex >= m_string.size() - 1 && m_firstDisplayedCharIndex > 0)
    {
        --m_firstDisplayedCharIndex;
        needAutoSizeUpdate();
        updateText();
        if(m_lastDisplayedCharIndex < m_string.size() - 1)
        {
            ++m_firstDisplayedCharIndex;
            needAutoSizeUpdate();
            updateText();
            break;
        }
    }
}

bool TextBox::hasMultipleCharSelected() const
{
    return (m_selectionLen != 0);
}

sf::Vector2f TextBox::getCharacterPosition(int index) const
{
    return sf::Vector2f(3.f, 3.f) + Renderer::getCharPosInText(m_displayedStr, *m_font, getTheme().getProperty<unsigned int>("font_size", 30), index - m_firstDisplayedCharIndex);
}

int TextBox::getCharacterIndexAt(float x, float) const
{
    unsigned int charIndex = 0;

    if(m_displayedStr.empty())
        return 0;

    auto getCharAt = std::bind(Renderer::getCharPosInText, m_displayedStr, *m_font, getTheme().getProperty<unsigned int>("font_size", 30), std::placeholders::_1);
    sf::Vector2f textSize = Renderer::getTextSize(m_displayedStr, *m_font, getTheme().getProperty<unsigned int>("font_size", 30));

    while((getCharAt(charIndex).x + 3.f /* the margin */ + (charIndex < (m_lastDisplayedCharIndex - m_firstDisplayedCharIndex) ?
                getCharAt(charIndex+1).x + 3.f :
                (textSize.x + 3.f)))/2.f < x &&
        charIndex < (m_lastDisplayedCharIndex - m_firstDisplayedCharIndex + 1))
    {
        ++charIndex;
    }

    return charIndex + m_firstDisplayedCharIndex;
}

void TextBox::eraseSelection()
{
    m_string.replace(
        std::min(m_selectionStart, m_selectionStart + m_selectionLen),
        std::max(m_selectionLen, -m_selectionLen),
        U""
    );

    setSelection(m_selectionStart + std::min(0, m_selectionLen));
}

}
