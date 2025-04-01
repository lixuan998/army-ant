#pragma once

#define FUNC_BEGIN(_name)        \
        .global _name;           \
        .type _name, %function;  \
        _name:

#define FUNC_END(_name)          \
        .size _name, .- _name
