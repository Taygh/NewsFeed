/**
 * @file NewsFeed.h - header for PQHeap based News Feed
 * @author Taygh Singh Atwal
 * @see "Seattle University, CPSC2430, Spring 2018"
 */
#include <iostream>
#include "DictHash.h"
#pragma once

/**
 * @class News Feed - PQHeap based news feed application that rates stories
 * by staleness
 *
 * Can load in stories (using headlines, staleness factor, and story body),
 * remove least stale stories, check the least stale story, empty the stories,
 * check if the news feed is empty, weigh the staleness fo stories, change
 * staleness of stories, get story body for headline, and iterate through
 * headlines (beginning/end)
 *
 * Has all of the Big 5
 */

class NewsFeed
{
    public:
        typedef std::string Headline;
  
        typedef std::string Story;
  
        typedef int Staleness;
  
        typedef std::hash<std::string> HeadlineHasher;
    
        //constructor
        NewsFeed();

        //copy constructor
        NewsFeed(const NewsFeed& other);

        //move constructor
        NewsFeed(NewsFeed&& temp);

        //destructor
        ~NewsFeed();

        //copy overloaded assignment operator
        NewsFeed& operator=(const NewsFeed& other);

        //copy overloaded assignment operator
        NewsFeed& operator=(NewsFeed&& temp);

        /**
         * Loads in a news story
         *
         * @param headline  name of story
         * @param story     body text of story
         * @param state     stale value of story
         * @post            story added to feed
         */
        void enqueue(Headline headline, Story story, Staleness stale);

        /**
         * Peek at least stale story
         *
         * @throw           if empty news feed
         * @return          least stale headline
         */
        Headline peek() const;

        /**
         * Remove the least stale story
         *
         * @throw           if empty news feed
         * @post            least stale story removed
         */
        void dequeue();

        /**
         * Check if feed is empty
         *
         * @return          true if empty, false if not
         */
        bool empty() const;

        /**
         * Clear out the news feed
         *
         * @post            feed is empty
         */
        void clear();

        /**
         * Check the staleness of a headline
         *
         * @param headline  headline to check
         * @throw           if headline does not exist
         * @return          staleness of headline
         */
        Staleness weight(Headline headline) const;

        /**
         * Change the staleness of a headline
         *
         * @param headline  headline to check
         * @param stale     new stale value
         * @post            headline staleness changed
         */
        void reweight(Headline headline, Staleness stale);

        /**
         * Get story body text for a headline
         *
         * @param headline  headline to check
         * @throw           headline doesn't exist
         * @return          body text of headline
         */
        Story get(Headline headline) const;

        /**
         * @class const_terator - iterator for news feed 
         *
         * Can print nodes via * operator, iterate the iterator
         * with ++, and check if iterated values are equal via
         * != operator
         *
         * Has standard constructor
         */
        
        class const_iterator
        {

        public:

            //constructor
            const_iterator(typename DictHash<Headline, size_t, HeadlineHasher>::const_iterator iter) : it(iter)
            {
            }

            /**
             * Overloaded * operator for printing 
             *
             * @return          string at iteration 
             */
            const std::string operator*() const
            {
                return *it;
            }

            /**
             * Overloaded ++ operator for iteration
             *
             * @return          iterated pointer
             */
            const const_iterator operator++()
            {
                ++it;
                return *this;
            }

            /**
             * Overloaded != operator for iterator comparison
             *
             * @return          true if equal, false if not
             */
            bool operator!=(const const_iterator& iter) const
            {
                return it != iter.it;
            }

        private:
             typename DictHash<Headline, size_t, HeadlineHasher>::const_iterator it;
        };

        /**
         * Gets start of iterator
         *
         * @return          start of iterator
         */
        const_iterator begin() const;

        /**
         * Gets end of iterator
         *
         * @return          end of iterator
         */
        const_iterator end() const;      
  
    private:
        typedef size_t HeapLocation;
        DictHash<Headline, Story, HeadlineHasher> stories;
        DictHash<Headline, Staleness, HeadlineHasher> staleness;
        DictHash<Headline, size_t, HeadlineHasher> locations;
        Headline *heap;   
        size_t arrayMax;         
        size_t capacity;  
  
        void bubble(size_t cNode);
  
        void percolate(size_t pNode);
  
        Staleness value(size_t i) const;
 
        void swap(size_t i, size_t j);
  
        static size_t parent(size_t child);
  
        static size_t left(size_t p);
  
        static size_t right(size_t p);
  
        bool valid(size_t i) const;
 
        bool is_root(size_t i) const;
  
        bool is_leaf(size_t i) const;
  
        bool has_right(size_t p) const;
  
        void resize();
 };
