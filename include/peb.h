// peb.h - wipe our own loader artifacts from the PEB so process enumeration
// heuristics (parent/cmdline/CWD based) can't attribute this exe.
#pragma once
namespace peb {
    void sanitize();  // erase loader entry, clear CWD, wipe command line
}
