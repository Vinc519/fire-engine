#pragma once

// This file was taken from this source:
// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a

/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */

// Regular text
constexpr char BLK[] = "\x1b[0;30m";
constexpr char RED[] = "\x1b[0;31m";
constexpr char GRN[] = "\x1b[0;32m";
constexpr char YEL[] = "\x1b[0;33m";
constexpr char BLU[] = "\x1b[0;34m";
constexpr char MAG[] = "\x1b[0;35m";
constexpr char CYN[] = "\x1b[0;36m";
constexpr char WHT[] = "\x1b[0;37m";

// Regular bold text
constexpr char BBLK[] = "\x1b[1;30m";
constexpr char BRED[] = "\x1b[1;31m";
constexpr char BGRN[] = "\x1b[1;32m";
constexpr char BYEL[] = "\x1b[1;33m";
constexpr char BBLU[] = "\x1b[1;34m";
constexpr char BMAG[] = "\x1b[1;35m";
constexpr char BCYN[] = "\x1b[1;36m";
constexpr char BWHT[] = "\x1b[1;37m";

// Regular underline text
constexpr char UBLK[] = "\x1b[4;30m";
constexpr char URED[] = "\x1b[4;31m";
constexpr char UGRN[] = "\x1b[4;32m";
constexpr char UYEL[] = "\x1b[4;33m";
constexpr char UBLU[] = "\x1b[4;34m";
constexpr char UMAG[] = "\x1b[4;35m";
constexpr char UCYN[] = "\x1b[4;36m";
constexpr char UWHT[] = "\x1b[4;37m";

// Regular background
constexpr char BLKB[] = "\x1b[40m";
constexpr char REDB[] = "\x1b[41m";
constexpr char GRNB[] = "\x1b[42m";
constexpr char YELB[] = "\x1b[43m";
constexpr char BLUB[] = "\x1b[44m";
constexpr char MAGB[] = "\x1b[45m";
constexpr char CYNB[] = "\x1b[46m";
constexpr char WHTB[] = "\x1b[47m";

// High intensty background
constexpr char BLKHB[] = "\x1b[0;100m";
constexpr char REDHB[] = "\x1b[0;101m";
constexpr char GRNHB[] = "\x1b[0;102m";
constexpr char YELHB[] = "\x1b[0;103m";
constexpr char BLUHB[] = "\x1b[0;104m";
constexpr char MAGHB[] = "\x1b[0;105m";
constexpr char CYNHB[] = "\x1b[0;106m";
constexpr char WHTHB[] = "\x1b[0;107m";

// High intensty text
constexpr char HBLK[] = "\x1b[0;90m";
constexpr char HRED[] = "\x1b[0;91m";
constexpr char HGRN[] = "\x1b[0;92m";
constexpr char HYEL[] = "\x1b[0;93m";
constexpr char HBLU[] = "\x1b[0;94m";
constexpr char HMAG[] = "\x1b[0;95m";
constexpr char HCYN[] = "\x1b[0;96m";
constexpr char HWHT[] = "\x1b[0;97m";

// Bold high intensity text
constexpr char BHBLK[] = "\x1b[1;90m";
constexpr char BHRED[] = "\x1b[1;91m";
constexpr char BHGRN[] = "\x1b[1;92m";
constexpr char BHYEL[] = "\x1b[1;93m";
constexpr char BHBLU[] = "\x1b[1;94m";
constexpr char BHMAG[] = "\x1b[1;95m";
constexpr char BHCYN[] = "\x1b[1;96m";
constexpr char BHWHT[] = "\x1b[1;97m";

// Reset
constexpr char reset[] = "\x1b[0m";
constexpr char CRESET[] = "\x1b[0m";
constexpr char COLOR_RESET[] = "\x1b[0m";
