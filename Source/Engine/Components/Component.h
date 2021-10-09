#pragma once
#include <Core/Math.h>

namespace Entities {
    class Entity;
}

namespace Component {
    struct IComponent
    {
        Uint m_TypeId = 0;

    private:
        inline static Uint m_MaxComponentTypeId = 0;

    public:
        template <typename T>
        static unsigned GetComponentTypeID()
        {
            static_assert(std::is_base_of<IComponent, T>::value, "Not a component");
            static Uint typeID = m_MaxComponentTypeId++;
            return typeID;
        }
    };
} // namespace Component
