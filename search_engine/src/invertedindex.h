#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <cstddef>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QRegularExpression>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>


struct Entry {
    size_t doc_id,
    count;
// This operator is required for test scenarios
bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
        count == other.count);
    }
};

class InvertedIndex : public QObject
{
    Q_OBJECT
public:

    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex &index)
    {
        docs = index.docs;
        freq_dictionary = index.freq_dictionary;
    }

    /**
    * Update or fill in the base of documents, according to which we will make
    search
    * @param texts_input document content
    */
    void UpdateDocumentBase(QVector<QString> input_docs);

    /**
    * The method determines the number of occurrences of the word word in the loaded database
    documents
    * @param word is the word whose occurrence frequency is to be determined
    * @return returns a prepared list with word frequency
    */
    QVector<Entry> GetWordCount(const QString& word);
    /**
    * The method determines how many times words occur in the searched document
    * @param document index
    * @return linking the search word with the document
    */
    /**
    * Method parses one document into components
    * @param document number
    * @return returns a list of the link of the searched word with the document
    */
    QMap<QString, Entry> indexOneDoc(int);

private:
    /**
    * Document content list
    */
    QVector<QString> docs;
    /**
    * Frequency dictionary
    */
    QMap<QString, QVector<Entry>> freq_dictionary;
    /**
     * Method builds a dictionary of possible words
    * @param связка искомого слова с документом
    */
    void makeDictionary(QMap<QString, Entry>);

};

#endif // INVERTEDINDEX_H
