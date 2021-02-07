#pragma once
template <typename T>
void SafeDelete(T& ptr)
{
    if (ptr)
    {
        delete ptr;
        ptr = nullptr;
    }
}

template <typename T>
void SafeDeleteArr(T& ptr)
{
    if (ptr)
    {
        delete[] ptr;
        ptr = nullptr;
    }
}