/*
    Author: Gary Travis
    Date:   08/10/2025
    Course: CS210

    File:   CornerGrocer.cpp
    Purpose:
    Implements the CornerGrocer class methods for loading grocery data,
    counting frequencies, writing backups, and handling menu operations.
*/

#include "ItemTracker.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <utility>

using std::string;

static inline bool isWordChar(unsigned char c) {
    return std::isalnum(c) || c == '\'' || c == '-';
}

string ItemTracker::normalize(const string& s) {
    // trim, strip leading/trailing non-word chars, lowercase
    size_t start = 0, end = s.size();
    while (start < end && !isWordChar(static_cast<unsigned char>(s[start]))) ++start;
    while (end > start && !isWordChar(static_cast<unsigned char>(s[end - 1]))) --end;
    string t = s.substr(start, end - start);
    std::transform(t.begin(), t.end(), t.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return t;
}

// Supports either format per line:
//   1) "apple"                    (counts as 1)
//   2) "apple 3"                  (counts as 3)
//   3) "  Apple,  2 "             (punctuation/spaces ok; case-insensitive)
bool ItemTracker::parseLine(const string& line, string& itemOut, int& qtyOut) {
    std::istringstream iss(line);
    string a, b;
    if (!(iss >> a)) return false;

    // Try to read a second token as quantity, but it's optional
    bool hasQty = static_cast<bool>(iss >> b);

    // Normalize item token (strip punctuation, lowercase)
    string item = normalize(a);
    if (item.empty()) return false;

    int qty = 1;
    if (hasQty) {
        // Try parse quantity as int
        try {
            size_t idx = 0;
            int val = std::stoi(b, &idx);
            if (idx == b.size() && val > 0) qty = val;
            // else keep default 1 if not a clean positive int
        }
        catch (...) {
            // leave qty = 1 if stoi fails
        }
    }

    itemOut = item;
    qtyOut = qty;
    return true;
}

ItemTracker::ItemTracker(const string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        loaded_ = false;
        return;
    }
    string line;
    while (std::getline(in, line)) {
        string item;
        int qty = 0;
        if (parseLine(line, item, qty)) {
            freq_[item] += qty;
        }
    }
    loaded_ = true;
}

int ItemTracker::getFrequency(const string& item) const {
    string key = normalize(item);
    std::unordered_map<string, int>::const_iterator it = freq_.find(key);
    return (it == freq_.end()) ? 0 : it->second;
}

void ItemTracker::printFrequencies(std::ostream& os) const {
    for (std::unordered_map<string, int>::const_iterator it = freq_.begin();
        it != freq_.end(); ++it) {
        const string& item = it->first;
        int count = it->second;
        os << item << " " << count << '\n';
    }
}

void ItemTracker::printHistogram(std::ostream& os, char barChar) const {
    for (std::unordered_map<string, int>::const_iterator it = freq_.begin();
        it != freq_.end(); ++it) {
        const string& item = it->first;
        int count = it->second;
        os << item << " ";
        for (int i = 0; i < count; ++i) os << barChar;
        os << '\n';
    }
}

// ---------- Sorted outputs ----------

void ItemTracker::printFrequenciesSortedAlpha(std::ostream& os) const {
    std::vector<std::pair<string, int> > v;
    v.reserve(freq_.size());
    for (std::unordered_map<string, int>::const_iterator it = freq_.begin();
        it != freq_.end(); ++it) {
        v.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(v.begin(), v.end(),
        [](const std::pair<string, int>& a, const std::pair<string, int>& b) {
            return a.first < b.first; // alpha by name
        });
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i].first << " " << v[i].second << '\n';
    }
}

void ItemTracker::printHistogramSortedAlpha(std::ostream& os, char barChar) const {
    std::vector<std::pair<string, int> > v;
    v.reserve(freq_.size());
    for (std::unordered_map<string, int>::const_iterator it = freq_.begin();
        it != freq_.end(); ++it) {
        v.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(v.begin(), v.end(),
        [](const std::pair<string, int>& a, const std::pair<string, int>& b) {
            return a.first < b.first; // alpha by name
        });
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i].first << " ";
        for (int k = 0; k < v[i].second; ++k) os << barChar;
        os << '\n';
    }
}

void ItemTracker::printFrequenciesSortedByCount(std::ostream& os) const {
    std::vector<std::pair<string, int> > v;
    v.reserve(freq_.size());
    for (std::unordered_map<string, int>::const_iterator it = freq_.begin();
        it != freq_.end(); ++it) {
        v.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(v.begin(), v.end(),
        [](const std::pair<string, int>& a, const std::pair<string, int>& b) {
            if (a.second != b.second) return a.second > b.second; // high count first
            return a.first < b.first; // tie-break alpha
        });
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i].first << " " << v[i].second << '\n';
    }
}

void ItemTracker::printHistogramSortedByCount(std::ostream& os, char barChar) const {
    std::vector<std::pair<string, int> > v;
    v.reserve(freq_.size());
    for (std::unordered_map<string, int>::const_iterator it = freq_.begin();
        it != freq_.end(); ++it) {
        v.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(v.begin(), v.end(),
        [](const std::pair<string, int>& a, const std::pair<string, int>& b) {
            if (a.second != b.second) return a.second > b.second; // high count first
            return a.first < b.first; // tie-break alpha
        });
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i].first << " ";
        for (int k = 0; k < v[i].second; ++k) os << barChar;
        os << '\n';
    }
}

// ---------- Export ----------

bool ItemTracker::exportFrequencies(const string& outPath) const {
    std::ofstream out(outPath);
    if (!out) return false;
    printFrequencies(out); // export unsorted
    return true;
}
