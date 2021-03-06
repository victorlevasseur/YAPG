#include "Tools/KeyStrings.hpp"

namespace yapg
{

std::string keyToString(sf::Keyboard::Key key)
{
    #define STRINGIFY_KEY_IF(X) case sf::Keyboard::X: return #X;

    switch(key)
    {
        STRINGIFY_KEY_IF(Unknown)
        STRINGIFY_KEY_IF(A)
        STRINGIFY_KEY_IF(B)
        STRINGIFY_KEY_IF(C)
        STRINGIFY_KEY_IF(D)
        STRINGIFY_KEY_IF(E)
        STRINGIFY_KEY_IF(F)
        STRINGIFY_KEY_IF(G)
        STRINGIFY_KEY_IF(H)
        STRINGIFY_KEY_IF(I)
        STRINGIFY_KEY_IF(J)
        STRINGIFY_KEY_IF(K)
        STRINGIFY_KEY_IF(L)
        STRINGIFY_KEY_IF(M)
        STRINGIFY_KEY_IF(N)
        STRINGIFY_KEY_IF(O)
        STRINGIFY_KEY_IF(P)
        STRINGIFY_KEY_IF(Q)
        STRINGIFY_KEY_IF(R)
        STRINGIFY_KEY_IF(S)
        STRINGIFY_KEY_IF(T)
        STRINGIFY_KEY_IF(U)
        STRINGIFY_KEY_IF(V)
        STRINGIFY_KEY_IF(W)
        STRINGIFY_KEY_IF(X)
        STRINGIFY_KEY_IF(Y)
        STRINGIFY_KEY_IF(Z)
        STRINGIFY_KEY_IF(Num0)
        STRINGIFY_KEY_IF(Num1)
        STRINGIFY_KEY_IF(Num2)
        STRINGIFY_KEY_IF(Num3)
        STRINGIFY_KEY_IF(Num4)
        STRINGIFY_KEY_IF(Num5)
        STRINGIFY_KEY_IF(Num6)
        STRINGIFY_KEY_IF(Num7)
        STRINGIFY_KEY_IF(Num8)
        STRINGIFY_KEY_IF(Num9)
        STRINGIFY_KEY_IF(Escape)
        STRINGIFY_KEY_IF(LControl)
        STRINGIFY_KEY_IF(LShift)
        STRINGIFY_KEY_IF(LAlt)
        STRINGIFY_KEY_IF(LSystem)
        STRINGIFY_KEY_IF(RControl)
        STRINGIFY_KEY_IF(RShift)
        STRINGIFY_KEY_IF(RAlt)
        STRINGIFY_KEY_IF(RSystem)
        STRINGIFY_KEY_IF(Menu)
        STRINGIFY_KEY_IF(LBracket)
        STRINGIFY_KEY_IF(RBracket)
        STRINGIFY_KEY_IF(SemiColon)
        STRINGIFY_KEY_IF(Comma)
        STRINGIFY_KEY_IF(Period)
        STRINGIFY_KEY_IF(Quote)
        STRINGIFY_KEY_IF(Slash)
        STRINGIFY_KEY_IF(BackSlash)
        STRINGIFY_KEY_IF(Tilde)
        STRINGIFY_KEY_IF(Equal)
        STRINGIFY_KEY_IF(Dash)
        STRINGIFY_KEY_IF(Space)
        STRINGIFY_KEY_IF(Return)
        STRINGIFY_KEY_IF(BackSpace)
        STRINGIFY_KEY_IF(Tab)
        STRINGIFY_KEY_IF(PageUp)
        STRINGIFY_KEY_IF(PageDown)
        STRINGIFY_KEY_IF(End)
        STRINGIFY_KEY_IF(Home)
        STRINGIFY_KEY_IF(Insert)
        STRINGIFY_KEY_IF(Delete)
        STRINGIFY_KEY_IF(Add)
        STRINGIFY_KEY_IF(Subtract)
        STRINGIFY_KEY_IF(Multiply)
        STRINGIFY_KEY_IF(Divide)
        STRINGIFY_KEY_IF(Left)
        STRINGIFY_KEY_IF(Right)
        STRINGIFY_KEY_IF(Up)
        STRINGIFY_KEY_IF(Down)
        STRINGIFY_KEY_IF(Numpad0)
        STRINGIFY_KEY_IF(Numpad1)
        STRINGIFY_KEY_IF(Numpad2)
        STRINGIFY_KEY_IF(Numpad3)
        STRINGIFY_KEY_IF(Numpad4)
        STRINGIFY_KEY_IF(Numpad5)
        STRINGIFY_KEY_IF(Numpad6)
        STRINGIFY_KEY_IF(Numpad7)
        STRINGIFY_KEY_IF(Numpad8)
        STRINGIFY_KEY_IF(Numpad9)
        STRINGIFY_KEY_IF(F1)
        STRINGIFY_KEY_IF(F2)
        STRINGIFY_KEY_IF(F3)
        STRINGIFY_KEY_IF(F4)
        STRINGIFY_KEY_IF(F5)
        STRINGIFY_KEY_IF(F6)
        STRINGIFY_KEY_IF(F7)
        STRINGIFY_KEY_IF(F8)
        STRINGIFY_KEY_IF(F9)
        STRINGIFY_KEY_IF(F10)
        STRINGIFY_KEY_IF(F11)
        STRINGIFY_KEY_IF(F12)
        STRINGIFY_KEY_IF(F13)
        STRINGIFY_KEY_IF(F14)
        STRINGIFY_KEY_IF(F15)
        STRINGIFY_KEY_IF(Pause)
        default: return "Invalid";
    }

    #undef STRINGIFY_KEY_IF
}

sf::Keyboard::Key stringToKey(const std::string& str)
{
    #define UNSTRINGIFY_KEY_IF(X) if(str == #X) return sf::Keyboard::X;

    UNSTRINGIFY_KEY_IF(Unknown)
    UNSTRINGIFY_KEY_IF(A)
    UNSTRINGIFY_KEY_IF(B)
    UNSTRINGIFY_KEY_IF(C)
    UNSTRINGIFY_KEY_IF(D)
    UNSTRINGIFY_KEY_IF(E)
    UNSTRINGIFY_KEY_IF(F)
    UNSTRINGIFY_KEY_IF(G)
    UNSTRINGIFY_KEY_IF(H)
    UNSTRINGIFY_KEY_IF(I)
    UNSTRINGIFY_KEY_IF(J)
    UNSTRINGIFY_KEY_IF(K)
    UNSTRINGIFY_KEY_IF(L)
    UNSTRINGIFY_KEY_IF(M)
    UNSTRINGIFY_KEY_IF(N)
    UNSTRINGIFY_KEY_IF(O)
    UNSTRINGIFY_KEY_IF(P)
    UNSTRINGIFY_KEY_IF(Q)
    UNSTRINGIFY_KEY_IF(R)
    UNSTRINGIFY_KEY_IF(S)
    UNSTRINGIFY_KEY_IF(T)
    UNSTRINGIFY_KEY_IF(U)
    UNSTRINGIFY_KEY_IF(V)
    UNSTRINGIFY_KEY_IF(W)
    UNSTRINGIFY_KEY_IF(X)
    UNSTRINGIFY_KEY_IF(Y)
    UNSTRINGIFY_KEY_IF(Z)
    UNSTRINGIFY_KEY_IF(Num0)
    UNSTRINGIFY_KEY_IF(Num1)
    UNSTRINGIFY_KEY_IF(Num2)
    UNSTRINGIFY_KEY_IF(Num3)
    UNSTRINGIFY_KEY_IF(Num4)
    UNSTRINGIFY_KEY_IF(Num5)
    UNSTRINGIFY_KEY_IF(Num6)
    UNSTRINGIFY_KEY_IF(Num7)
    UNSTRINGIFY_KEY_IF(Num8)
    UNSTRINGIFY_KEY_IF(Num9)
    UNSTRINGIFY_KEY_IF(Escape)
    UNSTRINGIFY_KEY_IF(LControl)
    UNSTRINGIFY_KEY_IF(LShift)
    UNSTRINGIFY_KEY_IF(LAlt)
    UNSTRINGIFY_KEY_IF(LSystem)
    UNSTRINGIFY_KEY_IF(RControl)
    UNSTRINGIFY_KEY_IF(RShift)
    UNSTRINGIFY_KEY_IF(RAlt)
    UNSTRINGIFY_KEY_IF(RSystem)
    UNSTRINGIFY_KEY_IF(Menu)
    UNSTRINGIFY_KEY_IF(LBracket)
    UNSTRINGIFY_KEY_IF(RBracket)
    UNSTRINGIFY_KEY_IF(SemiColon)
    UNSTRINGIFY_KEY_IF(Comma)
    UNSTRINGIFY_KEY_IF(Period)
    UNSTRINGIFY_KEY_IF(Quote)
    UNSTRINGIFY_KEY_IF(Slash)
    UNSTRINGIFY_KEY_IF(BackSlash)
    UNSTRINGIFY_KEY_IF(Tilde)
    UNSTRINGIFY_KEY_IF(Equal)
    UNSTRINGIFY_KEY_IF(Dash)
    UNSTRINGIFY_KEY_IF(Space)
    UNSTRINGIFY_KEY_IF(Return)
    UNSTRINGIFY_KEY_IF(BackSpace)
    UNSTRINGIFY_KEY_IF(Tab)
    UNSTRINGIFY_KEY_IF(PageUp)
    UNSTRINGIFY_KEY_IF(PageDown)
    UNSTRINGIFY_KEY_IF(End)
    UNSTRINGIFY_KEY_IF(Home)
    UNSTRINGIFY_KEY_IF(Insert)
    UNSTRINGIFY_KEY_IF(Delete)
    UNSTRINGIFY_KEY_IF(Add)
    UNSTRINGIFY_KEY_IF(Subtract)
    UNSTRINGIFY_KEY_IF(Multiply)
    UNSTRINGIFY_KEY_IF(Divide)
    UNSTRINGIFY_KEY_IF(Left)
    UNSTRINGIFY_KEY_IF(Right)
    UNSTRINGIFY_KEY_IF(Up)
    UNSTRINGIFY_KEY_IF(Down)
    UNSTRINGIFY_KEY_IF(Numpad0)
    UNSTRINGIFY_KEY_IF(Numpad1)
    UNSTRINGIFY_KEY_IF(Numpad2)
    UNSTRINGIFY_KEY_IF(Numpad3)
    UNSTRINGIFY_KEY_IF(Numpad4)
    UNSTRINGIFY_KEY_IF(Numpad5)
    UNSTRINGIFY_KEY_IF(Numpad6)
    UNSTRINGIFY_KEY_IF(Numpad7)
    UNSTRINGIFY_KEY_IF(Numpad8)
    UNSTRINGIFY_KEY_IF(Numpad9)
    UNSTRINGIFY_KEY_IF(F1)
    UNSTRINGIFY_KEY_IF(F2)
    UNSTRINGIFY_KEY_IF(F3)
    UNSTRINGIFY_KEY_IF(F4)
    UNSTRINGIFY_KEY_IF(F5)
    UNSTRINGIFY_KEY_IF(F6)
    UNSTRINGIFY_KEY_IF(F7)
    UNSTRINGIFY_KEY_IF(F8)
    UNSTRINGIFY_KEY_IF(F9)
    UNSTRINGIFY_KEY_IF(F10)
    UNSTRINGIFY_KEY_IF(F11)
    UNSTRINGIFY_KEY_IF(F12)
    UNSTRINGIFY_KEY_IF(F13)
    UNSTRINGIFY_KEY_IF(F14)
    UNSTRINGIFY_KEY_IF(F15)
    UNSTRINGIFY_KEY_IF(Pause)

    #undef UNSTRINGIFY_KEY_IF

    return sf::Keyboard::Unknown;
}

}
