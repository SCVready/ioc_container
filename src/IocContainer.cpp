
#include "IocContainer.hpp"

IocContainer& IocContainer::GetSingleton()
{
    static IocContainer singleton;
    return singleton;
}