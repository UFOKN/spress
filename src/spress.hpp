#pragma once
#ifndef _SPRESS_H_
#define _SPRESS_H_

#include "cpp11.hpp"
using cpp11::data_frame;
using cpp11::integers;
using cpp11::doubles;

data_frame compress_(integers encoded);
integers decompress_(integers x, integers order);
integers encode_(size_t n, doubles x, doubles y);
data_frame decode_(size_t n, integers h, doubles bb);
data_frame grid_(size_t n, doubles bb);

#endif