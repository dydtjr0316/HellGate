#pragma once
#define CHILD_NODE_SIZE 4
#define MAX_PLAYER_IN_NODE 4

template <typename T>
void SafeDelete(T& ptr)
{
    if (ptr)
    {
        delete ptr;
        ptr = nullptr;
    }
}