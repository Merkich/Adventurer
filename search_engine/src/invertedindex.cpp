#include "invertedindex.h"

void InvertedIndex::UpdateDocumentBase(QVector<QString> input_docs)
{
    docs.clear();
    freq_dictionary.clear();
    docs = input_docs;
    QList<QFuture<QMap<QString, Entry>>> handlers;
    QFutureWatcher<QMap<QString, Entry>> watcher;

    for(int i = 0; i < docs.size(); i++)
    {
        handlers.append(QtConcurrent::run(&InvertedIndex::indexOneDoc, this, i));
    }

    for(auto &f: handlers)
    {
        f.waitForFinished();
    }
    for(auto &f: handlers)
    {
        makeDictionary(f.result());
    }

}

QVector<Entry> InvertedIndex::GetWordCount(const QString &word)
{
    return freq_dictionary[word];
}

QMap<QString, Entry> InvertedIndex::indexOneDoc(int docNum)
{
    QString str = docs.at(docNum);
    QStringList strList = str.split(QRegularExpression("\\s+"));
    QVector<QString> vecList;
    vecList = strList.toVector();

    QMap<QString, Entry> resultDict;

    QString word;
    foreach(word, vecList)
    {
        if(!resultDict.contains(word)) // this word has not been used yet
        {
            int count = vecList.count(word);
            if(count > 0)
            {
                Entry entry;
                entry.doc_id = docNum;
                entry.count = count;
                resultDict.insert(word, entry);
            }
        }
    }
    return resultDict;
}

void InvertedIndex::makeDictionary(QMap<QString, Entry> dictFromOneDoc)
{
    QString key;
    QList keys = dictFromOneDoc.keys();
    foreach(key, keys)
    {
        if(!freq_dictionary.contains(key))
        {
            QVector<Entry> vec;
            vec.push_back(dictFromOneDoc.value(key));
            freq_dictionary.insert(key, vec);
        }
        else
        {
            freq_dictionary[key].push_back(dictFromOneDoc.value(key));
        }
    }
}

