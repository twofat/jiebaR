#ifndef CPPJIEBA_DICT_TRIE_HPP
#define CPPJIEBA_DICT_TRIE_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <stdint.h>
#include <cmath>
#include <limits>
#include "Limonp/StringUtil.hpp"
#include "Limonp/Logger.hpp"
#include "TransCode.hpp"
#include "Trie.hpp"
#include "Rcpp.h"

using namespace Rcpp;

namespace CppJieba
{
using namespace Limonp;
const double MIN_DOUBLE = -3.14e+100;
const double MAX_DOUBLE = 3.14e+100;
const size_t DICT_COLUMN_NUM = 3;
const char *const UNKNOWN_TAG = "x";



struct DictUnit
{
    Unicode word;
    double weight;
    string tag;
};

inline ostream &operator << (ostream &os, const DictUnit &unit)
{
    string s;
    s << unit.word;
    return os << string_format("%s %s %.3lf", s.c_str(), unit.tag.c_str(), unit.weight);
}

typedef map<size_t, const DictUnit *> DagType;

class DictTrie
{
public:
    typedef Trie<Unicode::value_type, DictUnit, Unicode, vector<Unicode>, vector<const DictUnit *> > TrieType;
private:
    vector<DictUnit> _nodeInfos;
    TrieType *_trie;

    double _minWeight;
private:
    unordered_set<Unicode::value_type> _userDictSingleChineseWord;
public:
    bool isUserDictSingleChineseWord(const Unicode::value_type &word) const
    {
        return isIn(_userDictSingleChineseWord, word);
    }
public:
    double getMinWeight() const
    {
        return _minWeight;
    };

public:
    DictTrie()
    {
        _trie = NULL;
        _minWeight = MAX_DOUBLE;
    }
    DictTrie(const string &dictPath, const string &userDictPath = "")
    {
        new (this) DictTrie();
        init(dictPath, userDictPath);
    }
    ~DictTrie()
    {
        if (_trie)
        {
            delete _trie;
        }
    }

public:
    bool init(const string &dictPath, const string &userDictPath = "")
    {

        if (_trie)
        {
            stop("_trie Fail  DictTrie.hpp : 92");
        }
        _loadDict(dictPath);
        _calculateWeight(_nodeInfos);
        _minWeight = _findMinWeight(_nodeInfos);

        if (userDictPath.size())
        {
            double maxWeight = _findMaxWeight(_nodeInfos);
            _loadUserDict(userDictPath, maxWeight, UNKNOWN_TAG);
        }
        _shrink(_nodeInfos);
        _trie = _createTrie(_nodeInfos);
        if (!_trie)
        {
            stop("_trie Fail  DictTrie.hpp : 107");
        }
        return true;
    }

public:
    const DictUnit *find(Unicode::const_iterator begin, Unicode::const_iterator end) const
    {
        return _trie->find(begin, end);
    }
    bool find(Unicode::const_iterator begin, Unicode::const_iterator end, DagType &dag, size_t offset = 0) const
    {
        return _trie->find(begin, end, dag, offset);
    }


private:
    TrieType *_createTrie(const vector<DictUnit> &dictUnits)
    {
        if (!(dictUnits.size()))
        {
            stop("dictUnits.size() == 0  DictTrie.hpp : 128");
        }
        vector<Unicode> words;
        vector<const DictUnit *> valuePointers;
        for (size_t i = 0 ; i < dictUnits.size(); i ++)
        {
            words.push_back(dictUnits[i].word);
            valuePointers.push_back(&dictUnits[i]);
        }

        TrieType *trie = new TrieType(words, valuePointers);
        return trie;
    }
    void _loadUserDict(const string &filePath, double defaultWeight, const string &defaultTag)
    {
        ifstream ifs(filePath.c_str());
        if (!(ifs))
        {
            stop("File Open Fail  DictTrie.hpp : 146");
        }
        string line;
        DictUnit nodeInfo;
        vector<string> buf;
        size_t lineno;
        for (lineno = 0; getline(ifs, line); lineno++)
        {
            buf.clear();
            split(line, buf, " ");
            if (!(buf.size() >= 1))
            {
                stop("buf.size()<1  DictTrie.hpp : 158");
            }
            if (!TransCode::decode(buf[0], nodeInfo.word))
            {
                // LogError("line[%u:%s] illegal.", lineno, line.c_str());
                continue;
            }
            if (nodeInfo.word.size() == 1)
            {
                _userDictSingleChineseWord.insert(nodeInfo.word[0]);
            }
            nodeInfo.weight = defaultWeight;
            nodeInfo.tag = (buf.size() == 2 ? buf[1] : defaultTag);
            _nodeInfos.push_back(nodeInfo);
        }
        // LogInfo("load userdict[%s] ok. lines[%u]", filePath.c_str(), lineno);
    }
    void _loadDict(const string &filePath)
    {
        ifstream ifs(filePath.c_str());
        if (!(ifs))
        {
            stop("File Open Fail  DictTrie.hpp : 180");
        }
        string line;
        vector<string> buf;

        DictUnit nodeInfo;
        for (size_t lineno = 0 ; getline(ifs, line); lineno++)
        {
            split(line, buf, " ");
            if (!(buf.size() == DICT_COLUMN_NUM))
            {
                Rcpp::Rcout << "Dict line: " << lineno << std::endl;
                Rcpp::Rcout << "Word column: " << buf.size() << std::endl;
                Rcpp::stop("buf.size() != DICT_COLUMN_NUM");
            }
            if (!TransCode::decode(buf[0], nodeInfo.word))
            {
                // LogError("line[%u:%s] illegal.", lineno, line.c_str());
                continue;
            }
            nodeInfo.weight = atof(buf[1].c_str());
            nodeInfo.tag = buf[2];

            _nodeInfos.push_back(nodeInfo);
        }
    }
    double _findMinWeight(const vector<DictUnit> &nodeInfos) const
    {
        double ret = MAX_DOUBLE;
        for (size_t i = 0; i < nodeInfos.size(); i++)
        {
            ret = min(nodeInfos[i].weight, ret);
        }
        return ret;
    }
    double _findMaxWeight(const vector<DictUnit> &nodeInfos) const
    {
        double ret = MIN_DOUBLE;
        for (size_t i = 0; i < nodeInfos.size(); i++)
        {
            ret = max(nodeInfos[i].weight, ret);
        }
        return ret;
    }

    void _calculateWeight(vector<DictUnit> &nodeInfos) const
    {
        double sum = 0.0;
        for (size_t i = 0; i < nodeInfos.size(); i++)
        {
            sum += nodeInfos[i].weight;
        }
        if (!(sum))
        {
            stop("sum==0  DictTrie.hpp : 234");
        }
        for (size_t i = 0; i < nodeInfos.size(); i++)
        {
            DictUnit &nodeInfo = nodeInfos[i];
            if (!(nodeInfo.weight))
            {
                Rcpp::Rcout << i << std::endl;
                Rcpp::stop("weight==0");

            }
            nodeInfo.weight = log(double(nodeInfo.weight) / double(sum));
        }
    }

    void _shrink(vector<DictUnit> &units) const
    {
        vector<DictUnit>(units.begin(), units.end()).swap(units);
    }


};
}

#endif
