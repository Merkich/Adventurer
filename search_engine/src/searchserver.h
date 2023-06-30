#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include <cstddef>
#include <algorithm>

#include "invertedindex.h"


struct RelativeIndex
{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer : public QObject
{
    Q_OBJECT
public:
    /**
    * @param dx a reference to the class is passed to the class constructor
    InvertedIndex,
    * so that SearchServer can find out the frequency of words found in
    request
    */
    SearchServer(InvertedIndex& idx) : _index(idx) {};
    /**
    * Search query processing method
    * @param queries_input search queries taken from file
        requests.json
    * @return returns a sorted list of relevant responses for
        given requests
    */
    QVector<QVector<RelativeIndex>> search(const QVector<QString>& queries_input);

    /**
    * The method sets the maximum number of requests
    * @param maximum number of requests
    */
    void setMaxResponce(int count)
    {
        maxResponce = count;
    };

private:
    InvertedIndex _index;
    // maximum number of requests
    int maxResponce = 0;

};

#endif // SEARCHSERVER_H
