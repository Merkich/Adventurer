
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../search_engine/src/invertedindex.h"
#include "../search_engine/src/searchserver.h"

#include <QString>
#include <QVector>

using namespace testing;

void TestInvertedIndexFunctionality(
    const QVector<QString>& docs,
    const QVector<QString>& requests,
    const QVector<QVector<Entry>>& expected
) {
    QVector<QVector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests)
    {
        QVector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}


TEST(TestCaseInvertedIndex, TestBasic)
{
    const QVector<QString> docs = {
    "london is the capital of great britain",
    "big ben is the nickname for the Great bell of the striking clock"
    };
    const QVector<QString> requests = {"london", "the"};
    const QVector<QVector<Entry>> expected = {
    {
    {0, 1}
    }, {
    {0, 1}, {1, 3}
    }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2)
{
    const QVector<QString> docs = {
    "milk milk milk milk water water water",
    "milk water water",
    "milk milk milk milk milk water water water water water",
    "americano cappuccino"
    };
    const QVector<QString> requests = {"milk", "water", "cappuccino"};
    const QVector<QVector<Entry>> expected = {
    {
        {0, 4}, {1, 1}, {2, 5}
    },
    {
        {0, 3}, {1, 2}, {2, 5}
    },
    {
        {3, 1}
    }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord)
{
    const QVector<QString> docs = {
    "a b c d e f g h i j k l",
    "statement"
    };
    const QVector<QString> requests = {"m", "statement"};
    const QVector<QVector<Entry>> expected = {
    {
    }, {
    {1, 1}
    }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseSearchServer, TestSimple) {
    const QVector<QString> docs = {
    "milk milk milk milk water water water",
    "milk water water",
    "milk milk milk milk milk water water water water water",
    "americano cappuccino"
    };
    const QVector<QString> request = {"milk water", "sugar"};

    const QVector<QVector<RelativeIndex>> expected = {
        {
        {2, 1},
        {0, 0.7},
        {1, 0.3}
        },
        {
        }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.setMaxResponce(5);
    QVector<QVector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5) {
    const QVector<QString> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };
    const QVector<QString> request = {"moscow is the capital of russia"};
    const QVector<QVector<RelativeIndex>> expected = {
    {
    {7, 1.0},
    {14, 1.0},
    {0, 0.666666687},
    {1, 0.666666687},
    {2, 0.666666687}
    }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.setMaxResponce(5);
    QVector<QVector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}
