#pragma once

#include <stdio.h>
#include <iostream>
#include <unordered_map> 

using namespace std;

void resetResults();
bool combineResults(unordered_map<string, list<int>> queryResults);
list<string> getResults(string selectedSynonym);

static bool synonymExists(string);
static void addOneSyn(string, unordered_map<string, list<int>>);
static void addTwoSyn(string, string);
static void filterSyn(string, list<int>);
static void filterSyn(string, list<int>);
static bool indexDoesNotExist(int, list<int>);
static void eraseTableRow(int, unordered_map<string, list<int>>&, string);
static void mergeOneSyn(string, string newKey);
static void mergeDiffTable(string, string);
void printTables(); // for debugging
