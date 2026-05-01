/// This file was taken from this source: https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a

/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */


// Regular text
constexpr char BLK[] = "\e[0;30m";
constexpr char RED[] = "\e[0;31m";
constexpr char GRN[] = "\e[0;32m";
constexpr char YEL[] = "\e[0;33m";
constexpr char BLU[] = "\e[0;34m";
constexpr char MAG[] = "\e[0;35m";
constexpr char CYN[] = "\e[0;36m";
constexpr char WHT[] = "\e[0;37m";

// Regular bold text
constexpr char BBLK[] = "\e[1;30m";
constexpr char BRED[] = "\e[1;31m";
constexpr char BGRN[] = "\e[1;32m";
constexpr char BYEL[] = "\e[1;33m";
constexpr char BBLU[] = "\e[1;34m";
constexpr char BMAG[] = "\e[1;35m";
constexpr char BCYN[] = "\e[1;36m";
constexpr char BWHT[] = "\e[1;37m";

// Regular underline text
constexpr char UBLK[] = "\e[4;30m";
constexpr char URED[] = "\e[4;31m";
constexpr char UGRN[] = "\e[4;32m";
constexpr char UYEL[] = "\e[4;33m";
constexpr char UBLU[] = "\e[4;34m";
constexpr char UMAG[] = "\e[4;35m";
constexpr char UCYN[] = "\e[4;36m";
constexpr char UWHT[] = "\e[4;37m";

// Regular background
constexpr char BLKB[] = "\e[40m";
constexpr char REDB[] = "\e[41m";
constexpr char GRNB[] = "\e[42m";
constexpr char YELB[] = "\e[43m";
constexpr char BLUB[] = "\e[44m";
constexpr char MAGB[] = "\e[45m";
constexpr char CYNB[] = "\e[46m";
constexpr char WHTB[] = "\e[47m";

// High intensty background
constexpr char BLKHB[] = "\e[0;100m";
constexpr char REDHB[] = "\e[0;101m";
constexpr char GRNHB[] = "\e[0;102m";
constexpr char YELHB[] = "\e[0;103m";
constexpr char BLUHB[] = "\e[0;104m";
constexpr char MAGHB[] = "\e[0;105m";
constexpr char CYNHB[] = "\e[0;106m";
constexpr char WHTHB[] = "\e[0;107m";

// High intensty text
constexpr char HBLK[] = "\e[0;90m";
constexpr char HRED[] = "\e[0;91m";
constexpr char HGRN[] = "\e[0;92m";
constexpr char HYEL[] = "\e[0;93m";
constexpr char HBLU[] = "\e[0;94m";
constexpr char HMAG[] = "\e[0;95m";
constexpr char HCYN[] = "\e[0;96m";
constexpr char HWHT[] = "\e[0;97m";

// Bold high intensity text
constexpr char BHBLK[] = "\e[1;90m";
constexpr char BHRED[] = "\e[1;91m";
constexpr char BHGRN[] = "\e[1;92m";
constexpr char BHYEL[] = "\e[1;93m";
constexpr char BHBLU[] = "\e[1;94m";
constexpr char BHMAG[] = "\e[1;95m";
constexpr char BHCYN[] = "\e[1;96m";
constexpr char BHWHT[] = "\e[1;97m";

// Reset
constexpr char reset[]     = "\e[0m";
constexpr char CRESET[]    = "\e[0m";
constexpr char COLOR_RESET[] = "\e[0m";
