#ifndef CG_SOLUTIONIMPL_H
#define CG_SOLUTIONIMPL_H

#include <map>

template <typename T, typename U>
class SolutionImpl
{
public:
    SolutionImpl(std::initializer_list<std::pair<T, U>> l);
    void Add(std::pair<T, U>);
    U Solute(T key);
    ~SolutionImpl();

private:
    std::map<T, U> solutions;
};


template <typename T, typename U>
SolutionImpl<T, U>::SolutionImpl(std::initializer_list<std::pair<T, U>> l)
        : solutions(l) {}

template <typename T, typename U>
void SolutionImpl<T, U>::Add(std::pair<T, U> pair)
{
    solutions.insert(pair);
}

template <typename T, typename U>
U SolutionImpl<T, U>::Solute(T key)
{
    auto it = solutions.find(key);
    if (it != solutions.end())
    {
        return it->second;
    }
    throw std::out_of_range("Key not found");
}

template <typename T, typename U>
SolutionImpl<T, U>::~SolutionImpl() {}

#endif //CG_SOLUTIONIMPL_H
