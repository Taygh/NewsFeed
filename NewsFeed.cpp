/**
 * @file NewsFeed.cpp - implementation for PQHeap based News Feed
 * @author Taygh Singh Atwal
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "NewsFeed.h"
using namespace std;

NewsFeed::NewsFeed() : heap(nullptr), arrayMax(15), capacity(0)
{
    heap = new Headline[15];
}

NewsFeed::NewsFeed(const NewsFeed& other) : NewsFeed()
{
    *this = other;
}

NewsFeed::NewsFeed(NewsFeed&& temp) : NewsFeed()
{
    *this = temp;
}

NewsFeed::~NewsFeed()
{
    delete [] heap;
}

NewsFeed& NewsFeed::operator=(const NewsFeed& other)
{
    if(this != &other)
    {
        arrayMax = other.arrayMax;
        capacity = other.capacity;
        delete [] heap;
        heap = new Headline[arrayMax];

        for(size_t i = 1; i < arrayMax; i++)
        {
            heap[i] = other.heap[i];
        }
    }

    return *this;
}

NewsFeed& NewsFeed::operator=(NewsFeed&& temp)
{
    std::swap(heap, temp.heap);
    std::swap(arrayMax, temp.arrayMax);
    std::swap(capacity, temp.capacity);
    return *this;
}

void NewsFeed::swap(size_t i, size_t j)
{
    std::swap(heap[i], heap[j]);
    locations.add(heap[i], i);
    locations.add(heap[j], j);
}

bool NewsFeed::valid(size_t i) const
{
    return i >= 1 && i <= capacity;
}

bool NewsFeed::is_root(size_t i) const
{
    //1 based heap/array
    return i == 1;
}

bool NewsFeed::is_leaf(size_t i) const
{
    return !valid(left(i));
}

bool NewsFeed::has_right(size_t i) const
{
    return valid(right(i));
}

size_t NewsFeed::parent(size_t cNode)
{
    return cNode / 2;
}

size_t NewsFeed::left(size_t pNode)
{
    return pNode * 2;
}

size_t NewsFeed::right(size_t pNode)
{
  return left(pNode) + 1;
}

void NewsFeed::resize()
{
    size_t oldC = arrayMax;
    Headline* oldH = heap;
    arrayMax = 2 * arrayMax + 1;
    heap = new string[arrayMax];

    for(size_t i = 0; i < oldC; i++)
    {
        heap[i] = oldH[i];
    }

    delete [] oldH;
}

NewsFeed::Staleness NewsFeed::value(size_t i) const
{
    //protect from out of bounds
    if(i > capacity)
    {
        throw invalid_argument("size bigger than heap");
    }
  
    return weight(heap[i]);
}

NewsFeed::Headline NewsFeed::peek() const
{
    if(empty())
    {
        throw invalid_argument("peek empty priority queue");
    }

    else
    {
        return heap[1];
    }
}

bool NewsFeed::empty() const
{
    return capacity == 0;
}

void NewsFeed::clear()
{
    capacity = 0;
}

void NewsFeed::enqueue(NewsFeed::Headline headline, NewsFeed::Story story, NewsFeed::Staleness stale)
{
    //check for resize
    if(arrayMax - 1 == capacity)
    {
        resize();
    }

    heap[++capacity] = headline;
    stories.add(headline, story);
    staleness.add(headline, stale);
    locations.add(headline, capacity);
    bubble(capacity);
}

void NewsFeed::dequeue()
{
    if(empty())
    {
        throw std::invalid_argument("dequeue empty priority queue");
    }

    stories.remove(heap[1]);
    staleness.remove(heap[1]);
    locations.remove(heap[1]);
    swap(1, capacity--);
    swap(locations.get(heap[1]), locations.get(heap[capacity]));
    percolate(1);
}

NewsFeed::Staleness NewsFeed::weight(Headline headline) const
{
    if(!staleness.has(headline))
    {
        throw invalid_argument("headline doesn't exist to weight");
    }
    return staleness.get(headline);
}

void NewsFeed::reweight(Headline headline, Staleness stale)
{
    Staleness s = weight(headline);
    staleness.get(headline) = stale;

    //bubble up since new < old
    if(weight(headline) < s)
    {
        bubble(locations.get(headline));
    }

    //percolate down since new > old
    else
    {
        percolate(locations.get(headline));
    }
}

NewsFeed::Story NewsFeed::get(Headline headline) const
{
    if(!stories.has(headline))
    {
      throw invalid_argument("headline doesn't exist for story body");
    }
    return stories.get(headline);
}

void NewsFeed::bubble(size_t cNode)
{
    //bubble recursively
    if(!is_root(cNode) && value(cNode) < value(parent(cNode)))
    {
        size_t pNode = parent(cNode);
        swap(locations.get(heap[cNode]), locations.get(heap[pNode]));
        swap(cNode, pNode);
        bubble(pNode);
    }
}

void NewsFeed::percolate(size_t pNode)
{
    if(!is_leaf(pNode))
    {
        size_t cNode = left(pNode);

        if(has_right(pNode) && staleness.get(heap[right(pNode)]) < staleness.get(heap[cNode]))
        {
            cNode = right(pNode);
        }

        //return because left/right are balanced
        if(value(pNode) <= value(cNode))
        {
            return;
        }

        swap(cNode, pNode);
        cNode = pNode;
        swap(locations.get(heap[cNode]), locations.get(heap[pNode]));
    }
}

NewsFeed::const_iterator NewsFeed::begin() const
{
    return const_iterator(locations.begin());
}

NewsFeed::const_iterator NewsFeed::end() const
{
    return const_iterator(locations.end());
}
