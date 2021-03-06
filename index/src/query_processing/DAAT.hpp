#ifndef DAAT_HPP
#define DAAT_HPP

#include <vector>
#include <unordered_map>

#include "sparse_lexicon.hpp"
#include "global_parameters.hpp"
#include "Structures/documentstore.h"

//Returns the vector of docIDs that were found, from low-high
std::vector<unsigned int> DAAT(std::vector<unsigned int>& termIDs, std::vector<unsigned int>& docscontaining,
    GlobalType::NonPosIndex& index, SparseExtendedLexicon& exlex, std::string staticpath, DocumentStore& docstore);

#endif