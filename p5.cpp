/**
 * @file p5.cpp - Mock up of the editors' and readers' apps functionality for NewsFeed
 * @author Taygh Singh Atwal
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#include <iostream>
#include <string>
#include "NewsFeed.h"
using namespace std;

int main() {
    NewsFeed feed;
    cout << "Some initial tests:" << endl;
    feed.enqueue("Flash", "story of a fast dude", 1);
    feed.enqueue("J", "JJJ story", 2);
    feed.enqueue("K", "KKK story", 3);
    feed.enqueue("L", "LLL story", -1);
    cout << "Test iterator (order irrelevant): " << endl;
    for(auto it = feed.begin(); it != feed.end(); ++it)
    {
      cout << *it << endl;
    }
    feed.reweight("L", 4);
    NewsFeed::Headline h = feed.peek();
    cout << "peek:  '" << h << "' (expect Flash)" << endl;
    cout << "story: '" << feed.get(h) << "'" << endl;
    cout << "staleness: " << feed.weight(h) << " (expect 1)" << endl;
    feed.dequeue();
    cout << feed.empty() << " (expect false)" << endl;
    h = feed.peek();
    cout << "peek:  '" << h << "' (expect J)" << endl;
    cout << "story: '" << feed.get(h) << "'" << endl;
    cout << "staleness: " << feed.weight(h) << " (expect 2)" << endl;
    feed.dequeue();
    cout << feed.empty() << " (expect false)" << endl;
    h = feed.peek();
    cout << "peek:  '" << h << "' (expect K)" << endl;
    feed.dequeue();
    h = feed.peek();
    cout << "peek:  '" << h << "' (expect L)" << endl;
    cout << "staleness: " << feed.weight(h) << " (expect 4)" << endl;
    feed.dequeue();
    cout << feed.empty() << " (expect true)" << endl << endl;

    return 0;
}
