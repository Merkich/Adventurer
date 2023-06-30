#include "searchserver.h"

QVector<QVector<RelativeIndex> > SearchServer::search(const QVector<QString> &queries_input)
{
    QVector<QVector<RelativeIndex> > result;
    QVector<QStringList> requestList;

    for(auto &query : queries_input)
    {
        requestList.push_back(query.split(QRegularExpression("\\s+")));
    }
    QVector<QVector<QString>> requestSingle;
    for(auto &request : requestList)
    {
        QString word;
        QVector<QString> timeVector;
        foreach(word, request)
        {
            if(!timeVector.contains(word))
                timeVector.push_back(word);
        }
        requestSingle.push_back(timeVector);
    }

    // search for all query words in documents
    QVector<QMap<size_t, int>> vectorResultInt; // doc_id, rel
    QMap<size_t, int> resultInt; // doc_id , all count of words
    for(auto &strRequest : requestSingle) // go through the query strings
    {
        for(auto &wordRequest : strRequest) // go by the words in the query string
        {
            QVector<Entry> resVec = _index.GetWordCount(wordRequest); // search for a word in all documents
            for(auto &res : resVec)
            {
                if(resultInt.contains(res.doc_id))
                {
                    resultInt[res.doc_id] += res.count;
                }
                else
                {
                    resultInt.insert(res.doc_id, res.count);
                }
            }
        }
        vectorResultInt.push_back(resultInt);
        resultInt.clear();
    }

//    we look for the maximum in each document and divide by it
    QMap<float, QVector<size_t>> resultMap; // divided by max and sorted, the second field is the document <rel, vector(doc_id)>
    QVector<QMap<float, QVector<size_t>>> vectorResultMap;
    for(auto &docResult : vectorResultInt ) // QMap<int, int>
    {
        if(!docResult.isEmpty()) // isn`t empty
        {
            // find max
            QList list = docResult.values();
            auto max = *std::max_element(list.begin(), list.end()); // max value in doc
            if(max == 0)
                continue;
            float timeValue;


            for(QMap<size_t, int>::iterator i = docResult.begin(); i != docResult.end(); i++)
            {
                timeValue = float(i.value()) / max;
                if(resultMap.contains(timeValue)) // find
                {
                    QVector<size_t> vec = resultMap[timeValue];
                    vec.push_back(i.key());
                    resultMap[timeValue] = vec;
                }
                else
                {
                    QVector<size_t> vec;
                    vec.push_back(i.key());
                    resultMap.insert(timeValue, vec);
                }
            }
        }
        vectorResultMap.push_back(resultMap);
        resultMap.clear();
    }
    // make result
   for(auto &map : vectorResultMap)
    {
        QVector<RelativeIndex> vec;
        for(QMap<float, QVector<size_t>>::iterator it = map.begin(); it != map.end(); it++)
        {

            RelativeIndex index;
            std::sort(it.value().begin(), it.value().end(), std::greater<int>( ));// sort from max to min
            for(auto &docNum : it.value())
            {

                index.doc_id = docNum;
                index.rank = it.key();
                vec.push_front(index);
            }

        }
        result.push_back(vec);
        vec.clear();
    }


   QVector<QVector<RelativeIndex> > resultWithMaxResponce;
   QVector<RelativeIndex> res;
   int count = 0;
   for(auto &resultA : result)
   {
       if(resultA.size() > maxResponce)
       {
           for(auto &resultB : resultA)
           {
               if(count < maxResponce)
               {
                   res.push_back(resultB);
                   count++;
               }
           }

       }
       else
           res = resultA;
       resultWithMaxResponce.push_back(res);
       res.clear();
       count = 0;
   }

   return resultWithMaxResponce;
}















