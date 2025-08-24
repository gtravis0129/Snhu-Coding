/*
    Author: Gary Travis
    Date:   08/10/2025
    Course: CS210

    File:   CornerGrocer.h
    Purpose:
    Header file for the CornerGrocer class, which reads grocery item
    data from a file, counts item frequencies, writes a backup file,
    and provides a menu interface for user interaction.
*/

#pragma once
#include <unordered_map>
#include <string>
#include <ostream>

class ItemTracker {
public:
    // Build the frequency table 
    explicit ItemTracker(const std::string& filePath);

    // Was loading the file successful?
    bool loaded() const { return loaded_; }

    // Get frequency of a single item 
    int getFrequency(const std::string& item) const;

    // --- Unsorted (original) ---
    void printFrequencies(std::ostream& os) const;
    void printHistogram(std::ostream& os, char barChar = '#') const;

    // --- Sorted alphabetically by item name ---
    void printFrequenciesSortedAlpha(std::ostream& os) const;
    void printHistogramSortedAlpha(std::ostream& os, char barChar = '#') const;

    // --- Sorted by count descending (highest count first; ties alpha) ---
    void printFrequenciesSortedByCount(std::ostream& os) const;
    void printHistogramSortedByCount(std::ostream& os, char barChar = '#') const;

    // Export "item count" lines to a file; returns success
    bool exportFrequencies(const std::string& outPath) const;

private:
    std::unordered_map<std::string, int> freq_;
    bool loaded_ = false;

    static std::string normalize(const std::string& s);
    static bool parseLine(const std::string& line, std::string& itemOut, int& qtyOut);
};