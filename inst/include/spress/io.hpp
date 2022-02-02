#pragma once
#ifndef _SPRESS_IO_H_
#define _SPRESS_IO_H_

#include "flatbuffers/flatbuffers.h"
#include "schema/spress_header_generated.h"
#include "schema/spress_feature_generated.h"

#include <vector>
#include <string>
#include <cstdarg>

using std::string;
using std::vector;

namespace spress
{
namespace io
{

template <class T>
struct Prop<T> {
    string name;
    std::uint8_t type; // see fbs/spress_header.fbs for corresponding enums
    vector<T> data;
};

void parse_properties();
void parse_geometry();
void write_feature();
void write_header();

}

void serialize();
void deserialize();
}


#endif